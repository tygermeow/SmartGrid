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
    int kW_per_unit = 4;
    float optimum_kW;
    float highest_over=0,lowest_under=0;
    int highest_over_index,lowest_under_index;
    float load_curve[TIME_STEPS_PER_DAY];
    int i;
    int ctrl_unit_counter=0;
    //make array to store # of units on and off in which time slot
    int units_switched[TIME_STEPS_PER_DAY];
    int heater_number,remaining_excess_heater_counter = 0;
    int switch_matrix[total_ctrl_units][3];
    store_load_curve_array(load_curve);
    optimum_kW = calc_optimum(load_curve);
    initialize_units_switched_to_zero(units_switched);
   //initialize file to store heater switch matrix
   FILE *fp4;
   fp4 = fopen("heater_switch_matrix.dat","w");
   fclose(fp4);
    do
    {
        ctrl_unit_counter++;
        heater_number = (float)ctrl_unit_counter/(TIME_STEPS_PER_DAY/24)+.99;
        #ifdef DEBUG
        printf("heater_number = %d\n",heater_number);
        #endif
        lowest_under = 0;
        highest_over = 0;
        //check for point furthest above and point furthest below and store in temp var
        for(i=0;i<=TIME_STEPS_PER_DAY-1;i++)
         {
             if(load_curve[i]<optimum_kW)
             {
                if(optimum_kW-load_curve[i]>lowest_under)
                {
                    lowest_under = optimum_kW-load_curve[i];
                    lowest_under_index = i;
                }
            }
             if(load_curve[i]>optimum_kW)
             {
                if(load_curve[i]-optimum_kW>highest_over)
                {
                    highest_over = load_curve[i]-optimum_kW;
                    highest_over_index = i;
                }
            }
         }
         #ifdef DEBUG
         printf("highest_over %f\nhighest_over_index %d\nlowest_under %f\nlowest_under_index %d\n",highest_over,highest_over_index,lowest_under,lowest_under_index);
         #endif
        //add to low while subtracting from high simultaneously until one is within 4kw
        //of ideal increment down the # of total controllable units

        //add 1 unit to time index furthest from optimum and vice versa
        load_curve[lowest_under_index] = load_curve[lowest_under_index] + kW_per_unit;
        load_curve[highest_over_index] = load_curve[highest_over_index] - kW_per_unit;
        total_ctrl_units--;
        
        //store the switch data
        switch_matrix[ctrl_unit_counter][1] = lowest_under_index;
        switch_matrix[ctrl_unit_counter][2] = highest_over_index;

        units_switched[lowest_under_index]=units_switched[lowest_under_index]+1;
        units_switched[highest_over_index]=units_switched[highest_over_index]-1;
        #ifdef DEBUG
        printf("NEW\nhighest_over %f\nhighest_over_index %d\nlowest_under %f\nlowest_under_index %d\nUNITS SWITCHED %d\n",load_curve[highest_over_index],highest_over_index,load_curve[lowest_under_index],lowest_under_index,ctrl_unit_counter);
        printf("Switch units left: %d\n",total_ctrl_units);
        #endif
         //loop again and repeat until you run out of controllable units
    }while(total_ctrl_units>0&&lowest_under>4&&highest_over>4);
    ctrl_unit_counter++;
    while(total_ctrl_units>0)
    {
        
        heater_number = (float)ctrl_unit_counter/(TIME_STEPS_PER_DAY/24)+.99;
        switch_matrix[ctrl_unit_counter][1]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY-2)+1);
        switch_matrix[ctrl_unit_counter][2]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY-2)+2);
        //switch_matrix[ctrl_unit_counter+1][1]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY-2))+heater_number%4+3;
        //switch_matrix[ctrl_unit_counter+1][2]=(remaining_excess_heater_counter%(TIME_STEPS_PER_DAY-2))+heater_number%4+4;
        remaining_excess_heater_counter=remaining_excess_heater_counter+2;
        total_ctrl_units=total_ctrl_units-1;
        ctrl_unit_counter=ctrl_unit_counter+1;
    }
    #ifdef DEBUG2
    for(i=1;i<=NUMBER_OF_HEATERS*TOTAL_HOURS_CONTROLLABLE_LOAD_TIME_EACH_HEATER*TIME_STEPS_PER_DAY/24-1;i++)
    {
        fp4=fopen("heater_switch_matrix.dat","a");
        fprintf(fp4,"%d %d\n",switch_matrix[i][1],switch_matrix[i][2]);
        fclose(fp4);
    }
    #endif
     //write to a file for output
      FILE *fp2;
      FILE *fp3;
      fp2 = fopen("on_off_data_out.dat","w");
      fp3 = fopen("new_load_curve_out.dat","w");
      for(i=0;i<=TIME_STEPS_PER_DAY-1;i++)
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