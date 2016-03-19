/*
Written By Matsu Thornton
11/6/2015
*/
#include <stdio.h>
#include <math.h>
#include "parameters.h"
#include "functions.h"
#include "matsus_grid_processor.h"


int main()
{
    //this algorithm is based off of number of controllable units in terms of 15 minute intervals
    //for example if there are 10 heaters at 4kW load each which are each on for 1 hour
    //that means that each heater can contribute a total of 4 control units and contributes 4 kW to load at each interval
    //controllable units would be 10 htr * 4 timeslots = 40 htr timeslots = 4 controllable units
    //this example is for 1000 X 4kW units
    //set total controllable units
    int total_ctrl_units = NUMBER_OF_HEATERS*TOTAL_HOURS_CONTROLLABLE_LOAD_TIME_EACH_HEATER*TIME_STEPS_PER_DAY/24;
    float optimum_kW;
    float load_curve[TIME_STEPS_PER_DAY];
    int i;
    int ctrl_unit_counter=0;
    //make array to store # of units on and off in which time slot
    int units_switched[TIME_STEPS_PER_DAY];
    int heater_number,remaining_excess_heater_counter = 0;
    int switch_matrix[total_ctrl_units][3];
    int heater_schedule[total_ctrl_units][3];
    store_load_curve_array(load_curve);
    optimum_kW = calc_optimum(load_curve);
    #ifdef DEBUG4
    printf("%f\n",optimum_kW);
    #endif
    initialize_units_switched_to_zero(units_switched);
   //initialize file to store heater switch matrix that is which individual heaters go on and off at which
   //time intervals
   FILE *fp4;
   fp4 = fopen("heater_switch_matrix.dat","w");
   fclose(fp4);
   //run optimization algorithm
    ctrl_unit_counter =  optimize_switching_times(load_curve,switch_matrix,units_switched,optimum_kW,total_ctrl_units); 
 
    total_ctrl_units = total_ctrl_units - ctrl_unit_counter;
    ctrl_unit_counter++;
    
    while(total_ctrl_units>0)
    {
        
        heater_number = (float)ctrl_unit_counter/(TIME_STEPS_PER_DAY/24)+.99;
        switch_matrix[ctrl_unit_counter][1]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY)+1);
        switch_matrix[ctrl_unit_counter][2]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY)+2);
        remaining_excess_heater_counter=remaining_excess_heater_counter+2;
        total_ctrl_units=total_ctrl_units-1;
        ctrl_unit_counter=ctrl_unit_counter+1;
    }
    #ifdef DEBUG2
        for(i=1;i<=NUMBER_OF_HEATERS*TOTAL_HOURS_CONTROLLABLE_LOAD_TIME_EACH_HEATER*TIME_STEPS_PER_DAY/24;i++)
        {
            fp4=fopen("heater_switch_matrix.dat","a");
            fprintf(fp4,"%d %d\n",switch_matrix[i][1],switch_matrix[i][2]);
            fclose(fp4);
        }
    #endif
    process_switch_matrix_into_heater_schedule(switch_matrix,heater_schedule);

     //write to a file for output
      FILE *fp2;
      FILE *fp3;
      FILE *fp5;
      fp2 = fopen("on_off_data_out.dat","w");
      fp3 = fopen("new_load_curve_out.dat","w");
      for(i=1;i<=TIME_STEPS_PER_DAY;i++)
      {
        fprintf(fp2,"%d\n",units_switched[i]);
        fprintf(fp3,"%f\n",load_curve[i]);
        #ifdef DEBUG
        printf("Time: %f: %f kW\n",(float)i/4+.25,load_curve[i]);
        #endif
      }
      fclose(fp2);
      fclose(fp3);
     
      
}