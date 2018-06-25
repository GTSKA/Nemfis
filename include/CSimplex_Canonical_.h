#ifndef CCanonicaSimplex_H
#define CCanonicaSimplex_H

#include "CMatrix_.h"
#include "CArray_.h"

#define SPLX_CANONICAL_VERSION	1.1
#define SPLX_CANONICAL_DEBUG_CONSOLE


#ifdef _CMP_VST_MFC
#include <BCGCBProInc.h>
template <typename T> struct CSimplex_DataMsg_
{
	CBCGPGridCtrl* m_gridMsg;
	void SendMsg(UINT nSteps, 
		WCHAR* msg,
		UINT xIn,
		UINT xOut,
		T z_value,
		double e_time, bool Only_Message = false);
};
template <typename T> void CSimplex_DataMsg_<T>::SendMsg(UINT nSteps, WCHAR *msg, UINT xIn, UINT xOut, T z_value, double e_time, bool Only_Message = false)
{
	m_gridMsg->AddRow(false);
	if (!Only_Message)
	{
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->m_strFormatDouble = "%.10lf";
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(0)->SetValue(nSteps,FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(1)->SetValue(msg,FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(2)->SetValue(xIn,FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(3)->SetValue(xOut,FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(4)->SetValue(double(z_value),FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(5)->SetValue(e_time,FALSE);
	}
	else
	{
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(0)->SetValue(L"-",FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(1)->SetValue(msg,FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(2)->SetValue(L"-",FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(3)->SetValue(L"-",FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(4)->SetValue(L"-",FALSE);
		m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(5)->SetValue(L"-",FALSE);
	}
	m_gridMsg->AdjustLayout();
}
#define WIN32_LEAN_AND_MEAN
#endif


//struct splx_info
//{
//	CListBox* m_list;
//	CListBox* m_z_list;
//	CListBox* m_b_list;
//};

//Enumerates the simplex options
typedef enum SIMPLEX_OPTIONS
{
	//Minimize function
	SX_MINIMIZE = 0, 
	//Maximize function
	SX_MAXIMIZE = 1,
}SIMPLEX_OPTIONS, *LPSIMPLEX_OPTIONS;

template <typename T> struct SIMPLEX_PARAMETERS
{
	CMatrix_<T> mTableau; 
	bool minimize; 
	CARRAY_UINT vInList; 
	CARRAY_UINT vOutList;
	UINT nLastInput; 
	UINT numSteps; 
	SIMPLEX_RESULT res;
};

//-----------
//Get a string message for SIMPLEX_RESULT value
static void SPLXRES_to_STR(LPSIMPLEX_RESULT value, char* msg)
{
	switch(*value)
	{
	case SX_FAIL:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "ERROR");
		break;

	case SX_NO_OPTIM:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "NOT OPTIM");
		break;

	case SX_OPTIM:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "OPTIM");
		break;

	case SX_NOT_FEASIBLE:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "NOT FEASIBLE");
		break;

	case SX_NOT_BOUNDED:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "NOT BOUNDED");
		break;

	case SX_NOT_SOLUTION:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "NOT SOLUTION");
		break;

	case SX_DUAL_NOT_FEASIBLE:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "DUAL NOT FEASIBLE");
		break;

	case SX_DUAL_NOT_BOUNDED:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), "DUAL NOT BOUNDED");
		break;

	default:
		if(msg==NULL)
		{
			msg = new CHAR[MAX_PATH];
		}
		sprintf_s(msg, MAX_PATH* sizeof(CHAR), " ");
		break;
	}
}


