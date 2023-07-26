# Latching-Relay-Board
Latching ESP32 relay board with analog and inputs

This is a 16 point Latching relay board by me, VE7FRG.  
This board was designed for solar powered, mountain top, radio repeater
sites but is certainly not limited to that use.  <br/><br/>

Ordering
--------
Bare and pre-built boards may be ordered from me, please email farrisg at gmsys.com<br/><br/>
![relay_board-800](https://github.com/horga83/Latching-Relay-Board/assets/2425304/6049794b-617f-4010-b2ef-43ba544a5afc)
![bare_board-800](https://github.com/horga83/Latching-Relay-Board/assets/2425304/fcb2d0c8-da5b-42c9-9c24-75382f7059b7)

Features
--------
* ESP32 microcontroller with watchdog code.
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


