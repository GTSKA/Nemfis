// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MatrixVectorOperations.h"
#include <iostream>

using namespace std;


int main()
{
	CMatrix_<float> m1, m2;
	m1 = CMatrix_<float>(4);
	m2 = CMatrix_<float>(4);
	CMatrix_<float> m5;
	m5 = CMatrix_<float>(4);
	m1[0][0] = 2; m1[0][1] = 1; m1[0][2] = 5; m1[0][3] = 7;
	m1[0][0] = 1; m1[0][1] = 3; m1[0][2] = 9; m1[0][3] = 2;
	m1[0][0] = 0; m1[0][1] = 1; m1[0][2] = 5; m1[0][3] = 2;
	m1[0][0] = 2; m1[0][1] = 0; m1[0][2] = 3; m1[0][3] = 1;

	m2[0][0] = 0; m2[0][1] = 1; m2[0][2] = 2; m2[0][3] = 1;
	m2[0][0] = 1; m2[0][1] = 0; m2[0][2] = 0; m2[0][3] = 2;
	m2[0][0] = 0; m2[0][1] = 1; m2[0][2] = 3; m2[0][3] = 2;
	m2[0][0] = 3; m2[0][1] = 0; m2[0][2] = 8; m2[0][3] = 0;

	CMATRIX_FLOAT4 m3, m4, m6;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m3[i][j] = m1[i][j];
			m4[i][j] = m2[i][j];
		}
	}
	m5.Multiply(&m1, &m2);
	m6 = m3 * m4;
	cout << "m5" << endl << m5 << endl << "m6" <<  m6 << endl;
	cin.get();

    return 0;
}

