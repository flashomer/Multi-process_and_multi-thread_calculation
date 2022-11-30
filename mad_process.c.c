#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>


#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

#define SMALL	0
#define BIG	1
#define MEAN		2
#define MAD		3

float pMean,cMean;
float write_global[BUFFER_SIZE];
float read_global[BUFFER_SIZE]; 

float parentArray[4]; 
float childArray[4]; 

float Mean(float numbers[], int count)
{
	float sum = 0;
	for (int i = 0; i < count; i++)
		sum = sum + numbers[i];
	return (sum / count);
}

float MADFunction(float numbers[], int count, float mean)
{
	float Sum = 0;
	for (int i = 0; i < count; i++)
		Sum = Sum + fabsf(numbers[i] - mean);

	return (Sum / count);
}


float* RangeFunction(float numbers[], int count)
{
	float small=numbers[0],big=numbers[0];
	static float Range[2];
	
	for (int i = 0; i<count; i++) {
	    
		if(numbers[i] > big) 	big = numbers[i];  
	  	if(numbers[i] < small)  small = numbers[i];  		
	}
	
	Range[0] = small;
	Range[1] = big;

	return Range;
}


float Child(float numbers[], int count, int n){
	
	int c = 0;
	float cNumbers[50];

	for(int j =0; j<n; j++){

		cNumbers[c] = numbers[j];
		
		c++;

	}

		cMean = Mean(cNumbers,c);
		
		childArray[SMALL] = RangeFunction(cNumbers,c)[SMALL];
		childArray[BIG]   = RangeFunction(cNumbers,c)[BIG];
		
		childArray[MEAN]   	 = Mean(cNumbers,c);

	return cMean;

}

float Parent(float numbers[], int count, int n){
	
	int c = 0;
	float pNumbers[50];

	for(int j =n; j<count; j++){

		pNumbers[c] = numbers[j];
		
		c++;

	}
		pMean = Mean(pNumbers,c);
		
		parentArray[SMALL] = RangeFunction(pNumbers,c)[SMALL];
		parentArray[BIG]   = RangeFunction(pNumbers,c)[BIG];
		
		parentArray[MEAN]   	 = Mean(pNumbers,c);


	return pMean;
}

int main(void)
{
  clock_t start, end;
  start = clock();
  double pTime;
	
  float numbers[50],small,big,range;
  int count=0;
  FILE *file;
  
  int pid;
  int fd1[2]; 
  int fd2[2];  

  if (file = fopen("tiny.txt", "r"))
  {
    while (fscanf(file, " %f", &numbers[count]) != EOF)
    {
      count++;
    }
    
    fclose(file);
  }

    
  if (pipe(fd1) == -1 || pipe(fd2) == -1) { fprintf(stderr,"Pipe failed"); return 1;}
  pid = fork();

  if (pid < 0) { fprintf(stderr, "Fork failed"); return 1; }

  if (pid > 0) {  /*parent */
  
	pMean = Parent(numbers,count,(int)(count/2));
  
  	close(fd1[READ_END]); 
	write(fd1[WRITE_END], parentArray, sizeof(parentArray)); 
	close(fd1[WRITE_END]);
	
	wait(NULL);
	
	float read_global2[BUFFER_SIZE]; 
	
	close(fd2[WRITE_END]); 
	read(fd2[READ_END], read_global2, sizeof(read_global2));
	printf("New Parent:  %.2f - %.2f - %.2f\n", read_global2[0],read_global2[1],read_global2[2]);
	close(fd2[READ_END]);
	
	wait(NULL);
	
	
	
	end = clock();
     	pTime = ((double) fabsf(end - start) / CLOCKS_PER_SEC)*1000;
	printf("Execution time for Range and MAD algorithm is  %.3f seconds.\n", pTime);

  } else {  /*child */
  
	cMean = Child(numbers,count,(int)(count/2));

	close(fd1[WRITE_END]); 
	read(fd1[READ_END], read_global, sizeof(read_global));
	
	//pMean = read_global[0];
	
	float newRangeArray[4] = {read_global[SMALL],read_global[BIG],childArray[SMALL],childArray[BIG]};
	float* comapreRange;
	comapreRange = RangeFunction(newRangeArray, 4);

	close(fd1[READ_END]);
	
	
	printf("Range:  %.2f\n", (comapreRange[BIG]-comapreRange[SMALL]) );
	printf("Mad:  %.2f\n", MADFunction(numbers, count, fabsf((read_global[MEAN]+childArray[MEAN])/2) ) );
	
	wait(NULL);
	
	childArray[0] = 99999.0;
	childArray[1] = 99999.1;
	childArray[2] = 99999.2;
	
  	close(fd2[READ_END]); 
	write(fd2[WRITE_END], childArray, sizeof(childArray)); 
	close(fd2[WRITE_END]);	
	
	wait(NULL);
	
  }
  

  exit(0);

  return 0;
}


