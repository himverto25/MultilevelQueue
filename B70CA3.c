// Name: Himanshu Goel
// Registration Number: 12102678
// Section: K21ZN
// Roll Number: B70
// Course: Operating Systems
// Course Code: CSE316 
// Academic Task: CA3  
// Question Assigned: Question number 8th

/*
Question Description:
Write a program for multilevel queue scheduling algorithm.
There must be three queues generated. There must be specific
range of priority associated with every queue. Now prompt the
user to enter number of processes along with their priority 
and burst time. Each process must occupy the respective queue
with specific priority range according to its priority.
Apply Round Robin algorithm with quantum time 3 on queue 
with highest priority range. Apply priority scheduling
algorithm on the queue with medium range of priority and 
SJF algorithm on the queue with lowest range of priority.
Each and every queue should get a quantum time of 10 seconds.
CPU will keep on shifting between queues after every 10 seconds.
*/

// ---------------------------Multilevel Queue Algorithm--------------------------------------------

#include<stdio.h>
#include<windows.h>
#include<limits.h>
#include<unistd.h>
#include <stdlib.h>
#include<stdbool.h>

// Defining Process class with the attributes of the process as data members
typedef struct process_structure
{   
	int process_ID;
	int priority;
	int burst_time;
	int waiting_time;
	int turnaround_time;
	int remaining_time;
	int arrival_time;
	int completion_time;
} process ;

// Declaring prototypes for the functions to be used:

void get_UserInput();
void compute_WaitingTime(process *q,int);
void compute_TAT(process *q,int);
void print_QueueFormat(process *q,int size);
void RoundRobin();
void Priority_Scheduling();
void SJF();
void print_Process_Data(process);
void print_QueueFormat(process *,int);

/* declaring 2 global variables to store the waiting time and turn-around time
   to calculate the overall average waiting time and turn-around time
*/
float sum_waitingtime;
float sum_turnaroundtime;

// Description of Queues

// Priority Ranges:
// Highest Priority : >6
// Intermediate Priority: 4<=priority<=6
// Lowest Priority: 1<=priority<=3

//Q1 = Batch Processes having Lowest Priority to be implemented using Shortest Job First Algorithm
//Q2 = Interactive Processes having Medium Level of Priority to be implemented using Priority Scheduling Algorithm
//Q3 = System Processes having Highest Priority with Round Robin Algorithm Quantum Time=3

int queue1=0; // count of processes in Q1 with lowest priority
int queue2=0; // count of processes in Q2 with intermediate priority
int queue3=0; // count of processes in Q3 wth highest priority
int n; // n = Total number of processes

process *q1,*q2,*q3;  // declaring uninitialised queues

int time_quantum = 3; // Time Quantum for Round-Robbin Algorithm = 3 time units

// Function to take input from user
void get_UserInput()
{
	printf("\nEnter Total Number of Processes to be entered into CPU:\n");
	scanf("%d",&n);

	q1 = (process *)malloc(n*sizeof(process));
	q2 = (process *)malloc(n*sizeof(process));
	q3 = (process *)malloc(n*sizeof(process));

	for(int i=0;i<n;i++){
		process p;
		printf("Enter priority and burst time for process %d: \n",(i+1));
		scanf("%d",&p.priority);
		scanf("%d",&p.burst_time);
		p.arrival_time = 0;
		p.process_ID = i+1;
		p.remaining_time = p.burst_time;
		if(p.priority>0 && p.priority<=3)
		{
			q1[queue1++]  = p;
		}
		else if(p.priority>3 && p.priority<=6)
		{
			q2[queue2++] = p;
		}
		else
		{
			q3[queue3++] = p;
		}		
	}
}
// Function to print the format of the queue while displaying it and all the caller function
// of the print_Process_Data compute_WaitingTime and compute_TAT functions

void print_queue(process *q,int size)
{	
	printf("\nPId\t\tPriority\t\tBurst Time\t\tWaiting Time\t\tTurnAround Time\t\tArrival");    
	printf("\n======================================================================================================================================\n");
	for(int i=0;i<size;i++){
		print_Process_Data(q[i]); // calling print_Process_Data function 
	}
	printf("\n\n");
}

// Function to print process data 
void print_Process_Data(process p)
{
	printf("\n%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d",p.process_ID,p.priority,p.burst_time,p.waiting_time,p.turnaround_time,p.arrival_time);
}

// Function to compute the waiting time of a process
void compute_WaitingTime(process *q,int size)
{
	q[0].waiting_time = 0;
	for(int i=1;i<size;i++)
	{
		q[i].waiting_time = q[i-1].waiting_time + q[i-1].burst_time; // standard computing paradigm
	}
}

// Function to compute the turnaround time of a process
void compute_TAT(process *q,int size)
{
	q[0].waiting_time = 0;
	for(int i=0;i<size;i++)
	{
		q[i].turnaround_time = q[i].waiting_time + q[i].burst_time; // standard formula
	}
}

// Function to simulate the Round-Robbin Algorithm
void RoundRobinAlgo(process *q,int size)
{
	int timeQuantum=3;
    int currentTime = 0;
	int remainingq3=size;
    currentTime = 0;
    for(int i=0;i<size;i=(i+1)%size)
    {
        if(q3[i].remaining_time > 0)
        {
        	if(q3[i].remaining_time <= timeQuantum)
            {
        		currentTime += q3[i].remaining_time;
        		q3[i].completion_time = currentTime;
        		q3[i].remaining_time = 0;
                remainingq3--;
        	}
        	else
            {
        		currentTime += timeQuantum;
        		q3[i].remaining_time -= timeQuantum;
        	}
        }

        if(remainingq3 == 0)
        {
            break;
        }
    }
}

