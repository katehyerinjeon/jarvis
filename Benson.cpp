#include <stdio.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include "Benson.h"

void Benson::set_B_and_R(const state a[M][N], state x)
{
	//-----------------------------------------------
	// initialize member variables
	//-----------------------------------------------
	bool done[M][N]; R_count = 0; B_count = 0;
	for(int i=0;i<M;i++)
	for(int j=0;j<N;j++)
	{
		this->a[i][j] = a[i][j];
		done   [i][j] = false;
		R      [i][j] = -1;
		B      [i][j] = -1;
	}
	this->x = x;
	//-----------------------------------------------
	// find blocks and regions
	//-----------------------------------------------
	int st_i[M*N];
	int st_j[M*N]; int st_count = 0;

	for(int i=0;i<M;i++)
	for(int j=0;j<N;j++)
	{
		//-----------------------------------------------
		// find blocks 
		//-----------------------------------------------
		if(a[i][j]==x && done[i][j]==false)
		{
			st_i[st_count] = i;
			st_j[st_count] = j; st_count++;
			while (st_count > 0)
			{
				st_count--;
				int m = st_i[st_count];
				int n = st_j[st_count];
				B[m][n] = B_count; done[m][n] = true;
				if (m > 0 && a[m - 1][n] == x && done[m - 1][n] == false)
				{
					st_i[st_count] = m-1;
					st_j[st_count] = n  ; st_count++;
				}
				if (m<M-1 && a[m + 1][n] == x && done[m + 1][n] == false)
				{
					st_i[st_count] = m+1;
					st_j[st_count] = n  ; st_count++;
				}
				if (n > 0 && a[m][n - 1] == x && done[m][n - 1] == false)
				{
					st_i[st_count] = m  ;
					st_j[st_count] = n-1; st_count++;
				}
				if (n<N-1 && a[m][n + 1] == x && done[m][n + 1] == false)
				{
					st_i[st_count] = m  ;
					st_j[st_count] = n+1; st_count++;
				}
			}
			B_count++;
		}
		//-----------------------------------------------
		// find regions
		//-----------------------------------------------
		if(a[i][j]!=x && done[i][j]==false)
		{
			st_i[st_count] = i;
			st_j[st_count] = j; st_count++;
			while (st_count > 0)
			{
				st_count--;
				int m = st_i[st_count];
				int n = st_j[st_count];
				R[m][n] = R_count; done[m][n] = true;
				if (m > 0 && a[m - 1][n] != x && done[m - 1][n] == false)
				{
					st_i[st_count] = m-1;
					st_j[st_count] = n  ; st_count++;
				}
				if (m<M-1 && a[m + 1][n] != x && done[m + 1][n] == false)
				{
					st_i[st_count] = m+1;
					st_j[st_count] = n  ; st_count++;
				}
				if (n > 0 && a[m][n - 1] != x && done[m][n - 1] == false)
				{
					st_i[st_count] = m  ;
					st_j[st_count] = n-1; st_count++;
				}
				if (n<N-1 && a[m][n + 1] != x && done[m][n + 1] == false)
				{
					st_i[st_count] = m  ;
					st_j[st_count] = n+1; st_count++;
				}
			}
			R_count++;
		}
	}
	//-----------------------------------------------
	// check small regions
	//-----------------------------------------------
	for(int r=0;r<R_count;r++) R_small[r]=true;

	for(int i=0;i<M;i++)
	for(int j=0;j<N;j++)
	{
		int r = R[i][j];
		if( r!=-1 &&
			(i==0   || R[i-1][j]==r) &&
			(i==M-1 || R[i+1][j]==r) &&
			(j==0   || R[i][j-1]==r) &&
			(j==N-1 || R[i][j+1]==r) &&
			a[i][j] != WHITE+BLACK-x )
			R_small[r]=false;
	}
}

void Benson::print_state(const state a[M][N])
{
	setlocale(LC_CTYPE, "");
	wchar_t black = 0x25CB;
	wchar_t white = 0x25CF;
	
	for(int j=N-1;j>=0;j--)
	{
		for(int i=0;i<M;i++)
			     if(a[i][j]==BLACK) wprintf(L"%lc", black);
			else if(a[i][j]==WHITE) wprintf(L"%lc", white);
			else                    wprintf(L"--" );
		printf("\n");
	}
	printf("\n");
}

void Benson::print_marker(const bool marker[M][N])
{
	setlocale(LC_CTYPE, "");
	wchar_t black = 0x25CB;
	wchar_t white = 0x25CF;

	for (int j = N - 1; j >= 0; j--)
	{
		for (int i = 0; i < M; i++)
			if (marker[i][j]) wprintf(L"%lc", black);
			else              wprintf(L"--");
			printf("\n");
	}
	printf("\n");
}

void Benson::print_Bs() const
{
	for(int j=N-1;j>=0;j--)
	{
		for(int i=0;i<M;i++)
			if (B [i][j] == -1) printf("-");
			else                printf("%d",B[i][j]);
		printf("\n");
	}
	printf("\n");
}

void Benson::print_Rs() const
{
	for(int j=N-1;j>=0;j--)
	{
		for(int i=0;i<M;i++)
			if (R[i][j] == -1 || R_small[R[i][j]]==false) printf("-");
			else               printf("%d",R[i][j]);
		printf("\n");
	}
	printf("\n");
}

