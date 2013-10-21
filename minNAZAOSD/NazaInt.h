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


// uncomment to use sum-signal instead of single channels
//#define USE_SUM_SIGNAL

// set your channel layout
#define SUM_CH_THROTTLE			3					// throttle channel
#define SUM_CH_SCREENSWITCH		8					// screenswitch channel

// choose your set for sum-signal
#if 0
#define PWM_PIN_SUM			7					// use pin 11 = PD7 = PCINT23
#define PIN_READ_SUM			(PIND & 0b10000000)			// faster than digitalRead
#define CALL_CHECK_SUM			(port == 2 && mask & 0b10000000)	// call check macro
#else
#define PWM_PIN_SUM			8					// use pin 12 = PB0 = PCINT0
#define PIN_READ_SUM			(PINB & 0b00000001)			// faster than digitalRead
#define CALL_CHECK_SUM			(port == 0 && mask & 0b00000001)	// call check macro
#endif


#define PWM_PIN_GIMBAL_F2		4					// use pin  2 = PD4 = PCINT20	for gimbal F2
#define PWM_PIN_THROTTLE		7					// use pin 11 = PD7 = PCINT23	for throttle
#define PWM_PIN_SCREENSWITCH		8					// use pin 12 = PB0 = PCINT0	for screen switching
#define PWM_PIN_GIMBAL_F1		9					// use pin 13 = PB1 = PCINT1	for gimbal F1

#define PIN_READ_GIMBAL_F2		(PIND & 0b00010000)			// faster than digitalRead
#define PIN_READ_THROTTLE		(PIND & 0b10000000)			// faster than digitalRead
#define PIN_READ_SCREENSWITCH		(PINB & 0b00000001)			// faster than digitalRead
#define PIN_READ_GIMBAL_F1		(PINB & 0b00000010)			// faster than digitalRead

#define CALL_CHECK_GIMBAL_F2		(port == 2 && mask & 0b00010000)	// call check macro
#define CALL_CHECK_THROTTLE		(port == 2 && mask & 0b10000000)	// call check macro
#define CALL_CHECK_SCREENSWITCH		(port == 0 && mask & 0b00000001)	// call check macro
#define CALL_CHECK_GIMBAL_F1		(port == 0 && mask & 0b00000010)	// call check macro


void naza_int_init(void);

int16_t naza_throttle_get(void);
int16_t naza_screenswitch_get(void);

int16_t naza_roll_get(void);
int16_t naza_pitch_get(void);


#if defined (SETUP_TS) || defined (NAZA_INT_DEBUG)
int16_t naza_throttle_us_get(void);
int16_t naza_roll_us_get(void);
int16_t naza_pitch_us_get(void);
#endif // NAZA_INT_DEBUG

#endif /* NAZA_INT_H_ */
