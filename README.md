# Description #

This projects aims to provide a reference implemention for OpenDroneID in
C language. It contains:

## odid library ##

The library generatess and parses OpenDrone ID message from/to a generic
structure which can be used in other applications. It also provides an
interface to dump a JSON description of the drone structure.

## wifi_controller ##

The wifi controller is a sample program to be used on a drone. It emits
OpenDroneID WiFi messages in regular intervals. The location and movement
information is taken from a GPS device which is connected using gpsd.

## wifi_drone_scanner ##

The wifi drone scanner receives OpenDrone ID WiFi messages, parses them and
writes a list of seen Drones on the command line.

# License #

All code is licensed under the MIT license. Please see the file 'COPYING'.

# Author #

This software has been written by Simon Wunderlich <sw@simonwunderlich.de>.
It was sponsored by Doodle Labs ( https://doodlelabs.com )

For any questions, please contact:
	Simon Wunderlich <sw@simonwunderlich.de>

# TODO #

 * Clean up Makefiles/build process
 * Implement gpsd data parsing
 * Implement building message packs and other odid messages
 * Implement parsing messages
 * Implement NAN sending with hostapd
 * wifi drone scanner
