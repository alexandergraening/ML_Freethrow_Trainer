#!/bin/bash
#
#
# This script removes the entries for two MAC addresses
#
# Edit this script to replace the MAC addresses below with 
# those of your SensorTile devices
#
# This script also restarts the bluetooth interface
#

#
# Remove addresses
#

address1=C0:83:24:31:2E:48
address2=C0:83:30:30:50:4D
bluetoothctl << eof
remove $address1
remove $address2
exit
eof

#
# Restart the bluetooth controller
#

sudo /etc/init.d/bluetooth restart
sleep 1

