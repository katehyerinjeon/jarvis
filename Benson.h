#pragma once

#define M 10
#define N  8

enum state { BLACK, WHITE, EMPTY };

class Benson
{
protected:
	state x;
	state a[M][N];
	int   B[M][N]; int B_count;
	int   R[M][N]; int R_count;
	bool  R_small[M*N];

	void set_B_and_R(const state a[M][N], state x);
public:
	int find_unconditionally_alive_blocks(const state a[M][N], state x, bool marker[M][N]);
	static void print_state ( const state a     [M][N]);
	static void print_marker( const bool  marker[M][N]);
	void print_Bs   () const;
	void print_Rs   () const;
};

static Benson theBension;

class Capture
{
protected:
	int st_i[M*N];
	int st_j[M*N];
	int st_count;
public:
	bool  find_xblock_captured(const state a[M][N], int i, int j, state x, bool marker[M][N]);
	bool find_yblocks_captured(const state a[M][N], int i, int j, state x, bool marker[M][N]);
};

static Capture theCapture;