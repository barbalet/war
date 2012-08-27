/****************************************************************
 
	win.c - Windows Implementation of the Generic Platform Interface
 
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
#ifdef	_WIN32

#include <windows.h>
#include <time.h>
#include <stdio.h>


/*NOBLEMAKE DEL=""*/

#include "gpi.h"



#ifdef GPI_KEY_FUNCTION
extern  unsigned char GPI_KEY_FUNCTION(unsigned short num);
#endif

#ifdef GPI_MOUSE_FUNCTION
extern  unsigned char GPI_MOUSE_FUNCTION(short px, short py);
#endif

extern		   void * GPI_INIT_FUNCTION(unsigned long kseed);


extern		   long   GPI_UPDATE_FUNCTION(unsigned char first);


#ifdef GPI_EXIT_FUNCTION
extern		   void   GPI_EXIT_FUNCTION();
#endif

#ifdef GPI_MENU_FUNCTION
extern		   void   GPI_MENU_FUNCTION(unsigned char menu_val);
#endif


/*NOBLEMAKE END=""*/

unsigned char * draw_buffer;

#ifdef GPI_MEMORY_USED



void plat_close();

void plat_copy(unsigned char * from, unsigned char * to, unsigned long number);

void * plat_new(unsigned long bytes, unsigned char critical);

void plat_free(void * ptr);


#endif

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif
/*NOBLEMAKE END=""*/

#ifdef _WIN32

#ifdef GPI_MOUSE_FUNCTION
unsigned char		firedown = 0;
#endif

#ifdef GPI_MEMORY_USED


void plat_close(){

#ifdef GPI_EXIT_FUNCTION
	GPI_EXIT_FUNCTION();
#endif

	PostQuitMessage(0);
}


void plat_copy(unsigned char * from, unsigned char * to, unsigned long number) {
	memcpy(to, from, number);
}

void * plat_new(unsigned long bytes, unsigned char critical) {
	void *	tmp = 0L;
	tmp = (void *) malloc(bytes);
	if (tmp == 0L && critical) {
		
	}
	return ( tmp );
}

void plat_free(void * ptr) {
	if ( ptr != 0L )
		free(ptr);
}





#endif


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow) {
	static TCHAR szAppName[] = TEXT ( GPI_PC_APP_NAME ) ;

	HWND hwnd ;
	MSG msg ;
	WNDCLASS wndclass ;

	wndclass.style = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc = WndProc ;
	wndclass.cbClsExtra = 0 ;
	wndclass.cbWndExtra = 0 ;
	wndclass.hInstance = hInstance ;
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass)) {
		MessageBox (NULL, TEXT ("Program requires Windows NT!"),
		            szAppName, MB_ICONERROR) ;
		return 0 ;
	}
#ifdef GPI_MENU_FUNCTION
	hwnd = CreateWindow (szAppName, TEXT ( GPI_WINDOW_NAME ),
	                     WS_OVERLAPPEDWINDOW - WS_SIZEBOX - WS_MAXIMIZEBOX,
	                     CW_USEDEFAULT, CW_USEDEFAULT,
	                     GPI_DIMENSION_X + 6,
	                     GPI_DIMENSION_Y + 25 + 19,
	                     NULL, NULL, hInstance, NULL) ;

#else
	hwnd = CreateWindow (szAppName, TEXT ( GPI_WINDOW_NAME ),
	                     WS_OVERLAPPEDWINDOW - WS_SIZEBOX - WS_MAXIMIZEBOX,
	                     CW_USEDEFAULT, CW_USEDEFAULT,
	                     GPI_DIMENSION_X + 6,
	                     GPI_DIMENSION_Y + 25,
	                     NULL, NULL, hInstance, NULL) ;

#endif
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HBITMAP hBitmap ;

#ifdef GPI_MENU_FUNCTION
	static HMENU   hMenu, hMenuPopup;
#endif
	PAINTSTRUCT ps ;
	HDC hdc, hdcMem ;


#ifdef GPI_MOUSE_FUNCTION

	if(firedown && message != WM_LBUTTONUP)
	if(GPI_MOUSE_FUNCTION(LOWORD(lParam), HIWORD(lParam))) {}

