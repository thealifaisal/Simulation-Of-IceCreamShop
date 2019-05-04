#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
// prices of flavours and toppings

#define priceFlav_1 1.05
#define priceFlav_2 2.00
#define priceFlav_3 1.67
#define priceTopp_1 0.8
#define priceTopp_2 0.5

// resources
int ticket = 30, _flavors[3] = {29, 34, 18}, _toppings[2] = {20, 34};
double revenue = 0.0;

// semaphores declaration
sem_t ticketC, flavorC, f1, f2, f3, toppingC, t1, t2, paymentC;

// function prototypes
void *iceCream(void *_id);

// main
int main()
{
	int noC;

	printf("\n\nEnter Number Of Customers [1-%d]: ", ticket);
	scanf("%d", &noC);
	printf("\n\n");
	
	if(noC > ticket || noC <= 0)
	{
		printf("\n\nInvalid Input!\n\n");
		return 0;
	}
	
	int _id[noC];
	
	for(int i=0; i<noC; i++)
	{
		_id[i] = i+100;
	}
	
	// semaphore initialization - START
	sem_init(&ticketC, 0, 1);
	
	sem_init(&flavorC, 0, 3);
	sem_init(&f1, 0, 1);
	sem_init(&f2, 0, 1);
	sem_init(&f3, 0, 1);
	
	sem_init(&toppingC, 0, 2);
	sem_init(&t1, 0, 1);
	sem_init(&t2, 0, 1);
	
	sem_init(&paymentC, 0, 1);
	
	// semaphore initialization - END
	
	
	// multithreading region - START
	pthread_t _customer[noC];
	
	for(int i=0; i<noC; i++)
	{
		pthread_create(&_customer[i], 0, &iceCream, (void*) &_id[i]);
	}
	
	for(int i=0; i<noC; i++)
	{
		pthread_join(_customer[i], NULL);
	}
	// multithreading region - END
	
	
	printf("\n\nBusiness Journal - At Closing\n\n");
	printf("Number Of Customers: %d", noC);
	printf("\nRevenue Generated: $ %f", revenue);
	printf("\nTickets Remaining: %d\n\n", ticket);
	
	
	// semaphore destroying - START
	
	sem_destroy(&ticketC);
	
	sem_destroy(&flavorC);
	sem_destroy(&f1);
	sem_destroy(&f2);
	sem_destroy(&f3);
	
	sem_destroy(&toppingC);
	sem_destroy(&t1);
	sem_destroy(&t2);
	
	sem_destroy(&paymentC);
	
	// semaphore destroying - END
	
	return 0;
}
void *iceCream(void *_id)
{
	int _ID = *(int*)_id, checkRaceCond_1 = 0;
	double bill = 0.0;
	
	// ticket counter - ENTER
	sem_wait(&ticketC);
	
	if(ticket <= 0)
	{
		//printf("\nCustomer[%d]: Leaving Shop. [REASON]: Tickets Finished\n", _ID);
		syscall(333,"Leaving Shop. [REASON]: Tickets Finished\n",_ID);
		sleep(1);
		pthread_exit(NULL);
	}
	
	ticket--;
	//printf("Customer[%d] Got Ticket.\n", _ID);
	syscall(333,"Got Ticket.\n",_ID);
	
	sem_post(&ticketC);
	// ticket counter - EXIT
	
	sleep(2);
	
	// flavours counter - ENTER
	sem_wait(&flavorC);
	
	// Race Condition will occur here but it will be handled through thread Local Variable
	// [REASON]: when this condition is checked by 4th thread
	//           and then _flavours[i] is decremented by any of first 2 threads
	//           a thread will proceed even though flavors have been finished.
	// WHY THIS CONDITION IS USED WHEN ITS WORK IS DONE BY LOCAL THREAD VARIABLE?
	// [REASON]: If this is not used then a thread will check each condition below
	//           which will be time consuming.
	if(_flavors[0] <= 0 && _flavors[1] <= 0 && _flavors[2] <= 0)
	{
		syscall(333,"Leaving Shop. [REASON]: Flavours Finished\n", _ID);
		sleep(1);
		pthread_exit(NULL);
	}
	else
	{
		// flavor1
		sem_wait(&f1);
		
		if(_flavors[0] > 0)
		{
			_flavors[0]--;
			checkRaceCond_1++;
			bill = bill + priceFlav_1;
			//printf("\nCustomer[%d]: Got Flavour[0].\n", _ID);
			syscall(333,"Got Flavour[0].\n",_ID);
			sleep(1);
		}
		
		sem_post(&f1);
		
		// flavor2
		sem_wait(&f2);
		
		if(_flavors[1] > 0)
		{
			_flavors[1]--;
			checkRaceCond_1++;
			bill = bill + priceFlav_2;
			//printf("Customer[%d]: Got Flavour[1].\n", _ID);
			syscall(333,"Got Flavour[1].\n",_ID);
			sleep(1);
		}
		
		sem_post(&f2);
		
		// flavor3
		sem_wait(&f3);
		
		if(_flavors[2] > 0)
		{
			_flavors[2]--;
			checkRaceCond_1++;
			bill = bill + priceFlav_3;
			//printf("Customer[%d]: Got Flavour[2].\n", _ID);
			syscall(333,"Got Flavour[2].\n",_ID);
			sleep(1);
		}
		
		sem_post(&f3);
		
		// if any of the threads did not get any flavour, 
		// 'checkRaceCond_1' will remain 0, 
		// threads will exit
		if(checkRaceCond_1 == 0)
		{
			//printf("\nCustomer[%d]: Leaving Shop. [REASON]: Flavours Finished\n", _ID);
			syscall(333,"Leaving Shop. [REASON]: Flavours Finished\n",_ID);
			sleep(1);
			pthread_exit(NULL);
		}
	}
	
	//printf("\nCustomer[%d]: Got Flavour(s). Leaving Flavor Counter\n", _ID);
	syscall(333,"Got Flavour(s). Leaving Flavor Counter\n",_ID);
	//sleep(2);
	
	sem_post(&flavorC);
	// flavors counter - EXIT
	
	
	// toppings counter - ENTER
	sem_wait(&toppingC);
	
	// topping1
	sem_wait(&t1);
	
	if(_toppings[0] > 0)
	{
		_toppings[0]--;
		bill = bill + priceTopp_1;
	}
	
	sem_post(&t1);
	
	// topping2
	sem_wait(&t2);
	
	if(_toppings[1] > 0)
	{
		_toppings[1]--;
		bill = bill + priceTopp_2;
	}
	
	sem_post(&t2);
	
	//printf("\nCustomer[%d]: Leaving Topping Counter.\n", _ID);
	syscall(333,"Leaving Topping Counter.\n",_ID);
	
	sem_post(&toppingC);
	// toppings counter - EXIT
	
	sleep(2);
	
	// payments counter - ENTER
	sem_wait(&paymentC);
	
	revenue = revenue + bill;
	
	//printf("\nCustomer[%d]: Billed: $ %f.\n", _ID, bill);
	char a[100];
	sprintf(a,"Billed: $ %f.\n",bill);
	syscall(333,a,_ID);	

	sem_post(&paymentC);
	// payments counter - EXIT
	
	sleep(2);
	
	//printf("\nCustomer[%d]: Leaving Ice-Cream Shop.\n\n", _ID);
	syscall(333,"Leaving Ice-Cream Shop.\n\n",_ID);
	
	return NULL;
}
