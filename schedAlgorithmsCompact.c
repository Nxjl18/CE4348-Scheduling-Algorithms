/*********************************************************************
*File: schedAlgorithmsCompact.c
*Author: Jose Lamuno
*Procedures:
*FCFS		-Function to run first-come-first-served algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*SPN		-Function to run shortest-process-next on a specified task stream into a memory partitioned to blocks amount of blocks
*RRq1		-Function to run round-robin on a specified task stream into amemory partitioned to blocks amount of blocks
*SRT		-Function to run shortest-remaining-time algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>
#include "streamMake.h"
#include "schedAlgorithmsCompact.h"

/*********************************************************************
*void FCFS (task stream[], int memory[], int blocks, results *Time)
*Author: Jose Lamuno
*Date: May 1 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns turnaround time and relative turnaround time. This version performs compaction and the first-come-first-served scheduling algorithm.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
*	Time[]		O/P		struct results	The array which will hold turnaround time and relative turnaround time
**********************************************************************/
void FCFS (task stream[], int memory[], int blocks, results *Time)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	int current = 0;			//Tracks which task has execution priority
	int tr,relTr;
	int tasksFinished = 0;
	tr = 0;
	relTr = 0;
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;			//Tracks whether the block is in use
        track[i].taskTime = 0;		//Tracks how much time the process in the block has left
        track[i].taskNum = 0;		//Tracks the task number of the process inside the block
		track[i].block = 0;			//Tracks which block the task is in
		track[i].activeTime = 0;	//Tracks the processing time
		track[i].waitTime = 0;		//Tracks the wait time
		track[i].arrivalTime = 0;
		track[i].completionTime = 0;
    }

	//Main loop to place tasks into memory
	for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i+1);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){	//If the task has not been placed..
                    track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
					track[i].activeTime = stream[i].time;	//Tracks burst time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed
					track[i].arrivalTime = timeConsumed;

                    //printf("\nTask %i placed into block %i with time %i", i+1, j+1, track[i].taskTime);

                    break;
                }
            }
        }

		if(taskPlaced < 1){		//If a task was not placed...
            //printf("\nTask %i not placed with time %i", i+1, stream[i].time);
			i -= 1;				//Reduces i to retry placing the failed task

			//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

			while(track[current].taskTime > 0){

				//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            	for(n=0; n<1000; n++){	
					if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                	if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    	track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
						timeConsumed += 1;	//Increases time consumed

                    	//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                	}
                	if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    	track[n].used = 0;								//Remove the task from the block
						temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
						track[n].completionTime = timeConsumed;
						tasksFinished += 1;
						track[n].trTime = timeConsumed - track[n].arrivalTime;

                    	//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
						//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                	}
            	}
			}
			current += 1;
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
			//printf("Task 1000 has been placed. Tasks finished until now %i", tasksFinished);
			while(tasksFinished != 1000){

				//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

				while(track[current].taskTime > 0){

					//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            		for(n=0; n<1000; n++){	
						if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                		if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    		track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
							timeConsumed += 1;	//Increases time consumed

                    		//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                		}
                		if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    		track[n].used = 0;								//Remove the task from the block
							temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
							track[n].completionTime = timeConsumed;
							tasksFinished += 1;
							track[n].trTime = timeConsumed - track[n].arrivalTime;

                    		//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
							//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                		}
            		}
				}
				current += 1;
			}
		}
    }
	
	for(n=0; n<1000; n++){
		tr += track[n].trTime;
		relTr += track[n].trTime/track[n].activeTime;
	}
	
	Time[0].tr += tr/1000;
	Time[0].relTr += relTr/1000;
}

