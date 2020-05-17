OBJECTS = main.o init.o seq.o

flightcode: $(OBJECTS)
	gcc -o flightcode $(OBJECTS) -O3

debug: $(OBJECTS)
	gcc -o flightcode $(OBJECTS) -Og

main.o : headers.h
init.o : headers.h
seq.o : headers.h

.PHONY : clean
clean:
	rm flightcode $(OBJECTS)