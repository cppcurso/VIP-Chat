
CC = g++
CFLAGS =  -Wall -c -std=c++11
LIBS = -lgtest -lpthread
TARGET = main

$(TARGET): $(TARGET).o Server.o Message.o
	$(CC) -Wall $(TARGET).o Server.o Message.o -o $(TARGET) $(LIBS)

$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp

Server.o: Server.cpp Server.h
	$(CC) $(CFLAGS) Server.cpp

Message.o: Message.cpp Message.h
	$(CC) $(CFLAGS) Message.cpp

# .PHONY evita que make trate clean como un target
.PHONY: clean
# Borra los ficheros resultado de manera recursiva y sin confirmación
clean:
	rm -rf *.o $(TARGET)
