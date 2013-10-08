/**
 ******************************************************************************
 *
 * @file       NAZATalk.ino
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements a subset of NAZA communication.
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/> or write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#ifdef PROTOCOL_NAZATALK

#include "NAZATalk.h"


#ifdef GPS_PROTOCOL_DJI
// see http://www.rcgroups.com/forums/showthread.php?t=1995704
#include "GPS_DJI.h"
#endif


#ifdef GPS_PROTOCOL_UBX
// see http://openpilot.org
#include "GPS_UBX.h"
#endif


#ifdef GPS_PROTOCOL_NMEA
// see http://arduiniana.org/libraries/tinygps/
#include <TinyGPS.h>
TinyGPS gps;
#endif


//----------------------------------------------------------------------------------
//
//  stage 1	easy soldering		solder the GPS puck cable and configure 1 cable to NAZA for +5V and GND
//	GPS lat/lon				ok
//	distance to home			ok
//	altitude				ok
//	groundspeed				ok
//	course					ok
//	direction to home			ok
//	sat count				ok
//	sat fix					ok
//	climb rate				ok
//	radar					ok
//	time display				ok
//
//
//
//  stage 2a	advanced soldering	solder 1 cable at Atmel 328P pin and 3 resistors at NAZA LED cable adapter
//	active LED symbol			ok	3 resistors + 1 ADC
//
//  stage 2b	advanced soldering	solder 2 cables at Atmel 328P pin and up to 4 resistors and use voltage/current sensor
//	battery voltage				ok	2 resistors + 1 ADC
//	battery current				ok	2 resistors + 1 ADC
//
//  stage 2c	advanced soldering	solder 1 cable at Atmel 328P pin and 2 resistors
//	analog RSSI				ok	2 resistors + 1 ADC
//
//  stage 2d	easy soldering		solder 1 cable at MinimOSD PAL pad (but only for interest for users of Simprop GigaScan RC-RX)
//	packet RSSI				ok	1 INT
//
//
//
//  stage 3a	advanced soldering	solder 2 cables at Atmel 328P pin and configure 1 double PWM cable to NAZA
//	horizon + pitch/roll			TOTEST	2 PCINT
//
//  stage 3b	advanced soldering	solder 2 cables at Atmel 328P pin and configure 2 PWM cables to RC-RX
//	throttle				TOTEST	1 PCINT
//	screen switching			TOTEST	1 PCINT
//
//----------------------------------------------------------------------------------
int nazatalk_read(void) {
	static uint8_t gps_seen = 0;
	static uint8_t crlf_count = 0;
	int ret = 0;
	
	// TODO implement some of the following
	// fake the info till available
	if (!osd_armed) {
		osd_armed	= 2;
		osd_mode	= 0;
		
		osd_roll	= (int16_t) 0;
		osd_pitch	= (int16_t) 0;
		osd_throttle	= 0;
		ch_toggle	= 6;
		osd_chan5_raw	= 1100;
		osd_chan6_raw	= 1100;
		osd_chan7_raw	= 1100;
		osd_chan8_raw	= 1100;
	}
	
	// grabbing data
	while (Serial.available() > 0) {
		uint8_t c = Serial.read();
	
#ifdef NAZA_LED
		naza_led_show(NAZA_LED_POS_X, NAZA_LED_POS_Y);
#endif
		
		// needed for MinimOSD char set upload
		if (!gps_seen && millis() < 20000 && millis() > 5000) {
			if (c == '\n' || c == '\r') {
				crlf_count++;
			} else {
				crlf_count = 0;
			}
			if (crlf_count == 3) {
				uploadFont();
			}
		}
		
#ifdef GPS_PROTOCOL_DJI
		if (parse_dji(c) == PARSER_COMPLETE_SET) {
			gps_seen = 1;
			osd_fix_type		= get_dji_status();
			osd_satellites_visible	= get_dji_satellites();
			osd_lat			= get_dji_latitude();
			osd_lon			= get_dji_longitude();
			osd_alt			= get_dji_altitude();
			osd_heading 		= get_dji_heading();
			osd_groundspeed 	= get_dji_groundspeed();
			osd_climb		= -1.0 * get_dji_down();
		}
#endif
		
#ifdef GPS_PROTOCOL_UBX
		if (parse_ubx(c) == PARSER_COMPLETE_SET) {
			gps_seen = 1;
			osd_fix_type		= get_ubx_status();
			osd_satellites_visible	= get_ubx_satellites();
			osd_lat			= get_ubx_latitude();
			osd_lon			= get_ubx_longitude();
			osd_alt			= get_ubx_altitude();
			osd_heading 		= get_ubx_heading();
			osd_groundspeed 	= get_ubx_groundspeed();
			osd_climb		= -1.0 * get_ubx_down();
		}
#endif

#ifdef GPS_PROTOCOL_NMEA
		long lat, lon;
		unsigned long fix_age;
	
		if (gps.encode(c)) {		// process new gps info
			gps_seen = 1;
			
			gps.get_position(&lat, &lon, &fix_age);
			osd_lat			= lat / 1000000.0;
			osd_lon			= lon / 1000000.0;
			
			// osd_fix_type should be  0-1=no fix, 2=2D, 3=3D
			if (fix_age == TinyGPS::GPS_INVALID_AGE)		// TODO	temporary solution, optionally use info from GSA message (but not implemented in TinyGPS lib yet)
				osd_fix_type = 0;
			else if (fix_age > 5000)
				osd_fix_type = 2;
			else
				osd_fix_type = 3;
			
			osd_satellites_visible	= gps.satellites();
			osd_heading 		= gps.f_course();
			osd_alt			= gps.f_altitude();
			osd_groundspeed 	= gps.f_speed_mps();
			//osd_climb		= 0;				// TODO not implemented yet
		}
#endif
	}
	
#ifdef NAZA_LED
	naza_led_show(NAZA_LED_POS_X, NAZA_LED_POS_Y);
#endif
	
#ifdef NAZA_INT	// TOTEST
	osd_roll	= naza_roll_get();
	osd_pitch	= naza_pitch_get();
	osd_throttle	= naza_throttle_get();
	ch_toggle	= 6;
	osd_chan6_raw	= naza_screenswitch_get();
#endif
	
        return ret;
}


int nazatalk_state(void)
{
	return 1;
}


#endif // PROTOCOL_NAZATALK
