/*********************************************************************
*File: schedAlgorithmsCompact.h
*Author: Jose Lamuno
*Procedures:
*FCFS		-Function to run first-come-first-served algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
*SPN		-Function to run shortest-process-next on a specified task stream into a memory partitioned to blocks amount of blocks
*RRq1		-Function to run round-robin on a specified task stream into amemory partitioned to blocks amount of blocks
*SRT		-Function to run shortest-remaining-time algorithm on a specified task stream into a memory partitioned to blocks amount of blocks
**********************************************************************/
#ifndef SCHEDALGORITHMSCOMPACT_H
#define SCHEDALGORITHMSCOMPACT_H

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
void FCFS (task stream[], int memory[], int blocks, results *Time);

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
void SPN (task stream[], int memory[], int blocks, results *Time);

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
void RRq1 (task stream[], int memory[], int blocks, results *Time);

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
void SRT (task stream[], int memory[], int blocks, results *Time);

#endif
