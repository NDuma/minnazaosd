/**
 ******************************************************************************
 *
 * @file       PWMRead.ino
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements PWM read
 *
 *****************************************************************************/


#include "PWMRead.h"


volatile long start;				// start time
volatile long pulse;				// pulse duration


void pwm_pin_int() {
	if (PIN_READ)
		start = micros();		// positive edge: start
	else
		pulse = micros() - start;	// negative edge: calculate pulsewidth
}


void pwm_read_init(void)
{
	cli();					// disable global interrupts
	pinMode(PWM_PIN, INPUT);
	attachInterrupt(PWM_INT, pwm_pin_int, CHANGE);
	sei();					// enable global interrupts
}


int pwm_get(void)
{
	return (int) pulse;
}
