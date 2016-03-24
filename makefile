CC = g++
CFLAGS = -std=c++14
TARGET = target

all:
	$(CC) $(CFLAGS) main.cpp -o $(TARGET)

run:
	make all
	target
	
clean:
	rm *.exe 
	rm *.gch 
	rm *.o 
	rm *.swp
	rm *~
	rm *.un