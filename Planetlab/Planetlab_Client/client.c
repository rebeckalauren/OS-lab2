/*********************************************
* client.c
*
* Author: mle@mdh.se
* Desc: lab-skeleton for the client side of an
* client-server application
* Date: 990309
*
* Revised 011018 by Dag Nyström & Juka Mäki-Turja
* NOTE: the server must be started BEFORE the
* client.
*********************************************/

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "wrapper.h"
LPTSTR Slot = TEXT("\\\\.\\mailslot\\sample_mailslot");
#define MESSAGE "Hej på dig!"

void main(void) 
{
	HANDLE mailSlot;
	DWORD bytesWritten;
	int loops = 2000;

	mailSlot = mailslotConnect(Slot); 

	while (mailSlot == INVALID_HANDLE_VALUE) 
	{
		printf("Failed to get a handle to the mailslot!!\nHave you started the server?\n");
		mailSlot = mailslotConnect(Slot); 
	}

	/* NOTE: replace code below for sending planet data to the server. */
	
	while(loops-- > 0) 
	{
		/* send a friendly greeting to the server */
		/* NOTE: The messages sent to the server need not to be of equal size.       */
		/* Messages can be of different sizes as long as they don't exceed the       */
		/* maximum message size that the mailslot can handle (defined upon creation).*/
		
		
		struct pt *planet = (struct pt*)malloc(sizeof(struct pt));  // Malloc = Allocates a block of size bytes of memory
		strcpy_s(planet->name, sizeof(planet->name), "Första planeten");
		planet->life = 10;
		/* 
		typedef struct pt 
		{
			char		name[20];	// Name of planet
			double		sx;			// X-axis position
			double		sy;			// Y-axis position
			double		vx;			// X-axis velocity
			double		vy;			// Y-axis velocity
			double		mass;		// Planet mass
			struct pt*	next;		// Pointer to next planet in linked list
			int			life;		// Planet life
			char		pid[30];	// String containing ID of creating process
		} planet_type;
		
		
		*/

		
		bytesWritten = mailslotWrite (mailSlot, planet, 424);		//MESSAGE, strlen(MESSAGE));	//Send planet instead of message

		if (bytesWritten!=-1)
			printf("data sent to server (bytes = %d)\n", bytesWritten);
		else
			printf("failed sending data to server\n");
	}

	mailslotClose (mailSlot);

	/* (sleep for a while, enables you to catch a glimpse of what the */
	/*  client prints on the console)                                 */
	Sleep(2000);
	return;
}

//createPlanet(char name, double mass, double Xposition, double Yposition, double Xvelocity, double Yvelocity int life)