#include <assert.h>
#include "DecisionTree.h"

RESULT DecisionTree::check_node(state a[M][N], state player_this, int i_this, int j_this) const
{
	//------------------------------------------------
	// find dead stones
	//------------------------------------------------
	bool marker_dead_this[M][N];
	bool marker_dead_that[M][N];
	bool exist_dead_this = theCapture. find_xblock_captured(a, i_this, j_this, player_this, marker_dead_this);
	bool exist_dead_that = theCapture.find_yblocks_captured(a, i_this, j_this, player_this, marker_dead_that);

	if (exist_dead_this == true &&
		exist_dead_that == false)
		return LAW_VIOLATED;

	//------------------------------------------------
	// remove dead stones
	//------------------------------------------------
	if (exist_dead_that)
	{
		for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			if (marker_dead_that[i][j])
				a[i][j] = EMPTY;
		if (exist_dead_this)
			exist_dead_this = false;
	}

	if (exist_dead_that && marker_dead_that[iObj][jObj]) return THAT_DEAD;
	if (exist_dead_this && marker_dead_this[iObj][jObj]) return THIS_DEAD;

	//------------------------------------------------
	// find unconditionally alive stones
	//------------------------------------------------
	bool marker_alive_this[M][N];
	int  exist_alive_this = theBension.find_unconditionally_alive_blocks(a, player_this, marker_alive_this);

	if (exist_alive_this>0 && marker_alive_this[iObj][jObj])
		return THIS_ALIVE;

	//------------------------------------------------
	// neither dead nor alive
	//------------------------------------------------
	return INCONCLUSIVE;

}

RESULT DecisionTree::check_node_recursive(state a[M][N], state player_this, int i_this, int j_this) const
{
	//------------------------------------------------------------
	// 현상태에서 판단완료
	//------------------------------------------------------------
	RESULT result = check_node(a, player_this, i_this, j_this);
	if (result != INCONCLUSIVE) return result;
	//------------------------------------------------------------
	// 상대편 입장에서 생각함
	//------------------------------------------------------------
	else
	{
		state player_that = (player_this == BLACK) ? WHITE : BLACK;
		RESULT result_best_for_that = LAW_VIOLATED;

		for(int i=imin_BG;i<=imax_BG;i++)
		for(int j=jmin_BG;j<=jmax_BG;j++)
		{
			if (a[i][j] == EMPTY)
			{
				state a_next[M][N]; copy(a, a_next);
				a_next[i][j] = player_that;
				RESULT result_that = check_node_recursive(a_next, player_that, i, j);
	
				//------------------------------------------------------------
				// 상대편 입장에서 최상의 결과
				//------------------------------------------------------------
				if (result_that == THIS_ALIVE||
					result_that == THAT_DEAD )
					return reverse_result(result_that);
				//------------------------------------------------------------
				// 상대편 입장에서 그나마 제일 나은 것을 선택
				//------------------------------------------------------------
				else
				{
					if (value(result_that) > value(result_best_for_that))
						result_best_for_that = result_that;
				}
			}
		}
		return reverse_result(result_best_for_that);
	}
}

RESULT DecisionTree::reverse_result(RESULT result) const
{
	switch(result)
	{
	case THIS_ALIVE: return THAT_ALIVE;
	case THAT_ALIVE: return THIS_ALIVE;
	case THIS_DEAD : return THAT_DEAD ; 
	case THAT_DEAD : return THIS_DEAD ;
	default:
		return result;
	};
}

void DecisionTree::copy(const state a[M][N], state a_copy[M][N]) const
{
	for(int i=0;i<M;i++)
	for(int j=0;j<N;j++)
		a_copy[i][j] = a[i][j];
}

int DecisionTree::value(RESULT res) const
{
	switch (res)
	{
	case THIS_ALIVE  : return 3;
	case THAT_DEAD   : return 3;
	case INCONCLUSIVE: return 2;
	case THIS_DEAD   :return 1;
	case THAT_ALIVE  :return 1;
	case LAW_VIOLATED:return 0;
	default:return 0;
	}
}

RESULT DecisionTree::find_best_move(state a[M][N], state player_this, int iObj, int jObj,
	int imin_BG, int imax_BG, int jmin_BG, int jmax_BG, int& i_this, int& j_this)
{
	Benson::print_state(a);
	this->iObj = iObj;
	this->jObj = jObj;
	this->imin_BG = imin_BG;
	this->imax_BG = imax_BG;
	this->jmin_BG = jmin_BG;
	this->jmax_BG = jmax_BG;

	//------------------------------------------------------------
	// 현재 상태에서 다음 수를 생각함
	//------------------------------------------------------------
	RESULT result_best = LAW_VIOLATED;

	for (int i = imin_BG; i <= imax_BG; i++)
	for (int j = jmin_BG; j <= jmax_BG; j++)
	{
		if (a[i][j] == EMPTY)
		{
			state a_next[M][N]; copy(a, a_next);
			a_next[i][j] = player_this;
			RESULT result = check_node_recursive(a_next, player_this, i, j);

			//------------------------------------------------------------
			// 최상의 결과
			//------------------------------------------------------------
			if (result == THIS_ALIVE ||
				result == THAT_DEAD)
			{
				i_this = i;
				j_this = j;
				return result;
			}
			//------------------------------------------------------------
			// 최상이 아니면 그나마 제일 나은 것을 선택
			//------------------------------------------------------------
			else
			{
				if (value(result) > value(result_best))
				{
					i_this = i;
					j_this = j;
					result_best = result;
				}
			}
		}
	}
	return result_best;
}