// Function to call RoundRobbinAlgo and printing queue format
void RoundRobin()
{
	printf("\n\n======================================================================================================================================");
	printf("\n\t\t\t\t\t\t\t\tRound-Robin Scheduling Algorithm\t");
	printf("\n======================================================================================================================================\n\n");
	RoundRobinAlgo(q3,queue3);
	float sumwaiting_time = 0;
    float sumturnaround_time = 0;
    int size = queue3;
    for(int i=0;i<size;i++)
    {
        q3[i].turnaround_time = q3[i].completion_time;
		q3[i].waiting_time=q3[i].turnaround_time - q3[i].burst_time;
        sumwaiting_time += q3[i].waiting_time;
		sum_waitingtime+=q3[i].waiting_time; // updating global average waiting time
        sumturnaround_time += q3[i].turnaround_time;
		// updating global average turn-around time
		sum_turnaroundtime += q3[i].turnaround_time;
		n--;
    }
	print_queue(q3,queue3);
	printf("Average Turn-Around Time: %f\n",(sumturnaround_time/size));
	printf("Average Waiting Time:  %f\n",(sumwaiting_time/size));
}

// Function to simulate Priority Scheduling Algorithm
void Priority_SchedulingAlgorithm(process *q,int size)
{
    int currentTime = 0;

    while(true){

    	int currentHighestPriorityIndex = -1;
    	int currentHighestPriority = INT_MAX;

    	bool isAllCompleted = true;

    	for(int i=0;i<size;i++){
    		if(q2[i].remaining_time > 0){
    			isAllCompleted = false;
                    if(q2[i].priority < currentHighestPriority){
                        currentHighestPriority = q2[i].priority;
                        currentHighestPriorityIndex = i;
                    }   			
    		}
    	}

    	if(isAllCompleted){
    		break;
    	}

    	q2[currentHighestPriorityIndex].remaining_time--;
        currentTime++;

		if(q2[currentHighestPriorityIndex].remaining_time == 0){
			q2[currentHighestPriorityIndex].completion_time = currentTime;
		}
    }
}

// Function to print queue format and to call Priority_SchedulingAlgorithm

void Priority_Scheduling()
{
	printf("\n\n======================================================================================================================================");
	printf("\n\t\t\t\t\t\t\t\tPriority Scheduling Algorithm\t");
	printf("\n======================================================================================================================================\n\n");
	Priority_SchedulingAlgorithm(q2,queue2);
	float sumwaiting_time = 0;
    float sumturnaround_time = 0;
    int size = queue2;
    for(int i=0;i<size;i++)
    {
        q2[i].turnaround_time = q2[i].completion_time - q2[i].arrival_time;
        q2[i].waiting_time = q2[i].turnaround_time - q2[i].burst_time;
        sumwaiting_time += q2[i].waiting_time;
		sum_waitingtime+=q2[i].waiting_time;
        sumturnaround_time += q2[i].turnaround_time;
		sum_turnaroundtime+=q2[i].turnaround_time;
		n--;
    }
	print_queue(q2,queue2);
	printf("Average Turn-Around Time: %f\n",(sumturnaround_time/size));
	printf("Average Waiting Time:  %f\n",(sumwaiting_time/size));
}

// Function to simulate Shortest Job Remaining Algorithm
void SJFAlgorithm(process *q,int size)
{   
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++){
			if(q[j].burst_time>q[i].burst_time)
            {
				process t = q[i];
				q[i] = q[j];
				q[j] = t;
			}
		}
	}
}

// Function to call SJFAlgorithm and to print the queue format
void SJF()
{
	printf("\n\n======================================================================================================================================");
	printf("\n\t\t\t\t\t\t\tShortest Job First Algorithm\t");
	printf("\n======================================================================================================================================\n\n");
	SJFAlgorithm(q1,queue1);
	compute_WaitingTime(q1,queue1);  // calling compute_WaitingTime function
	compute_TAT(q1,queue1);
	float sumwaiting_time = 0;
    float sumturnaround_time = 0;
    int size = queue1;
    for(int i=0;i<size;i++)
    {
        sumwaiting_time += q1[i].waiting_time;
		sum_waitingtime+=q1[i].waiting_time;
        sumturnaround_time += q1[i].turnaround_time;
		sum_turnaroundtime+=q1[i].turnaround_time;
		n--;
    }
	print_queue(q1,queue1);
    printf("Average Turn-Around Time: %f\n",(sumturnaround_time/size));
	printf("Average Waiting Time:  %f\n",(sumwaiting_time/size));
}

// The main function

int main()
{   
    printf("==============================================Multi-Level Queue Simulation======================================\n");
	get_UserInput(); // calling get_UserInput function 
	int number_of_processes = n ; // n: global variable
	while(n>0)
	{
		SJF();
		Priority_Scheduling();
		RoundRobin();
		sleep(10); // a simulation based hypothetical implementation
	}
	printf("\n");
	printf("======================================================================================================================================");
	printf("\n");
	printf("Overall Average Turn-Around Time: %f\n",(sum_turnaroundtime/number_of_processes));
	printf("Overall Average Waiting Time: %f\n",(sum_waitingtime/number_of_processes));
	printf("\n\n");
}