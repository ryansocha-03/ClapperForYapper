#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << DDB5);
	for(;;)
	{
		PORTB |= (1 << PORTB5);
		_delay_ms(3000);
		PORTB &= ~(1 << PORTB5);
		_delay_ms(500);
	}
}
