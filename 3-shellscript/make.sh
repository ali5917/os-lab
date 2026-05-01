# You are building a temperature converter that converts Celsius to Fahrenheit and vice versa. 
# The system consists of: 
    # Header file (temperature.h) that declares conversion functions 
    # C file (convert.c) that implements the conversions
    # C file (main.c) that takes user input calls the functions, and displays the result. 
# Write a Makefile that:
    # Compiles convert.c and main.c separately into object files
    # Links them to produce an executable named temperature_converter
    # Ensures efficient compilation by recompiling only modified files 
# Additionally, include a clean target to remove compiled files

all: temperature_converter

temperature_converter: convert.o main.o
    gcc convert.o main.o -o temperature_converter

convert.o: convert.c temperature.h
    gcc -c convert.c


main.o: main.c temperature.h
    gcc -c main.c

clean:
    rm -f temperature_converter convert.o main.o