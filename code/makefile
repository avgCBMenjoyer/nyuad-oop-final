CC = g++
CFLAGS = -pthread -std=gnu++17

EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): main.cpp system.cpp ancillary.cpp vehicle.cpp customer.cpp records.cpp
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.cpp system.cpp ancillary.cpp vehicle.cpp customer.cpp records.cpp

clean:
	$(RM) $(EXECUTABLE)

run:
	./$(EXECUTABLE)