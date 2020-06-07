#include "Display.h"
#include <math.h>
#include <stdio.h>
#include <iostream>


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))


//
typedef unsigned short WORD; // 16 bit unsigned integer
typedef int LONG; // 32 bit integer
typedef unsigned int DWORD; // 32 bit unsigned integer

struct BITMAPINFOHEADER_LINUX
{
	DWORD biSize;
	LONG biwidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};

struct BITMAPFILEHEADER_LINUX
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
};

//
void Display::save_bmp(const char* file_name)
{
	// create file name
	char file_name_new[100];

	if (file_name == 0)
	{
		static int count = 0;
		sprintf(file_name_new, "image_%04d.bmp", count++);

		//POINT3 pm; from_logi_to_phys_coords(m_imax - 150,m_jmax - 50, 0, pm);
		//POINT3 pM; from_logi_to_phys_coords(m_imax - 150,m_jmax - 50, 0, pM);

		// draw_number(count, pm.x, pM.x, pm.y, y, pM.y, 0);
	}
	else
		sprintf(file_name_new, "%s", file_name);

	printf("saving %s\n", file_name_new);

	//
	BITMAPFILEHEADER_LINUX bmFileHeader;
	BITMAPINFOHEADER_LINUX bmInfoHeader;

	bmFileHeader.bfSize = 14;
	bmFileHeader.bfType = 0x4D42;
	bmFileHeader.bfOffBits = 14 + 40;
	bmInfoHeader.biSize = 40;
	bmInfoHeader.biwidth = m_imax + 1;
	bmInfoHeader.biHeight = m_jmax + 1;
	bmInfoHeader.biPlanes = 1;
	bmInfoHeader.biBitCount = 24;
	bmInfoHeader.biCompression = 0;
	bmInfoHeader.biSizeImage = bmInfoHeader.biwidth
		*bmInfoHeader.biHeight
		*(bmInfoHeader.biBitCount / 8);

	bmInfoHeader.biXPelsPerMeter = 0; //indicates no suggested real-world size of the image in the units of pixels
	bmInfoHeader.biYPelsPerMeter = 0;
	bmInfoHeader.biClrUsed = 0;
	bmInfoHeader.biClrImportant = 0;

	//
	FILE* fp = fopen(file_name_new, "wb"); long size_w;

	size_w = fwrite((const void*)&(bmFileHeader.bfType     ), 1, 2, fp);
	size_w = fwrite((const void*)&(bmFileHeader.bfSize     ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmFileHeader.bfReserved1), 1, 2, fp);
	size_w = fwrite((const void*)&(bmFileHeader.bfReserved2), 1, 2, fp);
	size_w = fwrite((const void*)&(bmFileHeader.bfOffBits  ), 1, 4, fp);

	size_w = fwrite((const void*)&(bmInfoHeader.biSize         ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biwidth        ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biHeight       ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biPlanes       ), 1, 2, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biBitCount     ), 1, 2, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biCompression  ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biSizeImage    ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biXPelsPerMeter), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biYPelsPerMeter), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biClrUsed      ), 1, 4, fp);
	size_w = fwrite((const void*)&(bmInfoHeader.biClrImportant ), 1, 4, fp);

	char* buffer = new char[(1+m_imax)*(1+m_jmax)*3];
	unsigned char r = 0, g = 0, b = 0; int count=0;

	for (int j = 0; j <= m_jmax; j++)
	for (int i = 0; i <= m_imax; i++)
	{
		RGB_SPLIT(m_rgb[i][j], r, g, b);
		buffer[count]=b; count++;
		buffer[count]=g; count++;
		buffer[count]=r; count++;
	}

	fwrite(buffer,1,(1+m_imax)*(1+m_jmax)*3,fp);
	delete[] buffer;
	fclose(fp);
}

//
void Display::load_bmp(const char* file_name)
{
	printf("loading %s\n", file_name);

	//
	BITMAPFILEHEADER_LINUX bmFileHeader;
	BITMAPINFOHEADER_LINUX bmInfoHeader;
	FILE* fp = fopen(file_name, "rb"); long size_w;

	size_w = fread((void*)&(bmFileHeader.bfType     ), 1, 2, fp);
	size_w = fread((void*)&(bmFileHeader.bfSize     ), 1, 4, fp);
	size_w = fread((void*)&(bmFileHeader.bfReserved1), 1, 2, fp);
	size_w = fread((void*)&(bmFileHeader.bfReserved2), 1, 2, fp);
	size_w = fread((void*)&(bmFileHeader.bfOffBits  ), 1, 4, fp);
			 
	size_w = fread((void*)&(bmInfoHeader.biSize         ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biwidth        ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biHeight       ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biPlanes       ), 1, 2, fp);
	size_w = fread((void*)&(bmInfoHeader.biBitCount     ), 1, 2, fp);
	size_w = fread((void*)&(bmInfoHeader.biCompression  ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biSizeImage    ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biXPelsPerMeter), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biYPelsPerMeter), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biClrUsed      ), 1, 4, fp);
	size_w = fread((void*)&(bmInfoHeader.biClrImportant ), 1, 4, fp);

	//
	m_imax = bmInfoHeader.biwidth -1;
	m_jmax = bmInfoHeader.biHeight-1;
	m_rgb.resize(m_imax+1,m_jmax+1);

	unsigned char r,g,b;

	for (int j = 0; j <= m_jmax;j++)
	for (int i = 0; i <= m_imax; i++)
	{
		size_w = fread((void*)&b, 1, 1, fp);
		size_w = fread((void*)&g, 1, 1, fp);
		size_w = fread((void*)&r, 1, 1, fp);
		m_rgb[i][j] = RGB_COMBINE(r, g, b);
	}

	fclose(fp);
}



void Display::draw_line(double x0,double y0,
						double x1,double y1, int rgb)
{
	int i0, j0; phy_to_log(x0,y0,i0,j0);
	int i1, j1; phy_to_log(x1,y1,i1,j1);
	if( i0<0||i0>m_imax||j0<0||j0>m_jmax||
		i1<0||i1>m_imax||j1<0||j1>m_jmax)
		return;
	int imin=(i0<i1)?i0:i1;
	int imax=(i0<i1)?i1:i0;
	int jmin=(j0<j1)?j0:j1;
	int jmax=(j0<j1)?j1:j0;

	x1-=x0;
	y1-=y0;

	for(int i = imin; i <=imax; i++)
	for(int j = jmin; j <=jmax; j++)
	{
		double x,y; log_to_phy(i,j,x,y);
		// the rectangle occupied by the pixel
		double xmin=x-m_dx/2, xmax=x+m_dx/2;
		double ymin=y-m_dy/2, ymax=y+m_dy/2;

		xmin-=x0; xmax-=x0;
		ymin-=y0; ymax-=y0;

		double minx = MIN(xmin/x1,xmax/x1), maxx = MAX(xmin/x1,xmax/x1); 
		double miny = MIN(ymin/y1,ymax/y1), maxy = MAX(ymin/y1,ymax/y1);
		if (MAX(MAX(0, minx), miny) <= 
			MIN(MIN(1, maxx), maxy))
				m_rgb[i][j] = rgb;
	}

}