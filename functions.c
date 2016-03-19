#include <stdio.h>
#include <math.h>
#include "parameters.h"
#include "functions.h"



void store_load_curve_array(float load_curve[])
{
    int i;
        //store data into array
    FILE *fp;
    fp = fopen("data.dat","r");
    for(i=1;i<=TIME_STEPS_PER_DAY;i++)
    {
        fscanf(fp,"%f",&load_curve[i]);
        #ifdef DEBUG
        printf("Time: %f: %f kW\n",(float)i/4+.25,load_curve[i]);
        #endif
        //fclose(fp);
    }
}
float calc_optimum(float load_curve[])
{
    int i;
    float load_holder=0;
   for(i=1;i<=TIME_STEPS_PER_DAY;i++)
   {
       load_holder=load_holder+load_curve[i];
   }
   return load_holder / TIME_STEPS_PER_DAY;
}


void initialize_units_switched_to_zero(int units_switched[])
{
    int i;
     for(i=1;i<=TIME_STEPS_PER_DAY;i++)
    {
        units_switched[i]=0;
    }
}

int optimize_switching_times(float load_curve[],int switch_matrix[][3],int units_switched[],float optimum_kW,int total_ctrl_units)
{
    float highest_over=0,lowest_under=0;
    int highest_over_index,lowest_under_index,i;
    int ctrl_unit_counter=0;
    int heater_number=1;
    
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
        for(i=1;i<=TIME_STEPS_PER_DAY;i++)
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
    return ctrl_unit_counter;
}

void process_switch_matrix_into_heater_schedule(int switch_matrix[][3],int heater_schedule[][3])
{
    int ctrl_unit_counter = 1; 
    int i,j;
       int position_holder;
       FILE *fp;
       fp=fopen("heater_schedule.dat","w");
       fclose(fp);
    //omg loop
    for(i=1;i<=NUMBER_OF_HEATERS;i++)
    {
     //need to find the first 8 unique values 4 from on column and 4 from off and erase as I go
     
        
            
            position_holder=1;
     
            while(switch_matrix[position_holder][1]==0)
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][1]=switch_matrix[position_holder][1];
            switch_matrix[position_holder][1]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp,"%d",heater_schedule[ctrl_unit_counter-1][1]);
            fclose(fp);
            #ifdef DEBUG3
            printf("%d\n",heater_schedule[ctrl_unit_counter-1][1]);
            #endif
            position_holder++;
            while(switch_matrix[position_holder][1]==0||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-1][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][1]=switch_matrix[position_holder][1];
            switch_matrix[position_holder][1]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][1]);
            fclose(fp);
            #ifdef DEBUG3
             printf("%d\n",heater_schedule[ctrl_unit_counter-1][1]);
            #endif
            position_holder++;
            while(switch_matrix[position_holder][1]==0||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-1][1]||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-2][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][1]=switch_matrix[position_holder][1];
            switch_matrix[position_holder][1]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][1]);
            fclose(fp);
            #ifdef DEBUG3
               printf("%d\n",heater_schedule[ctrl_unit_counter-1][1]);
            #endif
            position_holder++;
            while(switch_matrix[position_holder][1]==0||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-1][1]||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-2][1]||switch_matrix[position_holder][1]==heater_schedule[ctrl_unit_counter-3][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][1]=switch_matrix[position_holder][1];
            switch_matrix[position_holder][1]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][1]);
            fclose(fp);
            #ifdef DEBUG3
              printf("%d\n",heater_schedule[ctrl_unit_counter-1][1]);
            #endif
    
            //now for the offs meow!!!!
            ctrl_unit_counter = ctrl_unit_counter-3;
            position_holder=1;
     
            while(switch_matrix[position_holder][2]==0||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+1][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+2][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+3][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][2]=switch_matrix[position_holder][2];
            switch_matrix[position_holder][2]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][2]);
            fclose(fp);
            #ifdef DEBUG3
            printf(" %d\n",heater_schedule[ctrl_unit_counter-1][2]);
            #endif
            position_holder++;
            while(switch_matrix[position_holder][2]==0||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][2]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+1][1]||switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+2][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][2]=switch_matrix[position_holder][2];
            switch_matrix[position_holder][2]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][2]);
            fclose(fp);
            #ifdef DEBUG3
             printf("%d\n",heater_schedule[ctrl_unit_counter-1][2]);
            #endif
            position_holder++;
            while(switch_matrix[position_holder][2]==0||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][2]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-2][2]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-2][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-0][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter+1][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][2]=switch_matrix[position_holder][2];
            switch_matrix[position_holder][2]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d",heater_schedule[ctrl_unit_counter-1][2]);
            fclose(fp);
            #ifdef DEBUG3
               printf("%d\n",heater_schedule[ctrl_unit_counter-1][2]);
            #endif
            position_holder++;
            while(
            switch_matrix[position_holder][2]==0||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][2]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-2][2]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-3][2]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-3][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-2][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-1][1]||
            switch_matrix[position_holder][2]==heater_schedule[ctrl_unit_counter-0][1])
            {
                position_holder++;
            }
            heater_schedule[ctrl_unit_counter][2]=switch_matrix[position_holder][2];
            switch_matrix[position_holder][2]=0;
            ctrl_unit_counter++;
            fp=fopen("heater_schedule.dat","a");
            fprintf(fp," %d\n",heater_schedule[ctrl_unit_counter-1][2]);
            fclose(fp);
            #ifdef DEBUG3
              printf("%d\n",heater_schedule[ctrl_unit_counter-1][2]);
            #endif
            position_holder++;
       
    }
    
    
        
}