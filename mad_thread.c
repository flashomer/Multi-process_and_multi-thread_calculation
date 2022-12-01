#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include <pthread.h>

#define BUFFER_SIZE 	25
#define NUMBER_SIZE 	500000
#define READ_END	0
#define WRITE_END	1

#define MAX_THREAD	3

#define SMALL	0
#define BIG	1
#define MEAN	2
#define MAD	3


struct args {
	float* numbers;
	int count;
};



float write_global[BUFFER_SIZE];
float read_global[BUFFER_SIZE]; 

float parentArray[4]; 
float childArray[4]; 

float MeanArray[MAX_THREAD] = {0};
int part = 0;


void* sum_array(void* arg){
	int thread_part = part++;
	//MeanArray[thread_part] += arg;
}


void* Mean(void *input)
{
	int thread_part = part++;
	float sum = 0;
	for (int i = 0; i < ((struct args*)input)->count; i++)
		sum = sum + (((struct args*)input)->numbers)[i];
	
	MeanArray[thread_part] = (sum / ((struct args*)input)->count);
}

float MADFunction(float numbers[], int count, float mean, int n, int c)
{
	float Sum = 0;
	for (int i = n; i < count; i++) 
		Sum = Sum + fabsf(numbers[i] - mean);

	return (Sum / c);
}


float* RangeFunction(float numbers[], int count)
{
	float small=numbers[0],big=numbers[0];
	
	static float Range[2];
	
	for (int i = 0; i<count; i++) {
	    
		if(numbers[i] > big) 	big = numbers[i];  
	  	if(numbers[i] < small)  small = numbers[i];  		
	}
	
	Range[SMALL] = small;
	Range[BIG] = big;

	return Range;
}


void Child(float numbers[], int count, int n){
	
	int c = 0;
	float cNumbers[NUMBER_SIZE];

	for(int j =0; j<n; j++){
		cNumbers[c] = numbers[j];
		c++;
	}
	
	childArray[SMALL] = RangeFunction(cNumbers,c)[SMALL];
	childArray[BIG]   = RangeFunction(cNumbers,c)[BIG];
	//childArray[MEAN]  = Mean(cNumbers,c);
}

void Parent(float numbers[], int count, int n)
{	
	int c = 0;
	float pNumbers[NUMBER_SIZE];

	for(int j =n; j<count; j++){
		pNumbers[c] = numbers[j];
		c++;
	}
		
	parentArray[SMALL] = RangeFunction(pNumbers,c)[SMALL];
	parentArray[BIG]   = RangeFunction(pNumbers,c)[BIG];
	//parentArray[MEAN]  = Mean(pNumbers,c);

}

int main(void)
{
  clock_t start, end;
  start = clock();
  double pTime;
	
  float numbers[NUMBER_SIZE];
  int count=0;
  FILE *file;
  
  struct args *obj = (struct args *)malloc(sizeof(struct args));
  
  pthread_t threads[MAX_THREAD];

  
  if (file = fopen("large.txt", "r"))
  {
    while (fscanf(file, " %f", &numbers[count]) != EOF)
    {
      count++;
    }
    
    fclose(file);
  }
  
  obj->numbers = numbers;
  obj->count = count;

  
  
  for(int i=0; i<MAX_THREAD; i++)
  {
  	pthread_create(&threads[i],NULL,Mean,(void *)obj);
  
  }
  
  for(int i=0; i<MAX_THREAD; i++)
  {
  	pthread_join(threads[i],NULL);
  
  }
  
  for(int i=0; i<MAX_THREAD; i++)
  {
  	printf("Thread: %d Mean: %f \n",i,MeanArray[i]);
  
  }
  
  
  
  
  /*

    
  if (pipe(fd1) == -1 || pipe(fd2) == -1) { fprintf(stderr,"Pipe failed"); return 1;}
  
  pid = fork();

  if (pid < 0) { fprintf(stderr, "Fork failed"); return 1; }

  if (pid > 0) {
  
  	Parent(numbers,count,(count/2));
  	

  	close(fd1[READ_END]); 
	write(fd1[WRITE_END], parentArray, sizeof(parentArray)); 
	close(fd1[WRITE_END]);
	
	wait(NULL);
	
	float read_global2[BUFFER_SIZE]; 
	
	close(fd2[WRITE_END]); 
	read(fd2[READ_END], read_global2, sizeof(read_global2));
	
	float parentMad = MADFunction(numbers, count, fabsf((parentArray[MEAN]+read_global2[MEAN])/2),(count/2),count );
	
	printf("Mad:  %.2f\n", (parentMad + read_global2[MAD]));

	close(fd2[READ_END]);
	
	wait(NULL);
	
	end = clock();
     	pTime = ((double) fabsf(end - start) / CLOCKS_PER_SEC)*1000;
	printf("Execution time for Range and MAD algorithm is  %.3f seconds.\n", pTime);

  } else { 
  
  	Child(numbers,count,(count/2));
  	
 	

	close(fd1[WRITE_END]); 
	read(fd1[READ_END], read_global, sizeof(read_global));
	

	float newRangeArray[4] = {read_global[SMALL],read_global[BIG],childArray[SMALL],childArray[BIG]};
	float* comapreRange;
	comapreRange = RangeFunction(newRangeArray, 4);

	close(fd1[READ_END]);
	
	printf("Range:  %.2f\n", (comapreRange[BIG]-comapreRange[SMALL]) );

	wait(NULL);
	
	childArray[MAD]  = MADFunction(numbers, (count/2), fabsf((read_global[MEAN]+childArray[MEAN])/2),0,count);
	
  	close(fd2[READ_END]); 
	write(fd2[WRITE_END], childArray, sizeof(childArray)); 
	close(fd2[WRITE_END]);	
	
	wait(NULL);
	
  }
  
  */
  
  exit(0);

  return 0;
}


