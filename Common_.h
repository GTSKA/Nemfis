#pragma once

//Macros definition for Infinitum values
#define INFINITE_MAX DBL_MAX
#define INFINITE_MIN -DBL_MAX

//Macros definition for float epsion test
//-------------------------------------------------------------------------------------------------------------
#define EPSILON_TEST //Comment or uncomment this line to active the EPSILON TEST for float a double values


#ifdef EPSILON_TEST
//Funcion para realizar la prueba de numeros flotantes
#define EPSILON_EXP 10000
#define FLOAT_EQ(x,v) (((v - (EPSILON_EXP*DBL_EPSILON)) < x) && (x <( v + (EPSILON_EXP*DBL_EPSILON))))
//#define DOUBLE_ZERO_TEST(x) ( x == 0 ? x : ( x < 0 ? ((1 + (-x)) <=  (1 + ( EPSILON_EXP*DBL_EPSILON)) ? 0 : x ) : (1 + x <= 1 + (EPSILON_EXP*DBL_EPSILON) ? 0 : x)))		
#define DOUBLE_ZERO_TEST(x) ( FLOAT_EQ(x,0) == 0 ? x : 0 )		
//#define DOUBLE_ZERO_TEST(x) (  )		

#define EPS_  10.0*DBL_EPSILON 
#define EPS_DIST_TEST(x) (((x - int(x)) - EPS_ > 0) ? x : int(x) )

#endif

#ifndef EPSILON_TEST
#define DOUBLE_ZERO_TEST(x) (x)	
#endif

#define _CONSOLE_HANDLE
#ifdef _CONSOLE_HANDLE
#include <iostream>
using namespace std;
#endif

//-------------------------------------------------------------------------------------------------------------

//Macros definition for float windows compiler
//-------------------------------------------------------------------------------------------------------------
#define _CMP_VST_WIN		//Windows
//#define _CMP_VST_MFC		//Windows MFC
//#define _CMP_GCC_WIN
//#define _CMP_GCC_LNX
//#define _CMP_GCC_UNX

#ifdef	_CMP_VST_WIN //Windows Compiling
#include <windows.h>
#include <strsafe.h>
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _CMP_VST_MFC
#include <windows.h>
#include <afxwin.h>
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _CMP_GCC_WIN //Not Windows Compiling
//Define and set several constants...
#define MAX_PATH 260
#define _MAX_PATH 260
typedef unsigned int UINT;
typedef char CHAR;
#endif
//-------------------------------------------------------------------------------------------------------------


#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <string>

//-----¿?
#include <sstream>
#include <typeinfo>








