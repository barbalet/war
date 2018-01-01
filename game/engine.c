/****************************************************************
 
	engine.c - Noble Warfare Skirmish
 
 =============================================================
 
 Copyright 1996-2018 Tom Barbalet. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 This software and Noble Ape are a continuing work of Tom Barbalet,
 begun on 13 June 1996. No apes or cats were harmed in the writing
 of this software.
 
 ****************************************************************/

#include "stdio.h"

#ifdef	_WIN32
#include "../noble/noble.h"
#else
#include "noble.h"
#endif
#include "battle.h"


#define BATTLE_FILE_LOCATION "./Noble Warfare.app/Contents/Resources/battle.txt"


n_byte          *local_board;

static n_general_variables  game_vars;

static n_unit	*units;
static n_byte2	number_units;
static n_type   *types;
static n_byte2  number_types;

#define	SIZEOF_MEMORY	 (16*1024*1024)

static n_byte	*memory_buffer;
static n_uint   memory_allocated;
static n_uint	memory_used;


n_int draw_error(n_constant_string error_text, n_constant_string location, n_int line_number)
{
    printf("ERROR: %s, %s line: %ld\n", error_text, location, line_number);
	return -1;
}

static void mem_init(n_byte start) {
	if(start) {		
		memory_buffer = NOTHING;
		memory_allocated = SIZEOF_MEMORY;
		
		memory_buffer = io_new_range((SIZEOF_MEMORY/4), &memory_allocated);
	}
	memory_used = 0;
}


static n_byte * mem_use(n_uint size) {
	n_byte * val = NOTHING;
	if(size > (memory_allocated - memory_used)) {
		engine_exit();
		/*plat_close();*/
	}
	val = &memory_buffer[memory_used];
	memory_used += size;
	return val;
}

#define NW_TYPE  (0x10)
#define NW_UNIT  (0x20)
#define NW_GAME  (0x30)

