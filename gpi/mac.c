/****************************************************************
 
	mac.c - Macintosh Implementation of the Generic Platform Interface
 
	=============================================================
  Copyright 1996-2005 Tom Barbalet. All rights reserved.
  
  This is an open source license. Please contact Tom Barbalet 
  <tom at nobleape dot com> for commercial licensing options.
  
  Redistribution and use in source and binary forms, with or 
  without modification, are permitted provided that the 
  following conditions are met:
  
    1. Redistributions of source code must retain the above 
  copyright notice, this list of conditions and the following 
  disclaimer. 
    2. Redistributions in binary form must reproduce the 
  above copyright notice, this list of conditions and the 
  following disclaimer in the documentation and/or other 
  materials provided with the distribution.
    3. Redistributions in any form must be accompanied by 
  information on how to obtain complete source code for this 
  software and any accompanying software that uses this 
  software. The source code must either be included in the 
  distribution or be available for no more than the cost of 
  distribution plus a nominal fee, and must be freely 
  redistributable under reasonable conditions. For an executable 
  file, complete source code means the source code for all modules 
  it contains. It does not include source code for modules or 
  files that typically accompany the major components of the 
  operating system on which the executable file runs.
  
  THIS SOFTWARE IS PROVIDED BY TOM BARBALET "AS IS" AND ANY 
  EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TOM 
  BARBALET, NOBLE APE OR ITS CONTRIBUTORS BE LIABLE FOR ANY 
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
  =============================================================
  
  This software and Noble Ape are a continuing work of Tom 
  Barbalet, begun on 13 June 1996. No apes or cats were harmed 
  in the writing of this software.
	
****************************************************************/

/*NOBLEMAKE VAR=""*/

#if __profile__
#include <Profiler.h>

#define	PROFILE_START ProfilerInit(collectDetailed, bestTimeBase, 500, 5000); \
					  ProfilerSetStatus(1)
					  
#define	PROFILE_END	  ProfilerDump("\pgpiprofile"); \
					  ProfilerTerm()
#else

#define	PROFILE_START /* */
#define	PROFILE_END   /* */

#endif

#if (__MWERKS__)

#include <Carbon.h>

#define MAIN_START void
#define MAIN_END   /* */

#else

/* includes for ProjectBuilder */
#include <Carbon/Carbon.h>

#define MAIN_START int
#define MAIN_END   return 1

#endif

#define			PLAT_FATAL_ERROR	SysBeep(2); plat_close


/* Some additional constants */

/* This GPI header makes it specific to the particular development */

/*NOBLEMAKE DEL=""*/
#include "gpi.h"
/*NOBLEMAKE END=""*/

/* Four basic platform functions */

void * plat_new(unsigned long bytes, unsigned char critical);
void plat_free(void * ptr);
void plat_close();

#define appleMID	128
#define fileMID		129
#define editMID		130
#define thirdMID	131

#define amAbout		1

#ifdef	GPI_FILE_ITEMS

#define QUIT_MENU_ITEM	(1+GPI_FILE_ITEMS)

#else

#define QUIT_MENU_ITEM	1

#endif

/*NOBLEMAKE END=""*/
/*NOBLEMAKE DEL=""*/

#ifdef GPI_KEY_FUNCTION
extern  unsigned char GPI_KEY_FUNCTION(unsigned short num);
#endif

#ifdef GPI_MOUSE_FUNCTION
extern  unsigned char GPI_MOUSE_FUNCTION(short px, short py);
#endif

#ifdef GPI_CMAP
extern       void  GPI_CMAP(unsigned short * cmap);
#endif

#ifdef GPI_MENU_FUNCTION
extern		   void   GPI_MENU_FUNCTION(unsigned char menu_val);
#endif

extern		   void * GPI_INIT_FUNCTION(unsigned long kseed);
extern		   void   GPI_UPDATE_FUNCTION(unsigned char first);
extern		   void   GPI_EXIT_FUNCTION();


/*NOBLEMAKE END=""*/

/* The basic requirements - menus, window and offscreen */

MenuHandle			apple_menu, file_menu, edit_menu;
#ifdef GPI_THIRD_MENU
MenuHandle			third_menu;
#endif
WindowRef			wind;
#ifdef GPI_CMAP
PixMap				offbits;
#else
BitMap				offbits;
#endif
Rect			    dragRect;

unsigned char		plat_quit;

/* If the mouse is held down, this global is needed */

#ifdef GPI_MOUSE_FUNCTION
unsigned char		firedown = 0;
#endif

static pascal OSErr plat_ae_quit(const AppleEvent *event, AppleEvent *reply, long refcon) {
#pragma unused (event, reply, refcon)
	plat_quit = 1;
	return noErr;
}

