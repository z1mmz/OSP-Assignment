//
// OSP assignment 2 2016 semester 2
// Created by s3495671 s3495671 on 10/09/16.
//

#include "external442t.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



struct {
	long priority;        //message priority
	int temp;            //temperature
	int pid;               //process id
	int stable;          //boolean for temperature stability
} msgp, cmbox;

//MAIN function
int main(int argc, char *argv[]) {

	/* Validate that a temperature and a Unique process ID was given via the command  */
	if (argc != 4) {
		printf("USAGE: Too few arguments --./central.out Temp UID mailbox");
		exit(0);
	}

	//Setup local variables
	int unstable = 1;
	int result, length, status;
	int initTemp = atoi(argv[1]);
	int uid = atoi(argv[2]);
	int CENTRAL_MAILBOX = atoi(argv[3]);

	//Create the Central Servers Mailbox
	int msqidC = msgget(CENTRAL_MAILBOX, 0600 | IPC_CREAT);

	//Create the mailbox for this process and store it's IDs
	int msqid = msgget((CENTRAL_MAILBOX + uid), 0600 | IPC_CREAT);
	printf("uid %d  mbox %d\n",uid, msqid);
	//Initialize the message to be sent
	cmbox.priority = 1;
	cmbox.pid = uid;
	cmbox.temp = initTemp;
	cmbox.stable = 1;

	/* The length is essentially the size of the structure minus sizeof(mtype) */
	length = sizeof(msgp) - sizeof(long);

	//While all the processes have different temps
	while (unstable == 1) {
		//Send the current temp to the central server
		result = msgsnd(msqidC, &cmbox, length, 0);

		//Wait for a new message from the central server
		result = msgrcv(msqid, &msgp, length, 1, 0);

		//If the new message indicates all the processes have the same temp
		//break the loop and print out the final temperature
		if (msgp.stable == 0) {
			unstable = 0;
			printf("\nProcess %d Temp: %d\n", cmbox.pid, cmbox.temp);
		} else { //otherwise calculate the new temp and store it
			int newTemp = (10 * cmbox.temp + msgp.temp) / 11;
//			printf("temp = %d uid = %d\n",cmbox.temp , uid);
			cmbox.temp = newTemp;
		}
	}

	//Remove the mailbox
	status = msgctl(msqid, IPC_RMID, 0);

	//Validate nothing when wrong when trying to remove mailbox
	if (status != 0) {
		printf("\nERROR closing mailbox\n");
	}
}
