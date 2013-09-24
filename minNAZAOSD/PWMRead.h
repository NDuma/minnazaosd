/**
 ******************************************************************************
 *
 * @file       PWMRead.h
 * @author     Joerg-D. Rothfuchs
 * @brief      Implements PWM read
 *
 *****************************************************************************/


#ifndef PWMREAD_H_
#define PWMREAD_H_

#define PWM_CHECK(x)	(x > 1700)

#define PWM_PIN		3			// MinimOSD PAL pin
#define PWM_INT  	1

//#define PIN_READ	digitalRead(PWM_PIN)	// slower than direct access
#define PIN_READ	(PIND & 0b00001000)	// faster than digitalRead

void pwm_read_init(void);
int pwm_get(void);

#endif /* PWMREAD_H_ */