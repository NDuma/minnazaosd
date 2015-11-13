# Introduction #

minNAZAOSD runs on the cheap (about 20$) MinimOSD hardware and realizes an OSD for the NAZA M V1/V2/Lite flight-controller with NAZA GPS.

This project is GNU GPL v3 and using it is at your own risk.

It does not need an additional arduino board as other non-DJI OSDs for NAZA.

One MinimOSD hardware with the ATmega 328p and Max 7456 chip is enough. If you like current measurement, you need a current sensor of course.


---


The only disadvantage: You have to solder and you have to know what you are doing.

Depending on the stage you want to use you need up to some very good soldering skills.


---


Only do this if you have some good soldering skills, especially soldering directly at the ATmega 328p.

Also only do this if you have the knowledge of what you are doing.

If you do not have both, please use the DJI iOSD.

Bad soldering or less knowledge can risk your aerial vehicle and even lives!


---


It's based on my other OSD project for OpenPilot, the minOPOSD: http://code.google.com/p/minoposd/

and on the ArduCamOSD code: http://code.google.com/p/arducam-osd/


---


An erroneous PDF from some other guy exists in the depth of the web, but caution:

It shows wrong wiring. So don't use it, use your own brain, or use the DJI iOSD!


---


By the way:

When using it with only a GPS connected (UBX tested, NMEA not tested) it's usable as a cheap standalone OSD independent of the flight-controller or kind of UAV...

Attach voltage/current sensor and/or RSSI and get these features also...

Got it?


---


Have fun