// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MatrixVectorOperations.h"
#include <iostream>

using namespace std;


int main()
{
	CVECTOR_FLOAT3 v1, v2;
	v1[0] = 1; v1[1] = 0; v1[2] = 0;
	v2[0] = 0; v2[1] = 1; v2[2] = 0;
	CVECTOR_FLOAT3 v3;
	v3 = v1+v2;
	cout <<v3[0]<< "|" << v3[1]<< "|" << v3[2]<<endl;

	system("pause");

    return 0;
}

