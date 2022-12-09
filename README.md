#Multi-process_and_multi-thread_calculation

**Implement three programs**

 **a)** mad_sequential.c ; Basic program

 **b)** mad_process.c ; Program with 2 processes

 **c)** mad_threads.c ; Program with 2,3,4 threads according to the parameter

For process communications you should handle inter-process communication (IPC) between processes by yourself. Please pay an extra attention for communication part which is the most important portion for (b) part.

For multithreaded program you need to take necessary precautions for data consistency for the shared variables in your project if it is necessary.


**Part A:**

gcc mad_sequential.c -o mad_sequential
./mad_sequentail tiny.txt
./mad_sequentail large.txt

![Screenshot](https://github.com/flashomer/Multi-process_and_multi-thread_calculation/blob/main/img/screenshot_1.png)

**Part B:**

gcc mad_process.c -o mad_process
./mad_process tiny.txt
./mad_process large.txt

![Screenshot](https://github.com/flashomer/Multi-process_and_multi-thread_calculation/blob/main/img/screenshot_2.png)

**Part C:**

gcc mad_thread.c -o mad_thread
./mad_thread tiny.txt 3    //for 3 thread
./mad_thread large.txt 4   

![Screenshot](https://github.com/flashomer/Multi-process_and_multi-thread_calculation/blob/main/img/screenshot_3.png)

