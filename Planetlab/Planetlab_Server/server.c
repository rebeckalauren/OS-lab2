/********************************************************************\
* server.c                                                           *
*                                                                    *
* Author: mle@mdh.se                                                 *
* Desc: example of the server-side of an application                 *
* Date: 990310                                                       *
* Revised: 011018 Dag Nystr�m & Jukka M�ki-Turja                     *
*                                                                    *
* Based on generic.c from Microsoft.                                 *
*                                                                    *
*  Functions:                                                        *
*     WinMain      - Application entry point                         *
*     MainWndProc  - main window procedure                           *
*                                                                    *
* NOTE: this program uses some graphic primitives provided by Win32, *
* therefore there are probably a lot of things that are unfamiliar   *
* to you. There are comments in this file that indicates where it is *
* appropriate to place your code.                                    *
* NOTE: you will stumble into some problems if you run this program  *
*       in Windows 95/98. don't do that, use NT instead.             *
\********************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "wrapper.h"

/* the server uses a timer to periodically update the presentation window */
/* here is the timer id and timer period defined                          */

#define UPDATE_FREQ     10	/* update frequency (in ms) for the timer */
LPTSTR Slot = TEXT("\\\\.\\mailslot\\sample_mailslot");
/* (the server uses a mailslot for incoming client requests) */
struct pt* root;
struct pt* iterator;
void checkPlanets(struct pt* Testplanet);


/*********************  Prototypes  ***************************/
/* NOTE: Windows has defined its own set of types. When the   */
/*       types are of importance to you we will write comments*/ 
/*       to indicate that. (Ignore them for now.)             */
/**************************************************************/

LRESULT WINAPI MainWndProc( HWND, UINT, WPARAM, LPARAM );
DWORD WINAPI mailThread(LPVOID);



HDC hDC;		/* Handle to Device Context, gets set 1st time in MainWndProc */
/* we need it to access the window for printing and drawin */

/********************************************************************\
*  Function: int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)    *
*                                                                    *
*   Purpose: Initializes Application                                 *
*                                                                    *
*  Comments: Register window class, create and display the main      *
*            window, and enter message loop.                         *
*                                                                    *
*                                                                    *
\********************************************************************/

