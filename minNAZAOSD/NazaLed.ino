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


char naza_led_char(void)
{
	uint16_t v = analogRead(NAZA_LED_PIN);
	
	if (v > NAZA_THRESHOLD_YELLOW)	return NAZA_CHAR_YELLOW;
	if (v > NAZA_THRESHOLD_RED)	return NAZA_CHAR_RED;
	if (v > NAZA_THRESHOLD_GREEN)	return NAZA_CHAR_GREEN;
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
	osd.setPanel(first_col, first_line);
	osd.openPanel();
	osd.printf("%c", naza_led_char());
	osd.closePanel();
#endif
}
