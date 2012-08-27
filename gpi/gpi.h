/****************************************************************
 
	gpi.h - header for the GPI Noble Warfare Skirmish
 
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

#define GPI_WINDOW_NAME			  "Noble Warfare Skirmish"
#define GPI_PC_APP_NAME			  "NobleWarfareSkirmish"

#define GPI_MEMORY_USED			  

#define	GPI_DIMENSION_X			  512
#define	GPI_DIMENSION_Y			  512

#define GPI_INIT_FUNCTION		  engine_init

#define GPI_MOUSE_FUNCTION        engine_mouse

#define GPI_UPDATE_FUNCTION		  engine_update
#define GPI_EXIT_FUNCTION		  engine_exit

#define GPI_AUTO_DIRTY			  1

/*NOBLEMAKE END=""*/

