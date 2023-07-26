/*
# Copyright (C) 2023  George Farris <farrisg@gmsys.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

OPERATION

SECRETS.H

Wifi credentials and board password is stored in a "secrets.h" file
You must have a secrets.h file that has the following:

#define SECRET_SSID "your ssid"
#define SECRET_PASS "your wifi password"
#define SECRET_PORT 80
#define BOARD_PASS "the board access password"

The secrets.h file must be in the same folder as your lrb-pinmode.ino file.
You must edit this file before uploading to your ESP32.

When a socket connection is made to the board it will request a "board"
password which is stored in the secrets.h file. On success you will able
to send the board any of the following commands:

If commands are not sent to the board within the watchdog timer time the
ESP32 will reset and you will have to re-authenticate.

The normal procedure would be to connect, authenticate, give one or
more commands to the board and disconnect.

NOTE: All traffic to and from the board must be strings.

+---------------------------------------------------------+
| Command | Description                                   | 
+---------+-----------------------------------------------+
| ST      | Get status (returns status of all 16 relays)  |
|         | See note below about status                   |
| RCnn    | Close relay nn - example RC00 or RC12         |
| ROnn    | Open relay nn                                 |
| RTnn    | Toggle relay nn                               |
| OA      | Open all relays                               |
| CL      | Close all relays                              |
| TE      | Test all relays - close all then open         |
|         |                                               |
| A0      | Get Analog A0 value                           |
| A1      | Get Analog A1 value                           |
| A2      | Get Analog A2 value                           |
| A3      | Get Analog A3 value                           |
|         |                                               |
| I1      | Get input I1 state - GPIO33                   |
| I2      | Get input I0 state - GPIO32                   |
| I3      | Get input I3 state - GPIO35                   |
| I4      | Get input I4 state - GPIO34                   |
+---------------------------------------------------------+

Pin numbers of the different chips
+-----------------------------------------------------+
| MCP23x08 Pin # | MCP23x17 Pin # | Pin Name | Pin ID |
| :------------: | :------------: | :------: | :----: |
|       10       |       21       |   GPA0   |   0    |
|       11       |       22       |   GPA1   |   1    |
|       12       |       23       |   GPA2   |   2    |
|       13       |       24       |   GPA3   |   3    |
|       14       |       25       |   GPA4   |   4    |
|       15       |       26       |   GPA5   |   5    |
|       16       |       27       |   GPA6   |   6    |
|       17       |       28       |   GPA7   |   7    |
|       --       |       1        |   GPB0   |   8    |
|       --       |       2        |   GPB1   |   9    |
|       --       |       3        |   GPB2   |   10   |
|       --       |       4        |   GPB3   |   11   |
|       --       |       5        |   GPB4   |   12   |
|       --       |       6        |   GPB5   |   13   |
|       --       |       7        |   GPB6   |   14   |
|       --       |       8        |   GPB7   |   15   |
+-----------------------------------------------------+

To turn a relay on you must turn the direction relay ON
then toggle the coil on the latching relay.


STATUS

Status is returned in two 8 bit bytes as a string.
Relays 1-7 are in the first response and relays 8-15
are in the second response.

1 = relay 1
2 = relay 2
4 = relay 3
8 = relay 4
16 = relay 5
32 = relay 6
64 = relay 7
128 = relay 8


*/

#include <WiFi.h>
#include <esp_task_wdt.h>
#include "secrets.h"
#include <Adafruit_MCP23X17.h>

// MCP23017 I2C addresses, hard wired on board
#define OUT_ADDR 0x20 // I2C address 0x20 for outputs
#define INP_ADDR 0x24  // I2C address 0x24 for inputs

// Direct ESP32 control on GPIO's
#define DIR_RELAY 18    //output
#define INP_1 33        //inputs
#define INP_2 32
#define INP_3 35
#define INP_4 34

// for register reads of MCP23017 (8 bits)
#define PORT_A 0  //relays 9-16
#define PORT_B 1  //relays 1-7

#define PASS 1  // control flow for state
#define CMD 2

#define WDT_TIMEOUT 300 //300 second Watchdog timer

Adafruit_MCP23X17 control;
Adafruit_MCP23X17 status;

// These vars hold data from the secrets.h file.
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int port = SECRET_PORT;
char board_pwd[] = BOARD_PASS;


// Start server on IP port "port" from secrets.h file
WiFiServer wifiServer(port);

void setup(void)
{
    Serial.begin(115200);
    control.begin_I2C(OUT_ADDR);
    status.begin_I2C(INP_ADDR);

    for(int i=0; i<16; i++){
        control.pinMode(i, OUTPUT);  // set all 16 pins to output
        status.pinMode(i, INPUT);  // set all pins to input for status   
    }

    pinMode(DIR_RELAY, OUTPUT);  // set D18 on ESP32 to output
    pinMode(INP_1, INPUT);       // set 33,32,35,34 to inputs
    pinMode(INP_2, INPUT);
    pinMode(INP_3, INPUT);
    pinMode(INP_4, INPUT);

    Serial.println("Configuring Watchdog Timer...");
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    
    WiFi.begin(ssid, pass);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());

    wifiServer.begin();
}

bool process_password(String pw)
{
    if (pw == board_pwd)
        return(true);
    else
        return(false);
}

void close_relay(int relay)
{
    digitalWrite(DIR_RELAY, HIGH);
    delay(100);
    control.digitalWrite(relay, HIGH);
    delay(100);
    control.digitalWrite(relay, LOW);
    delay(100);
    digitalWrite(DIR_RELAY, LOW);
    delay(100);
}

