//#include "CSimplex_Branch_Bounds_.h"
//#include "BranchBounds1.h"
#include "CSimplex_2Phases_.h"
//CFPM_FLOAT
typedef CFPM_FLOAT PRUEBA;
void test1()
{
	CMatrix_<PRUEBA> tab;//= CMatrix_<long double>(3,5);
	bool min = true;
	CARRAY_UINT in = CARRAY_UINT(3);
	CARRAY_UINT out = CARRAY_UINT(3);
	UINT last = 0;
	UINT pasos = 0;
	SIMPLEX_RESULT y;

	/*tab[0][0]=-5;
	tab[0][1]=-2;
	tab[0][2]=0;
	tab[0][3]=0;
	tab[0][4]=0;
	tab[1][0]=2;
	tab[1][1]=2;
	tab[1][2]=1;
	tab[1][3]=0;
	tab[1][4]=9;
	tab[2][0]=3;
	tab[2][1]=1;
	tab[2][2]=0;
	tab[2][3]=1;
	tab[2][4]=11;

	*/

	//CMatrix_Branch_and_Bounds(&tab, min, &in, &out, &last, &pasos, &y);
	////////////////////////////////////////////////II Phases///////////////////////////////////////////////////////////////////
	tab.OpenFromTextFile("C:\\2fases.txt");
	CMatrix_<PRUEBA> ObjF(1,5);
	ObjF[0][0] = -1.0f;
	ObjF[0][1] = 2;
	ObjF[0][2] = 0;
	ObjF[0][3] = 0;
	ObjF[0][4] = 0;
	in[0] = 4;
	in[1] = 5;
	in[2] = 6;
	CARRAY_UINT A_list(2);
	out = in;
	A_list[0] = 5;
	A_list[1] = 6;
	CMatrix_Simplex_2Phases<PRUEBA>(&tab,&ObjF, min,&A_list, &in, &out, &last, &pasos, &y);

}