static void plat_c_pas(char * in, unsigned char * out) {
	unsigned short	lp = 0, len;
	while(in[lp]!=0)
		lp++;
	len = lp;
	while(lp>0) {
		out[lp] = in[lp-1];
		lp--;
	}
	out[0] = len;
}


/* Handles the menu calls - a traditional platform function */

static void plat_menu(unsigned long mResult) {
	unsigned short	theItem = mResult & 65535, theComm = mResult >> 16;
	switch (theComm) {
		case appleMID:
#ifdef GPI_MENU_FUNCTION
			if(theItem == amAbout) {
				GPI_MENU_FUNCTION(255);
			}
#endif
			break;
		case fileMID:
			if(theItem == QUIT_MENU_ITEM)
				plat_quit = 1;
#ifdef GPI_MENU_FUNCTION
			else {
				GPI_MENU_FUNCTION(theItem);
			}
#endif
			break;
#ifdef GPI_THIRD_MENU
		case thirdMID:
			GPI_MENU_FUNCTION(theItem|16);
			break;
#endif
	}
	HiliteMenu(0);
}

static void plat_event() {
	EventRecord	myEvent;
	unsigned char	update = GPI_AUTO_DIRTY;
#ifdef GPI_MOUSE_FUNCTION
	Point			clickpoint;
#endif
	if (GetNextEvent(everyEvent, &myEvent)) {
		switch (myEvent.what) {
			case mouseDown:
				{
					WindowRef	whichWindow = nil;
					switch (FindWindow( myEvent.where, &whichWindow )) {
						case inMenuBar:
							plat_menu( MenuSelect(myEvent.where));
						case inDrag:
							if ((wind != NULL) && (whichWindow == wind))
								DragWindow(whichWindow, myEvent.where, &dragRect);
							break;
						case inContent:
							if((wind!=NULL)&&(whichWindow==wind)) {
								if (whichWindow != FrontWindow())
									SelectWindow(whichWindow);
#ifdef GPI_MOUSE_FUNCTION
								else
									firedown=1;
#endif
							}
							break;
					}
				}
				break;
#ifdef GPI_MOUSE_FUNCTION
			case mouseUp:
				firedown=0;
				break;
#endif
			case keyDown:
			case autoKey:
				if ((myEvent.modifiers & cmdKey) != 0)
					plat_menu( MenuKey( myEvent.message & charCodeMask ) );
#ifdef GPI_KEY_FUNCTION
				else
					if(GPI_KEY_FUNCTION((myEvent.message & charCodeMask) | (myEvent.modifiers & optionKey))) {
#if (GPI_AUTO_DIRTY == 0)
						update = 1;
#endif
					}
#endif
				break;
			case updateEvt:
			case activateEvt:
				if (wind == (WindowRef)myEvent.message) {
					update = 3;
				}
				break;
		}
	}
#ifdef GPI_MOUSE_FUNCTION
	if (firedown!=0) {
		GetMouse(&clickpoint);
		LocalToGlobal(&clickpoint);
		SetPort(GetWindowPort(wind));
		GlobalToLocal(&clickpoint);
		if(GPI_MOUSE_FUNCTION(clickpoint.h, clickpoint.v)) {
		}
	}
#endif

#if (GPI_AUTO_DIRTY == 0)
	if(update != 0)
#endif
	{
		GPI_UPDATE_FUNCTION(1);
		if(update == 3) BeginUpdate(wind);
		SetPort(GetWindowPort(wind)); {
			BitMap  * local_bitmap = (BitMap *) &offbits;
			StdBits (local_bitmap, &(local_bitmap->bounds), &(local_bitmap->bounds), srcCopy, nil);
		}
		if(update == 3) EndUpdate(wind);
		GPI_UPDATE_FUNCTION(0);
	}


}

static void plat_init() {
	Rect	windRect = {0, 0, GPI_DIMENSION_Y, GPI_DIMENSION_X};
	Rect	windowBounds = { 50, 50, 50 + GPI_DIMENSION_Y, 50 + GPI_DIMENSION_X };
	unsigned char	pascal_window_name[256];
	OSErr 	error;
	FlushEvents( everyEvent, 0 );
	InitCursor();

	error = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication,
	                               NewAEEventHandlerUPP( plat_ae_quit ), 0, false );

	plat_c_pas(GPI_WINDOW_NAME,pascal_window_name);

	apple_menu = NewMenu(appleMID, "\p\024");
#ifdef GPI_MENU_FUNCTION
	{
		unsigned char	about_out[256]={0};
		unsigned long	loop = pascal_window_name[0];

		while(loop>0) {
			about_out[loop+6] = pascal_window_name[loop];
			loop --;
		}
		about_out[0] = pascal_window_name[0] + 6;
		about_out[1] = 'A';
		about_out[2] = 'b';
		about_out[3] = 'o';
		about_out[4] = 'u';
		about_out[5] = 't';
		about_out[6] = ' ';
		about_out[++ about_out[0]] = 'É';
		about_out[++ about_out[0]] = '/';
		about_out[++ about_out[0]] = 'A';
		AppendMenu(apple_menu, about_out);
	}
