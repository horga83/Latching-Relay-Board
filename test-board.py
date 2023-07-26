#!/usr/bin/python

# +---------------------------------------------------------+
# | Command | Description                                   | 
# +---------+-----------------------------------------------+
# | ST      | Get status (returns status of all 16 relays)  |
# |         | See note below about status                   |
# | RCnn    | Close relay nn - example RC00 or RC12         |
# | ROnn    | Open relay nn                                 |
# | RTnn    | Toggle relay nn                               |
# | OA      | Open all relays                               |
# | CL      | Close all relays                              |
# | TE      | Test all relays - close all then open         |
# |         |                                               |
# | A0      | Get Analog A0 value                           |
# | A1      | Get Analog A1 value                           |
# | A2      | Get Analog A2 value                           |
# | A3      | Get Analog A3 value                           |
# |         |                                               |
# | I1      | Get input I1 state - GPIO33                   |
# | I2      | Get input I2 state - GPIO32                   |
# | I3      | Get input I3 state - GPIO35                   |
# | I4      | Get input I4 state - GPIO34                   |
# +---------------------------------------------------------+


# You must change these
HOST='192.168.20.72'
# Your board comes with a default unique password, please set that here.
# if you want to change it you must change the secrets.h file and upload
# the firmware to the ESP32.
PASSWORD = ""

# Basically Port B of the MCP23017 is relays 1-8 Port A is 9-15
# Why, well it was for ease of board layout.
# So it's easier to just access them as relay[n] and have it 
# turn on the correct relay
relays = [8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7]
inputs = [-1,33,32,35,34]
# Direction relay is tied to D18 of ESP32
dir_relay = '18'

import socket
import time
import sys

def tx(s):
    sock.send(bytes(s, 'UTF-8'))

def rx():
    sock.settimeout(3.0)
    s = sock.recv(1024)
    sock.settimeout(None)
    print(s)
    return(s.decode("UTF-8"))

# sends the 'TEST' command to the relay board
# the board will cycle through all the relays
def test_relays():
    tx('TE\n')


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, 8032))
while True:
    while True:
        time.sleep(1)
        if rx() == 'Password?':
            #print("rx == password")
            tx(PASSWORD+'\n')
            print("Sent password...")
            break
        else:
            tx('\n')
    print("Password successful...\n")
    break

inpt = ""

print("Press H for help")
while True:
    inpt = input('Send command to board [ H T S O C In RCnn ROnn RTnn, Q ]... ')
    s = inpt.upper()
    print(s)
    if s == 'H':
        print("Enter one of the following commands")
        print("H     - Print this help.")
        print("T     - Test relays by closing all and then opening all.")
        print("S     - Show status of all relays.")
        print("O     - Open all relays.")
        print("C     - Close all relays.")
        print("In    - Show status of Input n, n is 1 to 4 ")
        print("RCnn  - Close relay nn, nn is 01 to 16.")
        print("ROnn  - Open relay nn, nn is 01 to 16.")
        print("RTnn  - Toggle relay nn, nn is 01 to 16.")
        print("Q     - Quit.")

    elif s == 'T':
        test_relays()
    elif s == 'S':
        tx('ST\n')
        print((rx()))
        print((rx()))
    elif s == 'O':
        tx('OA\n')
    elif s == 'C':
        tx('CL\n')
    elif s == 'Q':
        tx('~')
        print("Exiting...")
        sys.exit()
    else:    
        if s[:1] == 'I':
            n = str(inputs[int(s[1:2])])
            print(n)
            tx('I' + n + '\n')
            print(rx())
        else:
            if len(s) == 4:
                # get the correct relay
                r = s[2:4]
                cmd = s[:2] + str(relays[int(r)-1])
                tx(cmd + '\n')
            else:
                print("\nCommand length error, too short...\n")

      
