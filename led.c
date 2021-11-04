#include <avr/io.h>
#include "led.h"
#include <stdbool.h>

volatile bool up = false;
volatile uint8_t target = 0xFF;

void LED_init() {
    PORTD&=~(1<<PD6); // pin 6
	//set PD6 as output
	DDRD|=(1<<PD6);

	// Init timer mode and prescaler
	TCCR0A = (1 << COM0A1) | (1 << COM0A0)|(1<<WGM00)|(1<<WGM01);  // pin 6 and fast PMW
	TCCR0B = (1 << CS01) | (1 << CS00);  // 64    

	// // Configure btn PD2 as an input using the Data Direction Register D (DDRD)
    // DDRD &= ~_BV(DDD2);

    // // Enable the pull-up resistor on PD2 using the Port D Data Register (PORTD)
    // PORTD |= _BV(PORTD2);
}

uint8_t simple_ramp() {
	if(target==0xFF) up=false;
	if(target==0x00) up=true;

	if(up) return target++;
	return target--;
}
