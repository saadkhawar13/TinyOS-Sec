#! /bin/bash
# script to compile and install software in a network of telosb motes.
# Usage pnetwork_install.sh

# Program the PAN coordinator on /dev/ttyUSB0 with address 0 
./pmote.sh i c 0 0

# Program 2 end devices on /dev/ttyUSB2 and /dev/ttyUSB3, with addresses 10 and 11, depth 2,
# to associate with the router (1) and at predefined positions (x,y)
./pmote.sh i e 1 10 1 0 120 150

