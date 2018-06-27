#ifndef CMatrix_H
#define CMatrix_H

#pragma once

#include "Common_.h"
#include "CArray_.h"
#ifdef CUDA_PROJECT
#include <cutil_inline.h>
#include <cuda_runtime_api.h>
#endif
#define CMATRIX_VERSION	1.141
#define CMATRIX_DEBUG_CONSOLE

//Enumerates the simplex procedure results
typedef enum SIMPLEX_RESULT_TYPE
{
	//A failure has ocurred in the process
	SX_FAIL					= 0,	
	//The system is no yet optimesed
	SX_NO_OPTIM				= 1,	
	//The system is optim
    SX_OPTIM				= 2,
	//The system is not feasible
	SX_NOT_FEASIBLE			= 3,
	//The system is optim, w/not bundary
    SX_NOT_BOUNDED	        = 4,	
	//The system is optim, w/not bundary
    SX_NOT_SOLUTION			= 5,	
	//
	SX_DUAL_NOT_FEASIBLE	= 6,
	//
	SX_DUAL_NOT_BOUNDED		= 7,
	

	//NOT USED JUST TO FORCE TO COMPILE THE TYPEDEF, TO 32 BITS
    //D3DDEVTYPE_FORCE_DWORD = 0xffffffff,
}SIMPLEX_RESULT, *LPSIMPLEX_RESULT;

//Enumerates the simplex procedure types
typedef enum SIMPLEX_PROCEDURE_TYPE
{
	//NONE
	SX_PROC_NONE			= 0,
	//Canonic simplex
	SX_PROC_CANONIC			= 1,
	//2 Phases procedure
	SX_PROC_2PHASES			= 2,
	//Primal Dual 
	SX_PROC_PRIMALDUAL		= 3,
	//Dual Simplex
	SX_PROC_DUALSIMPLEX		= 4,
	//Integer Programing
	SX_PROC_INTPROGRAMMING	= 5,
	//Primal Simplex
	SX_PROC_PRIMALSIMPLEX	= 6,
	
	//NOT USED JUST TO FORCE TO COMPILE THE TYPEDEF, TO 32 BITS
    //D3DDEVTYPE_FORCE_DWORD = 0xffffffff,

}SIMPLEX_PROCEDURE_TYPE, *LPSIMPLEX_PROCEDURE_TYPE;