const noble_file_entry   battle_commands[]= { /* (number_commands<<3)|4 */
    {"ntype{", NW_TYPE | 0,              0, 0,  "Unit Type"},    /* n_type */
    {"defen=", NW_TYPE | FILE_TYPE_BYTE, 1, 0,  "Defense Value"},/*defence;*/
    {"melat=", NW_TYPE | FILE_TYPE_BYTE, 1, 1,  "Melee Attack"}, /*melee_attack;*/
    {"melda=", NW_TYPE | FILE_TYPE_BYTE, 1, 2,  "Melee Damage"}, /*melee_damage;*/
    {"melap=", NW_TYPE | FILE_TYPE_BYTE, 1, 3,  "Melee Armor Pierce"}, /*melee_armpie;*/

    {"misat=", NW_TYPE | FILE_TYPE_BYTE, 1, 4,  "Missile Attack"},/*missile_attack;*/
    {"misda=", NW_TYPE | FILE_TYPE_BYTE, 1, 5,  "Missile Damage"},/*missile_damage;*/
    {"misap=", NW_TYPE | FILE_TYPE_BYTE, 1, 6,  "Missile Armor Pierce"},/*missile_armpie;*/
    {"misrt=", NW_TYPE | FILE_TYPE_BYTE, 1, 7,  "Missile Rate"},/*missile_rate;*/

    {"misrg=", NW_TYPE | FILE_TYPE_BYTE2,1, 8,  "Missile Range"}, /*missile_range;*/
    {"speed=", NW_TYPE | FILE_TYPE_BYTE, 1, 10, "Speed Maximum"}, /*speed_maximum;*/
    {"statu=", NW_TYPE | FILE_TYPE_BYTE, 1, 11, "Stature"},/*stature;*/
    {"leade=", NW_TYPE | FILE_TYPE_BYTE, 1, 12, "Leadership"}, /*leadership;*/

    {"weapc=", NW_TYPE | FILE_TYPE_BYTE, 1, 13,  "Wounds per combatant"},/*wounds_per_combatant;*/
    {"tyidn=", NW_TYPE | FILE_TYPE_BYTE, 1, 14,  "Type ID number"}, /*type ID number*/


    {"nunit{", NW_UNIT | 0, 0, 0, "Specific Unit"},/* n_unit */

    {"tidun=", NW_UNIT | FILE_TYPE_BYTE, 1, 0,  "Type (ID number)"}, /*type (ID number)*/
    {"angle=", NW_UNIT | FILE_TYPE_BYTE, 1, 1,  "Angle"}, /*angle;*/

    {"averx=", NW_UNIT | FILE_TYPE_BYTE2, 1, 2, "Average X"}, /*average_x;*/
    {"avery=", NW_UNIT | FILE_TYPE_BYTE2, 1, 4, "Average Y"}, /*average_y;*/

    {"width=", NW_UNIT | FILE_TYPE_BYTE2, 1, 6, "Width"}, /*width;*/
    {"numco=", NW_UNIT | FILE_TYPE_BYTE2, 1, 8, "Number Combatants"},/*number_combatants;*/

    {"align=", NW_UNIT | FILE_TYPE_BYTE, 1, 10, "Alignment"},/*alignment;*/
    {"misnu=", NW_UNIT | FILE_TYPE_BYTE, 1, 11, "Missile Number"},/*missile_number;*/


    {"ngame{", NW_GAME | 0, 0, 0, "Game Variables"},/* game_vars */

    {"randv=", NW_GAME | FILE_TYPE_BYTE2, 2, 0,   "Random values"},/*randoms;*/
    {"vari1=", NW_GAME | FILE_TYPE_BYTE2, 1, 4,   "Value 1"}, /*value_1;*/
    {"vari2=", NW_GAME | FILE_TYPE_BYTE2, 1, 6,   "Value 2"}, /*value_2;*/
    {"vari3=", NW_GAME | FILE_TYPE_BYTE2, 1, 8,   "Value 3"}, /*value_3;*/
    {"vari4=", NW_GAME | FILE_TYPE_BYTE2, 1, 10,  "Value 4"}, /*value_4;*/
    {"vari5=", NW_GAME | FILE_TYPE_BYTE2, 1, 12,  "Value 5"}, /*value_5;*/

    {{0, 0, 0, 0, 0, 0},0, 0, 0} /* end commands */
};

static n_byte	engine_filein(n_file * file_pass, n_byte val) {
	n_byte	temp_store[sizeof(n_unit)+sizeof(n_type)]={0};
	n_int   ret_val = io_read_buff(file_pass, temp_store, battle_commands);
	if(ret_val == FILE_EOF)
    {
		return 6;
	}
	if(val == ret_val)
    {
		if(val == NW_TYPE)
        { /* type */
			io_copy(temp_store, (n_byte*)&types[number_types++], sizeof(n_type));
			(void)mem_use(sizeof(n_type));
		}
		if(val == NW_UNIT)
        { /* unit */
			io_copy(temp_store, (n_byte *)&units[number_units++], sizeof(n_unit));
			(void)mem_use(sizeof(n_unit));
		}
		if(val == NW_GAME)
        { /* game_vars */
			io_copy(temp_store, (n_byte *)&game_vars, ( sizeof(n_general_variables) ) );
		}
	}
	return 0;
}

