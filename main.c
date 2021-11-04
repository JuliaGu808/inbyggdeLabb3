#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

#define BTN_DOWN 1
#define BTN_UP 0

int status = BTN_UP;

int toggle2;

bool begin = false;
bool second = false;
bool third = false;
int count = 0;

volatile unsigned int adc_value = 0;

ISR(ADC_vect)
{
	// read result of convert left
	adc_value = ADCH; // 0-255
}

ISR(TIMER2_COMPA_vect){
  if (toggle2)
	{
		if (PIND & _BV(PIND2)) // button is pressed
		{
			switch (status)
			{
			case BTN_UP:
				status = BTN_DOWN;
				break;
			default:
				break;
			}
		}
		else // button is released
		{
			switch (status)
			{
			case BTN_DOWN:
				begin=true;
				second=false;
				third=false;
				count=0;
				status = BTN_UP;
				break;
			default:
				break;
			}
		}
		begin_VG();
		toggle2 = 0;
	}
	else toggle2 = 1;
	
}

void begin_VG()
{
	if(begin){
			OCR0A = simple_ramp();
			if(OCR0A==0) count++;
			if(count==5) {
				begin=false;
				second = true;
			}
		}
		if(second){
			OCR0A = adc_value;
			count++;
			if(count==3000){
				second=false;
				third=true;
			}
		}
		if(third){ // 1Hz = 1s = 100 * 10ms
			if(count%2==0){
				OCR0A = 0x00;
				for (int i = 0; i < 100; i++){
					uart_putstr("high\r\n");
				}
				count++;
			}
			if(count%2!=0){
				OCR0A = 0xFF;
				for (int i = 0; i < 100; i++){
					uart_putstr("low\r\n");
				}
				count++;
			}
			if(count==3020) {
				third=false;
			} 
		}
}

void ADC_setup()
{
	ADMUX = 0; // clear ADMUX register
	ADMUX |= (1 << REFS0) | (1 << ADLAR); // AVCC voltage reference,it connects to +5V in Arduino UNO
	// ADEN:enable ADC. ADIE:enable interrupt. ADPS[2:0]=011: prescaler 8
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC); // start to convert	
}

void pin_setup()
{
	DDRC &= ~(1 << PC0); // set PORTC0 as input A0
}

void main (void) {
	cli();
	LED_init();
	uart_init();
	timer_init();
	pin_setup();
	ADC_setup();
	sei();				   // enable global interrupt
	while (1)
	{
		if (!(ADCSRA & (1 << ADSC)))
		{
			ADCSRA |= (1 << ADSC); // start to convert again
		}
	}


	// while (1) {
	// 	if (PIND & _BV(PIND2)) // button is pressed
    //     {
	// 		switch (status)
	// 		{
	// 			case BTN_UP:
	//            		uart_putstr("pushed\r\n");
	// 				status=BTN_DOWN;
	// 				break;
	// 			default:	
	// 				break;

	// 		}			            
    //     }
    //     else // button is released
    //     {
	// 		switch (status)
	// 		{
	// 			case BTN_DOWN:
	//            		uart_putstr("released\r\n");
	// 				status=BTN_UP;
	// 				break;
	// 			default:	
	// 				break;
	// 		}                     
    //     }
	// }
}
