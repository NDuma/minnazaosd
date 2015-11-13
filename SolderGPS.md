# How to connect the GPS #


---


Remember: This project is GNU GPL v3 and using it is at your own risk.


---


With the findings of a guy at rcgroups, user pawelsky, it is now easy to connect the NAZA GPS to the MinimOSD.

No soldering at the NAZA GPS is necessary anymore, only an easy adaptor is needed, and some code of course.


See his findings here:

http://www.rcgroups.com/forums/showthread.php?t=1995704


I adapted his findings to my GPS\_DJI parser code and you can use it by commenting/uncommenting as in the following in the OSD\_Config.h file:

#define GPS\_PROTOCOL\_DJI

//#define GPS\_PROTOCOL\_UBX


You can choose between GPS calculated heading and mag heading by commenting/uncommenting in the OSD\_DJI.h file:

For GPS calculated heading:

//#define DJI\_HEADING\_FROM\_MAG

For mag heading:

#define DJI\_HEADING\_FROM\_MAG

But the mag heading currently is not correctly working and needs some further testing!


---


If you are using the NAZA-M or NAZA-M Lite then connect the adaptor directly to the NAZA Main Controller EXP port.

If you are using the NAZA-M V2 then connect the adaptor to the NAZA PMU V2 GPS port.


---


You can solder the adaptor like this:

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_001.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_001.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_002.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_002.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_003.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_003.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_004.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_004.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_005.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_005.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_006.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/GPSAdaptor_006.jpg)


But don't forget to insulate the 4 contacts using e.g. PlastiDip and heatshrink the adaptor after that!


---




---




---


For legacy reasons here is the description how to solder directly at the NAZA GPS.

You don't need this with the new version described above!

If you like to try this anyway, then you have to commenting/uncommenting as in the following in the OSD\_Config.h file:

//#define GPS\_PROTOCOL\_DJI

#define GPS\_PROTOCOL\_UBX


---


For using the NAZA GPS you have to solder one wire to it.

There are some descriptions and videos on the web how to do it, so here I only show some pictures as an abstract.

Doing this manipulations will void your warranty for your GPS!


---


First open the GPS using a Torx T5 bit:

![http://minnazaosd.googlecode.com/svn/wiki/images/010_GPS_opened.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/010_GPS_opened.jpg)


---


Then carefully solder the cable:

![http://minnazaosd.googlecode.com/svn/wiki/images/011_GPS_soldered.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/011_GPS_soldered.jpg)

You need a little notch to let the cable out of the GPS.

After that close the GPS.


---


For connecting the GPS data wire to the OSD and to power the OSD you have to solder an adaptor cable as shown here:

![http://minnazaosd.googlecode.com/svn/wiki/images/012_GPS_adaptor_cable.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/012_GPS_adaptor_cable.jpg)


---


Connection is done like shown in the following pictures:

(shown without the LED adaptor and minNAZAOSD soldering, described in the next chapter, for simplicity)

![http://minnazaosd.googlecode.com/svn/wiki/images/013_GPS_adaptor_connected_001.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/013_GPS_adaptor_connected_001.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/014_GPS_adaptor_connected_002.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/014_GPS_adaptor_connected_002.jpg)

![http://minnazaosd.googlecode.com/svn/wiki/images/015_GPS_adaptor_connected_003.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/015_GPS_adaptor_connected_003.jpg)

As you can see in this pictures, the minNAZAOSD is powered through the NAZA +5V and GND (red and brown cables) and the GPS data is feeded by the third cable.


---


Connection overview:

![http://minnazaosd.googlecode.com/svn/wiki/images/016_overview.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/016_overview.jpg)