#endif

	switch (message) {
		case WM_CREATE:


#ifdef GPI_MENU_FUNCTION
			/* This is a terrible anti-GPI hack for the Noble Ape Simulation.
			 * It will be removed, post haste.
			 */
			hMenu = CreateMenu();
			hMenuPopup = CreateMenu();

			AppendMenu(hMenuPopup, MF_STRING, 40001, "&New");
			AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenuPopup, MF_STRING, 40003, "E&xit");
			AppendMenu(hMenu, MF_POPUP|MF_STRING,(UINT)hMenuPopup,"&File");
			hMenuPopup = CreateMenu();


			AppendMenu(hMenuPopup, MF_STRING, 40256, "&Undo");
			AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenuPopup, MF_STRING, 40257, "Cu&t");
			AppendMenu(hMenuPopup, MF_STRING, 40258, "&Copy");
			AppendMenu(hMenuPopup, MF_STRING, 40259, "&Paste");
			AppendMenu(hMenu, MF_POPUP|MF_STRING,(UINT)hMenuPopup,"&Edit");
			hMenuPopup = CreateMenu();


			AppendMenu(hMenuPopup, MF_STRING, 40017, "&Brain");
			AppendMenu(hMenuPopup, MF_STRING, 40018, "&Terrain");
			AppendMenu(hMenuPopup, MF_STRING, 40019, "&Map");

			AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenuPopup, MF_STRING, 40021, "P&revious Ape");
			AppendMenu(hMenuPopup, MF_STRING, 40022, "&Next Ape");
			AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenuPopup, MF_STRING, 40024, "&Pause");
			AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
			AppendMenu(hMenuPopup, MF_STRING,40255, TEXT("&About..."));

			AppendMenu(hMenu, MF_POPUP|MF_STRING,(UINT)hMenuPopup,"&Control");

			SetMenu(hwnd,hMenu);
#endif
			draw_buffer = (unsigned char *) GPI_INIT_FUNCTION( clock() );

			if(draw_buffer == 0L){
				plat_close();
			}

			hBitmap = CreateBitmap(GPI_DIMENSION_X, GPI_DIMENSION_Y, 1, 1, NULL) ;

			InvalidateRect (hwnd, NULL, TRUE) ;
			return 0 ;

#ifdef GPI_MENU_FUNCTION

		case WM_COMMAND:
			{

				unsigned short	response = LOWORD(wParam);
				if(response == (40001+GPI_FILE_ITEMS))
					SendMessage( hwnd, WM_CLOSE, 0 , 0);
				else if(response>40000 && response < 40256)
					GPI_MENU_FUNCTION((unsigned char)(response-40000));

			}

			return 0;

#endif


		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_PAINT:

			if(GPI_UPDATE_FUNCTION(1) != 0)
				plat_close();

			SetBitmapBits(hBitmap, ((GPI_DIMENSION_X*GPI_DIMENSION_Y)>>3), draw_buffer);
			hdc = BeginPaint (hwnd, &ps) ;
			hdcMem = CreateCompatibleDC (hdc) ;
			SelectObject (hdcMem, hBitmap) ;
			BitBlt (hdc, 0, 0, GPI_DIMENSION_X, GPI_DIMENSION_Y, hdcMem, 0, 0, SRCCOPY) ;
			DeleteDC (hdcMem) ;
			EndPaint (hwnd, &ps) ;

			if(GPI_UPDATE_FUNCTION(0) != 0)
				plat_close();


			InvalidateRect (hwnd, NULL, TRUE) ;
			return 0 ;
#ifdef GPI_KEY_FUNCTION
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
				unsigned short	resid = (wParam + 3) & 3;
				if((wParam>36) && (wParam<41)) {
					resid = 28 + ((resid>>1) | ((resid&1)<<1)) ;
					if(GPI_KEY_FUNCTION(resid)) {}

				}
			}
			return 0;
#endif

#ifdef GPI_MOUSE_FUNCTION
		case WM_LBUTTONDOWN:
			firedown = 1;
			return 0 ;
		case WM_LBUTTONUP:
			firedown = 0;
			return 0 ;
#endif

		case WM_DESTROY:
			DeleteObject (hBitmap) ;
			plat_close();
			return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

#endif