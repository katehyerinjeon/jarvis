#ifndef DISPLAY_H
#define DISPLAY_H

#include "ARRAY2D.h"

/*Display structure, where we can display functions and their contours.*/

class Display
{
public:
	/*-----------------------------------------------------------------------*\
	*colours and z-buffers
	\*-----------------------------------------------------------------------*/
	int m_imax; // i=0...imax
	int m_jmax; // j=0...jmax

	ARRAY2D<int> m_rgb;
	//
	Display(int isize = 600, int jsize = 600)
	{
		m_imax = isize - 1;
		m_jmax = jsize - 1;

		m_rgb.resize(isize, jsize);
	}
	//
	void make_it_white()
	{
		//r=g=b=255
		m_rgb = 0xffffff;
	}
	//
	void save_bmp(const char* file_name = 0);
	void load_bmp(const char* file_name    );
	//-----------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------
	void set_physics_coor( double xmin, double xmax,
						   double ymin, double ymax)
	{
		m_xmin=xmin;m_xmax=xmax; m_dx=(xmax-xmin)/m_imax;
		m_ymin=ymin;m_ymax=ymax; m_dy=(ymax-ymin)/m_jmax;
	}
	//

	void log_to_phy( int i, int j, 
					 double& x, double& y) const
	{
		x= (i)*m_dx+m_xmin;
		y= (j)*m_dy + m_ymin;
	}
	//
	
	void phy_to_log( double x, double y,
					 int& i, int& j) const
	{
		i=(int)((x-m_xmin)/m_dx+0.5);
		j=(int)((y-m_ymin)/m_dy+0.5);
	}
	//

	void draw_line(	double x0, double y0,
					double x1, double y1, int rgb);

	double m_xmin, m_xmax, m_dx;
	double m_ymin, m_ymax, m_dy;
};

static int RGB_COMBINE( unsigned char r,
						unsigned char g,
						unsigned char b){ return (r << 16) + (g << 8) + b;}

static void RGB_SPLIT(int rgb,  unsigned char& r,
								unsigned char& g,
								unsigned char& b )
{
	b = rgb & 0x0000ff; rgb >>= 8;
	g = rgb & 0x0000ff; rgb >>= 8;
	r = rgb;
}

#endif