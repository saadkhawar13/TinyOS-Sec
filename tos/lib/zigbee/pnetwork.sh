#! /bin/bash
# script to compile and install software in a network of telosb motes.
# Usage pnetwork_install.sh

#./pmote.sh <install type> <device type> <USB port number> <device address> [<depth> <address parent> <xPos> <yPos>]

# Program the PAN coordinator on /dev/ttyUSB0 with address 0 
./pmote.sh i c 0 0

# Program 2 end devices on /dev/ttyUSB1 with addresses 10, depth 2,
# to associate with the router (1) and at predefined positions (x,y)
./pmote.sh i e 1 10 1 0 120 150

