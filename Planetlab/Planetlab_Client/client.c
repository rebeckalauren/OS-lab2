/*********************************************
* client.c
*
* Author: mle@mdh.se
* Desc: lab-skeleton for the client side of an
* client-server application
* Date: 990309
*
* Revised 011018 by Dag Nystr�m & Juka M�ki-Turja
* NOTE: the server must be started BEFORE the
* client.
*********************************************/

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "wrapper.h"
LPTSTR Slot = TEXT("\\\\.\\mailslot\\sample_mailslot");
#define MESSAGE "Hej p� dig!"
void main(void) 
{
	struct pt newplanet;
	double x, y;
	int a = 1;
	int b = 4;
	char message[100000];
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

	while(a<b) 
	{
		/* send a friendly greeting to the server */
		/* NOTE: The messages sent to the server need not to be of equal size.       */
		/* Messages can be of different sizes as long as they don't exceed the       */
		/* maximum message size that the mailslot can handle (defined upon creation).*/
		
			
			strcpy_s(newplanet.name, sizeof(newplanet.name), "F�rsta planeten");
			printf("x-position");
			fgets(message, sizeof (message), stdin);
			sscanf_s(message, "%lf", &x);
			printf("y-position");
			fgets(message, sizeof (message), stdin);
			sscanf_s(message, "%lf", &y);
			newplanet.sx = x;											
			newplanet.sy = y;											
			newplanet.vx = 1;											
			newplanet.vy = 1;											
			newplanet.mass = 60;											
			newplanet.life = 60;
			newplanet.next = NULL;


		//struct pt planet = {"Planet1",0,0,0,0,0,0,0,0}; 
		/*(struct pt*)malloc(sizeof(struct pt));  // Malloc = Allocates a block of size bytes of memory
		strcpy_s(planet->name, sizeof(planet->name), "F�rsta planeten");*/

		bytesWritten = mailslotWrite (mailSlot, (void*)&newplanet, sizeof(newplanet));

		if (bytesWritten!=-1)
			printf("data sent to server (bytes = %d), (name = %s) \n", bytesWritten, newplanet.name);
		else
			printf("failed sending data to server\n");

		a++;
	}

	mailslotClose (mailSlot);

	/*(sleep for a while, enables you to catch a glimpse of what the client prints on the console)*/
	Sleep(2000);
	return;
}