/*********************************************************************
*void SPN (task stream[], int memory[], int blocks, results *Time)
*Author: Jose Lamuno
*Date: May 1 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns turnaround time and relative turnaround time. This version performs compaction and the shortest-process-next scheduling algorithm.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
*	Time[]		O/P		struct results	The array which will hold turnaround time and relative turnaround time
**********************************************************************/
void SPN (task stream[], int memory[], int blocks, results *Time)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	int current = 0;			//Tracks which task has execution priority
	int tr,relTr;
	tr = 0;
	relTr = 0;
	int smallest = 17;
	int tasksFinished = 0;
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;			//Tracks whether the block is in use
        track[i].taskTime = 0;		//Tracks how much time the process in the block has left
        track[i].taskNum = 0;		//Tracks the task number of the process inside the block
		track[i].block = 0;			//Tracks which block the task is in
		track[i].activeTime = 0;	//Tracks the processing time
		track[i].waitTime = 0;		//Tracks the wait time
		track[i].arrivalTime = 0;
		track[i].completionTime = 0;
    }

	//Main loop to place tasks into memory
	for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i+1);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){	//If the task has not been placed..
                    track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
					track[i].activeTime = stream[i].time;	//Tracks burst time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed
					track[i].arrivalTime = timeConsumed;
                    
					//printf("\nTask %i placed into block %i at time %i with duration %i", i+1, j+1, timeConsumed, track[i].taskTime);

                    break;
                }
            }
        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...
            //printf("\nTask %i not placed with time %i", i+1, stream[i].time);
			i -= 1;				//Reduces i to retry placing the failed task

			for(n=0; n<1000; n++){
				if(track[n].activeTime < smallest && track[n].used > 0){
					smallest = track[n].activeTime;
					current = n;
					//printf("\nCurrently the smallest task is %i with %i time", n+1, track[n].activeTime);
				}
			}

			//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

			while(track[current].taskTime > 0){

				//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            	for(n=0; n<1000; n++){	
					if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                	if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    	track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
						timeConsumed += 1;	//Increases time consumed

                    	//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                	}
                	if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    	track[n].used = 0;								//Remove the task from the block
						temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
						track[n].completionTime = timeConsumed;
						tasksFinished += 1;
						track[n].trTime = timeConsumed - track[n].arrivalTime;
						smallest = 17;

                    	//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
						//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                	}
            	}
			}
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
			//printf("Task 1000 has been placed. Tasks finished until now %i", tasksFinished);
			while(tasksFinished != 1000){
				for(n=0; n<1000; n++){
					if(track[n].activeTime < smallest && track[n].used > 0){
						smallest = track[n].activeTime;
						current = n;
						//printf("\nCurrently the smallest task is %i with %i time", n+1, track[n].activeTime);
					}
				}

				//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

				while(track[current].taskTime > 0){

					//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            		for(n=0; n<1000; n++){	
						if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                		if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    		track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
							timeConsumed += 1;	//Increases time consumed

                    		//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                		}
                		if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    		track[n].used = 0;								//Remove the task from the block
							temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
							track[n].completionTime = timeConsumed;
							tasksFinished += 1;
							smallest = 17;
							track[n].trTime = timeConsumed - track[n].arrivalTime;

                    		//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
							//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                		}
            		}
				}
			}
		}
    }
	
	for(n=0; n<1000; n++){
		tr += track[n].trTime;
		relTr += track[n].trTime/track[n].activeTime;
	}
	
	Time[1].tr += tr/1000;
	Time[1].relTr += relTr/1000;
}

