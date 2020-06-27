#OBJECTS = main.o init.o seq.o daq_interface.o

#flightcode: $(OBJECTS)
#	gcc -o flightcode $(OBJECTS) -O3

#debug: $(OBJECTS)
#	gcc -o flightcode $(OBJECTS) -Og

#main.o : headers.h
#init.o : headers.h
#seq.o : headers.h
#daq_interface.o : headers.h

#.PHONY : clean
#clean:
#	rm flightcode $(OBJECTS)
EXE := flightcode

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)

OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude
CFLAGS := -Wall -Wextra -O3
LDFLAGS := -Llib
LDLIBS := -luldaq


all: $(EXE)

$(EXE): $(OBJ)
	gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: all clean
clean:
	rm -rf $(OBJ_DIR) $(EXE)