template <typename T> void CMatrix_MakeRandomTableau(UINT nR, UINT nV, CMatrix_<T>* tableau, CARRAY_UINT* vInList)
{
	WCHAR msg[255] ={0}; 

	//First resize the tableau,...
	CMatrix_Random<T>(tableau, -100, 500, nR, nV);

	//Identity Matrix
	CMatrix_<T> I;
	CMatrix_Identity(&I, nR);
	I.Row_Insert(0);

	//Z Row
	CMatrix_<T> Z;
	CMatrix_Random<T>(&Z, -100, 100, 1, nV);

	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, 0, 200, nR + 1, 1);
	B.Matrix[0][0] = 0;

	tableau->Join_Up(&Z);
	tableau->Join_Right(&I);
	tableau->Join_Right(&B);

	vInList->Resize(I.GetColumnSize());
	for(UINT i=0; i<vInList->GetSize(); i++)
	{
		(*vInList)[i] = vInList->GetSize() + i;
	}
}
template <typename T> void CMatrix_MakeRandomTableau(UINT nR, UINT nV, CMatrix_<T>* tableau, CARRAY_UINT* vInList, T min_A, T max_A, T min_Z, T max_Z, T min_b, T max_b)
{
	WCHAR msg[255] ={0}; 

	//First resize the tableau,...
	CMatrix_Random<T>(tableau, min_A, max_A, nR, nV);

	//Identity Matrix
	CMatrix_<T> I;
	CMatrix_Identity(&I, nR);
	I.Row_Insert(0);

	//Z Row
	CMatrix_<T> Z;
	CMatrix_Random<T>(&Z, min_Z, max_Z, 1, nV);

	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, min_b, max_b, nR + 1, 1);
	B.Matrix[0][0] = 0;

	tableau->Join_Up(&Z);
	tableau->Join_Right(&I);
	tableau->Join_Right(&B);

	vInList->Resize(I.GetColumnSize());
	for(UINT i=0; i<vInList->GetSize(); i++)
	{
		(*vInList)[i] = vInList->GetSize() + i;
	}
}
template <typename T> void CMatrix_MakeRandomIntegerTableau(UINT nR, UINT nV, CMatrix_<T>* tableau, CARRAY_UINT* vInList, T min_A, T max_A, T min_Z, T max_Z, T min_b, T max_b)
{
	//First resize the tableau,...
	CMatrix_IntegerRandom<T>(tableau, min_A, max_A, nR, nV);

	//Identity Matrix
	CMatrix_<T> I;
	CMatrix_Identity(&I, nR);
	I.Row_Insert(0);

	//Z Row
	CMatrix_<T> Z;
	CMatrix_IntegerRandom<T>(&Z, min_Z, max_Z, 1, nV);

	//B Column
	CMatrix_<T> B;
	CMatrix_IntegerRandom<T>(&B, min_b, max_b, nR + 1, 1);
	B.Matrix[0][0] = 0;

	tableau->Join_Up(&Z);
	tableau->Join_Right(&I);
	tableau->Join_Right(&B);

	vInList->Resize(I.GetColumnSize());
	for(UINT i=0; i<vInList->GetSize(); i++)
	{
		(*vInList)[i] = vInList->GetSize() + i;
	}	
}
//
//---------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------
template <typename T> void CMatrix_OpenTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList)
{
	ifstream fStream(fileName);

	if(!fStream)
	{
		return;
	}

	UINT nRows = 0;
	UINT nCols = 0;
	UINT nVinListSize = 0;

	//Read the size of the tableau
	//-----------------------------------------------------------------------------
    char strCommand[256] = {0};
	do
    {
        fStream >> strCommand;
		//cout << strCommand << endl;
        if(!fStream)
		{
            break;
		}

		if( strcmp(strCommand, "#nRows") == 0)
		{
			fStream >> nRows;
		}

		if( strcmp(strCommand, "#nCols") == 0)
		{
			fStream >> nCols;
		}
	}while(nRows == 0 || nCols == 0);
	fStream.ignore();

	//Read the simplex procedure type...
	//-----------------------------------------------------------------------------
	char strProcedure[256] = {0};
	if(!fStream)
	{
		fStream.close();
		return;
	}
	fStream >> strCommand;
	if( strcmp(strCommand, "#Procedure") == 0)
	{
		fStream >> strProcedure;
		//MessageBoxA(NULL, strProcedure, "Procedure", 0);
	}
	

	//Read the minimize or maximixe flag
	//-----------------------------------------------------------------------------
	*minimize = false;
	char minimizeFlag[256] = {0};
	if(!fStream)
	{
		fStream.close();
		return;
	}
	fStream >> strCommand;
	if(strcmp(strCommand, "#Minimize") == 0)
	{
		fStream >> minimizeFlag;
		//MessageBoxA(NULL, minimizeFlag, "Minimize Flag", 0);
		if(strcmp(minimizeFlag, "true") == 0)
		{
			*minimize = false;
		}
	}

	//Read the result
	//-----------------------------------------------------------------------------
	char result[256] = {0};
	if(!fStream)
	{
		fStream.close();
		return;
	}
	fStream >> strCommand;
	if(strcmp(strCommand, "#Result") == 0)
	{
		fStream >> minimizeFlag;
		//MessageBoxA(NULL, #Result, "result", 0);
	}

	//Read the number of input variables 
	//-----------------------------------------------------------------------------
	do
    {
        fStream >> strCommand;
		if(!fStream)
		{
            break;
		}
		if( strcmp(strCommand, "#nVinSize") == 0)
		{
			fStream >> nVinListSize;
		}
	}while(nVinListSize == 0);
	
	//Resize the vInput
	if(nVinListSize > 0)
	{
		vinList->Resize(nVinListSize);
	}

	WCHAR str[255] = {0};
	//wsprintf(str, L"Vin Size = %d", vinList->GetSize());
	//MessageBox(NULL, str, L"Vin Size", 0);

	//Read the input variables values
	//-----------------------------------------------------------------------------
	int i = 0;
	do
	{
		fStream >> strCommand;
		if(!fStream)
		{
            break;
		}
		if( strcmp(strCommand, "#X") == 0)
		{
			fStream >> (*vinList)[i];
			//wsprintf(str, L"Vin = %d", (*vinList)[i]);
			//MessageBox(NULL, str, L"Vin Value", 0);
			i++;
		}
	}
	while(i < vinList->GetSize());
	*voutList = *vinList;

	//Read the matrix values
	//-----------------------------------------------------------------------------
	if(nRows > 0 && nCols > 0)
	{
		mTableau->Resize(nRows, nCols);
		for(register UINT i=0; i<mTableau->m_nRows; i++)
		{
			for(register UINT j=0; j<mTableau->m_nCols; j++)
			{
				if(!fStream)
				{
					break;
				}
				fStream >> mTableau->Matrix[i][j];
			}
		}
	}
	fStream.close();
}

