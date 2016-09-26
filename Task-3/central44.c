//
// OSP assignment 2 2016 semester 2
// Created by s3495671 s3495671 on 10/09/16.
//

#include "central44.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

// #define CENTRAL_MAILBOX 3495671    //Central Mailbox number
// #define CENTRAL_MAILBOX_2 34956712
#define NUM_PROCESSES 8           //Total number of external processes




//MAIN function
int main(int argc, char *argv[]) {
    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

    //Validate that a temperature was given via the command line
    if (argc != 4) {
        printf("USAGE: Too few arguments --./central.out Temp mailbox1 mailbox2");
        exit(0);
    }

    printf("\nStarting Server...\n");

    //Set up local variables
                             //central process ID
    int initTemp = atoi(argv[1]);        //starting temperature
    //array of process temperatures
    int CENTRAL_MAILBOX = atoi(argv[2]);
    int CENTRAL_MAILBOX_2 = atoi(argv[3]);
	int group1FinalTemp, group2FinalTemp;
    pthread_t tid1, tid2;
	struct args args1,args2;

	args1.mailbox = CENTRAL_MAILBOX;
	args1.startingT = initTemp;
	args1.startingUID = 1;
    pthread_create(&tid1,NULL,run,&args1);
//	group1FinalTemp = run(&args1);


	args2.mailbox = CENTRAL_MAILBOX_2;
	args2.startingT = initTemp;
	args2.startingUID = 5;
//	group2FinalTemp = run(&args2);
    pthread_create(&tid2,NULL,run,&args2);
//
	pthread_join(tid1,&group1FinalTemp);
	pthread_join(tid2,&group2FinalTemp);

    printf("Temperatures Stabilized\nGroup 1 temp %d\nGroup 2 temp %d\n",group1FinalTemp,group2FinalTemp);
    printf("\nShutting down Server...\n");

    //Remove the mailbox

    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("The elapsed time is %fms\n", elapsedTime);

    return 0;
}
void *run(void * arguments) {
	struct {
		long priority;         //message priority
		int temp;             //temperature
		int pid;                //process id
		int stable;            //boolean for temperature stability
	} msgp, cmbox;
	struct args * args_s = arguments;
  int i, result, length, status;             //counter for loops
  int uid = 0;                               //central process ID
  int initTemp = args_s->startingT;        //starting temperature
  int msqid[NUM_PROCESSES];       //mailbox IDs for all processes
  int unstable = 1;          //boolean to denote temp stability
  int tempAry[NUM_PROCESSES];   //array of process temperatures
  int CENTRAL_MAILBOX = args_s->mailbox;
  int startingUID = args_s->startingUID;
  int FinalTemp;



  for (i = startingUID; i <= (NUM_PROCESSES / 2 )+ startingUID -1; i++) {

      msqid[(i - 1)] = msgget((CENTRAL_MAILBOX + i), 0600 | IPC_CREAT);
      printf("Group %d mbox %d\n",startingUID,msqid[(i - 1)] );
  }
    int msqidC = msgget(CENTRAL_MAILBOX, 0600 | IPC_CREAT);
  length = sizeof(msgp) - sizeof(long);

  msgp.priority = 1;
  msgp.pid = uid;
  msgp.temp = initTemp;
  msgp.stable = 1;


  /* The length is essentially the size of the structure minus sizeof(mtype) */
  length = sizeof(msgp) - sizeof(long);

  //While the processes have different temperatures
  while (unstable == 1) {
      int sumTemp = 0;        //sum up the temps as we loop
      int stable = 1;            //stability trap

      // Get new messages from the processes
      for (i = 0; i < NUM_PROCESSES/2; i++) {
          result = msgrcv(msqidC, &cmbox, length, 1, 0);
//		  printf(" group %d cmbox id %d \n",startingUID, cmbox.pid);
          // printf("Group 1\n" );
          /* If any of the new temps are different from the old temps then we are still unstable. Set the
new temp to the corresponding process ID in the array */
          if (tempAry[(cmbox.pid - 1)] != cmbox.temp) {
            // printf("%d\n",cmbox.pid -1 );
              stable = 0;
              tempAry[(cmbox.pid - 1)] = cmbox.temp;
          }

          //  Add up all the temps as we go for the temperature algorithm
          sumTemp += cmbox.temp;
          //  printf("got %d messages \n",i);
      }


      /*When all the processes have the same temp twice: 1) Break the loop 2) Set the messages stable field
to stable*/
      if (stable) {
          printf("Temperature Stabilized: %d\n", msgp.temp);
          FinalTemp = msgp.temp;
          unstable = 0;
          msgp.stable = 0;

      } else { //Calculate a new temp and set the temp field in the message
          int newTemp = (msgp.temp + 1000 * sumTemp) / (1000 * NUM_PROCESSES/2 + 1);
          usleep(100000);
          msgp.stable = 1;
          msgp.temp = newTemp;
          printf("%d The new temp in central is %d\n", startingUID,newTemp);
      }

      /* Send a new message to all processes to inform of new temp or stability */
      for (i = startingUID - 1; i < NUM_PROCESSES/2 + startingUID - 1; i++) {
//         printf("sent message to %d and i = %d\n", msqid[i],i);
          result = msgsnd(msqid[i], &msgp, length, 0);
      }
  }

  status = msgctl(msqidC, IPC_RMID, 0);


  //Validate nothing when wrong when trying to remove mailbox
  if (status != 0) {
      printf("\nERROR closing mailbox\n");
  }
return FinalTemp;
}
