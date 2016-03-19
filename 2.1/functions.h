#include "parameters.h"

//reads data from load data from the utility and stores it into an array
void store_load_curve_array(float load_curve[]);

//takes the entire area under the load curve and converts it into a rectangle
//The top of this rectangle represents an ideal flat load curve with the same
//total energy use as the original.  The level of the top of the rectangle in kW 
//is returned
float calc_optimum(float load_curve[]);
//set switch values to initial of 0
void initialize_units_switched_to_zero(int units_switched[]);
