------------------------------------------------------ Ice Cream Factory Problem --------------------------------------------------

COURSE: OPERATING SYSTEM  
OS: LINUX 4.13.1  
VERSION: v1.0  
LANGUAGE: C  

PROJECT OBJECTIVES:

1) Create A System Call
2) Use The System Call In The Project
2) Thread Synchronization
3) Deadlock Handling

* Create A System Call
	[YOUTUBE TUTORIAL] https://www.youtube.com/watch?v=AP-tBd84vbM
	
	// A Manual Is Also Added To Help Around In Updating The Kernel.

* System Call
	A Simple System Call Was Created For Printing The Activities Of A Thread In This Project.
	
	/*[System Call Prototype]*/ long sys_PrintStatement(char *buff ,int number);
	
	// Parameter 1: String To Be Printed
	// Parameter 2: Integer Passed To Be Printed
	// Returns 0

* Makefile
	// Makefile For System Call Is Also Added.

* Thread Synchronization Was Implemented With Semaphores.
* Deadlock Handling Was Implemented With Semaphores.

* Semaphores
	// A Manual Is Added To Help In Implementing Semaphores.

* Threads
	// A Manual Is Added To Help In Implementing Threads.

* [USER-LEVEL] Program
	// The Main Project Also Has Necessary Documentation.

* Problem That May Occur
	// Deadlock May Occur When Number Of Threads To Be Created Is Greater Than The Limit Set In The Project 	
	// (Which Is 30 Threads For Project v1.0).
