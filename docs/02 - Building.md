# Chapter 2: Building

Now that we have our code, it is time to compile it to a form that our microcontroller can understand. For this we will be using a few tools from our toolchain that we installed at the beginning.

First, check to make sure your AVR cross-compiler, avr-gcc, is installed.

```
avr-gcc --version
```

It is called a cross-compiler because it runs on one machine (the host), but generates code for a target machine, in this case our ATMega329p. But of course avr-gcc supports many other AVR MCUs.

Start off with the following command:
```
avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL hello-world.c -o hello-world.o
```

Taking a look at each option and argument:
- -c Compiles the source files but does not link them
- -Wall enables all warnings
- -0s optimizes for size. We are working with limited space here
- -mmcu is where we specify our MCU, the atmega328p. This is also how the compiler knows which io.h to use.
- -DF_CPU tells the compiler the clock speed. Here we say 16 MHz because that is the Nano's clock speed (Note the atmega can actually do 20 MHz when powered by 5V).
- hello-world.c is our source file
- -o shows we are going to put the output to hello-world.o

After running this command, we will see an object file hello-world.o

We will not run the linking process. We only have one file, so we don't have to do much besides generate our executable.

```
avr-gcc -Wall -0s -mmcu=atmega328p -DF_CPU=16000000UL hello-world -o hello-world.elf
```

Notice here we have the same options, but we omit the -c, since we want to link.

The resulting file has extension .elf, short for Executable and Linking Format. Essentially an executable but with extra symbol and debugging info that we can't upload to our Arduino.

The final step before we can upload it we use another tool, avr-objcopy:
```
avr-objcopy -j .text -j .data -O ihex hello-world.elf hello-world.hex
```

The tutorial that this follows doesn't explain this one so you're on your own! All it does it turns the elf file into a hex file we can upload.

## Uploading
Since we are using the Arduino board, which comes with a bootloader, we can use that for uploading our hex file. 

If we don't have a build in bootloader, we would need some special hardware called a programmer (thank god we don't need one).

So for now, we can upload our hex file with
```
avrdude -v -patmega328p -carduino -b115200 -D -Uflash:w:hello-world.hex:i
```

Here we tell avrdude to upload to an ATMega328p process (-patmega328p) using the arduino bootloader (-carduino) with a baud rate of 11520 bits per second (-b11520). We will want to delete whatever is there currently before uploading (-D). The last section uploads our hex file.

You may also have to specify the port that your Arduino is connected to. For my Macbook Pro I opened a terminal, connected my Arduino, and ran the command:

```
ls -lha /dev/tty*
```

Among the list outputted I saw "/dev/tty.usbserial-1110" which disappeared when I disconnected the Arduino and reran the command.

You should then add the port to the avrdude command above by doing "-P" followed by a space, followed by the port.

The onboard LED on the Arduino Nano should now be blinking. Play around with the values you give to _delay_ms(), and rerun the compile and upload commands and see how it changes how the LED blinks.

## Build Systems
If you've had some fun messing with the delay times, it may have been ruined by the constant need to run 4 commands each and every time. Luckily, we can automate this using a build system. The build system we will be using is the simplest, and is the one most people learn first, Make.

Make uses something called a "Makefile" for automating builds. It also has the advantage of only running build steps when needed (i.e. when changes are made).

A Makefile consists of a set of rules that generally look like this: 

```
targets: prerequisites
	command
	command
```

The targets are your files that you would like to build, typically only one target per rule. Prerequisites, or dependencies, are the files that need to exist in order to build your target.

Finally, the list of commands (1 or more) are the commands that need to be run in order that build the target. Note that commands MUST start with a single indent (not spaces) or else it won't work.

### The make command
To actually use a Makefile, you use the command "make" followed by the name of your target. If we don't specify a target, make will run the first rule that doesn't start of a ".".

When you do run a make command, the following happens:
- Make finds the rule(s) that have the target specified.
- Make decides if it should run the rule. It does this by checking if:
	- The target doesn't exist OR
	- One or more of its dependencies is newer than the target.

This gives us the fun functionality that when we change a dependency, all we have to do is run make for that target, and make will rebuild it, otherwise it won't. When it comes to large projects with lots of targets and dependencies this can come in handy.

### Building our Makefile
With all this in mind, let's start with our first compilation step and insert it into the Makefile:

```
hello-world.o: hello-world.c
	avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL hello-world.c -o hello-world.o
```

Now running make we will get our generated hello-world.o file. Running make again we will see that make knows we didn't change anything, so it won't rebuild.

Adding in the rest of the steps of our build process with:

```
hello-world.hex: hello-world.elf
	avr-objcopy -j .text -j .data -O ihex hello-world.elf hello-world.hex

hello-world.elf: hello-world.o
	avr-gcc -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL hello-world.o -o hello-world.elf

hello-world.o: hello-world.c
	avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL hello-world.c -o hello-world.o
```

Now when running make, we will check if our hex file exists, as well as it's dependencies. So, we'll then check the elf files and its dependencies. So then we'll check the object file and it's dependencies, our source file. So if we change hello-world.c, we need only run "make" and all our build steps will be run. Try it out.

### Using make variables
Thinking about adding more and more files to our project, keeping them all straight and adding them to the Makefile can get out of control. We can extract the general steps we take to build and use some of makes automatic variables instead of having to tell make explicitly every single step of building our files.

The common automatic variables in make are:
- $@ is the target name
- $? is all dependencies newer than the target
- $^ is all dependencies
- $< is the first dependency

We will replace our rule to build the .o file with:

```
%.o: %.c
	avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@
```

Now we can modify our elf file rule to be:

```
hello-world.elf: hello-world.o
	avr-gcc -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@
```

If we add more files in the future, all we need to do is add the object file dependency after "hello-world.o". Then when we run make, it will get picked up, and make will use the generic rule we created for building .o files.
