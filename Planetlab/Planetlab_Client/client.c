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
	struct pt *newplanet = (struct pt*)malloc(sizeof(struct pt));
	DWORD WINAPI threadRead( LPVOID lpParam );
	double _sx, _sy, _vx, _vy, _mass, _life;
	char planetName[100];
	char message[100000];
	HANDLE mailSlot;
	DWORD bytesWritten;
	int loops = 2000;

	Sleep(2000);
	mailSlot = mailslotConnect(Slot); 

	//while (mailSlot == INVALID_HANDLE_VALUE) 
	//{
	//	system("CLS");
	//	printf("Failed to get a handle to the mailslot!!\nHave you started the server?\n");
	//	mailSlot = mailslotConnect(Slot);
	//}

	/* NOTE: replace code below for sending planet data to the server. */
	threadCreate(threadRead, 0);
	while(1) 
	{
		/* send a friendly greeting to the server */
		/* NOTE: The messages sent to the server need not to be of equal size.       */
		/* Messages can be of different sizes as long as they don't exceed the       */
		/* maximum message size that the mailslot can handle (defined upon creation).*/

		printf("Name of planet");
		gets_s(newplanet->name,sizeof(newplanet->name));

		printf("x-position");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_sx);

		printf("y-position");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_sy);

		printf("x-velocity");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_vx);

		printf("y-velocity");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_vy);

		printf("planet mass");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_mass);

		printf("planet life");
		fgets(message, sizeof (message), stdin);
		sscanf_s(message, "%lf", &_life);

		newplanet->sx = _sx;											
		newplanet->sy = _sy;											
		newplanet->vx = _vy;											
		newplanet->vy = _vx;											
		newplanet->mass = _mass;											
		newplanet->life = _life;
		newplanet->next = NULL;

		//struct pt planet = {"Planet1",0,0,0,0,0,0,0,0}; 
		/*(struct pt*)malloc(sizeof(struct pt));  // Malloc = Allocates a block of size bytes of memory
		strcpy_s(planet->name, sizeof(planet->name), "Första planeten");*/

		bytesWritten = mailslotWrite (mailSlot, (void*)newplanet, sizeof(struct pt));

		if (bytesWritten!=-1)
			printf("data sent to server (bytes = %d), (name = %s) \n", bytesWritten, newplanet->name);
		else
			printf("failed sending data to server\n");
	}
	mailslotClose (mailSlot);
	return;
}

DWORD WINAPI threadRead( LPVOID lpParam ) // read if planet is dead
{
	struct pt *newplanet = (struct pt*)malloc(sizeof(struct pt));
	HANDLE mailSlot;
	while (1)
	{
		//mailslotRead(mailSlot, newplanet, 424);
			// read if planet is dead
	}
}