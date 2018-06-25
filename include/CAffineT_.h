#ifndef CAFFINET_H
#define CAFFINET_H
#include "CArray_.h"
class CAffineT_
{
public:
	CAffineT_();

	//Set the Screen size
	//-----------------------------------------------------
	void SetScreen(int screenW, int screenH);

	//Set the World size
	//-----------------------------------------------------
	void SetWorld(long double XMin, long double YMin, long double XMax, long double YMax);

	//Get the World size
	//-----------------------------------------------------
	void GetWorld(long double *XMin, long double *YMin, long double *XMax, long double *YMax);

	//Set World and Screen size
	//-----------------------------------------------------
	void SetWorldScreen(long double XMin, long double YMin, long double XMax, long double YMax,
		int screenW, int screenH);

	//Set Xmin
	//-----------------------------------------------------
	void SetXmin(long double XMin);
		
	//Get Xmin
	//-----------------------------------------------------
	void GetXmin(long double *XMin);

	//Set Xmax
	//-----------------------------------------------------
	void SetXmax(long double XMax);

	//Get Xmax
	//-----------------------------------------------------
	void GetXmax(long double *XMax);

	//Set Ymin
	//-----------------------------------------------------
	void SetYmin(long double YMin);

	//Get Ymin
	//-----------------------------------------------------
	void GetYmin(long double *YMin);

	//Set Ymax
	//-----------------------------------------------------
	void SetYmax(long double YMax);

	//Get Ymax
	//-----------------------------------------------------
	void GetYmax(long double *YMax);

	//Calculate the relation between the Screen and the World
	//-----------------------------------------------------
	void Calculate();

	//Transform a point in the World to a point in the Screen
	//-----------------------------------------------------
	void FromWorldToScreen(long double x, long double y, int *xs, int *ys);

	//Transform points in the World to points in the Screen
	//-----------------------------------------------------
	void FromWorldToScreen(CArray_<long double> x, CArray_<long double> y,CArray_<int> *xs, CArray_<int> *ys);

	//Transform a point in the Screen to a point in the World
	//-----------------------------------------------------
	void FromScreenToWorld(int xs, int ys, long double *x, long double *y);

	//Transform points in the Screen to points in the World
	//-----------------------------------------------------
	void FromScreenToWorld(CArray_<int> xs, CArray_<int> ys, CArray_<long double> *x, CArray_<long double> *y);
	
private:
	long double Xmin;
	long double Ymin;
	long double Xmax;
	long double Ymax;
	int Width;
	int Height;
	long double bx;
	long double by;
	long double mx;
	long double my;
};
CAffineT_::CAffineT_()
{
}
void CAffineT_::SetWorldScreen(long double XMin, long double YMin,
							   long double XMax, long double YMax,
							   int screenW, int screenH)
{
	Xmin = XMin;
	Ymin = YMin;
	Xmax = XMax;
	Ymax = YMax;
	Width = screenW;
	Height = screenH;
	Calculate();
}
void CAffineT_::SetScreen(int screenW, int screenH)
{
	Width = screenW;
	Height = screenH;
	Calculate();
}
void CAffineT_::SetWorld(long double XMin, long double YMin, long double XMax, long double YMax)
{
	Xmin = XMin;
	Ymin = YMin;
	Xmax = XMax;
	Ymax = YMax;
	Calculate();
}
void CAffineT_::GetWorld(long double *XMin, long double *YMin, long double *XMax, long double *YMax)
{
	*XMin = Xmin;
	*YMin = Ymin;
	*XMax = Xmax;
	*YMax = Ymax;
}
void CAffineT_::SetXmax(long double XMax)
{
	Xmax = XMax;
	Calculate();
}
void CAffineT_::GetXmax(long double *XMax)
{
	*XMax = Xmax;
}
void CAffineT_::SetXmin(long double XMin)
{
	Xmin = XMin;
	Calculate();
}
void CAffineT_::GetXmin(long double *XMin)
{
	*XMin = Xmin;
}
void CAffineT_::SetYmax(long double YMax)
{
	Ymax = YMax;
	Calculate();
}
void CAffineT_::GetYmax(long double *YMax)
{
	*YMax = Ymax;
}
void CAffineT_::SetYmin(long double YMin)
{
	Ymin = YMin;
	Calculate();
}
void CAffineT_::GetYmin(long double *YMin)
{
	*YMin = Ymin;
}
void CAffineT_::Calculate()
{
	INT auxH;
	INT auxW;
	if(((Xmax-Xmin)*(long double)Height) == ((Ymax-Ymin)*(long double)Width))
	{
		mx = (long double)(Width)/(Xmax-Xmin);
		my = (long double)(Height)/(Ymax-Ymin);
		bx = mx * (-1.0 * Xmin);
		by = my * (-1.0 * Ymin);
	}
	if(((Xmax-Xmin)*Height) < ((Ymax-Ymin)*Width))
	{
		auxW = ((Xmax-Xmin)*Height)/(Ymax-Ymin);
		mx = ((long double)auxW)/(Xmax-Xmin);
		my = (long double)(Height)/(Ymax-Ymin);
		bx = (long double)(Width -auxW)/2.0 - mx * (Xmin);
		by = my * (-1.0 * Ymin);
	}
	if(((Xmax-Xmin)*Height) > ((Ymax-Ymin)*Width))
	{
		auxH = ((Ymax-Ymin)*Width)/(Xmax-Xmin);
		mx = (long double)(Width) / (Xmax-Xmin);
		my = (long double)(auxH) / (Ymax-Ymin);
		bx = mx * (-1.0 * Xmin);
		by = (long double)(Height - auxH)/2.0 - my * (Ymin);
	}
}
void CAffineT_::FromScreenToWorld(int xs, int ys, long double *x, long double *y)
{
	*x = ((long double)xs - bx) / mx;
	*y = ((long double)(Height - ys) - by) / my;
}
void CAffineT_::FromScreenToWorld(CArray_<int> xs, CArray_<int> ys,
								  CArray_<long double> *x, CArray_<long double> *y)
{
	for(register UINT i = 0; i < xs.GetSize(); ++i)
	{
		(*x)[i]= ((long double)xs[i] - bx) / mx;
	}
	for(register UINT i = 0; i < ys.GetSize(); ++i)
	{
		(*y)[i] = ((long double)(Height - ys[i]) - by) / my;
	}
}
void CAffineT_::FromWorldToScreen(long double x, long double y, int *xs, int *ys)
{
	*xs = mx * (x) + bx;
	*ys = Height - (my * (y) + by);
}
void CAffineT_::FromWorldToScreen(CArray_<long double> x, CArray_<long double> y,
								  CArray_<int> *xs, CArray_<int> *ys)
{
	for(register UINT i = 0; i < x.GetSize(); ++i)
	{
		(*xs)[i] = mx * (x[i]) + bx;
	}
	for(register UINT i = 0; i < y.GetSize(); ++i)
	{
		(*ys)[i] = Height - (my * (y[i]) + by);
	} 
}
#endif