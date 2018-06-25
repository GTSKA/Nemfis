#include "stdafx.h"
#include "CBVSimplex_.h"


void test()
{
	CMatrix_<double> tab = CMatrix_<double>(3,6);
	CArray_<double> c = CArray_<double>(3);
	CArray_<double> up = CArray_<double>(3);
	CArray_<double> low = CArray_<double>(3);
	CARRAY_UINT indices = CARRAY_UINT(3);
	bool _min = true;
	UINT ultimo;
	CARRAY_UINT in = CARRAY_UINT(2);
	CARRAY_UINT out = CARRAY_UINT(2);								   
	UINT pasos = 0;
	SIMPLEX_RESULT y;
	tab.Matrix[0][0]=2;
	tab.Matrix[0][1]=4;
	tab.Matrix[0][2]=1;
	tab.Matrix[0][3]=0;
	tab.Matrix[0][4]=0;
	tab.Matrix[0][5]=0;
	tab.Matrix[1][0]=2;
	tab.Matrix[1][1]=1;
	tab.Matrix[1][2]=1;
	tab.Matrix[1][3]=1;
	tab.Matrix[1][4]=0;
	tab.Matrix[1][5]=10;
	tab.Matrix[2][0]=1;
	tab.Matrix[2][1]=1;
	tab.Matrix[2][2]=-1;
	tab.Matrix[2][3]=0;
	tab.Matrix[2][4]=1;
	tab.Matrix[2][5]=4; 
	c[0]=2;   
	c[1]=4;
	c[2]=1;
	low[0]=0;
	low[1]=0;
	low[2]=1;
	
	CArray_<Bound_<double>> limites = CArray_<Bound_<double>>(5);
	limites[0].m_lBound = 0;
	limites[0].m_uBound = 4;
	limites[1].m_lBound = 0;
	limites[1].m_uBound = 6;
	limites[2].m_lBound = 1;
	limites[2].m_uBound = 4;
	limites[3].m_lBound = 0;
	limites[3].m_uBound = INFINITE_MAX;
	limites[4].m_lBound = 0;
	limites[4].m_uBound = INFINITE_MAX;
	
	indices[0]=0;
	indices[1]=1;
	indices[2]=2;

	in[0] = 3;
	in[1] = 4;
	CMatrix_CBVSimplex<double>(&tab,&c,&limites,_min,&in,&out,&ultimo,&indices,&pasos,&y);

	

}

