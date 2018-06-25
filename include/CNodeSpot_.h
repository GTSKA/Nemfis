#ifndef CNODESPOT_H
#define CNODESPOT_H
#include "CAffineT_.h"
#include <windows.h>
class CNodeSpot_
{
public:
	CNodeSpot_();

	//Create a new node with center (x,y) and colors ColorFill, ColorLine
	//-----------------------------------------------------
	CNodeSpot_(long double x, long double y,int ColorFill, int ColorLine);
	
	//Create a new node with center (x,y) and colors ColorFill, ColorLine and a Pixel radio
	//-----------------------------------------------------
	CNodeSpot_(long double x, long double y,int ColorFill, int ColorLine, UINT PixRadio);

	//Calculate a RECT to paint the node
	//-----------------------------------------------------
	void SetScreenPoints(CAffineT_* wT);

	//Get the RECT to paint the node
	//-----------------------------------------------------
	void GetNodeRect(RECT *rect);

	//Get the Center to paint the node
	//-----------------------------------------------------
	void GetCenter(int *x, int *y);

	//Get the Color to paint the node
	//-----------------------------------------------------
	void GetColors(int *Fill, int *Line);

	//Get the Pixel Radio from the node
	//-----------------------------------------------------
	void GetPixelRadio(UINT *PixelRadio);
	
	//Set the Pixel Radio from the node
	//-----------------------------------------------------
	void SetPixelRadio(UINT PixelRadio,CAffineT_ *wT);

	//Get the Node Coordinates
	//-----------------------------------------------------
	void GetNodePad(long double *x, long double *y);

private:
	//World position
	//-----------------------------------------------------
	long double xw;
	long double yw;
	
	//Screen position
	//-----------------------------------------------------
	int S[4];
	//S[0] := LEFT
	//S[1] := TOP
	//S[2] := RIGHT
	//S[3] := BOTTOM
	int xs;
	int ys;
	UINT pr;
	
	//Colors
	//-----------------------------------------------------
	COLORREF Fill;
	COLORREF Line;
	int CFill;
	int CLine;
};
CNodeSpot_::CNodeSpot_()
{
}
CNodeSpot_::CNodeSpot_(long double x, long double y, int ColorFill, int ColorLine)
{
	this->xw = x;
	this->yw = y;
	this->CFill = ColorFill;
	this->CLine = ColorLine;
	pr = 10;
}
CNodeSpot_::CNodeSpot_(long double x, long double y, int ColorFill, int ColorLine,UINT PixRadio)
{
	this->xw = x;
	this->yw = y;
	this->CFill = ColorFill;
	this->CLine = ColorLine;
	pr = PixRadio;
}
void CNodeSpot_::SetScreenPoints(CAffineT_ *wT)
{
	wT->FromWorldToScreen(xw,yw,&xs,&ys);
	S[0] = xs - pr;
	S[1] = ys - pr;
	S[2] = xs + pr;
	S[3] = ys + pr;
}
void CNodeSpot_::GetNodeRect(RECT *rect)
{
	rect->left = (LONG)S[0];
	rect->top = (LONG)S[1];
	rect->right = (LONG)S[2];
	rect->bottom = (LONG)S[3];
}
void CNodeSpot_::GetCenter(int *x, int *y)
{
	*x = xs;
	*y = ys;
}
void CNodeSpot_::GetColors(int *Fill, int *Line)
{
	*Fill = CFill;
	*Line = CLine;
}
void CNodeSpot_::GetNodePad(long double *x, long double *y)
{
	*x = xw;
	*y = yw;
}
void CNodeSpot_::GetPixelRadio(UINT *PixelRadio)
{
	*PixelRadio = pr;
}
void CNodeSpot_::SetPixelRadio(UINT PixelRadio,CAffineT_ *wT)
{
	pr = PixelRadio;
	SetScreenPoints(wT);

}
#endif