void open_relay(int relay)
{
    digitalWrite(DIR_RELAY, LOW);
    delay(100);
    control.digitalWrite(relay, HIGH);
    delay(100);
    control.digitalWrite(relay, LOW);
    delay(100);
}

void toggle_relay(int relay)
{
    bool state = status.digitalRead(relay);
    if (state) {
        open_relay(relay);
        delay(3000);
        close_relay(relay);
    } else {
        close_relay(relay);
        delay(3000);
        open_relay(relay);
    }
}

void open_all_relays()
{
    //Serial.println("Opening all relays...");
    for (int i=0; i<16; i++) {
        control.digitalWrite(i, HIGH);
        delay(100);
        control.digitalWrite(i,LOW);
    }
}

void close_all_relays()
{
    //Serial.println("Closing all relays...");
    digitalWrite(DIR_RELAY, HIGH);
    delay(100);
    for (int i=0; i<16; i++) {
        control.digitalWrite(i, HIGH);
        delay(100);
        control.digitalWrite(i,LOW);
    }
    delay(100);
    digitalWrite(DIR_RELAY, LOW);
}

// used in test routine only
void toggle_latch(int relay)
{
    control.digitalWrite(relay, HIGH);
    delay(100);
    control.digitalWrite(relay, LOW);
    //delay(250);
}

void test_relays() {
    digitalWrite(DIR_RELAY, HIGH);
    for (int i=8; i<16; i++) {
        toggle_latch(i);
    }
    for (int i=0; i<8; i++) {
        toggle_latch(i);
    }

    delay(1000);
        
    digitalWrite(DIR_RELAY, LOW);
    for (int i=7; i>=0; i--) {
        toggle_latch(i);
    }
    for (int i=15; i>=8; i--) {
        toggle_latch(i);
    }
}

bool process_command(String cmd, WiFiClient client)
{
 
    String operation = "";
    String relay = "";
    String cmd1 = "";
    String pin = "";
    
    Serial.println(cmd);
    operation = cmd.substring(0,2);
    cmd1 = cmd.substring(0,1);
    
    //if (operation == "RC" or operation == "RO" or operation == "RT") {
    if (cmd1 == "R") {
        relay = cmd.substring(2,5);
        // Close relay
        if (operation == "RC") {
            close_relay(relay.toInt());
        }
            
        // Open relay
        if (operation == "RO") {
            open_relay(relay.toInt());
        }
        
        if (operation == "RT") {
            toggle_relay(relay.toInt());
        }
        return(true);

    } else if (cmd1 == "I") {
        pin = cmd.substring(1,3);
        client.print(String(digitalRead(pin.toInt())));
        return(true);

    } else if (operation == "TE") {
        test_relays();
        return(true);

    } else if (operation == "ST") {
        client.print(String(status.readGPIO(PORT_B)));  // relays 1-7
        client.print(String(status.readGPIO(PORT_A)));  // relays 8-15
        return(true);

    } else if (operation == "OA") {
        open_all_relays();
        return(true);

    } else if (operation == "CL") {
        close_all_relays();
        return(true);
    }     

    //Serial.println(control.readGPIOB());
    return(false);
}

//int i = 0;
int last = millis();

void loop(void)
{
    // resetting WDT every WDT_TIMEOUT - 1
    // if (millis() - last >= WDT_TIMEOUT -1) 
    // {
        //Serial.println("Resetting WDT...");
        //Serial.println(last);
        esp_task_wdt_reset();
    //     last = millis();
    //     // i++;
    //     // if (i == 80)
    //     //     Serial.println("Stopping WDT reset. CPU should reboot in 3s");
    // }
    int flow;
    WiFiClient client = wifiServer.available();
    String command = ""; 
    String password = "";

    if (client) {
        Serial.write("Password? ");
        client.write("Password?");
        flow = PASS;

        while (client.connected()) {
            switch (flow) {
                // get the password
                case PASS: {
                    while (client.available() > 0) {
                        
                        char c = client.read();
                        //Serial.println(c);
                        if (c == '\n') {     // if the byte is a newline character
                            if (process_password(password))
                            {
                                Serial.println("OK");
                                //client.println("OK");
                                flow = CMD;
                                esp_task_wdt_reset();  // reset the watchdog
                                break;
                            } else {
                                password = "";
                                client.write("Password?");
                                esp_task_wdt_reset();  // reset the watchdog
                                client.flush();
                            }
                            // password should send 'OK' back if good
                        } else {  // if you got anything else but a carriage return character,
                            password += c;      // add it to the end of the currentLine
                        }
                        delay(10);
                    }
                    break;
                }
                // get the command and process
                case CMD: {
                    command = "";
                    while (client.available() > 0) {
                        char c = client.read();

                        if (c == '~') {     // if the byte is a newline character
                            // if the current line is blank, you got two newline characters in a row.
                            // that's the end of the client HTTP request, so send a response:
                            Serial.println(c);
                            client.stop();
                            flow = PASS;
                        } else if (c == '\n') {
                            if (process_command(command, client))
                            {
                                //Serial.print("Cmd - ");
                                //Serial.println(command);
                                //client.println("OK");
                                esp_task_wdt_reset();  // reset the watchdog
                                command = "";
                                //break;
                            } else {
                                command = "";
                                Serial.println("Bad command...");
                                esp_task_wdt_reset();  // reset the watchdog
                                break;
                            }
                        } else {  // if you got anything else but a carriage return character,
                            command += c;      // add it to the end of the currentLine
                        }
                    }
                    break;
                }
            }
        }
        client.stop();
        Serial.println("Client disconnected");
    }
}

