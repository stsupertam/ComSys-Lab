/*****************************************************************************
*  This example code shows how to use most of PAPI's High level functions    * 
*  to start,count,read and stop on an event set. We use two preset events    *
*  here:                                                                     *
*     PAPI_TOT_INS: Total instructions executed in a period of time		     *
*     PAPI_TOT_CYC: Total cpu cycles in a period of time		             *
******************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include "papi.h"

#define NUM_EVENTS 2
#define THRESHOLD 1000000
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

void sort(int* array, unsigned int length){
	int n,c,d,t;
	for(c=1; c < length ; c++){
		d = c;
		while(d>0 && array[d] < array[d-1]){
			t = array[d];
			array[d] = array[d-1];
			array[d-1] = t;
			d--;
		}
	}
	printf("Sorted list in ascending order:\n");
	/* For check output */	
	//for(c=0; c < i; c++){
	//	printf("%d ", data[c]);
	//}
}

int main()
{
   /*Read file and array declartion*/
	FILE* file;
	unsigned int length;
	int* data;

	int n,c,d,t;
	file = fopen("data", "r");
  	fseek(file, 0L, SEEK_END);
  	length = ftell(file);
  	fseek(file, 0L, SEEK_SET);
	data = malloc(length * sizeof(int) + 1);
 	memset(data, 0, length);
	fread(data, sizeof(int), length, file);
	unsigned int i = 0;
	while(1){
		if(data[i] == 0) break;
		i++;
	}
	sort(data, i);
   /*Declaring and initializing the event set with the presets*/
   int Events[2] = {PAPI_TOT_INS, PAPI_TOT_CYC};
   /*The length of the events array should be no longer than the 
     value returned by PAPI_num_counters.*/	
	
   /*declaring place holder for no of hardware counters */
   int num_hwcntrs = 0;
   int retval, retval2;
   char errstring[PAPI_MAX_STR_LEN];
   /*This is going to store our list of results*/
   long long values[NUM_EVENTS];

  float real_time, proc_time,mflops;
  long long flpops;
  float ireal_time, iproc_time, imflops;
  long long iflpops;
   /***************************************************************************
   *  This part initializes the library and compares the version number of the*
   * header file, to the version of the library, if these don't match then it *
   * is likely that PAPI won't work correctly.If there is an error, retval    *
   * keeps track of the version number.                                       *
   ***************************************************************************/

   if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   {
      fprintf(stderr, "Error: %d %s\n",retval, errstring);
      exit(1);
   }

   /**************************************************************************
    * PAPI_num_counters returns the number of hardware counters the platform *
    * has or a negative number if there is an error                          *
    **************************************************************************/
   if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK)
   {
      printf("There are no counters available. \n");
      exit(1);
   }

   printf("There are %d counters in this system\n",num_hwcntrs);
		
   /**************************************************************************
    * PAPI_start_counters initializes the PAPI library (if necessary) and    *
    * starts counting the events named in the events array. This function    *
    * implicitly stops and initializes any counters running as a result of   *
    * a previous call to PAPI_start_counters.                                *
    **************************************************************************/
	
   if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
       ERROR_RETURN(retval);

   printf("\nCounter Started: \n");

   /* Your code goes here*/
   sort(data, i);
	

  

   /**********************************************************************
    * PAPI_read_counters reads the counter values into values array      *
    **********************************************************************/

   if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
      ERROR_RETURN(retval);

   printf("Read successfully\n");



   printf("The total instructions executed for sort are %lld \n",values[0]);
   printf("The total cycles used are %lld \n", values[1] );
	
   printf("\nNow we try to use PAPI_accum to accumulate values\n"); 

   /* Do some computation here */
   sort(data, i);
     
 
   /************************************************************************
    * What PAPI_accum_counters does is it adds the running counter values  *
    * to what is in the values array. The hardware counters are reset and  *
    * left running after the call.                                         *
    ************************************************************************/

   if ( (retval=PAPI_accum_counters(values, NUM_EVENTS)) != PAPI_OK)
      ERROR_RETURN(retval);

   printf("The total instructions executed for addition are %lld \n",
			values[0] );
   printf("The total cycles used are %lld \n", values[1] );
   printf("CPI = %lf \n", (double) values[1]/values[0] );

   /***********************************************************************
    * Stop counting events(this reads the counters as well as stops them  *
    ***********************************************************************/

   PAPI_shutdown(); 

   exit(0);	
}