int Benson::find_unconditionally_alive_blocks(const state a[M][N], state x, bool marker[M][N])
{
	set_B_and_R(a, x);
	//print_state(a);
	//print_Bs();
	//print_Rs();

	bool Z[M*N]; // set of blocks
	bool S[M*N]; // set of regions
	for(int b=0;b<B_count;b++) Z[b]=true;
	for(int r=0;r<R_count;r++) S[r]=R_small[r];

	while (true)
	{
		bool update_change = false;
		//-----------------------------------------------------------
		// update Z = {b\in Z| there exist R1 and R2 in S s.t. 
		//                     R1\cap S, R2\cap S \subset Ext(b)}
		//-----------------------------------------------------------
		for(int b=0;b<B_count;b++)
		{
			if (Z[b])
			{
				bool Healthy[M*N]; int Healthy_count = 0;
				for(int r=0;r<R_count;r++)
					if (S[r]) { Healthy[r] = true; Healthy_count++; }
					else        Healthy[r] = false;
				for(int i=0;i<M;i++)
				for(int j=0;j<N;j++)
				{
					int r = R[i][j];
					if (r != -1 && Healthy[r] && a[i][j] == EMPTY)
					{
						if((i==  0 || B[i-1][j]!=b) &&
						   (i==M-1 || B[i+1][j]!=b) &&
						   (j==  0 || B[i][j-1]!=b) &&
						   (j==N-1 || B[i][j+1]!=b))
						{
							Healthy[r] = false; 
							Healthy_count--;
						}
					}
				}
				if (Healthy_count <= 1)
				{
					Z[b] = false; update_change = true;
				}
			}
		}

		if (update_change == false) break;

		//-----------------------------------------------------------
		// update S = {R:small| NB(R)<Z}
		//-----------------------------------------------------------
		for(int i=0;i<M;i++)
		for(int j=0;j<N;j++)
		{
			int r = R[i][j];
			if (r != -1 && S[r] )
			{
				if((i>  0 && B[i-1][j]!=-1 && Z[B[i-1][j]]==false) ||
				   (i<M-1 && B[i+1][j]!=-1 && Z[B[i+1][j]]==false) ||
				   (j>  0 && B[i][j-1]!=-1 && Z[B[i][j-1]]==false) ||
				   (j<N-1 && B[i][j+1]!=-1 && Z[B[i][j+1]]==false))
					S[r] = false;
			}
		}
	}

	for(int i=0;i<M;i++)
	for(int j=0;j<N;j++)
		marker[i][j] = B[i][j]!=-1 && Z[B[i][j]];
	int Z_count = 0;
	for(int b=0;b<B_count;b++)
		if(Z[b]) Z_count++;
	return Z_count;
}



bool Capture::find_xblock_captured(const state a[M][N], int i, int j, state x, bool marker[M][N])
{
	assert(a[i][j] == x);
	for (int m = 0; m<M; m++)
		for (int n = 0; n<N; n++) marker[m][n] = false;

	st_count = 0; st_i[st_count] = i; st_j[st_count] = j; st_count++;

	while (st_count>0)
	{
		st_count--; int m = st_i[st_count], n = st_j[st_count];
		marker[m][n] = true;
		if ((m>  0 && a[m - 1][n] == EMPTY) ||
			(m<M - 1 && a[m + 1][n] == EMPTY) ||
			(n>  0 && a[m][n - 1] == EMPTY) ||
			(n<N - 1 && a[m][n + 1] == EMPTY))
			return false;

		if (m>  0 && a[m - 1][n] == x && marker[m - 1][n] == false) { st_i[st_count] = m - 1; st_j[st_count] = n; st_count++; }
		if (m<M - 1 && a[m + 1][n] == x && marker[m + 1][n] == false) { st_i[st_count] = m + 1; st_j[st_count] = n; st_count++; }
		if (n>  0 && a[m][n - 1] == x && marker[m][n - 1] == false) { st_i[st_count] = m; st_j[st_count] = n - 1; st_count++; }
		if (n<N - 1 && a[m][n + 1] == x && marker[m][n + 1] == false) { st_i[st_count] = m; st_j[st_count] = n + 1; st_count++; }
	}
	return true;
}

bool Capture::find_yblocks_captured(const state a[M][N], int i, int j, state x, bool marker[M][N])
{
	assert(a[i][j] == x);
	bool result = false;
	bool temp[M][N]; state y = (x == BLACK) ? WHITE : BLACK;
	for (int m = 0; m<M; m++)
	for (int n = 0; n<N; n++) marker[m][n] = false;

	if (i>0 && a[i - 1][j] == y && marker[i - 1][j] == false)
	{
		if (find_xblock_captured(a, i - 1, j, y, temp))
		{
			for (int i = 0; i<M; i++)
			for (int j = 0; j<N; j++) marker[i][j] = marker[i][j] || temp[i][j];
			result = true;
		}
	}

	if (i<M - 1 && a[i + 1][j] == y && marker[i + 1][j] == false)
	{
		if (find_xblock_captured(a, i + 1, j, y, temp))
		{
			for (int i = 0; i<M; i++)
			for (int j = 0; j<N; j++) marker[i][j] = marker[i][j] || temp[i][j];
			result = true;
		}
	}

	if (j>0 && a[i][j - 1] == y && marker[i][j - 1] == false)
	{
		if (find_xblock_captured(a, i, j - 1, y, temp))
		{
			for (int i = 0; i<M; i++)
			for (int j = 0; j<N; j++) marker[i][j] = marker[i][j] || temp[i][j];
			result = true;
		}
	}

	if (j<N - 1 && a[i][j + 1] == y && marker[i][j + 1] == false)
	{
		if (find_xblock_captured(a, i, j + 1, y, temp))
		{
			for (int i = 0; i<M; i++)
			for (int j = 0; j<N; j++) marker[i][j] = marker[i][j] || temp[i][j];
			result = true;
		}
	}

	return result;
}