static n_int engine_conditions(n_string location)
{
	n_byte    		ret_val;
	n_file			*file_pass = io_file_new();
	number_units = 0;
	number_types = 0;

	mem_init(0);

	local_board = (n_byte *)mem_use(BATTLE_BOARD_SIZE);
    
    if (local_board == NOTHING)
    {
        return SHOW_ERROR("Local board not allocated");
    }
    
	io_erase(local_board, BATTLE_BOARD_SIZE);
	board_init(local_board);
    
	if(io_disk_read(file_pass, location) != 0)
    {
		return SHOW_ERROR("Read file failed");
	}

	io_whitespace(file_pass);  
	file_pass->location = 0;
	

  /* load the types first */
	types = (n_type *) mem_use(0);
	do {
		ret_val = engine_filein(file_pass, NW_TYPE);
	} while(ret_val == 0);

	/* load the units next */
	units = (n_unit *) mem_use(0);

	file_pass->location = 0;

	do {
		ret_val = engine_filein(file_pass, NW_UNIT);
	} while(ret_val == 0);

	if((number_types == 0) || (number_units == 0) || (number_types > 255))
    {
		SHOW_ERROR("Type/Unit Logic Failed");
	}

	file_pass->location = 0;
	
	/* load the game variables last */
  
    do {
        ret_val = engine_filein(file_pass, NW_GAME);
    } while(ret_val == 0);

	io_file_free(&file_pass);
        
    /* resolve the units with types and check the alignments */
	{
		n_byte	resolve[256] = {0};
		n_uint	check_alignment[2] = {0};
		n_byte	loop = 0;
		while(loop < number_types) {
			resolve[types[loop].points_per_combatant] = loop;
			loop++;
		}
		loop = 0;
		while(loop < number_units) {
			n_byte2	local_combatants = units[loop].number_combatants;
			units[loop].unit_type = &types[resolve[units[loop].morale]];
			units[loop].morale = 255;
			units[loop].number_living = local_combatants;
			units[loop].combatants = (n_combatant *)mem_use(sizeof(n_combatant)*local_combatants);
			check_alignment[ (units[loop].alignment) & 1 ]++;
			loop++;
		}

		/* if there are none of one of the alignments, there can be no battle */
		if((check_alignment[0] == 0) || (check_alignment[1] == 0))
        {
			SHOW_ERROR("Alignment Logic Failed");
		}
	}
	/* get the drawing ready, fill the units with spaced combatants and draw it all */
    
	battle_loop(&battle_fill, units, number_units, NOTHING);
	return 0;
}

void * engine_init(n_uint random_init)
{
	game_vars.random0 = (n_byte2) (random_init & 0xFFFF);
	game_vars.random1 = (n_byte2) (random_init >> 16);

    game_vars.attack_melee_dsq = 5;
    game_vars.declare_group_facing_dsq = 8000;
    game_vars.declare_max_start_dsq = 0xffff;
    game_vars.declare_one_to_one_dsq = 0xffff;
    game_vars.declare_close_enough_dsq = 5;
    
	mem_init(1);

	if (engine_new() == -1)
    {
		return NOTHING;
    }

	return ((void *) local_board);
}

n_byte sm_down = 0, sm_last = 0;
n_int startx = 0, starty = 0, endx = 0, endy = 0;

unsigned char engine_mouse(short px, short py)
{
	sm_down = 1;

	if(sm_last == 0)
    {
		startx = px;
		starty = py;
		endx = px;
		endy = py;
	} else {
		endx = px;
		endy = py;
	}
	return 1;
}


n_int engine_new(void)
{
    if(engine_conditions(BATTLE_FILE_LOCATION) != 0)
    {
        return SHOW_ERROR("Update conditions failed");
    }
    return 0;
}

n_int engine_update(void)
{
    n_byte result = battle_opponent(units, number_units);
    
    if (result != 0)
    {
        return engine_new();
    }
    
    battle_loop(&battle_move, units, number_units, &game_vars);

    battle_loop(&battle_declare, units, number_units, &game_vars);
    battle_loop(&battle_attack, units, number_units, &game_vars);
    battle_loop(&battle_remove_dead, units, number_units, NOTHING);
    
	return 0;
}


void engine_draw(void)
{
    draw_init();
    battle_loop(&draw_cycle, units, number_units, NOTHING);
}

void engine_exit()
{
	io_free((void **)&memory_buffer);
}


