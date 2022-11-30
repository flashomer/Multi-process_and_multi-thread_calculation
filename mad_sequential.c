#include <stdio.h>
#include<stdlib.h>  

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
	float small,big;
    for (int i = 0; i<count; i++) {
    
		if(numbers[i] > big) 	big = numbers[i];  
  		if(numbers[i] < small)  small = numbers[i];  		
	}

	return (big-small);
}

int main(void)
{
  float numbers[50],small,big,range;
  int count=0;
  FILE *file;

  if (file = fopen("tiny.txt", "r"))
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
     
  }

  return 0;
}