/*********************************************************************
*void RRq1 (task stream[], int memory[], int blocks, results *Time)
*Author: Jose Lamuno
*Date: May 1 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns turnaround time and relative turnaround time. This version performs compaction and the round-robin scheduling algorithm.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
*	Time[]		O/P		struct results	The array which will hold turnaround time and relative turnaround time
**********************************************************************/
void RRq1 (task stream[], int memory[], int blocks, results *Time)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	int taskDone = 0;			//Tracks which task has execution priority
	int tr,relTr;
	int tasksFinished = 0;
	tr = 0;
	relTr = 0;
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;			//Tracks whether the block is in use
        track[i].taskTime = 0;		//Tracks how much time the process in the block has left
        track[i].taskNum = 0;		//Tracks the task number of the process inside the block
		track[i].block = 0;			//Tracks which block the task is in
		track[i].activeTime = 0;	//Tracks the processing time
		track[i].waitTime = 0;		//Tracks the wait time
		track[i].arrivalTime = 0;
		track[i].completionTime = 0;
    }

	//Main loop to place tasks into memory
	for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i+1);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){	//If the task has not been placed..
                    track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
					track[i].activeTime = stream[i].time;	//Tracks burst time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed
					track[i].arrivalTime = timeConsumed;

                    //printf("\nTask %i placed into block %i with time %i", i+1, j+1, track[i].taskTime);

                    break;
                }
            }
        }

		if(taskPlaced < 1){		//If a task was not placed...
            //printf("\nTask %i not placed with time %i", i+1, stream[i].time);
			i -= 1;				//Reduces i to retry placing the failed task

			//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

			while(taskDone < 1){

				//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            	for(n=0; n<1000; n++){	
                	if(track[n].used > 0 && track[n].taskTime > 0){		//If the block is in use
                    	track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
						timeConsumed += 1;	//Increases time consumed

                    	//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                	}
                	if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    	track[n].used = 0;								//Remove the task from the block
						temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
						track[n].completionTime = timeConsumed;
						tasksFinished += 1;
						taskDone = 1;
						track[n].trTime = timeConsumed - track[n].arrivalTime;

                    	//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
						//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                	}
            	}
			}
			taskDone = 0;
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
			//printf("Task 1000 has been placed. Tasks finished until now %i", tasksFinished);
			while(tasksFinished != 1000){

				//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

				while(taskDone < 1){

					//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            		for(n=0; n<1000; n++){	
                		if(track[n].used > 0 && track[n].taskTime > 0){		//If the block is in use
                    		track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
							timeConsumed += 1;	//Increases time consumed

                    		//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                		}
                		if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    		track[n].used = 0;								//Remove the task from the block
							temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
							track[n].completionTime = timeConsumed;
							tasksFinished += 1;
							taskDone = 1;
							track[n].trTime = timeConsumed - track[n].arrivalTime;

                    		//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
							//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                		}
            		}
				}
				taskDone = 0;
			}
		}
    }
	
	for(n=0; n<1000; n++){
		tr += track[n].trTime;
		relTr += track[n].trTime/track[n].activeTime;
	}
	
	Time[2].tr += tr/1000;
	Time[2].relTr += relTr/1000;
}

