/*
 * Laboving8_Servomotor.c
 *
 * Created: 28-Feb-19 8:56:44 AM
 * Author : erlen
 */ 
#define F_CPU 16e6

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void ADC_init(void);

void TMR_init(void);

volatile uint16_t data; //declaring a generic data var, just in case.

int main(void)
{
    DDRB = (1<<PORTB1); //Sets output.
	
	TMR_init();
	ADC_init();  // start ADC
	sei();  //Sets global interrupt.
	ADCSRA |= (1<<ADSC);  //Start converting.
			
    while (1) 
    {
		/*OLD CODE
		OCR1A = 900;
		_delay_ms(500);
		OCR1A = 4350;
		_delay_ms(1000);
		*/
	}
}

void TMR_init(void)
{
		TCCR1A = (1<<WGM11) | (1<<COM1A1);  //non-inverting mode, Sets Fast PWM mode with ICR as top.
		TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS11); //CLK_IO/8 prescaler.
		ICR1 = 40000; //Sets top at 40000. With a 16MHz clock divided by 8 this gives the counter a clock cycle of 50Hz
}

void ADC_init(void)
{
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Enables ADC, sets interrupt flag, sets pre-scaling.
	ADMUX = (1<<REFS0) | (1<<ADLAR); //Sets AVcc as reference, sets ADLAR for ease of reading the ADC output.
}

ISR(ADC_vect)
{
	data = (ADC>>3); //Rightshift to scale where in the ADC we start reading, thus making more use of the Potmeter.
		
	//Makes sure the servo doesn't get destroyed by too high or too low input values.
	if(data>4300){
		data=4300;
	}
	if(data<1000){
		data=1000;
	}
	
	OCR1A = data; // Apply data to Servo.
	ADCSRA |= (1<<ADSC); //Start the converting again.
}