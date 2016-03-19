CC = gcc
CFLAGS = -lm
EXECUTABLE = process_grid
OBJECT_DIRECTORY = ./obj
SOURCE_DIRECTORY = .

process_grid: functions.c matsus_grid_processor.c
	$(CC) -o $(EXECUTABLE) $^ 
clean:
	rm *.o process_grid