//
//---------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------
template <typename T> void CMatrix_SaveTableauData(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult)
{
	ofstream fStream(fileName);
	if(fStream)
	{
		//Write the matrix values
		//-----------------------------------------------------------------------------
		fStream << "#nRows " << mTableau->m_nRows << endl;
		fStream << "#nCols " << mTableau->m_nCols << endl;
		fStream << endl;

		//Write the procedure type
		//-----------------------------------------------------------------------------
		fStream << "#Procedure " << "SMPLX_CANONIC" << endl;

		//Write minimize flag
		//-----------------------------------------------------------------------------
		if(*minimize)
		{
			fStream << "#Minimize " << "true" << endl;
		}
		else
		{
			fStream << "#Minimize " << "false" << endl;
		}

		//Write the result
		//-----------------------------------------------------------------------------
		fStream << "#Result " << *splxResult << endl;

		//Write inputa variables size
		//-----------------------------------------------------------------------------
		fStream << "#nVinSize " << vinList->GetSize() << endl;

		for(register UINT i=0; i<vinList->GetSize(); i++)
		{
			fStream << "#X " << (*vinList)[i] << endl;
		}

		//Write the matrix values
		//-----------------------------------------------------------------------------
		fStream << endl;
		for(register UINT i=0; i<mTableau->m_nRows; i++)
		{
			for(register UINT j=0; j<mTableau->m_nCols; j++)
			{
				fStream << mTableau->Matrix[i][j] << " ";
			}
			fStream << endl;
		}
	}
	fStream.close();
}
template <typename T> void CMatrix_SaveTableauDataStandard(const wchar_t *fileName, CMatrix_<T>* mTableau, bool* minimize, LPSIMPLEX_PROCEDURE_TYPE procedure, CARRAY_UINT* vinList, CARRAY_UINT* voutList, LPSIMPLEX_RESULT splxResult)
{
	ofstream fStream(fileName);
	if(fStream)
	{
		//Write the matrix values
		//-----------------------------------------------------------------------------
		fStream << "#nRows " << mTableau->m_nRows << endl;
		fStream << "#nCols " << mTableau->m_nCols << endl;
		fStream << endl;

		//Write the procedure type
		//-----------------------------------------------------------------------------
		fStream << "#Procedure " << "SMPLX_CANONIC" << endl;

		//Write minimize flag
		//-----------------------------------------------------------------------------
		if(*minimize)
		{
			fStream << "#Minimize " << "true" << endl;
		}
		else
		{
			fStream << "#Minimize " << "false" << endl;
		}

		//Write the result
		//-----------------------------------------------------------------------------
		fStream << "#Result " << *splxResult << endl;

		//Write inputa variables size
		//-----------------------------------------------------------------------------
		fStream << "#nVinSize " << vinList->GetSize() << endl;

		for(register UINT i=0; i<vinList->GetSize(); i++)
		{
			fStream << "#X " << (*vinList)[i] << endl;
		}

		//Write the matrix values
		//-----------------------------------------------------------------------------
		fStream << endl;
		for(register UINT i=0; i<mTableau->m_nRows; i++)
		{
			for(register UINT j=0; j<mTableau->m_nCols; j++)
			{
				if ( j < mTableau->m_nCols-1 )
				{
					if ( mTableau->Matrix[i][j] >= mTableau->Z )
					{
						fStream << " +"<<mTableau->Matrix[i][j] << "*x" << j << "\t";
					}
					else
					{
						fStream << " "<<mTableau->Matrix[i][j] << "*x" << j << "\t";
					}
				}
				else if ( j == mTableau->m_nCols-1 )
				{
					fStream << " <=  "<<mTableau->Matrix[i][j] << " ;";
				}
				else
				{
					fStream << mTableau->Matrix[i][j] ;
				}
			}
			fStream << endl;
		}
	}
	fStream.close();
}
//Make the Canonical Simplex procedure
//-----------------------------------------------------------------
//-tableau: The matrix-tableau, to process the simplex
//-----------------------------------------------------------------
template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	//Variables
	UINT iSrow = 0;

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;
	#endif

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;
		
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif



		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != T(0))
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "Seting Up the Tableau.... " << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			//--------------------------------------------	---------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <= T(0))
			{
				*res = SX_OPTIM;
				bStop = true;

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						(*numSteps)++;

						


						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintfToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >= T(0))
			{
				*res = SX_OPTIM;
				bStop = true;

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					cout << "Resulting variables:" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					for(register UINT i = 0; i<vInList->GetSize(); i++)
					{
						cout << "V[" << i << "] = " << (*vInList)[i] << endl;
					}
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
	
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;

						(*numSteps)++;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintfToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
}
//Make the Canonical Simplex procedure
//-----------------------------------------------------------------
//-tableau: The matrix-tableau, to process the simplex
//-----------------------------------------------------------------
template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, WCHAR* outMsg)
{
	//Variables
	UINT iSrow = 0;

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	
	//wsprintf(outMsg, L"Beginning the simplex procedure...");
	Sleep(2000);
	

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	/*cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;*/
	#endif

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;

		//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULR: NOT FEASIBLE");
		Sleep(2000);
		
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		/*cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;*/
		#endif

		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != 0)
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

	//wsprintf(outMsg, L"SETING UP THE TABLEAU...");
	Sleep(2000);

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	//cout << endl;
	//cout << "Seting Up the Tableau.... " << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	//cout << endl;
	//mTableau->PrintToConsole();
	//cout << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <=0)
			{
				*res = SX_OPTIM;
				bStop = true;

				//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: OPTIM");
				Sleep(2000);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULR: NOT FEASIBLE");
					Sleep(2000);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULT: NOT BOUNDED");
						Sleep(2000);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						
						(*numSteps)++;

						//wsprintf(outMsg, L"SIMPLEX ITEARTION %d, PIVOT( %d, %d )", *numSteps, mRowIndex, *nLastInput);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >=0)
			{
				*res = SX_OPTIM;
				bStop = true;

				//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: OPTIM");
				Sleep(2000);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: NOT FEASIBLE");
					Sleep(2000);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: NOT BOUDED");
						Sleep(2000);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
	
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;

						(*numSteps)++;

						//wsprintf(outMsg, L"SIMPLEX ITEARTION %d, PIVOT( %d, %d )", *numSteps, mRowIndex, *nLastInput);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						/*cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
}



//Make the Canonical Simplex procedure
//-----------------------------------------------------------------
//-tableau: The matrix-tableau, to process the simplex
//-----------------------------------------------------------------
#ifdef __AFXWIN_H__

//Make the Canonical Simplex procedure
//-----------------------------------------------------------------
//-sMsg: Mesage to write in the first cell of the CListCtrl control
//-sXin: Last input variable
//-sXout: Last output variable
//-Z: Objective function value
//-mList: Pointer to an CListCtrl control, to output the message
//-----------------------------------------------------------------
static void Write_SimplexMSG_ToCListCtrl(WCHAR* sMsg, UINT sXin, UINT sXout, long double Z,  CListCtrl* mList, bool bWriteX = false)
{
	mList->InsertItem(mList->GetItemCount(), sMsg);

	WCHAR str[255] = {0};

	if(bWriteX)
	{
		StringCchPrintf(str, sizeof(str), L"%d", sXin);
		mList->SetItemText(mList->GetItemCount() - 1, 1, str);
		
		StringCchPrintf(str, sizeof(str), L"%d", sXout);
		mList->SetItemText(mList->GetItemCount() - 1, 2, str);

		StringCchPrintf(str, sizeof(str), L"%lf", Z);
		mList->SetItemText(mList->GetItemCount() - 1, 3, str);
	}
	else
	{
		mList->SetItemText(mList->GetItemCount() - 1, 1, L" - ");
		mList->SetItemText(mList->GetItemCount() - 1, 2, L" - ");
		mList->SetItemText(mList->GetItemCount() - 1, 3, L" - ");
	}
	
	
}

template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, CListBox* msgBox)
{
	WCHAR outMsg[255] = {0};

	//Variables
	UINT iSrow = 0;

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	//wsprintf(outMsg, L"Beginning the simplex procedure...");
	msgBox->AddString(outMsg);
	Sleep(2000);
	

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	/*cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;*/
	#endif

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;

		//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULR: NOT FEASIBLE");
		msgBox->AddString(outMsg);
		Sleep(2000);
		
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		/*cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;*/
		#endif

		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != 0)
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