template < typename T > class CMatrix_
{
public:

	//Variables
	//--------------------------------------------------------------------------------------------------------------
	//Matrix values
	T **Matrix;

	//Identity value for the elements this matrix
	T I;

	//Zero value for the elements this matrix
	T Z;

	//Negative value for the elements in the matrix
	T N;

	//Builds a new CMatrix object fo size 1 x 1
	//---------------------------------------------------------
	CMatrix_(void); 

	//Builds a new CMatrix object of size n x n
	//---------------------------------------------------------
	//-n: Number of columns and rows, for the matrix
	//---------------------------------------------------------
	CMatrix_(UINT n);

	//Builds a new CMatrix object of size n x m
	//---------------------------------------------------------
	//-n: Number of columns, for the matrix
	//-m: Number of rows, for the matrix
	//---------------------------------------------------------
	CMatrix_(UINT n, UINT m);
	
	//Builds a new CMatrix object, for another CMatrix instance
	//---------------------------------------------------------
	//-pCMatrix: CMatrix reference instance
	//---------------------------------------------------------
	CMatrix_( const CMatrix_<T> &pCMatrix);

	//DESTRUCTOR
	//---------------------------------------------------------
	virtual ~CMatrix_(void);

	//Set a new size, for the matrix, erasing the matrix content
	//---------------------------------------------------------
	//-n: Number of columns, for the matrix
	//-m: Number of rows, for the matrix
	//---------------------------------------------------------
	void SetSize(UINT n, UINT m);

	//Resize the matrix, without losing the matrix conten
	//---------------------------------------------------------
	//-n: New column size, for the matrix
	//-m: New row size, for the matrix
	//---------------------------------------------------------
	bool Resize(UINT newRowSize, UINT newColSize);

	//Return the matrix row size
	//---------------------------------------------------------
	UINT GetRowSize();

	//Return the matrix row size
	//---------------------------------------------------------
	UINT GetRowSize() const;

	//Return the matrix columns size
	//---------------------------------------------------------
	UINT GetColumnSize();

	//Return the matrix columns size
	//---------------------------------------------------------
	UINT GetColumnSize() const;

	//Return last column index for the matrix
	//---------------------------------------------------------
	UINT GetLastRowIndex();

	//Return last column index for the matrix
	//---------------------------------------------------------
	UINT GetLastRowIndex() const;

	//Return last column index for the matrix
	//---------------------------------------------------------
	UINT GetLastColumnIndex();

	//Return last column index for the matrix
	//---------------------------------------------------------
	UINT GetLastColumnIndex() const;

	//Convert the Matrix[iRow][iCol] value in to a string
	//---------------------------------------------------------
	//-str: string object to save the value
	//-iRow: Row value`s position
	//-iColo: Column value`s position
	//---------------------------------------------------------
	void ToString(string* str, UINT iRow, UINT iCol)	;

	//Convert the Matrix[iRow][iCol] value in to a string
	//---------------------------------------------------------
	//-str: char buffer to save the value
	//-iRow: Row value`s position
	//-iColo: Column value`s position
	//---------------------------------------------------------
	void ToString(char* str, UINT iRow, UINT iCol);

	//Fill the value in the Matrix[iRow][iCol] position, from string 
	//---------------------------------------------------------
	//-str: value`s string source 
	//-iRow: Row value`s position
	//-iCol: Column value`s position
	//---------------------------------------------------------
	void FromString(string* str, UINT iRow, UINT iCol);

	//Fill the value in the Matrix[iRow][iCol] position, from string 
	//---------------------------------------------------------
	//-str: value`s char buffer source 
	//-iRow: Row value`s position
	//-iColo: Column value`s position
	//---------------------------------------------------------
	void FromString(char* str, UINT iRow, UINT iCol);

	//Set the values in the iRow, iCol position from another matrix object
	//---------------------------------------------------------
	//-iRow: Row position where to begin to set the values
	//-iCol: Column position where to begin to set the values
	//-matValues: Matrix of values for set in
	//---------------------------------------------------------
	void SetValue(UINT iRow, UINT iCol, CMatrix_<T>* matValues);

	//Save the matrix object into a text format file
	//---------------------------------------------------------
	//-str: File name 
	//---------------------------------------------------------
	void SaveToTextFile(char* fileName);

	//Save the matrix object into a binary file
	//---------------------------------------------------------
	//-str: File name
	//---------------------------------------------------------
	void SaveToBinaryFile(char* fileName);

	//Open a text format file, to fill de Matrix values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	void OpenFromTextFile(char* fileName);

	//Open a text format file, to fill de Matrix values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	void OpenFromTextFile(wchar_t *fileName);

	//Open a binary format file, to fill de Matrix values
	//---------------------------------------------------------
	//-str: File name
	//---------------------------------------------------------
	void OpenFromBinaryFile(char* fileName);
	
	//Print the Matrix to the console output
	//---------------------------------------------------------
	virtual void PrintToConsole();

	//Print the Matrix to the console output
	//---------------------------------------------------------
	virtual void PrintfToConsole();

	//Join a CMatrix object to the right
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_Right(const CMatrix_<T>* pCMatrix);
	
	//Join a CMatrix object to the right
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_RightExplicit(const CMatrix_<T>* pCMatrix);

	//Join a CMatrix object to the left
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_Left(const CMatrix_<T>* pCMatrix);

	//Join a CMatrix object to the left
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_LeftExplicit(const CMatrix_<T>* pCMatrix);

	//Join a CMatrix object to the up
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_Up(const CMatrix_<T>* pCMatrix);

	//Join a CMatrix object to the up
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_UpExplicit(const CMatrix_<T>* pCMatrix);

	//Join a CMatrix object to the down
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_Down(const CMatrix_<T>* pCMatrix);
	
	//Join a CMatrix object to the down
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to join
	//---------------------------------------------------------
	bool Join_DownExplicit(const CMatrix_<T>* pCMatrix);

	//Insert a CMatrix object in the specified position
	//---------------------------------------------------------
	//-pCMatrix: CMatrix object to insert
	//-iRow: Row position to insert the matrix
	//-iCol: Column position to insert the matrix
	//---------------------------------------------------------
	bool Insert(const CMatrix_<T>* pCMatrix, UINT iRow, UINT iCol);

	//Insert a CMatrix object in the specified position
	//---------------------------------------------------------
	//-iRow:
	//-iCol:
	//---------------------------------------------------------
	void Floor(	UINT iRow, UINT iCol, T* floorValue);

	//Finds the determinant of the matrix
	//---------------------------------------------------------
	//-det:
	//---------------------------------------------------------
	void Determinant(T* det);

	//Add a empty column in the matrix.
	//---------------------------------------------------------
	bool Column_Add();

	//Add several empty columns to the  matrix.
	//---------------------------------------------------------
	//-numColumns: Number of columns to add in to the matrix
	//---------------------------------------------------------
	bool Column_Add(UINT numColumns);

	//Insert a empty column in the matrix.
	//---------------------------------------------------------
	//-iCol: Index position to insert the new colum
	//---------------------------------------------------------
	bool Column_Insert(UINT iCol);

	//Remove a column from the matrix, in the iCol position
	//---------------------------------------------------------
	//-iCol: Index position to remove from the matrix
	//---------------------------------------------------------
	bool Column_Remove(UINT iCol);

	//Remove a set of columns from the matrix, in the indexList positions
	//---------------------------------------------------------
	//-indexList: Positions index list to remove from the matrix
	//---------------------------------------------------------
	bool Column_Remove(CARRAY_UINT* indexList);

	//Multiplies the iCol Column by sC times
	//---------------------------------------------------------
	//-iCol: Column to multiply for
	//-sC: value to multiply
	//---------------------------------------------------------
	void Column_OpScMultiply(UINT iCol, T sC);

	//Multiplies the iCol Column by sC times, excluding the xRow
	//---------------------------------------------------------
	//-iCol: Column to multiply for
	//-sC: value to multiply
	//-xRow: Row to exclude
	//---------------------------------------------------------
	void Column_OpScMultiply(UINT iCol, UINT xRow, T sC);
	
	//Interchanges the iCol for jCol and viceversa
	//---------------------------------------------------------
	//iCol: Column to Interchange
	//jCol: Column to Interchange
	//---------------------------------------------------------
	void Column_OpInterchange(UINT iCol, UINT jCol);

	//Interchange the Column M[iCol] by M[iCol] + C*M[jCol]
	//---------------------------------------------------------
	//iCol: Column to Interchange
	//jCol: Column to Interchange
	//C: Value for jCol be multiplied
	//---------------------------------------------------------
	void Column_OpInnerAdd(UINT iCol, UINT jCol, T C);
	
	//Interchange the Column M[iCol] by M[iCol] + C*M[jCol]
	//---------------------------------------------------------
	//iCol: Column to Interchange
	//jCol: Column to Interchange
	//xRow: 
	//C: Value for jCol be multiplied
	//---------------------------------------------------------
	void Column_OpInnerAdd(UINT iCol, UINT jCol, UINT xRow, T C);

	//Makes a full matrix pivot for the position (iRow, iCol)
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//---------------------------------------------------------
	void Column_Pivot(UINT iRow, UINT iCol);

	//Makes a full matrix pivot for the position (iRow, iCol), excluding the xRow
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//-xRow: xRow index position, to exclude in the pivot
	//---------------------------------------------------------
	void Column_Pivot(UINT iRow, UINT iCol, UINT xCol);

	//Makes a full matrix pivot for the position (iRow, iCol), excluding the xRow row and the xCol Column
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//-xRow: xRow index position, to exclude in the pivot
	//---------------------------------------------------------
	void Column_Pivot(UINT iCol, UINT iRow, UINT xCol, UINT xRow);

	//Makes a partial matrix pivot for the position (iRow, iCol)
	//---------------------------------------------------------
	//-iCol: Column position for the operation
	//-iRow: Row position for the operation
	//-right: direction for the operation
	void Column_Partial_Pivot(UINT iRow, UINT iCol, bool right);

	//Get the min value for the given column, iCol
	//---------------------------------------------------------
	//-iCol: Column index to sear for the min value
	//-index: Resulting index for the min value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMinValue(UINT iCol, UINT* index, T* value);

	//Get the min value for the given column iCol, between the rows, iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to sear for the min value
	//-iRowMin:	Row index to begin the search
	//-iRowMax:	Row index to end the search
	//-index: Resulting index for the min value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMinValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* index, T* value);

	//Get the min value for the given column iCol, for each row in the index row list
	//---------------------------------------------------------
	//-iCol: Column index to sear for the min value
	//-indexList: Row index list to search for
	//-index: Resulting index for the min value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMinValue(UINT iCol, CARRAY_UINT indexList, UINT* index, T* value);

	//Get the max value for the given column, iCol
	//---------------------------------------------------------
	//-iCol: Column index to search for the max value
	//-index: Resulting index for the max value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMaxValue(UINT iCol, UINT* index, T* value);

	//Get the min value for the given column iCol, between the rows, iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to search for the max value
	//-iRowMin:	Row index to begin the search
	//-iRowMax:	Row index to end the search
	//-index: Resulting index for the max value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMaxValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* index, T* value);

	//Get the max non integer value given iCol, between the rows iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to search for the max value
	//-iRowMin: Row index to begin the search
	//-iRowMax: Row index to end the search
	//-index: Resulting index for the max value
	//-value: Resulting value
	//----------------------------------------------------------
	void Column_GetMaxNonIntegerValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* index);

	//Get the min value for the given column iCol, for each row in the index row list
	//---------------------------------------------------------
	//-iCol: Column index to search for the max value
	//-indexList: Row index list to search for
	//-index: Resulting index for the max value
	//-value: Resulting value
	//---------------------------------------------------------
	void Column_GetMaxValue(UINT iCol, CARRAY_UINT indexList, UINT* index, T* value);

	//Test if all the elements in the column iCol, are negative, betewen iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to begin the search
	//-iRowMin: Minimum row index variable to include in the search
	//-iRowMax: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsNegative(UINT iCol, UINT iRowMin, UINT iRowMax);

	//Test if all the elements in the column iCol, are negative or zero, betewen iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to begin the search
	//-iRowMin: Minimum row index variable to include in the search
	//-iRowMax: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsNegativeZero(UINT iCol, UINT iRowMin, UINT iRowMax);

	//Test if all the elements in the column iCol, are positive, betewen iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to begin the search
	//-iRowMin: Minimum row index variable to include in the search
	//-iRowMax: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsPositive(UINT iCol, UINT iRowMin, UINT iRowMax);

	//Test if all the elements in the column iCol, are positive or zero, betewen iRowMin and iRowMax
	//---------------------------------------------------------
	//-iCol: Column index to begin the search
	//-iRowMin: Minimum row index variable to include in the search
	//-iRowMax: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsPositiveZero(UINT iCol, UINT iRowMin, UINT iRowMax);

	//Test if all the number in the Column iCol, are Integers or Not, Save the first non integer row-index in iSRow
	//---------------------------------------------------------
	//-iCol: Index of column to test
	//-iSRow: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsInteger(UINT iCol, UINT* iSRow);

	//Test if all the number in the Column iCol, are Integers or Not, between the iRowMin and iRowMax Save the first non integer row-index in iSRow
	//---------------------------------------------------------
	//-iCol: Index of column to test
	//-iRowMin: Min row index
	//-iRowMax: Max row index
	//-iSRow: Variable to save the first row index of the integer value row index
	//---------------------------------------------------------
	bool Column_IsInteger(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* iSRow);

	//Tests if all the number in the Column iCol, are Integers or Not, between the iRowMin and iRowMax Save the first non integer row-index in iSRow
	//---------------------------------------------------------
	//-iCol: Index of column to test
	//-iRowMin: Min row index
	//-iRowMax: Max row index
	//-iSRow: Variable to save the first row index of the integer value row index
	//-nRows: members of the column to test.
	//---------------------------------------------------------
	bool Column_IsInteger(UINT iCol, CARRAY_UINT xRows, UINT* iSRow);


	//Test if the Column in the position: iCol is canonical, betwen the rows: iRowMin and iRowMax, 
	//return the index for the (I) value in the canonical column
	//---------------------------------------------------------
	//-iCol: Index of column to test
	//-iRowMin: Row minimum index to begin the search
	//-iRowMax: Row maximum index to end the search
	//-iSRow: Save the row index for the (I) value in the canonical column
	//---------------------------------------------------------
	bool Column_IsCanonical(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* iSRow);

	//Test if the Column in the position iCol is related to an artificial variable
	//return the index of the (I) value in the canonical part
	//---------------------------------------------------------
	//iCol: Index of column to apply the test
	//iSRow: index for the identity element in the column
	bool Column_IsArtificial(UINT iCol, UINT* iSRow);

	//Apply the quotient test for M[k][iCol]/M[k][jCol] for positives values, to all the rows, 
	//between the iRowMin, iRowMax, saves the minimum index and value
	//---------------------------------------------------------
	//-iCol: Column index for the dividor
	//-jCol: Column index for the dividend 
	//-iRowMin: Row index to begin the inspection for the quotient rule
	//-iRowMax: Row index to end the inspection for the quotient rule
	//-minIndex: Resulting row index, for the quotient rule procedure
	//-minValue: Resulting value, for the quotien rule procedure
	//---------------------------------------------------------
	bool Column_MakeQuotientRule_MinPositive(UINT iCol, UINT jCol, UINT iRowMin, UINT iRowMax, UINT* minIndex, T* minValue);
	
	//Apply the lexicografic sort between iRow and jRow, for optimize the quotient rule, Matrix[i][iCol]/Matrix[i][jCol]
	//---------------------------------------------------------
	//-iCol: Column index for the divisor
	//-jCol: Column index for the dividend 
	//-iRowMin: Min row index to begin the exploration 
	//-iRowMax: Max row index to finish the exploration 
	//-minIndex: Resulting index for the Bland-Lexicografic rule
	//-minValue: Resulting value for the Bland-Lexicografic rule
	//---------------------------------------------------------
	bool Column_LexicographicalRule_MinPositive(UINT iCol, UINT jCol, UINT iRowMin, UINT iRowMax, CARRAY_UINT *priorityIndex, UINT* minIndex, T* minValue);

	//Aply the lexicografic sort for the rows index in dataArray iRow and jRow, for optimize the quotient rule, Matrix[i][iCol]/Matrix[i][jCol]
	//---------------------------------------------------------
	//-iCol: Column index for the divisor
	//-jCol: Column index for the dividend 
	//-dataArray: Array of row index to serch for, in the process
	//-minIndex: Resulting index for the Bland-Lexicografic rule
	//-minValue: Resulting value for the Bland-Lexicografic rule
	//---------------------------------------------------------
	bool Column_LexicographicalRule_MinPositive(UINT iCol, UINT jCol, CARRAY_UINT* indexList, CARRAY_UINT *priorityIndex, UINT* minIndex, T* minValue);

	//Evaluate the column iCol, ci*M[i][iCol] for each column between iRowMin and iRowMax
	//---------------------------------------------------------
	//-iRow: 
	//---------------------------------------------------------
	void Column_Evaluate(UINT iCol, UINT iRowMin, UINT iRowMax, CArray_<T>* cList, T* value);

	//Evaluate the column iCol, ci*M[i][iCol] for each column in the indexList
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Column_Evaluate(UINT iCol, CARRAY_UINT* indexList, CArray_<T> *cList, T *value);

	//Add a empty row in the matrix.
	//---------------------------------------------------------
	bool Row_Add();

	//Add several empty rows to the  matrix.
	//---------------------------------------------------------
	//-numColumns: Number of rows to add in to the matrix
	//---------------------------------------------------------
	bool Row_Add(UINT numRows);

	//Insert a empty row in the matrix.
	//---------------------------------------------------------
	//-iRow: Index position to insert the new row
	//---------------------------------------------------------
	bool Row_Insert(UINT iRow);

	//Remove a row from the matrix, in the iCol position
	//---------------------------------------------------------
	//-iCol: Row index position to remove from the matrix
	//---------------------------------------------------------
	bool Row_Remove(UINT iRow);

	//Remove a set of rows from the matrix, in the indexList positions
	//---------------------------------------------------------
	//-indexList: Rows list position to remove from the matrix
	//---------------------------------------------------------
	bool Row_Remove(CARRAY_UINT* indexList);

	//Multiplying the M[iRow] by a non-zero element sC 
	//---------------------------------------------------------
	//-iRow: Row index position
	//-sC: Value to multiply in the M[iRow] position
	//---------------------------------------------------------
	void Row_OpScMultiply(UINT iRow, T sC);

	//Multiplying the M[iRow] by a non-zero element sC 
	//---------------------------------------------------------
	//-iRow: Row index position
	//-xCol: Column to exclude in the multiply
	//-sC: Value to multiply in the M[iRow] position
	//---------------------------------------------------------
	void Row_OpScMultiply(UINT iRow, UINT xCol, T sC);

	//Interchange the row iRow by the row jRow
	//---------------------------------------------------------
	//-iRow: iRow position to interchange
	//-jRow: jRow position to interchange
	//---------------------------------------------------------
	void Row_OpInterchange(UINT iRow, UINT jRow);

	//Interchange the row M[iRow] by M[iRow] + C*M[jRow] 
	//---------------------------------------------------------
	//-iRow: iRow position to interchange
	//-jRow: jRow position to interchange
	//-C: Constant to multyply by M[jRow] value
	//---------------------------------------------------------
	void Row_OpInnerAdd(UINT iRow, UINT jRow, T C);

	//Interchange the column M[iRow] by M[iRow] + C*M[jRow] 
	//---------------------------------------------------------
	//-iRow: iRow position to interchange
	//-jRow: jRow position to interchange
	//-C: Constant to multyply by M[jRow] value
	//-xCol: 
	//---------------------------------------------------------
	void Row_OpInnerAdd(UINT iRow, UINT jRow, UINT xCol, T C);

	//Makes a full matrix pivot for the position (iRow, iCol)
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//---------------------------------------------------------
	void Row_Pivot(UINT iRow, UINT iCol);

	//Makes a full matrix pivot for the position (iRow, iCol), excludin the xRow
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//-xRow: xRow index position, to exclude in the pivot
	//---------------------------------------------------------
	void Row_Pivot(UINT iRow, UINT iCol, UINT xRow);

	//Makes a full matrix pivot for the position (iRow, iCol), excludin the xRow
	//---------------------------------------------------------
	//-iRow: iRow position for the operation
	//-jRow: jRow position for the operation
	//-xRow: xRow index position, to exclude in the pivot
	//---------------------------------------------------------
	void Row_Pivot(UINT iRow, UINT iCol, UINT xRow, UINT xCol);
	
	//Makes a partial matrix pivot for the position (iRow, iCol)
	//---------------------------------------------------------
	//-iRow: Row position for the operation
	//-iCol: Column position for the operation
	//-bool: if true, operates to the right, if false, operates to the left
	//---------------------------------------------------------
	void Row_Partial_Pivot(UINT iRow, UINT iCol, bool down);

	//Get the maximum value for the Matrix[iRow][], Saves the max value in the maxValue variable
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMinValue(UINT iRow, T* minValue);

	//Get the maximum index for the iRow, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMinValue(UINT iRow, UINT *minIndex, T* minValue);

	//Get the maximum index for the row iRow, between the column iColMin and iColMax, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMinValue(UINT iRow, UINT iColMin, UINT iColMax, UINT *minIndex, T* minValue);

	//Get the maximum index for the row iRow, between the column iColMin and iColMax, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMinValue(UINT iRow, CARRAY_UINT* indexList, UINT *minIndex, T *minValue);

	//Get the maximum value for the Matrix[iRow][], Saves the max value in the maxValue variable
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMaxValue(UINT iRow, T* maxValue);

	//Get the maximum index for the iRow, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMaxValue(UINT iRow, UINT *index, T* maxValue);

	//Get the maximum index for the row iRow, between the column iColMin and iColMax, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMaxValue(UINT iRow, UINT iColMin, UINT iColMax, UINT *index, T* maxValue);

	//Get the maximum index for the row iRow, between the column iColMin and iColMax, saves the index and the value
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_GetMaxValue(UINT iRow, CARRAY_UINT* indexList, UINT *maxIndex, T *maxValue);

	//Test if all the elements in the row are positive
	//---------------------------------------------------------
	bool Row_IsPositive(UINT iRow);

	//Test if all the elements between iColMin and iColMax are positive
	//---------------------------------------------------------
	bool Row_IsPositive(UINT iRow, UINT iColMin, UINT iColMax);

	//Test if all the elements in the row are positive 
	//---------------------------------------------------------
	bool Row_IsPositive(UINT iRow, CARRAY_UINT indexList);

	//Test if all the elements in the row are positive or zero
	//---------------------------------------------------------
	bool Row_IsPositiveZero(UINT iRow);

	//Test if all the elements between iColMin and iColMax are positive or zero
	//---------------------------------------------------------
	bool Row_IsPositiveZero(UINT iRow, UINT iColMin, UINT iColMax);

	//Test if all the elements in the row related with indexList are positive or zero
	//---------------------------------------------------------
	bool Row_IsPositiveZero(UINT iRow, CARRAY_UINT indexList);

	//Test if all the elements in the row are negative
	//---------------------------------------------------------
	bool Row_IsNegative(UINT iRow);

	//Test if all the elements between iColMin and iColMax are negative
	//---------------------------------------------------------
	bool Row_IsNegative(UINT iRow, UINT iColMin, UINT iColMax);

	//Test if all the elements in the row are negative
	//---------------------------------------------------------
	bool Row_IsNegative(UINT iRow, CARRAY_UINT indexList);

	//Test if all the elements in the row are negative
	//---------------------------------------------------------
	bool Row_IsNegativeZero(UINT iRow);

	//Test if all the elements between iColMin and iColMax are negative
	//---------------------------------------------------------
	bool Row_IsNegativeZero(UINT iRow, UINT iColMin, UINT iColMax);

	//Test if all the elements in the row are negative
	//---------------------------------------------------------
	bool Row_IsNegativeZero(UINT iRow, CARRAY_UINT indexList);

	//Aply the lexicographical sort between iRow and jRow, for optimize the quotient rule, Matrix[i][iCol]/Matrix[i][jCol]
	//---------------------------------------------------------
	//-iCol: Column index for the divisor
	//-jCol: Column index for the dividend 
	//-iRowMin: Min row index to begin the exploration 
	//-iRowMax: Max row index to finish the exploration 
	//-minIndex: Resulting index for the Bland-Lexicografic rule
	//-minValue: Resulting value for the Bland-Lexicografic rule
	//---------------------------------------------------------
	bool Row_LexicographicalRule_Min_Dual(UINT iRow, UINT jRow, UINT iColMin, UINT iColMax, UINT *minIndex, T *minValue);

	//Aply the lexicographical sort between iRow and jRow, for optimize the quotient rule, Matrix[i][iCol]/Matrix[i][jCol]
	//---------------------------------------------------------
	//-iCol: Column index for the divisor
	//-jCol: Column index for the dividend 
	//-iRowMin: Min row index to begin the exploration 
	//-iRowMax: Max row index to finish the exploration 
	//-minIndex: Resulting index for the Bland-Lexicografic rule
	//-minValue: Resulting value for the Bland-Lexicografic rule
	//---------------------------------------------------------
	bool Row_LexicographicalRule_Max_Dual(UINT iRow, UINT jRow, UINT iColMin, UINT iColMax, UINT *minIndex, T *minValue);

	//Applies the Driebeek rule for accelerating the Branch & Bounds Method
	//---------------------------------------------------------
	//-iRow: Row to apply the rule
	//-iColMin: Minimum Column index to search
	//-iColMax: Maximum Column index to search
	//-value: Value to be the optim penalization
	//-row: Row to be located the optim penalization
	//---------------------------------------------------------
	void Row_DriebeekRule_Max_Penalization(UINT iColMin, UINT iColMax, T* value, UINT* row);

	//Applies the Driebeek rule for accelerating the Branch & Bounds Method
	//---------------------------------------------------------
	//-iRow: Row to apply the rule
	//-iColMin: Minimum Column index to search
	//-iColMax: Maximum Column index to search
	//-value: Value to be the optim penalization
	//-row: Row to be located the optim penalization
	//---------------------------------------------------------
	void Row_DriebeekRule_Max_Penalization(UINT iRow, UINT iColMin, UINT iColMax, T* value);

	//Applies the Driebeek rule for accelerating the Branch & Bounds Method
	//---------------------------------------------------------
	//-iRow: Row to apply the rule
	//-iColMin: Minimum Column index to search
	//-iColMax: Maximum Column index to search
	//-value: Value to be the optim penalization
	//-row: Row to be located the optim penalization
	//-ex_list: array of indexes to be exclude
	//---------------------------------------------------------
	bool Row_DriebeekRule_Max_Penalization(UINT iColMin, UINT iColMax, CARRAY_UINT ex_list, CARRAY_UINT in_list, T* value, UINT* index);

	//Evaluate the iRow, ci*M[iRow][i] for each column between iColMin and iColMax
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_Evaluate(UINT iRow, UINT iColMin, UINT iColMax, CArray_<T>* cList, T* value);

	//Evaluate the iRow, ci*M[iRow][i] for each column in the indexList
	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	void Row_Evaluate(UINT iRow, CARRAY_UINT* indexList, CArray_<T> *cList, T *value);

	//Get a submatrix Sub from the original, from M[iRowMin][iColMin] to M[iRowMax][iColMax]
	//---------------------------------------------------------
	//-iRowMin: initial index for the rows
	//-iRowMax: last index for the rows
	//-iColMin: initial index for the columns
	//-iColMax: last index for the columns
	//-Sub: submatrix to extract
	//---------------------------------------------------------
	bool GetSubMatrix(UINT iRowMin, UINT iRowMax, UINT iColMin, UINT iColMax, CMatrix_<T>* Sub);

	void Row_Memcpy(T* arr,UINT row,UINT start = 0,bool getset = true/*true = get//false = set*/);
	#ifdef CUDA_PROJECT
	void Row_Memcpy_CUDA(T* arr,UINT row,UINT start = 0,bool getset = true/*true = get//false = set*/);

	void CUDA_memcpy_to_Array(T** Array, bool getset = true);
	#endif

	//Get a submatrix Sub from the original from a column index list called colIndexList 
	//and/or a row index list called rowIndexList
	//---------------------------------------------------------
	//-colIndexList: indexes of the columns to extract
	//-Sub: submatrix to extract
	//---------------------------------------------------------
	bool GetSubMatrix(CARRAY_UINT* colIndexList, CARRAY_UINT* rowIndexList, CMatrix_<T>* Sub);

	void HermiteNormForm(CMatrix_<T>* Matrix);

	void LDDeterminant(long double *det);

	bool NonZeroDiagonalByChangesRow();

	bool NonZeroDiagonalByChangesColumn();

	void Plus(CMatrix_<T>* A, CMatrix_<T>* B);

	void Multiply(CMatrix_<T>* A, CMatrix_<T>* B);

	void Minus(CMatrix_<T>* A, CMatrix_<T>* B);


	bool Equals(const CMatrix_<T> A)
	{
		Resize(A.GetRowSize(), A.GetColumnSize());
		if ( A.GetRowSize() == m_nRows )
		{
			if ( A.GetColumnSize() == m_nCols )
			{
				for ( int i = 0; i < A.GetRowSize(); i++ )
				{
					for ( int j = 0; j < A.GetColumnSize(); j++ )
					{
						Matrix[i][j] = A[i][j];
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}


	void DiagonalMaxCols(UINT num);

	bool ChangeDiagonalMax(UINT num);

	void LowerTriangularSystem(CMatrix_<T> *b, CMatrix_<T> *x, bool front);

	

	void memcpy_to_Array(T** Array, bool getset = true);

	void InterchangeRows(UINT row1, UINT row2);

	void InterchangeRows(UINT* rows);

	void InterchangeRows(int* rows);

	void InterchangeRows(CArray_<int>* rows);

	void InterchangeRows(CArray_<UINT>* rows);

	T* operator [] (const UINT index)
	{
		return Matrix[index];
	}

	T* operator [] (const UINT index) const
	{
		return Matrix[index];
	}

	const CMatrix_& operator = (const CMatrix_& obj) 
	{
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				#ifdef _CMP_VST_WIN
 				CopyMemory(&Matrix[i], obj.Matrix[i], m_nCols*sizeof(T));
				#endif
				#ifdef _CMP_GCC_WIN
				memcpy(&Matrix[i], obj.Matrix[i], m_nCols*sizeof(T));
				#endif
			}
		}
		else
		{
			if(Resize(obj.m_nRows, obj.m_nCols))
			{
				#ifdef _CMP_VST_WIN
				for(UINT i=0; i<m_nRows; i++)
				{
 					CopyMemory(&Matrix[i], obj.Matrix[i], m_nCols*sizeof(T));
				}
				#endif
				#ifdef _CMP_GCC_WIN
				for(UINT i=0; i<m_nRows; i++)
				{
 					memcpy(&Matrix[i], obj.Matrix[i], m_nCols*sizeof(T));
				}
				#endif	
			}
		}
		return *this;
	}	
	const CMatrix_ operator + (const CMatrix_& obj)
	{
		CMatrix_<T> sCopy = *this;
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					sCopy.Matrix[i][j] = Matrix[i][j] + obj.Matrix[i][j];
				}
			}
		}
		return sCopy;
	}


	const CMatrix_ operator - (const CMatrix_& obj)
	{
		CMatrix_<T> sCopy = *this;
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					sCopy.Matrix[i][j] = Matrix[i][j] - obj.Matrix[i][j];
				}
			}
		}
		return sCopy;
	}


	const CMatrix_ operator*(const CMatrix_& obj)
	/*{
		assert(m_nRows == obj.m_nCols);
		CMatrix_<T> mRes = CMatrix_<T>(m_nCols, obj.m_nRows);
		for(register UINT i=0; i<mRes.m_nRows; i++)
		{
			for(register UINT j=0; j<mRes.m_nCols; j++)
			{
				mRes.Matrix[i][j] = 0;
				for(register UINT r = 0; r<m_nCols; r++)
				{
					mRes.Matrix[i][j] += (Matrix[i][r]*obj.Matrix[r][j]);
				}
			}
		}
		return mRes;
	}*/
	{
		assert(m_nCols == obj.m_nRows);
		CMatrix_<T> mRes = CMatrix_<T>(m_nRows,obj.m_nCols);
		for ( register UINT i = 0; i < mRes.m_nRows; i++)
		{
			for ( register UINT j = 0; j < mRes.m_nCols; j++)
			{
				mRes[i][j] = 0;
				for ( register UINT k = 0; k < m_nCols; k++ )
				{
					mRes[i][j] += (Matrix[i][k]*obj[k][j]);
				}
			}
		}
		return mRes;
	}


	const CMatrix_& operator += (const CMatrix_& obj)
	{
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					Matrix[i][j] += obj.Matrix[i][j];
				}
			}
		}
		return *this;
	}

	const CMatrix_& operator -= (const CMatrix_& obj)
	{
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					Matrix[i][j] -= obj.Matrix[i][j];
				}
			}
		}
		return *this;
	}


	bool operator == (const CMatrix_& obj)
	{
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					if(Matrix[i][j] != obj.Matrix[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator != (const CMatrix_& obj)
	{
		if(m_nRows == obj.m_nRows && m_nCols == obj.m_nCols)
		{
			for(UINT i=0; i<m_nRows; i++)
			{
				for(UINT j=0; j<m_nCols; j++)
				{
					if(Matrix[i][j] != obj.Matrix[i][j])
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	friend ostream& operator << (ostream &out, CMatrix_<T> &obj)
	{
		for(UINT i=0; i<obj.m_nRows; i++)
		{
			for(UINT j=0; j<obj.m_nCols; j++)
			{
				out << obj.Matrix[i][j] << " ";
			}
		}
		return out;
	}
	friend istream& operator >> (istream &in, CMatrix_<T> &obj)
	{
		//in.Read();
		for(UINT i=0; i<obj.m_nRows; i++)
		{
			for(UINT j=0; j<obj.m_nCols; j++)
			{
				if(!in)
				{
					break;
				}
				//in.Read((char*)&obj.Matrix[i][j]
				in >> obj.Matrix[i][j];
			}
		}
		return in;
	}
	
	//Friend Functions
	//----------------------------------------------------------------------------------------
	#ifdef	_CMP_VST_WIN //Windows Compiling
	template < typename T > friend void CMatrix_Identity(CMatrix_<T>* pCMatrix, UINT n);
	template < typename T > friend void CMatrix_Random(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m);
	template < typename T > friend void CMatrix_IntegerRandom(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m);
	template < typename T > friend void CMatrix_OpenTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList);
	template < typename T > friend void CMatrix_SaveTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult);
	template < typename T > friend void CMatrix_SaveTableauDataStandard(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult);

	template < typename T > friend void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x);
	template < typename T > friend void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x);
	template < typename T > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x);
	template < typename T > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x);

	template < typename T > friend bool Get_TriangularMatrix(const CMatrix_<T>* Matrix, CARRAY_UINT* indexList, bool* bUpperMatrix);
	template < typename T > friend void LU_Factorization(CMatrix_<T>* Matrix, CMatrix_<T>* L_Matrix, CMatrix_<T>* U_Matrix);

	template < typename T > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vinList, CARRAY_UINT* voutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	template < typename T > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, WCHAR* outMsg);
	template < typename T > friend void CMatrix_Simplex_2Phases(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	template < typename T > friend void CMatrix_SimplexDual(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	template < typename T > friend void CMatrix_SRSimplex(CMatrix_<T>* sRTableau, CArray_<T>* cList, CMatrix_<T>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res);
	#endif

	#ifdef __AFXWIN_H__
	template < typename T > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, CListBox* msgBox);
	template < typename T > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list);
	template < typename T > friend void CMatrix_SimplexDual(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list);
	template < typename T > friend void CMatrix_IntegerProgramming(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, CARRAY_UINT* vAList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE t, LPVOID out_list);
	template < typename T > friend void CMatrix_Simplex_2Phases(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list);
	template < typename T > friend void CMatrix_SRSimplex(CMatrix_<T>* sRTableau, CArray_<T>* cList, CMatrix_<T>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res, LPVOID out_list);
	#endif //__AFXWIN_H__ 
	
	#ifdef _CMP_GCC_WIN //Not Windows Compiling

	template < typename U > friend void CMatrix_Identity(CMatrix_<U>* pCMatrix, UINT n);
	template < typename U > friend void CMatrix_Random(CMatrix_<U>* pCMatrix, U minValue, U maxValue, UINT n, UINT m);
	       //template < typename T > void CMatrix_Random(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m)
	template < typename U > friend void CMatrix_OpenTableauData(const wchar_t *fileName, CMatrix_<U>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList);
	template < typename U > friend void CMatrix_SaveTableauData(const wchar_t *fileName, CMatrix_<U>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult);

	template < typename U > friend void CMatrix_Solve_SysTryINF(const CMatrix_<U>* m, const CMatrix_<U>* b, CMatrix_<U>* x);
	template < typename U > friend void CMatrix_Solve_SysTryINF(const CMatrix_<U>* m, const CMatrix_<U>* b, CArray_<U>* x);
	template < typename U > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<U>* m, const CMatrix_<U>* b, CMatrix_<U>* x);
	template < typename U > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<U>* m, const CMatrix_<U>* b, CArray_<U>* x);

	template < typename U > friend bool Get_TriangularMatrix(const CMatrix_<U>* Matrix, CARRAY_UINT* indexList, bool* bUpperMatrix);
	template < typename U > friend void LU_Factorization(CMatrix_<U>* Matrix, CMatrix_<U>* L_Matrix, CMatrix_<U>* U_Matrix);
	
	template < typename U > friend void CMatrix_SimplexCanonical(CMatrix_<U>* mTableau, bool minimize, CARRAY_UINT* vinList, CARRAY_UINT* voutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	//template < typename U > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, WCHAR* outMsg);
	template < typename U > friend void CMatrix_Simplex_2Phases(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	template < typename U > friend void CMatrix_SimplexDual(CMatrix_<U>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
	template < typename U > friend void CMatrix_SRSimplex(CMatrix_<U>* sRTableau, CArray_<U>* cList, CMatrix_<U>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res);
/*
	friend void CMatrix_Identity();
	friend void CMatrix_Random();
	friend void CMatrix_OpenTableauData();
	friend void CMatrix_SaveTableauData();
	friend void CMatrix_Solve_SysTryINF();
	friend void CMatrix_Solve_SysTrySUP();
	friend void CMatrix_SimplexCanonical();
	friend void CMatrix_Simplex_2Phases();
	friend void CMatrix_SimplexDual():
	friend void CMatrix_SRSimplex();*/
	#endif
	//----------------------------------------------------------------------------------------

protected:

	//Matrix rows numbers
	UINT m_nRows;
	//Matrix columns numbers
	UINT m_nCols;
	//Last matrix row
	UINT m_nLastRowIndex;
	//Last matrix column
	UINT m_nLastColumnIndex;
	//Flag for calling the destructor
	bool m_bUseDestructor;

	bool m_bFromFile;

private:



	//Free the matrix memory
	//---------------------------------------------------------
	void Dispose();
	T detaux(CArray_<bool> *col,CMatrix_<T> *matriz)
	{
		UINT cont, cont2;
		T aux, deter = 0;
		cont = 0;

		for(register UINT i = 0 ; i < (*col).GetSize() ; ++i)
		{
			if((*col)[i]){
				++cont;
			}
		}

		if(cont>2)
		{
			cont2=0;
			for(register UINT i = 0 ; i < (*col).GetSize() ; ++i)
			{
				if((*col)[i])
				{
					if((*matriz)[(*col).GetSize()-cont][i]!= 0)
					{
						CArray_<bool> cols = CArray_<bool>((*col).GetSize());
						for(register UINT j = 0; j < (*col).GetSize() ; ++j)
						{
							cols[j] = (*col)[j];
						}
						cols[i] = false;
						aux = pow((T)-1,(T)(2 + cont2))* ((*matriz)[(*col).GetSize()-cont][i])*detaux(&cols,matriz);
						deter += aux;
						cols.~CArray_();
						++cont2;
					}
				}
			}
			return deter;
		}
		else
		{
			CMatrix_<T> aux2 = CMatrix_<T>(2);
			cont2=0;
			for(register UINT i = 0; i<(*col).GetSize();++i)
			{
				if((*col)[i]){
					aux2[0][cont2] = (*matriz)[(*col).GetSize()-2][i];
					aux2[1][cont2] = (*matriz)[(*col).GetSize()-1][i];
					++cont2;
				}
			}
			return  aux2[0][0]*aux2[1][1]  -  aux2[1][0]*aux2[0][1];
		}
	}


	long double LDDeterminant(CArray_<bool> *col,CMatrix_<T> *matriz)
	{
		UINT cont, cont2;
		long double aux, deter = 0;
		cont = 0;

		for(register UINT i = 0 ; i < (*col).GetSize() ; ++i)
		{
			if((*col)[i]){
				++cont;
			}
		}

		if(cont>2)
		{
			cont2=0;
			for(register UINT i = 0 ; i < (*col).GetSize() ; ++i)
			{
				if((*col)[i])
				{
					if((*matriz)[(*col).GetSize()-cont][i]!= 0)
					{
						CArray_<bool> cols = CArray_<bool>((*col).GetSize());
						for(register UINT j = 0; j < (*col).GetSize() ; ++j)
						{
							cols[j] = (*col)[j];
						}
						cols[i] = false;
						aux = powl((T)-1,(T)(2 + cont2))* ((long double)((*matriz)[(*col).GetSize()-cont][i]))*LDDeterminant(&cols,matriz);
						deter += aux;
						cols.~CArray_();
						++cont2;
					}
				}
			}
			return deter;
		}
		else
		{
			CMatrix_<T> aux2 = CMatrix_<T>(2);
			cont2=0;
			for(register UINT i = 0; i<(*col).GetSize();++i)
			{
				if((*col)[i]){
					aux2[0][cont2] = (*matriz)[(*col).GetSize()-2][i];
					aux2[1][cont2] = (*matriz)[(*col).GetSize()-1][i];
					++cont2;
				}
			}
			return  aux2[0][0]*aux2[1][1]  -  aux2[1][0]*aux2[0][1];
		}
	}
template < typename U > friend void CMatrix_Identity(CMatrix_<T>* pCMatrix, UINT n);
template < typename U > friend void CMatrix_Random(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m);
template < typename U > friend void CMatrix_OpenTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList);
template < typename U > friend void CMatrix_SaveTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult);

template < typename U > friend void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x);
template < typename U > friend void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x);
template < typename U > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x);
template < typename U > friend void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x);

