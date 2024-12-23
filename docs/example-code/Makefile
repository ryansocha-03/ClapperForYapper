hello-world.hex: hello-world.elf
	avr-objcopy -j .text -j .data -O ihex hello-world.elf hello-world.hex

hello-world.elf: hello-world.o
	avr-gcc -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@

%.o: %.c
	avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@ 
