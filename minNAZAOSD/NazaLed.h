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


//#define LED_FIND_VALUES


// TODO use ADC2 later, currently analog RSSI pin ADC1 is used
//#define NAZA_LED_PIN			2			// A2 is pin 25
#define NAZA_LED_PIN			1			// A1 is pin 24
								// Any 'free' analog input will do -- A0-5 are available, 
								// which are pins 23-28 on the ATmega328p

#define REF_VOLTAGE			1.1			// INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328


//#define NAZA_LED_POS_X			13
//#define NAZA_LED_POS_Y			0
#define NAZA_LED_POS_X			(panFMod_XY[0][panel] + 3)
#define NAZA_LED_POS_Y			(panFMod_XY[1][panel])


#define NAZA_THRESHOLD_YELLOW		110			// TODO choose the threshold
#define NAZA_THRESHOLD_RED		 70			// TODO choose the threshold
#define NAZA_THRESHOLD_GREEN		 30			// TODO choose the threshold

#define NAZA_CHAR_YELLOW		0xEB			// TODO choose the char
#define NAZA_CHAR_RED			0xEA			// TODO choose the char
#define NAZA_CHAR_GREEN			0xEC			// TODO choose the char
#define NAZA_CHAR_OFF			0x00			// TODO choose the char


void naza_led_init(void);
char naza_led_char(void);
void naza_led_show(int first_col, int first_line);


#endif /* NAZA_LED_H_ */
