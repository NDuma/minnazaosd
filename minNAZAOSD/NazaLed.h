/**
 ******************************************************************************
 *
 * @file       NazaLed.h
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


#ifndef NAZA_LED_H_
#define NAZA_LED_H_


//#define LED_DEBUG
//#define LED_FIND_VALUES


#define LED_TIMEOUT		3000
#define LED_MODE_CHECK		1000
#define LED_HOME_CHECK		2750

#define LED_HOME_CHECK_MIN_CNT	25

#define LED_SAMPLE_TIME		5
#define LED_SAMPLES		2
#define LED_REDRAW_TIME		100

#define LED_MODE_UNKNOWN	0x00
#define LED_MODE_GOT_HOME_POINT	0x01
#define LED_MODE_MAN		0x02
#define LED_MODE_ATT		0x04
#define LED_MODE_GPS		0x08
#define LED_MODE_IOC		0x10
#define LED_MODE_FS		0x20


#define NAZA_LED_PIN			2			// ADC2 is pin 25


#define REF_VOLTAGE			1.1			// INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328


#define NAZA_LED_POS_X			(panFMod_XY[0][panel] + 4)
#define NAZA_LED_POS_Y			(panFMod_XY[1][panel])

#define NAZA_THRESHOLD_YELLOW		900
#define NAZA_THRESHOLD_RED		550
#define NAZA_THRESHOLD_GREEN		250

#define NAZA_CHAR_OFF			0x00
#define NAZA_CHAR_GOT_HOME		0xF7
#define NAZA_CHAR_YELLOW		0xEB
#define NAZA_CHAR_RED			0xEA
#define NAZA_CHAR_GREEN			0xEC


void naza_led_init(void);
char naza_led_char(void);
void naza_led_show(int first_col, int first_line);


#endif /* NAZA_LED_H_ */