//	wsprintf(outMsg, L"SETING UP THE TABLEAU...");
	msgBox->AddString(outMsg);
	Sleep(2000);

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	//cout << endl;
	//cout << "Seting Up the Tableau.... " << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	//cout << endl;
	//mTableau->PrintToConsole();
	//cout << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <=0)
			{
				*res = SX_OPTIM;
				bStop = true;

				//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: OPTIM");
				msgBox->AddString(outMsg);
				Sleep(2000);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULR: NOT FEASIBLE");
					msgBox->AddString(outMsg);
					Sleep(2000);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						//wsprintf(outMsg, L"FINISHING SIMPLEX PROCEDURE, RESULT: NOT BOUNDED");
						msgBox->AddString(outMsg);
						Sleep(2000);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						
						(*numSteps)++;

						//wsprintf(outMsg, L"ITERATION %d, PIVOT( %d, %d )", *numSteps, mRowIndex, *nLastInput);
						msgBox->AddString(outMsg);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >=0)
			{
				*res = SX_OPTIM;
				bStop = true;

				///wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: OPTIM");
				msgBox->AddString(outMsg);
				Sleep(2000);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: NOT FEASIBLE");
					msgBox->AddString(outMsg);
					Sleep(2000);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						//wsprintf(outMsg, L"FINISHING THE SIMPLEX PROCEDURE, RESULT: NOT BOUDED");
						msgBox->AddString(outMsg);
						Sleep(2000);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
	
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;

						(*numSteps)++;

						//wsprintf(outMsg, L"ITERATION %d, PIVOT( %d, %d )", *numSteps, mRowIndex, *nLastInput);
						msgBox->AddString(outMsg);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						/*cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}

	//delete[] outMsg;
}




template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, CListCtrl* mList)
{
	WCHAR outMsg[255] = {0};

	//Variables
	UINT iSrow = 0;

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	Write_SimplexMSG_ToCListCtrl(L"Beginning the simplex procedure", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
	Sleep(100);

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	/*cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;*/
	#endif

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;

		Write_SimplexMSG_ToCListCtrl(L"Finishing the simplex procedure, Result: NOT FEASIBLE", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
		Sleep(100);
		
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		/*cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;*/
		#endif

		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != 0)
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

	Write_SimplexMSG_ToCListCtrl(L"Setting up the tableau", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
	Sleep(100);

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	//cout << endl;
	//cout << "Seting Up the Tableau.... " << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	//cout << endl;
	//mTableau->PrintToConsole();
	//cout << endl;
	//cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <=0)
			{
				*res = SX_OPTIM;
				bStop = true;
				
				Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: OPTIM", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
				Sleep(100);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: NOT FEASIBLE", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
					Sleep(100);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: NOT BOUNDED", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
						Sleep(100);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						StringCchPrintf(outMsg, sizeof(outMsg), L"Iteration %d, Pivot( %d, %d )", *numSteps, mRowIndex, *nLastInput);
						Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: NOT BOUNDED", 
							mRowIndex, 
							(*vInList)[mRowIndex - 1], 
							(long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], 
							mList, true); 

						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						(*numSteps)++;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >=0)
			{
				*res = SX_OPTIM;
				bStop = true;

				Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: OPTIM", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
				Sleep(100);

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
			/*	cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;*/
				#endif
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;
					
					Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: NOT FEASIBLE", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
					Sleep(100);

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;*/
					#endif
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;
						
						Write_SimplexMSG_ToCListCtrl(L"Finishing Simplex Procedure, Result: NOT BOUNDED", 0, 0, (long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], mList); 
						Sleep(100);

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					/*	cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;*/
						#endif
					}
					else
					{
						StringCchPrintf(outMsg, sizeof(outMsg), L"Iteration %d, Pivot( %d, %d )", *numSteps, mRowIndex, *nLastInput);
						Write_SimplexMSG_ToCListCtrl(outMsg, 
							mRowIndex, 
							(*vInList)[mRowIndex - 1], 
							(long double)(*mTableau)[0][mTableau->GetLastColumnIndex()], 
							mList, true); 

						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						(*numSteps)++;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						/*cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;*/
						#endif
					}
				}
			}
		}
		while(!bStop);
	}

	//delete[] outMsg;
}




