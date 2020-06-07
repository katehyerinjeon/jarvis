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
struct Board
{
	//-----------------------------------------------------------------------------
	// member variables
	//-----------------------------------------------------------------------------
	int data[7][6];           // takes one value among EMPTY, WHITE, BLACK
	int count;                 // starting from zero, count the number of playings
	int i_bk_prev, j_bk_prev;  // store the latest position of the black stone
	int i_wt_prev, j_wt_prev;  // store the latest position of the white stone

	//-----------------------------------------------------------------------------
	// memory related functions
	//-----------------------------------------------------------------------------
	Board();
	Board(const Board&);
	void operator=(const Board&);

	//-----------------------------------------------------------------------------
	// the rules of Baduk
	//-----------------------------------------------------------------------------
	bool check_wt_dead(bool marker_dead[7][6]) ; // check the white stones dead, that is, surrounded by black
	bool check_bk_dead(bool marker_dead[7][6]) ; // check the black stones dead, that is, surrounded by white
	bool set_stone(int i, int j, bool marker_dead[7][6]); // place a stone on [i][j], return false if there is a rule violated
														  // return true with the stones dead by the placement.

};


