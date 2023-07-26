# WIFI Latching-Relay-Board
ESP32 based latching relay board with digital inputs and optional 16 bit analog inputs.

This board was designed for solar powered, mountain top, radio repeater
sites, but is certainly not  
limited to that use.  <br/><br/>
Most relay boards use standard relays that are energised continuously when they are closed and  
use anywhere in the range of 30 to 100mA or more. With 10 relays taking say 40mA, on all the time,  
plus the board electronics, you are looking at approximately .5A continuous current draw 24/7.  
On a solar powered site this may be too much.
  
In addition, with standard relays, if the power is removed the relays will all drop out removing power from  
equipment you may not want to go down.  A latching relay board solves these issues with no current  
being drawn by coils and static state contacts on power cycle.

Ordering
--------
Bare and pre-built boards may be ordered from me, please email \<**farrisg at gmsys.com**\><br/>  
Bare boards are US $29.00 plus shipping.  
Currently Pre-built board prices are available on a quote basis, due to component prices.  
The pre-built board may be ordered with 8 relays to save on cost.  
36pin ESP32 modules are available with the bare board as an option.  

Features
--------
* 36pin ESP32 (doit dev v1) microcontroller with watchdog code.
* 16 latching relays each one with a single usable contact rated at 2A.
* 7 of the 16 relays are in parallel with sink outputs of a repeater controller such as RLC-4.  
* Low current indicator LEDS for all relays.
* 4 Inputs for door switch alarms or other uses.  
* 4  Analog I/O points available by including optional ADS1115 board.  
* Remaining pins on ESP32 brought out to header for use.  
* I2C bus brought out to terminal blocks.  
* Operates on 12VDC at approx 60mA.
* Communication is by WIFI.
* Python test and "relay" application.
 

Commands
--------
The table below outlines the ascii command sequences the
board accepts.  

**Note:** All commands must be terminated with a '\n' character.   

| Command | Description                                   | 
|:--------|:----------------------------------------------|
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
| I2      | Get input I2 state - GPIO32                   |
| I3      | Get input I3 state - GPIO35                   |
| I4      | Get input I4 state - GPIO34                   |
|         |                                               |


![relay_board-800](https://github.com/horga83/Latching-Relay-Board/assets/2425304/6049794b-617f-4010-b2ef-43ba544a5afc)
![bare_board-800](https://github.com/horga83/Latching-Relay-Board/assets/2425304/fcb2d0c8-da5b-42c9-9c24-75382f7059b7)