template < typename U > friend bool Get_TriangularMatrix(const CMatrix_<T>* Matrix, CARRAY_UINT* indexList, bool* bUpperMatrix);
template < typename U > friend void LU_Factorization(CMatrix_<T>* Matrix, CMatrix_<T>* L_Matrix, CMatrix_<T>* U_Matrix);

template < typename U > friend void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vinList, CARRAY_UINT* voutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);

template < typename U > friend void CMatrix_Simplex_2Phases(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
template < typename U > friend void CMatrix_SimplexDual(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res);
template < typename U > friend void CMatrix_SRSimplex(CMatrix_<T>* sRTableau, CArray_<T>* cList, CMatrix_<T>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res);

template <typename U> friend void CMatrix_SimplexCanonicalP(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list);
template <typename U> friend void CMatrix_SimplexDualP(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list);

};

//Long double matrix type
//---------------------------------------------------------
typedef CMatrix_<long double> CMATRIX_LONGDOUBLE;

//Long double matrix type
//---------------------------------------------------------
typedef CMatrix_<double> CMATRIX_DOUBLE;

//Unsigned integer matrix type
//---------------------------------------------------------
typedef CMatrix_<UINT> CMATRIX_UINT;

//Float matrix type
//---------------------------------------------------------
typedef CMatrix_<float> CMATRIX_FLOAT;

class CMATRIX_FLOAT4 : public CMATRIX_FLOAT
{
public:
	CMATRIX_FLOAT4();
};



#endif 
