/**
 ******************************************************************************
 *
 * @file       NazaInt.ino
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


#include "NazaInt.h"


//#define NAZA_INT_MINIMAL


// see http://playground.arduino.cc/Main/PcInt

#include "pins_arduino.h"

/*
 * an extension to the interrupt support for arduino.
 * add pin change interrupts to the external interrupts, giving a way
 * for users to have interrupts drive off of any pin.
 * Refer to avr-gcc header files, arduino source and atmega datasheet.
 */

/*
 * Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
 * The PCINT corresponding to the pin must be enabled and masked, and
 * an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
 * must use some logic to actually implement a per-pin interrupt service.
 */

/* Pin to interrupt map:
 * D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
 * D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
 * A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
 */

volatile uint8_t *port_to_pcmask[] = {
  &PCMSK0,
  &PCMSK1,
  &PCMSK2
};

volatile static uint8_t PCintLast[3];

#ifndef NAZA_INT_MINIMAL
static int PCintMode[24];
typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr PCintFunc[24] = { NULL };
#endif

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
 void PCattachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *pcmask;

  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  }
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }

#ifndef NAZA_INT_MINIMAL
  uint8_t slot;
// -- Fix by Baziki. In the original sources it was a little bug, which cause analog ports to work incorrectly.
  if (port == 1) {
     slot = port * 8 + (pin - 14);
  }
  else {
     slot = port * 8 + (pin % 8);
  }
// --Fix end
  PCintMode[slot] = mode;
  PCintFunc[slot] = userFunc;
#endif

  // set the mask
  *pcmask |= bit;
  // enable the interrupt
  PCICR |= 0x01 << port;
}

#if 0	// currently unused
void PCdetachInterrupt(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *pcmask;

  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  }
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }

  // disable the mask.
  *pcmask &= ~bit;
  // if that's the last one, disable the interrupt.
  if (*pcmask == 0) {
    PCICR &= ~(0x01 << port);
  }
}
#endif

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
static void PCint(uint8_t port) {
  uint8_t curr;
  uint8_t mask;

  // get the pin states for the indicated port.
  curr = *portInputRegister(port+2);
  mask = curr ^ PCintLast[port];
  PCintLast[port] = curr;
  // mask the pins that have changed. screen out non pcint pins.
  if ((mask &= *port_to_pcmask[port]) == 0) {
    return;
  }
#ifdef NAZA_INT_MINIMAL
  // only check used interrupts of the ports
  if (CALL_CHECK_GIMBAL_F1)	int_gimbal_f1();
  if (CALL_CHECK_GIMBAL_F2)	int_gimbal_f2();
  if (CALL_CHECK_THROTTLE)	int_throttle();
  if (CALL_CHECK_SCREENSWITCH)	int_screenswitch();
#else
  uint8_t bit;
  uint8_t pin;
  // mask is pcint pins that have changed.
  for (uint8_t i=0; i < 8; i++) {
    bit = 0x01 << i;
    if (bit & mask) {
      pin = port * 8 + i;
      // Trigger interrupt if mode is CHANGE, or if mode is RISING and
      // the bit is currently high, or if mode is FALLING and bit is low.
      if ((PCintMode[pin] == CHANGE
          || ((PCintMode[pin] == RISING) && (curr & bit))
          || ((PCintMode[pin] == FALLING) && !(curr & bit)))
          && (PCintFunc[pin] != NULL)) {
        PCintFunc[pin]();
      }
    }
  }
#endif
}

SIGNAL(PCINT0_vect) {
  PCint(0);
}
SIGNAL(PCINT1_vect) {
  PCint(1);
}
SIGNAL(PCINT2_vect) {
  PCint(2);
}




volatile long throttle_start;				// start time
volatile long throttle_pulse		= 1100;		// pulse duration

volatile long screenswitch_start;			// start time
volatile long screenswitch_pulse	= 1100;		// pulse duration

volatile long gimbal_f1_start;				// start time
volatile long gimbal_f1_pulse		= 1500;		// pulse duration

volatile long gimbal_f2_start;				// start time
volatile long gimbal_f2_pulse		= 1500;		// pulse duration


void int_throttle(void) {
	if (PIN_READ_THROTTLE)
		throttle_start = micros();				// positive edge: start
	else
		throttle_pulse = micros() - throttle_start;		// negative edge: calculate pulsewidth
}

void int_screenswitch(void) {
	if (PIN_READ_SCREENSWITCH)
		screenswitch_start = micros();				// positive edge: start
	else
		screenswitch_pulse = micros() - screenswitch_start;	// negative edge: calculate pulsewidth
}

void int_gimbal_f1(void) {
	if (PIN_READ_GIMBAL_F1)
		gimbal_f1_start = micros();				// positive edge: start
	else
		gimbal_f1_pulse = micros() - gimbal_f1_start;		// negative edge: calculate pulsewidth
}

void int_gimbal_f2(void) {
	if (PIN_READ_GIMBAL_F2)
		gimbal_f2_start = micros();				// positive edge: start
	else
		gimbal_f2_pulse = micros() - gimbal_f2_start;		// negative edge: calculate pulsewidth
}


void naza_int_init(void)
{
	pinMode(PWM_PIN_THROTTLE, INPUT);
	PCattachInterrupt(PWM_PIN_THROTTLE, int_throttle, CHANGE);
	pinMode(PWM_PIN_SCREENSWITCH, INPUT);
	PCattachInterrupt(PWM_PIN_SCREENSWITCH, int_screenswitch, CHANGE);
	pinMode(PWM_PIN_GIMBAL_F1, INPUT);
	PCattachInterrupt(PWM_PIN_GIMBAL_F1, int_gimbal_f1, CHANGE);
	pinMode(PWM_PIN_GIMBAL_F2, INPUT);
	PCattachInterrupt(PWM_PIN_GIMBAL_F2, int_gimbal_f2, CHANGE);
}


// throttle in percent
int16_t naza_throttle_get(void)
{
	return constrain((int16_t)((float)(throttle_pulse - THROTTLE_LOWEST) / (float)(THROTTLE_HIGHEST - THROTTLE_LOWEST) * 100.0 + 0.5), 0, 100);
}


// screenswitch in micro seconds
int16_t naza_screenswitch_get(void)
{
	return (int16_t) screenswitch_pulse;
}


// roll angle in degree
int16_t naza_roll_get(void)
{
	return (int16_t) (((int16_t) gimbal_f1_pulse - GIMBAL_ROLL_MIDDLE) * GIMBAL_ROLL_FACTOR);
}


// pitch angle in degree
int16_t naza_pitch_get(void)
{
	return (int16_t) (((int16_t) gimbal_f2_pulse - GIMBAL_PITCH_MIDDLE) * GIMBAL_PITCH_FACTOR);
}


#if defined (SETUP_TS) || defined (NAZA_INT_DEBUG)

// throttle  in micro seconds
int16_t naza_throttle_us_get(void)
{
	return (int16_t) throttle_pulse;
}


// roll in micro seconds
int16_t naza_roll_us_get(void)
{
	return (int16_t) gimbal_f1_pulse;
}


// pitch in micro seconds
int16_t naza_pitch_us_get(void)
{
	return (int16_t) gimbal_f2_pulse;
}

#endif // NAZA_INT_DEBUG
