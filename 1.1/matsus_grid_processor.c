/*
Written By Matsu Thornton
11/6/2015
*/

#include <stdio.h>
#include <math.h>
#define DEBUG
//#define EXCESS_CONTROLLABLE_LOAD
#define NUMBER_OF_HEATERS 500
#define TIME_STEPS 96
//later version of this program should

int main()
{
    //this algorithm is based off of number of controllable units in terms of 15 minute intervals
    //for example if there are 10 heaters at 4kW load each which are each on for 1 hour
    //that means that each heater can contribute a total of 4 control units and contributes 4 kW to load at each interval
    //controllable units would be 10 htr * 4 timeslots = 40 htr timeslots = 4 controllable units
    //this example is for 1000 X 4kW units
    //set total controllable units
    int total_ctrl_units = 2000;
    int kW_per_unit = 4;
    float optimum_kW = 1039.594067;
    float highest_over=0,lowest_under=0;
    int highest_over_index,lowest_under_index;
    float load_curve[96];
    int i;
    int ctrl_unit_counter;
    //store data into array
    FILE *fp;
    fp = fopen("data.dat","r");
    for(i=0;i<=95;i++)
    {
        fscanf(fp,"%f",&load_curve[i]);
        #ifdef DEBUG
        printf("Time: %f: %f kW\n",(float)i/4+.25,load_curve[i]);
        #endif
        //fclose(fp);
    }
    //make array to store # of units on and off in which time slot
    int units_switched[96];
    //initialize this array to zero
    for(i=0;i<=95;i++)
    {
        units_switched[i]=0;
    }
    
    do
    {
        lowest_under = 0;
        highest_over = 0;
        //check for point furthest above and point furthest below and store in temp var
        for(i=0;i<=95;i++)
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
        ctrl_unit_counter = 0;
        //if there is more than enough controllable load to balance the system, this function can be more efficient
        //if there is less than enough controllable load it is better to check through each iteration 
        #ifdef EXCESS_CONTROLLABLE_LOAD
        while(fabs(load_curve[lowest_under_index]-optimum_kW)>4&&fabs(load_curve[highest_over_index]-optimum_kW>4)&&total_ctrl_units>0)
        {
        #endif
            load_curve[lowest_under_index] = load_curve[lowest_under_index] + kW_per_unit;
            load_curve[highest_over_index] = load_curve[highest_over_index] - kW_per_unit;
            total_ctrl_units--;
            ctrl_unit_counter++;
        #ifdef  EXCESS_CONTROLLABLE_LOAD
        }
        #endif
        units_switched[lowest_under_index]=units_switched[lowest_under_index]+ctrl_unit_counter;
        units_switched[highest_over_index]=units_switched[highest_over_index]-ctrl_unit_counter;
        #ifdef DEBUG
        printf("NEW\nhighest_over %f\nhighest_over_index %d\nlowest_under %f\nlowest_under_index %d\nUNITS SWITCHED %d\n",load_curve[highest_over_index],highest_over_index,load_curve[lowest_under_index],lowest_under_index,ctrl_unit_counter);
        printf("Switch units left: %d\n",total_ctrl_units);
        #endif
         //loop again and repeat until you run out of controllable units
    }while(total_ctrl_units>0&&lowest_under>4&&highest_over>4);
     //write to a file for output
      FILE *fp2;
      FILE *fp3;
      fp2 = fopen("on_off_data_out.dat","w");
      fp3 = fopen("new_load_curve_out.dat","w");
      for(i=0;i<=95;i++)
      {
        fprintf(fp2,"%d\n",units_switched[i]);
        fprintf(fp3,"%f\n",load_curve[i]);
        #ifdef DEBUG
        printf("Time: %f: %f kW\n",(float)i/4+.25,load_curve[i]);
        #endif
      }
      fclose(fp2);
      
}