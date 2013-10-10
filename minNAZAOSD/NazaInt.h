/**
 ******************************************************************************
 *
 * @file       NazaInt.h
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements RC-RX channel detection and NAZA artificial horizon 
 *             using interrupts.
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


// !!! For using this, you have to solder a little bit on the MinimOSD !!!


#ifndef NAZA_INT_H_
#define NAZA_INT_H_


//#define NAZA_INT_DEBUG


#define THROTTLE_LOWEST			1100			// trim throttle lowest here
#define THROTTLE_HIGHEST		1900			// trim throttle highest here

#define GIMBAL_ROLL_MIDDLE		1500			// trim the horizon roll middle here
#define GIMBAL_ROLL_FACTOR		0.1			// trim the horizon roll alignment here

#define GIMBAL_PITCH_MIDDLE		1500			// trim the horizon pitch middle here
#define GIMBAL_PITCH_FACTOR		0.1			// trim the horizon pitch alignment here




#define PWM_PIN_THROTTLE		7
#define PWM_PIN_SCREENSWITCH		8
#define PWM_PIN_GIMBAL_F1		9
#define PWM_PIN_GIMBAL_F2		10


#define PIN_READ_THROTTLE		(PIND & 0b10000000)	// faster than digitalRead
#define PIN_READ_SCREENSWITCH		(PINB & 0b00000001)	// faster than digitalRead
#define PIN_READ_GIMBAL_F1		(PINB & 0b00000010)	// faster than digitalRead
#define PIN_READ_GIMBAL_F2		(PINB & 0b00000100)	// faster than digitalRead


void naza_int_init(void);

int16_t naza_throttle_get(void);
int16_t naza_screenswitch_get(void);

int16_t naza_roll_get(void);
int16_t naza_pitch_get(void);


#ifdef NAZA_INT_DEBUG
int16_t naza_throttle_us_get(void);
int16_t naza_roll_us_get(void);
int16_t naza_pitch_us_get(void);
#endif // NAZA_INT_DEBUG

#endif /* NAZA_INT_H_ */
