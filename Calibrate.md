# How to calibrate voltage/current measurement #


If you have soldered the screenswitch and throttle cables doing the insane soldering you can use the screenswitch and throttle for calibrating the voltage/current measurement.

You have to compile a special hex files to calibrate the voltage, the current offset and the current itself.

To generate this hex file uncomment the following #define and compile the code.

// to generate the calibration software for using with throttle and screenswitch

#define SETUP\_TS

After that, load this hex for calibrating the voltage, the current offset and the current itself.

Don't forget to load a hex file compiled with the option disabled after calibration is ready.


---


---


If you don't have soldered the screenswitch and throttle cables, you need another way to calibrate the voltage/current measurement.

You need to solder one additional cable at the bottom of the minNAZAOSD as shown here:

![http://minnazaosd.googlecode.com/svn/wiki/images/004_minNAZAOSD_ConnectionsBottom.jpg](http://minnazaosd.googlecode.com/svn/wiki/images/004_minNAZAOSD_ConnectionsBottom.jpg)

Connect this to the signal pin of a free channel at your RC-RX.


---


Then you need 3 different hex files to calibrate the voltage, the current offset and the current itself.

To generate these 3 hex files go to the file OSD\_Config.h and uncomment the following #defines step by step but only one per step to generate the 3 different hex files.

// to generate the three needed calibration software versions

//#define SETUP\_VOLT\_DIFF\_RATIO

//#define SETUP\_AMP\_OFFSET

//#define SETUP\_AMP\_PER\_VOLT

After that, load the needed hex for calibrating the voltage, the current offset and the current itself step by step.

Don't forget to load a hex file compiled with all three options disabled after calibration is ready.