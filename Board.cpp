#include "Board.h"

#pragma once
#define EMPTY 2
#define	WHITE 1
#define BLACK 0

//-----------------------------------------------------------------------------
// 
//  This class implements the rules of Baduk.
//  For simplicity, small size [7][6] is taken, rather than the original [19][19].
// 
//  Copyright, Chohong MIN (chohong@ewha.ac.kr)
//             Dept. Math. Ewha womans university, Jan 2020
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// memory related functions
//-----------------------------------------------------------------------------
Board::Board()
{
	count = 0; 
	i_bk_prev = -1; j_bk_prev = -1;
	i_wt_prev = -1; j_wt_prev = -1;
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		data[i][j] = EMPTY;
}
Board::Board(const Board& board)
{
	count = board.count; 
	i_bk_prev = board.i_bk_prev; j_bk_prev = board.j_bk_prev;
	i_wt_prev = board.i_wt_prev; j_wt_prev = board.j_wt_prev;
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		data[i][j] = board.data[i][j];
}
void Board::operator=(const Board& board)
{
	count = board.count; 
	i_bk_prev = board.i_bk_prev; j_bk_prev = board.j_bk_prev;
	i_wt_prev = board.i_wt_prev; j_wt_prev = board.j_wt_prev;
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		data[i][j] = board.data[i][j];
}

//-----------------------------------------------------------------------------
// the rules of Baduk
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// check the white stones dead, that is, surrounded by black
//-----------------------------------------------------------------------------
bool Board::check_wt_dead(bool marker_dead[7][6]) 
{
	//-----------------------------------------------------------------------------
	// check each white stones dead if it does not have an EMPTY ngbd.
	//-----------------------------------------------------------------------------
	int num_dead = 0;
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
	{
		if(data[i][j]!=WHITE ||
			i==6 || j==5 ||
			(i>0 && data[i-1][j]==EMPTY) ||
			(i<6 && data[i+1][j]==EMPTY) ||
			(j>0 && data[i][j-1]==EMPTY) ||
			(j<5 && data[i][j+1]==EMPTY) )
			marker_dead[i][j] = false;
		else
		{
			marker_dead[i][j] = true;
			num_dead++;
		}
	}

	//-----------------------------------------------------------------------------
	// mark each dead white stone that is adjacent to alive white stone, as alive.
	// this process goes on.
	//-----------------------------------------------------------------------------
	while (true )
	{
		bool need_more_iteration = false;
		for(int i=0;i<7;i++)
		for(int j=0;j<6;j++)
		{
			if( marker_dead[i][j] &&
			   ((i>0 && data[i-1][j]==WHITE && marker_dead[i-1][j]==false) ||
				(i<6 && data[i+1][j]==WHITE && marker_dead[i+1][j]==false) ||
				(j>0 && data[i][j-1]==WHITE && marker_dead[i][j-1]==false) ||
				(j<5 && data[i][j+1]==WHITE && marker_dead[i][j+1]==false)) )
			{
				marker_dead[i][j] = false; num_dead--;
				need_more_iteration = true;
			}
		}
		if (need_more_iteration == false)
			break;
	}

	return num_dead > 0;
}

//-----------------------------------------------------------------------------
// check the white stones dead, that is, surrounded by black
//-----------------------------------------------------------------------------
bool Board::check_bk_dead(bool marker_dead[7][6]) 
{
	// exchange white and black stones.
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		if(data[i][j]!=EMPTY)
			data[i][j]= BLACK+WHITE-data[i][j];

	// use check_wt_dead
	bool result=check_wt_dead(marker_dead);

	// exchange white and black stones.
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		if(data[i][j]!=EMPTY)
			data[i][j]= BLACK+WHITE-data[i][j];

	return result;
}

//-----------------------------------------------------------------------------
// place a stone on [i][j], return false if there is a rule violated
// return true with the stones dead by the placement.
//-----------------------------------------------------------------------------
bool Board::set_stone(int i, int j, bool marker_dead[7][6])
{
	//-----------------------------------------------------------------
	// rule #1 : the spot should be empty
	//-----------------------------------------------------------------
	if( i<0 || i>=7 ||
		j<0 || j>=6 ||data[i][j]!=EMPTY) return false;

	//-----------------------------------------------------------------
	// rule #2 : (i,j) should be different from the previous
	//-----------------------------------------------------------------
	if(count%2==0 && i==i_wt_prev && j==j_wt_prev) return false;
	if(count%2==1 && i==i_bk_prev && j==j_bk_prev) return false;

	//-----------------------------------------------------------------
	// rule #3 : if the addition of (i,j) is eaten, then
	//           at the same time, it should eat.
	//-----------------------------------------------------------------
	bool be_eaten; bool marker_dead_thisplayer[7][6];
	bool to_eat  ;
	data[i][j] = (count % 2 == 0) ? BLACK : WHITE;
	if(count%2 == 0) { be_eaten = check_bk_dead(marker_dead_thisplayer);
	                   to_eat   = check_wt_dead(marker_dead           );}
	else             { be_eaten = check_wt_dead(marker_dead_thisplayer);
	                   to_eat   = check_bk_dead(marker_dead           );}
	if (be_eaten == true && to_eat == false)
	{
		data[i][j] = EMPTY; return false;
	}

	//-----------------------------------------------------------------
	// now, all the rules were satisfied.
	//-----------------------------------------------------------------
	for(int i=0;i<7;i++)
	for(int j=0;j<6;j++)
		if(marker_dead[i][j])
			data[i][j] = EMPTY;
	count++;
	if(count%2==0){ i_bk_prev=i; j_bk_prev=j;}
	else          { i_wt_prev=i; j_wt_prev=j;}
	return true;
}
														  


