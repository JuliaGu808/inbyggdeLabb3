#include <avr/io.h>

#include "timer.h"

void timer_init() {
	// Init timer mode and prescaler
	TCCR0A = (1 << COM0A1) | (1 << COM0A0)|(1<<WGM00)|(1<<WGM01);  // pin 6 and fast PMW
	TCCR0B = (1 << CS01) | (1 << CS00);  // 64   

	TCCR2A = (1<<WGM01);  // CTC
	TCCR2B = (1 << CS02) | (1 << CS00);  // 1024

	// enable overflow interrupt
    TIMSK2 |= (1 << OCIE2A);
    // TIMSK2 |= (1 << TOIE2A);

	TCNT2 = 0;
	// OCR2A = (0.01s * 16000000Hz)/1024 = 156 = 0x9C
	OCR2A = 0x9C;
}

