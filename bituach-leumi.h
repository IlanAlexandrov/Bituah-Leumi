/*
 * BituachLeumi.h
 *
 *  Created on: Jun 8, 2020
 *      
 */

#ifndef BITUACHLEUMI_H_
#define TM_BITUACHLEUMI_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include<string.h>
#include <semaphore.h>
#include <sys/types.h>



/*
 * TM_CLOCK_TICK - the basic clock step (in microseconds)
 *
 * Relative times are multiplied by this factor to yield real running times
 */
#define TM_CLOCK_TICK  1000

/**
 * Customer is a struct for keeping all customer related information.
 */
typedef struct customer
{
    int          id;
    int          service;

    int          sort_time;//if stand in line,
    int          service_time; //how much it will take to take care

    struct timeval       enter_time;
    struct timeval       exit_time;
    int          wait_time;
    int          total_time;
    struct customer *next;
}
    Customer,
   *pCustomer;


/*
 * Clerk is a struct for keeping all clerk related information
 */
typedef struct clerk
{
    int       id;
    pthread_t tid;
    int       service;
    int       work_time;
    int 		wait_time;
}
Clerk, *pClerk;

//
typedef struct queue
{
	pCustomer head;
	pCustomer end;

}qu;
typedef struct station
{
	char name[20];
	int state;
	sem_t area;
	sem_t empty;
	struct queue q;


}St;


#endif /* BITUACHLEUMI_H_ */
