# CE4348-Scheduling-Algorithms

Program to recreate memory scheduling algorithms in a uni-processor environment and measure computing time consumed by each approach. The memory allocation algorithm used for this program is first come first served and compaction is implemented. A fixed partition approach is taken for sake of simplicity. A choice is given to the user as to whether to use predetermined memory partition block sizes or to randomize them.

# Problem definition

The system has 56 units of memory available.

Scheduling is to work as follows:
1. A task is considered to arrive the first time it is inspected for placing in memory.
2. Until a task cannot be placed, place the next task in the stream
3. Increment the time consumed by one.
4. Decrement time remaining for appropriate task in memory by one.
5. If tasks remain in the stream, go back to 2.
6. Continue processing the tasks remaining in memory until memory is empty.\
	a. Increment the time consumed by one.\
	b. Decrement time remaining for appropriate task in memory by one.\

The program runs for 1000 trials and returns the average times obtained in the process. 
	
# Task info stream

The task info stream consists of 1000 elements of type "task", each of which contains a size variable of type int and a time variable of type int. Both of those variables are generated randomly through a normal random number generator which uses the current time as the seed.
