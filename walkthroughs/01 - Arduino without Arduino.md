# Chapter 1: Arduino without Arduino

As per the reqs, we want to avoid using the Arduino libraries if we can. But, we still opted for an Arduino board because of the many resources that are out there. This is a common starting point for delving into embedded systems deeper than what using Arduino libraries allows.

## Toolchain
The MCU that is on our Arduino Nano board is the ATmega328P from AVR (formerly Atmel). AVR has graciously provided us with a group of tools, called a toolchain to aid in writing software for their MCUs
[AVR-LibC Documentation](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/index.html)

### Components
The main components we will be using from this toolchain are listed below.

#### avr-gcc
The version of the GCC cross compiler that targets AVR.

#### avr-objcopy
A tool from GNU Binutils (Binary Utilities) for copying and translating object files to different formats, built to target AVR.

#### avr-dude
AVR Downloader Uploader is a command line tool for downloading and uploading the on-chip memories of AVR microcontrollers. It is what we will use to flash our program onto our board.

### Installing and Building the Toolchain

The host system I am using is a MacBook Pro with an M2 chip. The easiest way to install the AVR toolchain is using Homebrew.

`brew tap osx-cross/avr`

This first command taps the osx-cross/avr repository adding it to the list of repos Homebrew tracks, pulls from, etc.

`brew install \
    avr-gcc \
    avrdude \
    minicom \
    cpputest \
    pkg-config \
    gcc`

Next we actually install the tools. You see a few of the tools we listed above here. avr-objcopy is installed as a part of avr-binutils which comes as a part of avr-gcc

Check the installation path for more info. For my machine it is 
/opt/homebrew/Cellar/avr-binutils/2.43.1/avr/bin/objcopy

/opt/homebrew/Cellar/avr-gcc@9/9.4.0_1/avr/include/avr

## Hello World

Based off tutorial found [here](https://github.com/rubberduck203/embedded-101)

Starting off with the basic hello world of microcontrollers, blinking the on board LED.

### Code
```
// hello-world.c  
#include <avr/io.h>  
#include <util/delay.h>  

int main(void)  
{
	DDRB |= (1 << DDB5);

	for(;;)
	{
		PORTB |= (1 << PORTB5);
		_delay_ms(500);
		PORTB &= ~(1 << PORTB5);
		_delay_ms(500);
	}
}
```
This task is simple enough where we only need a single source file
