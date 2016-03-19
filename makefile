CC = gcc
CFLAGS = -lm
EXECUTABLE = process_grid
OBJECT_DIRECTORY = ./obj
SOURCE_DIRECTORY = .

process_grid: functions.o matsus_grid_processor.o
	$(CC) -o $(EXECUTABLE) $^ 

functions.o: functions.c
	$(CC) -c functions.c

main.o: matsus_grid_processor.c
	$(CC) -c $^
clean:
	rm new_load_curve_out.dat on_off_data_out.dat heater_schedule.dat heater_switch_matrix.dat process_grid *.o