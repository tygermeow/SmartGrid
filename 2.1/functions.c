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
    for(i=0;i<=TIME_STEPS_PER_DAY-1;i++)
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
   for(i=0;i<=TIME_STEPS_PER_DAY-1;i++)
   {
       load_holder=load_holder+load_curve[i];
   }
   return load_holder / TIME_STEPS_PER_DAY;
}


void initialize_units_switched_to_zero(int units_switched[])
{
    int i;
     for(i=0;i<=TIME_STEPS_PER_DAY-1;i++)
    {
        units_switched[i]=0;
    }
}

int optimize_switching_times(float load_curve[],int switch_matrix[],int units_switched[],float optimum_kW)
{
    
}