//template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
//{
//	WCHAR msg[255] = {0};
//	WCHAR msg1[255] = {0};
//	WCHAR msgb[255] = {0};
//	WCHAR msgo[255] = {0};
//
//
//
//	//Variables
//	UINT iSrow = 0;
//	
//	
//
//	//Soupouse a not solution sistem
//	*res = SX_NOT_SOLUTION;
//
//	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//	cout << endl;
//	cout << "=======================================================================================================================" << endl;
//	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
//	if(minimize)
//	{
//		cout << "MINIMIZE" << endl;
//	}
//	else
//	{
//		cout << "MAXIMIZE" << endl;
//	}
//	cout << "=======================================================================================================================" << endl;
//	cout << endl;
//	#endif
//
//	
//
//	UINT mRowIndex = 0;
//	T value = 0;
//
//	//Feasibility conditions
//	//------------------------------------------------------------------------------------------------------------------------------------------------
//	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
//	{
//		*res = SX_NOT_FEASIBLE;
//		
//		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//		cout << endl;
//		cout << "=======================================================================================================================" << endl;
//		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
//		cout << "=======================================================================================================================" << endl;
//		cout << endl;
//		#endif
//		((splx_info*)out_list)->m_b_list->AddString(L"Not Feasible");
//		MessageBox(NULL, L"NOT FEASIBLE", L"Simplex Result", 0);
//
//		return;
//	}
//
//	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
//	//------------------------------------------------------------------------------------------------------------------------------------------------
//	for(register UINT i=0; i<vInList->GetSize(); i++)
//	{
//		if(mTableau->Matrix[0][(*vInList)[i]] != 0)
//		{
//			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
//			{
//				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
//				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
//				mTableau->Row_OpInnerAdd(0, iSrow, value);
//			}
//		}
//	}
//
//	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//	cout << endl;
//	cout << "Seting Up the Tableau.... " << endl;
//	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//	cout << endl;
//	mTableau->PrintToConsole();
//	cout << endl;
//	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//	#endif
//	
//
//	//Begin the simplex loop
//	//------------------------------------------------------------------------------------------------------------------------------------------------
//	if(minimize)
//	{
//		//MINIMIZE
//		//------------------------------------------------------------------------------------------------------------------------------------------------
//		bool bStop = false;
//		do
//		{
//			//Check for the maximum index 
//			//------------------------------------------------------------------------------------------------------------------------------------------------
//			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
//			if(value <=0)
//			{
//				*res = SX_OPTIM;
//				bStop = true;
//
//				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//				cout << "Resulting variables:" << endl;
//				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//				for(register UINT i = 0; i<vInList->GetSize(); i++)
//				{
//					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
//				}
//				cout << endl;
//				cout << "=======================================================================================================================" << endl;
//				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
//				cout << "=======================================================================================================================" << endl;
//				cout << endl;
//				#endif
//
//				MessageBox(NULL, L"OPTIM", L"Simplex Result", 0);
//
//				for ( int i = 1; i < mTableau->GetRowSize()-1; i++)
//				{
//					StringCchPrintf(msgb, 255 * sizeof(WCHAR), L" b[%d] = %lf ",i, (*mTableau)[i][mTableau->GetLastColumnIndex()]);
//					//((splx_info*)out_list)->m_b_list->InsertString(((splx_info*)out_list)->m_b_list->GetCount() - 1, msgb);
//					((splx_info*)out_list)->m_b_list->AddString(msgb);
//				}
//			}
//			else
//			{
//				//Check for feasibility conditions
//				//------------------------------------------------------------------------------------------------------------------------------------------------
//				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
//				{
//					*res = SX_NOT_FEASIBLE;
//					bStop = true;
//
//					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//					cout << endl;
//					cout << "=======================================================================================================================" << endl;
//					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
//					cout << "=======================================================================================================================" << endl;
//					cout << endl;
//					#endif
//					((splx_info*)out_list)->m_b_list->AddString(L"Not Feasible");
//					MessageBox(NULL, L"NOT FEASIBLE", L"Simplex Result", 0);
//
//				}
//				else
//				{
//					mRowIndex = -1;
//					//Make the quotient rule,...
//					//------------------------------------------------------------------------------------------------------------------------------------------------
//					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
//						1, mTableau->m_nLastRowIndex, NULL, 
//						&mRowIndex, &value))
//					{
//						*res = SX_NOT_BOUNDED;
//						bStop = true;
//
//						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//						cout << endl;
//						cout << "=======================================================================================================================" << endl;
//						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
//						cout << "=======================================================================================================================" << endl;
//						cout << endl;
//						#endif
//						((splx_info*)out_list)->m_b_list->AddString(L"Not Bounded");
//						MessageBox(NULL, L"NOT BOUNDED", L"Simplex Result", 0);
//					}
//					else
//					{
//						mTableau->Row_Pivot(mRowIndex, *nLastInput);
//						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
//						(*vInList)[mRowIndex - 1] = *nLastInput;
//						(*numSteps)++;
//
//						StringCchPrintf(msg, 255 * sizeof(WCHAR), L"Step %d Pivot(%d,%d) ", *numSteps, mRowIndex, *nLastInput);
//						//((splx_info*)out_list)->m_list->InsertString(((splx_info*)out_list)->m_list->GetCount() - 1, msg);
//						((splx_info*)out_list)->m_list->AddString(msg);
//
//						StringCchPrintf(msg1, 255 * sizeof(WCHAR), L" Z = %lf ", (*mTableau)[0][mTableau->GetLastColumnIndex()]);
//						//((splx_info*)out_list)->m_z_list->InsertString(((splx_info*)out_list)->m_z_list->GetCount() - 1, msg1);
//						((splx_info*)out_list)->m_z_list->AddString(msg1);
//
//						StringCchPrintf(msgo, 255 * sizeof(WCHAR), L" in = %d out %d ", *nLastInput,(*vOutList)[mRowIndex - 1]);
//						//((splx_info*)out_list)->m_z_list->InsertString(((splx_info*)out_list)->m_z_list->GetCount() - 1, msg1);
//						((splx_info*)out_list)->m_out_list->AddString(msgo);
//
//			
//						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//						cout << endl;
//						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
//						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//						cout << endl;
//						mTableau->PrintfToConsole();
//						cout << endl;
//						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//						cout << endl;
//						#endif
//					}
//				}
//			}
//		}
//		while(!bStop);
//	}
//	else
//	{
//		//MAXIMIZE
//		//------------------------------------------------------------------------------------------------------------------------------------------------
//		bool bStop = false;
//		do
//		{
//			//Check for the maximum index 
//			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
//			//cout << "MIN VALUE = " << value << endl;
//			//cout << "MIN INDEX = " << *nLastInput << endl;
//			if(value >=0)
//			{
//				*res = SX_OPTIM;
//				bStop = true;
//
//				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//				cout << "Resulting variables:" << endl;
//				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//				for(register UINT i = 0; i<vInList->GetSize(); i++)
//				{
//					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
//				}
//				cout << endl;
//				cout << "=======================================================================================================================" << endl;
//				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
//				cout << "=======================================================================================================================" << endl;
//				cout << endl;
//				#endif
//				for ( int i = 1; i < mTableau->GetRowSize()-1; i++)
//				{
//					StringCchPrintf(msgb, 255 * sizeof(WCHAR), L" b[%d] = %lf ",i, (*mTableau)[i][mTableau->GetLastColumnIndex()]);
//					//((splx_info*)out_list)->m_b_list->InsertString(((splx_info*)out_list)->m_b_list->GetCount() - 1, msgb);
//					((splx_info*)out_list)->m_b_list->AddString(msgb);
//				}
//				MessageBox(NULL, L"OPTIMO", L"Simplex...", 0);
//			}
//			else
//			{
//				//Check for feasibility conditions
//				//------------------------------------------------------------------------------------------------------------------------------------------------
//				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
//				{
//					*res = SX_NOT_FEASIBLE;
//					bStop = true;
//
//					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//					cout << endl;
//					cout << "=======================================================================================================================" << endl;
//					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
//					cout << "=======================================================================================================================" << endl;
//					cout << endl;
//					cout << "Resulting variables:" << endl;
//					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//					for(register UINT i = 0; i<vInList->GetSize(); i++)
//					{
//						cout << "V[" << i << "] = " << (*vInList)[i] << endl;
//					}
//
//					#endif
//					((splx_info*)out_list)->m_b_list->AddString(L"Not Feasible");
//					MessageBox(NULL, L"NOT FEASIBLE", L"Simplex Result", 0);
//				}
//				else
//				{
//					mRowIndex = -1;
//					//Make the quotient rule,...
//					//------------------------------------------------------------------------------------------------------------------------------------------------
//					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
//						1, mTableau->m_nLastRowIndex, NULL, 
//						&mRowIndex, &value))
//					{
//						*res = SX_NOT_BOUNDED;
//						bStop = true;
//
//						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//						cout << endl;
//						cout << "=======================================================================================================================" << endl;
//						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
//						cout << "=======================================================================================================================" << endl;
//						cout << endl;
//						#endif
//						((splx_info*)out_list)->m_b_list->AddString(L"Not Bounded");
//						MessageBox(NULL, L"NOT BOUNDED", L"Simplex Result", 0);
//					}
//					else
//					{
//						mTableau->Row_Pivot(mRowIndex, *nLastInput);
//	
//						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
//						(*vInList)[mRowIndex - 1] = *nLastInput;
//
//						(*numSteps)++;
//
//						StringCchPrintf(msg, 255 * sizeof(WCHAR), L"Step %d Pivot(%d,%d) ", *numSteps, mRowIndex, *nLastInput);
//						//((splx_info*)out_list)->m_list->InsertString(((splx_info*)out_list)->m_list->GetCount() - 1, msg);
//						((splx_info*)out_list)->m_list->AddString(msg);
//
//						StringCchPrintf(msg1, 255 * sizeof(WCHAR), L" Z = %lf ", (*mTableau)[0][mTableau->GetLastColumnIndex()]);
//						//((splx_info*)out_list)->m_z_list->InsertString(((splx_info*)out_list)->m_z_list->GetCount() - 1, msg1);
//						((splx_info*)out_list)->m_z_list->AddString(msg1);
//
//						StringCchPrintf(msgo, 255 * sizeof(WCHAR), L" in = %d out %d ", *nLastInput,(*vOutList)[mRowIndex - 1]);
//						//((splx_info*)out_list)->m_z_list->InsertString(((splx_info*)out_list)->m_z_list->GetCount() - 1, msg1);
//						((splx_info*)out_list)->m_out_list->AddString(msgo);
//
//
//						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
//						cout << endl;
//						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
//						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//						cout << endl;
//						mTableau->PrintToConsole();
//						cout << endl;
//						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
//						cout << endl;
//						#endif
//					}
//				}
//			}
//		}
//		while(!bStop);
//	}
//}
template <typename T> void CMatrix_SimplexCanonical(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0; 
	//Variables
	UINT iSrow = 0;

	p_time.Start();	
	t_time.Start();

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	WCHAR msg[255]={0};
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	e_time = t_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);
	


	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;
	#endif

	

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;
				
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif

		WCHAR msg[255]={0};
		StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
		
		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != T(0))
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "Seting Up the Tableau.... " << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			
			//Check for the maximum index 
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <=T(0))
			{
				*res = SX_OPTIM;
				bStop = true;
				

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif

						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						(*numSteps)++;
										
						WCHAR msg[25]={0};
						StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

						e_time = p_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);


						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintfToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
					}
				}
			}
		}
		while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >=T(0))
			{
				*res = SX_OPTIM;
				bStop = true;

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					bStop = true;

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					cout << "Resulting variables:" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
	
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;

						(*numSteps)++;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
						e_time = p_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
				}
			}
		}
		while(!bStop);
	
	}
}
template <typename T> void CMatrix_SimplexCanonicalP(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0; 
	//Variables
	UINT iSrow = 0;

	p_time.Start();	
	t_time.Start();

	//Soupouse a not solution sistem
	
	WCHAR msg[255]={0};
	if(*numSteps ==0)
	{
	*res = SX_NOT_SOLUTION;
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	e_time = t_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);
	}
	
	


	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE CANONICAL SIMPLEX PROCEDURE, "; 
	if(minimize)
	{
		cout << "MINIMIZE" << endl;
	}
	else
	{
		cout << "MAXIMIZE" << endl;
	}
	cout << "=======================================================================================================================" << endl;
	cout << endl;
	#endif

	

	UINT mRowIndex = 0;
	T value = 0;

	//Feasibility conditions
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
	{
		*res = SX_NOT_FEASIBLE;
				
		#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif

		WCHAR msg[255]={0};
		StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
		
		return;
	}

	//Preparing the simplex, Seach for the canonical vectors, for each one of the vinList index
	//------------------------------------------------------------------------------------------------------------------------------------------------
	for(register UINT i=0; i<vInList->GetSize(); i++)
	{
		if(mTableau->Matrix[0][(*vInList)[i]] != T(0))
		{
			if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
			{
				//If the entry (*vinList)[i] is != 0 and is canonical, make a row operation, to make the value, (*vinList)[i] = 0
				value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
				mTableau->Row_OpInnerAdd(0, iSrow, value);
			}
		}
	}

	#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
	cout << endl;
	cout << "Seting Up the Tableau.... " << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	

	//Begin the simplex loop
	//------------------------------------------------------------------------------------------------------------------------------------------------
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		
		
			
			//Check for the maximum index 
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, (UINT)0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			if(value <=T(0))
			{
				*res = SX_OPTIM;
				
				

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif

						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;
						(*numSteps)++;
										
						WCHAR msg[25]={0};
						StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

						e_time = p_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);


						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintfToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
					}
				}
			}
		
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		
		
			//Check for the maximum index 
			mTableau->Row_GetMinValue(0, 0, mTableau->m_nLastColumnIndex - 1, nLastInput, &value);
			//cout << "MIN VALUE = " << value << endl;
			//cout << "MIN INDEX = " << *nLastInput << endl;
			if(value >=T(0))
			{
				*res = SX_OPTIM;
			

				#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
			}
			else
			{
				//Check for feasibility conditions
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
				{
					*res = SX_NOT_FEASIBLE;
					

					#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					cout << "Resulting variables:" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{
					mRowIndex = -1;
					//Make the quotient rule,...
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
						1, mTableau->m_nLastRowIndex, NULL, 
						&mRowIndex, &value))
					{
						*res = SX_NOT_BOUNDED;
						

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: NOT BOUNDED" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{
						mTableau->Row_Pivot(mRowIndex, *nLastInput);
	
						(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
						(*vInList)[mRowIndex - 1] = *nLastInput;

						(*numSteps)++;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						mTableau->PrintToConsole();
						cout << endl;
						cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
						cout << endl;
						#endif
						WCHAR msg[255]={0};
						StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
						e_time = p_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
				}
			}
	
	}
}
#endif //__AFXWIN_H__
#endif //CCanonicaSimplex_H 