/*********************************************************************
*void SRT (task stream[], int memory[], int blocks, results *Time)
*Author: Jose Lamuno
*Date: May 1 2020
*Description: Loads a task stream into a given fixed partition memory through first fit choice while counting the time. It then returns turnaround time and relative turnaround time. This version performs compaction and the shortest-remaining-time scheduling algorithm.
*
*Parameters:
*	stream[]	I/P		struct task		The array which contains all tasks
*	memory[]	I/P		int				The memory upon which to place tasks
*	blocks		I/P		int				The number of blocks within the memory
*	Time[]		O/P		struct results	The array which will hold turnaround time and relative turnaround time
**********************************************************************/
void SRT (task stream[], int memory[], int blocks, results *Time)
{
	//Initializes iterators and variables for tracking
    int timeConsumed = 0;		//Returned variable. Tracks time spent
    int i,j,n;					//Iterators
    int taskPlaced = 0;			//Tracks whether a task has been placed
	int current = 0;			//Tracks which task has execution priority
	int tr,relTr;
	tr = 0;
	relTr = 0;
	int smallest = 17;
	int tasksFinished = 0;
	
	int temp[blocks];			//Initializes a temporary location for memory

	for(i=0; i<blocks; i++)		//Copies the memory and its blocks to a temporary location
		temp[i] = memory[i];

	//Initializes a tracker with various information regarding blocks in use, where processes are in memory and how much time they have left
    tracker track[1000];
    
	//Initializes blocks amount of trackers for all possible blocks
    for(i=0; i<1000; i++){
        track[i].used = 0;			//Tracks whether the block is in use
        track[i].taskTime = 0;		//Tracks how much time the process in the block has left
        track[i].taskNum = 0;		//Tracks the task number of the process inside the block
		track[i].block = 0;			//Tracks which block the task is in
		track[i].activeTime = 0;	//Tracks the processing time
		track[i].waitTime = 0;		//Tracks the wait time
		track[i].arrivalTime = 0;
		track[i].completionTime = 0;
    }

	//Main loop to place tasks into memory
	for(i=0; i<1000; i++){
        taskPlaced=0;		//Sets the task placed flag to 0

        //printf("\nTrying to place task %i", i+1);

		//Loop to check all blocks in memory for a fit
        for(j=0; j<blocks; j++){
            if(stream[i].size <= temp[j]){				//If the block is the same size or larger than the process size it moves on to the next step
                if(taskPlaced < 1){	//If the task has not been placed..
                    track[i].taskNum = i;					//Places the task into block j
                    track[i].taskTime = stream[i].time;		//Tracks remaining task time
					track[i].activeTime = stream[i].time;	//Tracks burst time
                    track[i].used = 1;						//Marks the block as in use
					track[i].block = j;						//Tracks the block in which task i was assigned
					temp[j] -= stream[i].size;				//Makes the block size memory smaller according to task i size
                    taskPlaced = 1;							//Marks the task as placed
					track[i].arrivalTime = timeConsumed;
                    
					//printf("\nTask %i placed into block %i at time %i with duration %i", i+1, j+1, timeConsumed, track[i].taskTime);

                    break;
                }
            }
        }

		//If statement to check if a task was placed or not
        if(taskPlaced < 1){		//If a task was not placed...
            //printf("\nTask %i not placed with time %i", i+1, stream[i].time);
			i -= 1;				//Reduces i to retry placing the failed task

			for(n=0; n<1000; n++){
				if(track[n].activeTime < smallest && track[n].used > 0){
					smallest = track[n].activeTime;
					current = n;
					//printf("\nCurrently the smallest task is %i with %i time", n+1, track[n].activeTime);
				}
			}

			//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

			while(track[current].taskTime > 0){

				//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            	for(n=0; n<1000; n++){	
					if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                	if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    	track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
						timeConsumed += 1;	//Increases time consumed

                    	//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                	}
                	if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    	track[n].used = 0;								//Remove the task from the block
						temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
						track[n].completionTime = timeConsumed;
						tasksFinished += 1;
						track[n].trTime = timeConsumed - track[n].arrivalTime;
						smallest = 17;

                    	//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
						//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                	}
            	}
			}
        }

		//If all tasks have been placed into memory...
        if(i == 999 && taskPlaced >0){
			//printf("Task 1000 has been placed. Tasks finished until now %i", tasksFinished);
			while(tasksFinished != 1000){
				for(n=0; n<1000; n++){
					if(track[n].activeTime < smallest && track[n].used > 0){
						smallest = track[n].activeTime;
						current = n;
						//printf("\nCurrently the smallest task is %i with %i time", n+1, track[n].activeTime);
					}
				}

				//printf("\nNow serving task %i which has waited for %i", current+1, track[current].waitTime);

				while(track[current].taskTime > 0){

					//Loop to decrement remaining time for tasks in memory and remove them if time is now 0. After removing them free up the space used in the block
            		for(n=0; n<1000; n++){	
						if(track[n].used >0 && track[n].taskNum != current)
							track[n].waitTime += 1;
                		if(track[n].used > 0 && track[n].taskNum == current){		//If the block is in use
                    		track[n].taskTime -= 1;	//Reduce the time of the task in the block by 1
							timeConsumed += 1;	//Increases time consumed

                    		//printf("\nTask %i is in block %i with %i time left", track[n].taskNum+1, n+1, track[n].taskTime);

                		}
                		if(track[n].taskTime < 1 && track[n].used > 0){		//If the block is in use and the time remaining is 0
                    		track[n].used = 0;								//Remove the task from the block
							temp[track[n].block] += stream[n].size;			//Restore the memory that was being used by the task
							track[n].completionTime = timeConsumed;
							tasksFinished += 1;
							smallest = 17;
							track[n].trTime = timeConsumed - track[n].arrivalTime;

                    		//printf("\nTask %i removed from memory at time %i with a relative time of %i", track[n].taskNum+1, timeConsumed, track[n].trTime);
							//printf("\nBlock %i now has %i space left", track[n].block+1, temp[track[n].block]);       
         
                		}
            		}
				}
			}
		}
    }
	
	for(n=0; n<1000; n++){
		tr += track[n].trTime;								//Calculates turnaround time
		relTr += track[n].trTime/track[n].activeTime;		//Calculates relative turnaround time
	}
	
	Time[3].tr += tr/1000;									//Returns turnaround time to the time variable
	Time[3].relTr += relTr/1000;							//Returns relative turnaround time to the time
}
