#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void init_led()
{
	DDRB |= (1 << DDB5);
}

void led_off()
{
	PORTB &= ~(1 << PORTB5);
}

void led_on()
{
	PORTB |= (1 << PORTB5);
}

void led_toggle()
{
	PINB |= (1 << PINB5);
}

void init_input()
{
	DDRD &= ~(1 << DDD2);
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);
	EIMSK |= (1 << INT0);	
}

ISR (INT0_vect)
{
	led_toggle();
}

int main(void)
{
	init_led();
	led_off();
	init_input();
	sei();

	set_sleep_mode(SLEEP_MODE_STANDBY);
	for (;;)
	{
		sleep_mode();
	}
}