/* NOTE: This function is not too important to you, it only */
/*       initializes a bunch of things.                     */
/* NOTE: In windows WinMain is the start function, not main */

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow ) {

	HWND hWnd;
	DWORD threadID;
	MSG msg;

	/* Create the window, 3 last parameters important */
	/* The tile of the window, the callback function */
	/* and the backgrond color */

	hWnd = windowCreate (hPrevInstance, hInstance, nCmdShow, "Himmel", MainWndProc, COLOR_WINDOW+1);

	/* start the timer for the periodic update of the window    */
	/* (this is a one-shot timer, which means that it has to be */
	/* re-set after each time-out) */
	/* NOTE: When this timer expires a message will be sent to  */
	/*       our callback function (MainWndProc).               */

	windowRefreshTimer (hWnd, UPDATE_FREQ);


	/* create a thread that can handle incoming client requests */
	/* (the thread starts executing in the function mailThread) */

	/* NOTE: See online help for details, you need to know how  */ 
	/*       this function does and what its parameters mean.   */
	/* We have no parameters to pass, hence NULL				*/


	threadID = threadCreate (mailThread, NULL); 


	/* (the message processing loop that all windows applications must have) */
	/* NOTE: just leave it as it is. */
	while( GetMessage( &msg, NULL, 0, 0 ) ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}


/********************************************************************\
* Function: mailThread                                               *
* Purpose: Handle incoming requests from clients                     *
* NOTE: This function is important to you.                           *
/********************************************************************/
DWORD WINAPI mailThread(LPVOID arg) {


	char buffer[1024];
	DWORD bytesRead;
	static int posY = 0;
	HANDLE mailbox;
	/* create a mailslot that clients can use to pass requests through   */
	/* (the clients use the name below to get contact with the mailslot) */
	/* NOTE: The name of a mailslot must start with "\\\\.\\mailslot\\"  */

	mailbox = mailslotCreate(Slot);


	for(;;) 
	{				
		/* (ordinary file manipulating functions are used to read from mailslots) 
		 in this example the server receives strings from the client side and   
		 displays them in the presentation window                               
		 NOTE: binary data can also be sent and received, e.g. planet structures*/

		struct pt planet;													//ska va s� h�r
		
		//bytesRead = mailslotRead (mailbox, buffer, strlen(buffer));
		bytesRead = mailslotRead (mailbox, (void*)&planet, sizeof(planet));	//ska va s� h�r
		checkPlanets(&planet);
		// Skapa ny tr�d f�r varje planet
		//threadCreate(checkPlanets, 0);
		
		if(bytesRead!= 0) 
		{
			
			printf("%s" ,planet.name);

			/* NOTE: It is appropriate to replace this code with something that match your needs here.*/
			posY++;  
			/* (hDC is used reference the previously created window) */							
			TextOut(hDC, 10, 50+posY%200, buffer, bytesRead);
		}
		else 
		{
			/* failed reading from mailslot (in this example we ignore this, and happily continue...) */
		}
	}

	return 0;
}


/********************************************************************\
* Function: LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM) *
*                                                                    *
* Purpose: Processes Application Messages (received by the window)   *
* Comments: The following messages are processed                     *
*                                                                    *
*           WM_PAINT                                                 *
*           WM_COMMAND                                               *
*           WM_DESTROY                                               *
*           WM_TIMER                                                 *
*                                                                    *
\********************************************************************/
/* NOTE: This function is called by Windows when something happens to our window */

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	PAINTSTRUCT ps;
	static int posX = 100;
	int posY = 100;
	HANDLE context;
	static DWORD color = 0;

	switch( msg ) {
		/**************************************************************/
		/*    WM_CREATE:        (received on window creation)
		/**************************************************************/
	case WM_CREATE:       
		hDC = GetDC(hWnd);  
		break;   
		/**************************************************************/
		/*    WM_TIMER:         (received when our timer expires)
		/**************************************************************/
	case WM_TIMER:

		/* NOTE: replace code below for periodic update of the window */
		/*       e.g. draw a planet system)                           */
		/* NOTE: this is referred to as the 'graphics' thread in the lab spec. */

		/* here we draw a simple sinus curve in the window    */
		/* just to show how pixels are drawn                  */
		//posX += 4;
		//posY -= 2; //(int) (10 * sin(posX / (double) 30) + 20);
		iterator = root;
		while(iterator != NULL)
		{
			SetPixel (hDC, iterator->sx, iterator->sy, (COLORREF) color);//(hDC, posX % 547, posY, (COLORREF) color);
			iterator = iterator->next;
		}
		
		color += 12;
		windowRefreshTimer (hWnd, UPDATE_FREQ);
		break;
		/****************************************************************\
		*     WM_PAINT: (received when the window needs to be repainted, *
		*               e.g. when maximizing the window)                 *
		\****************************************************************/

	case WM_PAINT:
		/* NOTE: The code for this message can be removed. It's just */
		/*       for showing something in the window.                */
		context = BeginPaint( hWnd, &ps );
		/* (you can safely remove the following line of code) */
		//TextOut( context, 10, 10, "Hello, World!", 13 ); /* 13 is the string length */
		EndPaint( hWnd, &ps );
		break;
		/**************************************************************\
		*     WM_DESTROY: PostQuitMessage() is called                  *
		*     (received when the user presses the "quit" button in the *
		*      window)                                                 *
		\**************************************************************/
	case WM_DESTROY:
		PostQuitMessage( 0 );
		/* NOTE: Windows will automatically release most resources this */
		/*       process is using, e.g. memory and mailslots.           */
		/*       (So even though we don't free the memory which has been*/     
		/*       allocated by us, there will not be memory leaks.)      */

		ReleaseDC(hWnd, hDC); /* Some housekeeping */
		break;

		/**************************************************************\
		*     Let the default window proc handle all other messages    *
		\**************************************************************/
	default:
		return( DefWindowProc( hWnd, msg, wParam, lParam )); 
	}
	return 0;
}

void createPlanet(char* name, double mass, double Xposition, double Yposition, double Xvelocity, double Yvelocity, int life)
{
	struct pt newplanet;

	strcpy_s(newplanet.name, sizeof(newplanet.name), "F�rsta planeten");
	newplanet.sx = Xposition;											
	newplanet.sy = Yposition;											
	newplanet.vx = Xvelocity;											
	newplanet.vy = Yvelocity;											
	newplanet.mass = mass;											
	newplanet.life = life;

	return (void)newplanet;
}
void checkPlanets(struct pt* Testplanet)
{
	if(root == 0)
	{
		//create root
		root = Testplanet;
		createPlanet(root->name, root->mass, root->sx, root->sy, root->vx, root->vy, root->life);
	}
	else
	{
		iterator = root;
		while(iterator->next != NULL)
		{
			iterator = iterator->next;
		}
		iterator = Testplanet;
		createPlanet(iterator->name, iterator->mass, iterator->sx, iterator->sy, iterator->vx, iterator->vy, iterator->life);
		//create planet last in the linked list
	}
}
