/*********************************************************************
*File: program5.c
*Author: Jose Lamuno
*Procedures:
*main	-test program which initializes data elements, memory, then calls on the memory allocation algorithms. Results are then printed for the user.
**********************************************************************/

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "memoryMake.h"
#include "streamMake.h"
#include "schedAlgorithmsCompact.h"

/*********************************************************************
*void main()
*Author: Jose Lamuno
*Date: April 19 2020
*Description: Generates a task stream, generates a memory, runs allocation algorithms and then prints the results.
**********************************************************************/
void main() 
{ 
    system("clear");	//Clears the terminal for better view

    results Time[4];	//Initialize array to hold times for all four algorithms

    int i = 0;	//Initialize iterator

	for(i=0; i<4; i++){
		Time[i].tr = 0;
		Time[i].relTr = 0;
	}
    
    task* stream = makeStream();   //Initialize task stream to 1000 tasks with random sizes and times

    //Ask the user whether they would like random memory block assignment or predetermined and initialize predetermined memory
    int blocks = 6;		//6 blocks for predetermined memory
    int choice = 0;		//User choice for predetermined or random partitions
    int* memory = malloc(10*sizeof(int));	//Allocates space for a memory of up to 10 partitions

	//Prints two prompts for the user to input their choices
    printf("Enter 0 for randomized fixed allocation memory. \nOr anything else for a predetermined fixed allocation memory.\n");
    scanf("%i", &choice);

	//Checks the user choice for memory
    if(choice < 1){
        memory = randomMem(&blocks);	//Generates a memory with random partitions and partition size
    }
    else{		//Initializes pretermined fixed partition memory if user didnt want random and lets the user know what the memory looks like
        memory[0] = 4;
        memory[1] = 4;
        memory[2] = 8;
        memory[3] = 12;
        memory[4] = 12;
        memory[5] = 16;
        printf("\nMemory allocated with %i blocks of sizes:\n", blocks);
        for(i=0; i<blocks; i++)
            printf("\nBlock %i of size %i", (i+1),memory[i]);
    }

	printf("\n\nLoading... 0%% ");
	fflush(stdout);

    //Main experimental loop that will run 1000 times
	for(i=0; i<1000; i++)
   	{
    	FCFS(stream, memory, blocks, Time);		//Runs the FCFS algorithm with compaction

     	//printf("\nTime elapsed for best fit: %i", Time[0]);

        SPN(stream, memory, blocks, Time);		//Runs the SPN algorithm with compaction

        //printf("\nTime elapsed for first fit: %i", Time[1]);

        RRq1(stream, memory, blocks, Time);		//Runs the RR algorithm with compaction

        //printf("\nTime elapsed for next fit: %i \n", Time[2]);

        //SRT(stream, memory, blocks, Time);		//Runs the SRT algorithm with compaction
        stream = makeStream();		//Makes a new stream with random tasks
		if(i%100 == 0 && i>0){
			printf("\b\b\b%i%%", i/10);
			fflush(stdout);
		}
	}
	
	printf("\b\b\b100%%");

	//Prints the average times for all the algorithms
    printf("\n\nAverage turnaround time for FCFS: %i \n", (Time[0].tr/1000));
    printf("Average relative turnaround time for FCFS: %i \n", (Time[0].relTr/1000));
    printf("Average turnaround time for SPN: %i \n", (Time[1].tr/1000));
    printf("Average relative turnaround time for SPN: %i \n", (Time[1].relTr/1000));
	printf("Average turnaround time for RRq1: %i \n", (Time[2].tr/1000));
    printf("Average relative turnaround time for RRq1: %i \n", (Time[2].relTr/1000));
	printf("Average turnaround time for SRT: %i \n", (Time[1].tr/1000));
    printf("Average relative turnaround time for SRT: %i \n", (Time[1].relTr/1000));
}
