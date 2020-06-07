#pragma once
#include "Benson.h"

enum RESULT
{ THIS_ALIVE, THIS_DEAD,
  THAT_ALIVE, THAT_DEAD, LAW_VIOLATED, INCONCLUSIVE};

//------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------
class DecisionTree
{
protected:
	//--------------------------------------------------------------------
	// a coordinates of the stones to kill or make alive
	//--------------------------------------------------------------------
	int iObj;
	int jObj;
	//--------------------------------------------------------------------
	// the rectangle containing the battleground
	//--------------------------------------------------------------------
	int imin_BG, imax_BG;
	int jmin_BG, jmax_BG;

	void copy(const state a[M][N], state a_copy[M][N]) const;
	int  value(RESULT res) const;

	RESULT check_node          (state a[M][N], state player_this, 
		                                       int        i_this, 
		                                       int        j_this) const;
	RESULT check_node_recursive(state a[M][N], state player_this, 
		                                       int        i_this, 
		                                       int        j_this) const;
	RESULT reverse_result(RESULT result ) const;

public:
	RESULT find_best_move(state a[M][N], state player_this, int iObj, int jObj,
		int imin_BG, int imax_BG, int jmin_BG, int jmax_BG, int& i_this, int& j_this);
};

static DecisionTree theDecisionTree;
