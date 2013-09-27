/**
 ******************************************************************************
 *
 * @file       NazaLed.ino
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements NAZA LED detection using built-in ADC.
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


// !!! For using this, you have to solder a little bit on the MinimOSD and NAZA LED module !!!


#include "NazaLed.h"


void naza_led_init(void)
{
	analogReference(INTERNAL);			// INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
}


/*----------------------------------------------------------------------------------------------------------------------------------------
Ziel: Erkennung folgender Modi:
				(für alle gilt: rot darf blinken)
	MAN	Manual Mode	kein gelb kein grün
	ATT	ATTI Mode	gelb oder gelb gelb kein grün
	GPS	GPS Mode	grün oder grün grün kein gelb
	IOC	IOC Mode	gelb grün oder gelb grün grün
	FS 	FailSafe	gelb gelb gelb gelb gelb gelb gelb gelb gelb gelb gelb ... kein grün
TODO:	'record home point'	ca.3000ms grün grün grün grün grün grün grün grün grün ... kein gelb

Umsetzung:
	- Wenn 3 Sekunden lang kein gelb oder grün		--> MAN
	- Wenn gelb oder grün steigende Flanke erkannt, dann
	  Timer starten und steigende gelb und grün Flanken zählen.
	  Nach 1 Sekunde Anzahl der Flanken auswerten:
		gelb = 1 oder 2 und grün = 0			--> ATT
		grün = 1 oder 2 und gelb = 0			--> GPS
		gelb = 1 und grün = 1 oder 2			--> IOC
		gelb > 3					--> FS
		grün > 3 und ca. 3000ms lang			--> 'record home point' evtl. erkannt
								    Achtung, kann aber auch 'Aircraft moved, sensor bias too big' sein
								    --> über extra Timer unterscheiden!
		Timer, Counter etc. auf Null, neue Auswertung
----------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t get_mode(char c)
{
	static unsigned long yg_trigger = 0;
	static unsigned long yg_timeout = 0;
	static uint8_t y_on = false;
	static uint8_t g_on = false;
	static uint8_t y_cnt = 0;
	static uint8_t g_cnt = 0;
	static uint8_t home = LED_MODE_UNKNOWN;
	static uint8_t mode = LED_MODE_UNKNOWN;
	
	unsigned long curr_ms = millis();
	
	switch (c) {
		case NAZA_CHAR_RED:
		break;
		case NAZA_CHAR_YELLOW:
			if (!yg_trigger) yg_trigger = curr_ms;
			if (!y_on) {
				yg_timeout = curr_ms;
				y_cnt++;
				y_on = true;
			}
		break;
		case NAZA_CHAR_GREEN:
			if (!yg_trigger) yg_trigger = curr_ms;
			if (!g_on) {
				yg_timeout = curr_ms;
				g_cnt++;
				g_on = true;
			}
		break;
		case NAZA_CHAR_OFF:
			y_on = false;
			g_on = false;
			
			if (yg_timeout && curr_ms - yg_timeout > LED_TIMEOUT) {
				mode = LED_MODE_MAN;
				yg_timeout = 0;
				y_cnt = 0;
				g_cnt = 0;
			}
	
			if (yg_trigger && curr_ms - yg_trigger > LED_MODE_CHECK) {
				if (y_cnt >= 1 && y_cnt <= 2 && g_cnt == 0)	mode = LED_MODE_ATT;
				if (y_cnt == 0 && g_cnt >= 1 && g_cnt <= 2)	mode = LED_MODE_GPS;
			// TODO	if (y_cnt == 1 && g_cnt >= 1 && g_cnt <= 2)	mode = LED_MODE_IOC;
				if (y_cnt >  3 && g_cnt == 0)			mode = LED_MODE_FS;
				if (y_cnt == 0 && g_cnt >  3) {
					if (yg_trigger && curr_ms - yg_trigger > LED_HOME_CHECK) {
						home = LED_MODE_GOT_HOME_POINT;
						yg_trigger = 0;
						y_cnt = 0;
						g_cnt = 0;
					}
				} else {
					yg_trigger = 0;
					y_cnt = 0;
					g_cnt = 0;
				}
			}
		break;
	}
	
	return (home | mode);
}


char naza_led_char(void)
{
	uint16_t v = analogRead(NAZA_LED_PIN);
	// order depending on voltage divider type and resulting thresholds !!!
	if (v > NAZA_THRESHOLD_YELLOW) return NAZA_CHAR_YELLOW;
	if (v > NAZA_THRESHOLD_RED)    return NAZA_CHAR_RED;
	if (v > NAZA_THRESHOLD_GREEN)  return NAZA_CHAR_GREEN;
	return NAZA_CHAR_OFF;
}


void naza_led_show(int first_col, int first_line)
{
#ifdef LED_FIND_VALUES
	osd.setPanel(first_col-1, first_line);
	osd.openPanel();
	osd.printf("%4u", analogRead(NAZA_LED_PIN));
	osd.closePanel();
#else
	static uint8_t old_mode = LED_MODE_UNKNOWN;
	static char* mode_str="---";
	char led = naza_led_char();
	uint8_t mode = get_mode(led);
	
	if (old_mode != mode) {
		old_mode = mode;
		mode = mode & ~LED_MODE_GOT_HOME_POINT;
		     if (mode == LED_MODE_UNKNOWN)	mode_str = "---";
		else if (mode == LED_MODE_MAN)		mode_str = "man";
		else if (mode == LED_MODE_ATT)		mode_str = "att";
		else if (mode == LED_MODE_GPS)		mode_str = "gps";
		else if (mode == LED_MODE_IOC)		mode_str = "ioc";
		else if (mode == LED_MODE_FS)		mode_str = "f-s";
	}
	osd.setPanel(first_col, first_line);
	osd.openPanel();
	osd.printf("%s%c", mode_str, led == NAZA_CHAR_OFF && old_mode & LED_MODE_GOT_HOME_POINT ? NAZA_CHAR_GOT_HOME : led);
	osd.closePanel();
#endif
}
