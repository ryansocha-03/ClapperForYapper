BUILD_PATH = ./build
SRC_PATH = ./src

$(BUILD_PATH)/main.hex: $(BUILD_PATH)/main.elf
	avr-objcopy -j .text -j .data -O ihex $(BUILD_PATH)/main.elf $(BUILD_PATH)/main.hex

$(BUILD_PATH)/main.elf: $(BUILD_PATH)/main.o
	avr-gcc -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	avr-gcc -c -Wall -Os -mmcu=atmega328p -DF_CPU=16000000UL $^ -o $@ 
