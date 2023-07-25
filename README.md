# Latching-Relay-Board
Latching ESP32 relay board with analog and inputs

This ia a 16 point Latching relay board by VE7FRG.  
This board was designed for solar powered, mountain top, radio repeater
sites but is certainly not limited to that use.

Features
--------
16 latching relays each one with a single usable contact rated at 2A.
   7 of the 16 relays are in parallel with sink outputs of a repeater controller
   such as a RLC-4 or RLC-6.
4  Inputs for door switch alarms or other uses.
4  Analog I/O points available by including optional ADS1115 board
Remaining pins on ESP32 brought out to header for use.
I2C bus brought out to terminal blocks.
Operates on 12VDC
Communication is by WIFI

The table below outlines the ascii command sequences the
board accepts. 

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
|_________|______________________________________________ |
| Note: All commands must be terminated with a '\n' char  |
+---------------------------------------------------------+
