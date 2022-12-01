#include <stdio.h>
#include<stdlib.h>
#include <math.h>  
#include <time.h> 

float Mean(float numbers[], int count)
{
	float sum = 0;
	for (int i = 0; i < count; i++)
		sum = sum + numbers[i];
	return (sum / count);
}

float MADFunction(float numbers[], int count)
{
	float Sum = 0;
	for (int i = 0; i < count; i++)
		Sum = Sum + abs(numbers[i] - Mean(numbers, count));

	return (Sum / count);
}


float RangeFunction(float numbers[], int count)
{
	float small=numbers[0],big=numbers[0];
    for (int i = 0; i<count; i++) {
    
		if(numbers[i] > big) 	big = numbers[i];  
  		if(numbers[i] < small)  small = numbers[i];  		
	}

	return (big-small);
}

int main(void)
{
  clock_t start, end;
  start = clock();
  double pTime;
  
  
  float numbers[500000],small,big,range;
  int count=0;
  FILE *file;

  if (file = fopen("large.txt", "r"))
  {
    while (fscanf(file, " %f", &numbers[count]) != EOF)
    {
      count++;
    }
    
    fclose(file);
    
    /*
    for(int i=0; i<count; i++){
    
    	printf(" %.2f\n",numbers[i]);
    }*/

	printf("Range:  %.2f\n", RangeFunction(numbers, count) );
	printf("Mad:  %.2f\n", MADFunction(numbers, count) );	

	end = clock();
     	pTime = ((double) fabsf(end - start) / CLOCKS_PER_SEC)*1000;
	printf("Execution time for Range and MAD algorithm is  %.3f seconds.\n", pTime);
     
  }

  return 0;
}


