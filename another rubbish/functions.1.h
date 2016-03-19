/*
Written By Matsu Thornton
11/6/2015
*/

//reads data from load data from the utility and stores it into an array
void store_load_curve_array(float load_curve[]);

//takes the entire area under the load curve and converts it into a rectangle
//The top of this rectangle represents an ideal flat load curve with the same
//total energy use as the original.  d/dt of the area in kW 
//is returned
float calc_optimum(float load_curve[]);
//set switch values to initial of 0
void initialize_units_switched_to_zero(int units_switched[]);
//gets the number of heaters per timeslot that need to be switched to optimize load curve
//returns the number of units used
int optimize_switching_times(float load_curve[],int switch_matrix[][3],int units_switched[],float optimum_kW,int total_ctrl_units);
//processes the unscheduled required switch signals into actual heater schedule
//I was a bit dissapointed with previous attempts to find a more elegant approach to this
//in the end I had to hard-code for 4 time steps per heater
//the code would need revision to account for more time increments per heater
void process_switch_matrix_into_heater_schedule(int switch_matrix[][3],int heater_schedule[][3]);