# How to load the char file #


As the config tool uses 57k6 baud for char file upload you need to load a firmware which also uses 57k6 baud.

The firmware baud rate depends on the GPS version you like to use.

The GPS\_PROTOCOL\_DJI uses 115k2 baud

The GPS\_PROTOCOL\_UBX uses 57k6 baud.

So, prior to char set upload, build and upload the firmware for the UBX GPS configuring the defines as in the following:

//#define GPS\_PROTOCOL\_DJI

#define GPS\_PROTOCOL\_UBX

After that firmware upload, do the char file upload.

How to upload the char file is explained here: http://code.google.com/p/arducam-osd/wiki/Cfg_Update_CharSet

Get the special minNAZAOSD MCM file from my code section trunk/configs

After char file upload, choose the GPS protocol you like e.g. the DJI protocol like:

#define GPS\_PROTOCOL\_DJI

//#define GPS\_PROTOCOL\_UBX

and build and upload this firmware.