#endif

	InsertMenu(apple_menu, 0);
	file_menu = NewMenu(fileMID, "\pFile");


#ifdef GPI_FILE_MENU
	{
		unsigned char	file_menu_str[256];
		plat_c_pas(GPI_FILE_MENU, file_menu_str);
		AppendMenu(file_menu,file_menu_str);
	}
#endif

	{
		long response;
		OSErr err = Gestalt (gestaltMenuMgrAttr, &response);
		if ((err == noErr) && !(response & gestaltMenuMgrAquaLayoutMask)) {
#ifdef GPI_FILE_MENU
			AppendMenu(file_menu,"\p(-;Quit/Q");
#else
			AppendMenu(file_menu,"\pQuit/Q");
#endif
		}
	}
	InsertMenu(file_menu, 0);

	edit_menu = NewMenu(editMID, "\pEdit");
	AppendMenu(edit_menu, "\pUndo/Z;(-;Cut/X;Copy/C;Paste/V;Clear");
	InsertMenu(edit_menu, 0);

#ifdef GPI_THIRD_MENU
	{
		unsigned char	third_menu_buffer[256];
		plat_c_pas(GPI_THIRD_MENU_TITLE,third_menu_buffer);
		third_menu = NewMenu(thirdMID, third_menu_buffer);
		plat_c_pas(GPI_THIRD_MENU,third_menu_buffer);
		AppendMenu(third_menu, third_menu_buffer);
	}
	InsertMenu(third_menu, 0);
#endif

	DrawMenuBar();
	GetRegionBounds(GetGrayRgn(), &dragRect);

#ifdef GPI_CMAP
	{
		CTabHandle cpalette = GetCTable (8);
		HandToHand ((Handle *)&cpalette); {
			unsigned short fit[256*3] = {0};
			unsigned short i = 0;
			GPI_CMAP(fit);
			while (i < 256) {
				BlockMove( &fit[(i*3)], &(*cpalette)->ctTable[i].rgb, sizeof(RGBColor));
				i ++;
			}
		}

		wind = NewCWindow(0L, &windowBounds, pascal_window_name, true, noGrowDocProc, (WindowPtr) - 1L, false, 0);
		offbits.rowBytes = (short) (GPI_DIMENSION_X | (0x8000));
		offbits.pmVersion = 1;
		offbits.pixelSize = 8;
		offbits.cmpCount=1;
		offbits.cmpSize=8;
		offbits.pmTable=(CTabHandle)NewHandle(257*8);

		BlockMove((*cpalette), *(offbits.pmTable), (257*8));
		AnimatePalette (wind, (offbits.pmTable), 1, 1, 255);
		ActivatePalette(wind);

		DisposeCTable(cpalette);
	}
#else
	wind = NewWindow(0L, &windowBounds, pascal_window_name, true, noGrowDocProc, (WindowPtr) - 1L, false, 0);
	offbits.rowBytes = (GPI_DIMENSION_X>>3);
#endif
	offbits.bounds = windRect;
	offbits.baseAddr = (char *)GPI_INIT_FUNCTION((unsigned long)TickCount());
#if (GPI_AUTO_DIRTY == 0)
{
	Rect	destRect;

	GetWindowPortBounds(wind, &destRect);
	SetPort(GetWindowPort(wind));
	InvalWindowRect(wind,&destRect);
}
#endif
}

void plat_close() {
	FlushEvents(everyEvent, 0);
	DisposeMenu(apple_menu);
	DisposeMenu(file_menu);
	DisposeMenu(edit_menu);
#ifdef GPI_THIRD_MENU
	DisposeMenu(third_menu);
#endif

#ifdef GPI_CMAP
	DisposeHandle((Handle)offbits.pmTable);
#endif
	DisposeWindow(wind);
	GPI_EXIT_FUNCTION();
	PROFILE_END;
	ExitToShell();
}

/* platform sped-up copy */

/* protected new */

void *	plat_new(unsigned long	bytes, unsigned char critical) {
	void *	tmp = 0L;
	tmp = (void *) NewPtr((long)bytes);
	if (MemError() != 0) {
		tmp = 0L;
	}
	if (tmp == 0L && critical) {
		PLAT_FATAL_ERROR();
	}
	return (tmp);
}

/* protected free */

void plat_free(void * ptr) {
	if (ptr != 0L) {
		DisposePtr(ptr);
		ptr = 0L;
	}
}

MAIN_START main() {
	PROFILE_START;
	plat_init();
	while (!plat_quit)
		plat_event();
	plat_close();
	MAIN_END;
}
