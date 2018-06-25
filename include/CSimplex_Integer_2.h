#ifndef CSIMPLEX_INTEGER_H
#define CSIMPLEX_INTEGER_H

#define EPS_  10.0 * DBL_EPSILON 
#define EPS_DIST_TEST(x) (((x - int(x)) - EPS_ > 0) ? x : int(x) )

#include "CSimplex_Dual_.h"
#include "CMatrix_.h"
#include "CSimplex_2Phases_.h"
#include "CSimplex_Canonical_.h"
#include "CTimer_.h"

#define INTEGER_SIMPLEX_DEBUG_CONSOLE


template < typename T > void CMatrix_IntegerProgramming(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, CARRAY_UINT* vAList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE t)
{
    //if t = null, then the tableau is already optimized and skips the switch options
	bool proc = true;
	
	if ( t == NULL )
	{
		*res = SX_OPTIM;
	}
	else
	{
		*res = SX_NOT_SOLUTION;
	}
	UINT iSRow;
	UINT g = 0;
	CArray_<T> m;
	CMatrix_<T> objFunction = CMatrix_<T>(1,mTableau->GetColumnSize());
	for (register UINT i = 0; i <= mTableau->GetLastColumnIndex(); i++)
	{
		objFunction[0][i] = (*mTableau)[0][i];
	}
	
	if ( t != NULL )
	{
		switch(*t)
		{
		case 1:
		CMatrix_SimplexCanonical(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
			break;
		case 2:
		for (register UINT i = 0; i < (*vAList)[0]; i++)
		{
			(*mTableau)[0][i] = 0;
		}
		for (register UINT i = (*vAList)[0]; i < mTableau->GetLastColumnIndex(); i++)
		{
			(*mTableau)[0][i] = -1;
		}
		CMatrix_Simplex_2Phases(mTableau, &objFunction, minimize, vAList, vInList, vOutList, nLastInput, numSteps, res);
			break;
		case 4:
			CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
			break;
		}
	}
	if ( *res == SX_OPTIM )
	{
		cout << " ============== Beggining Cutting Planes ============== "<< endl;
		mTableau->PrintfToConsole();
		cout << " ====================================================== " << endl;
		CMatrix_<T> org(*mTableau);
		CArray_<T> m;
		CARRAY_UINT cut_in(*vInList);
		CARRAY_UINT cut_out(*vOutList);
		CARRAY_UINT past_ind;
		UINT o_size = cut_in.GetSize();
		UINT s = 0;
	
	//	for ( UINT i = 0; i <= 1 ; i++ )//Ciclo de iteraciones
		do
		{
			if ( mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&s) )
			{
				cout << " ============= OPTIM INTEGER ===================== " << endl;
				break;
			}
			m.Resize(mTableau->GetRowSize());
			//if ( proc )
			//{
			m[0] = mTableau->Z;
				for ( UINT j = 1; j < m.GetSize(); j++ )
				{
					if ( !past_ind.Find(j) )
					{
						//cout << " !ind.Find("<<j<<") = "<<past_ind.Find(j)<<endl;
						m[j] = (*mTableau)[j][mTableau->GetLastColumnIndex()] - floor( (*mTableau)[j][mTableau->GetLastColumnIndex()] );
					}
					else
					{
					//	cout << " m["<<j<<"] = 0 " << endl;
						m[j] = 0;
					}
				}
			//	proc = false;
			//}
			T value = 0;
			UINT index = 0;
			//m.GetMaxValue(&value,&index);
			m.GetMaxAbsoluteValue(&value,&index);
			past_ind.Add(index);
			//
			////----------------------------------------------TRUCO
			//index = 2;
			////----------------------------------------------
			cout << " ================= Inserting Plane index : "<<index<< " ============== " << endl;
		
			mTableau->Column_Insert(mTableau->GetLastColumnIndex());
			mTableau->Row_Add();
			org.Column_Insert(org.GetLastColumnIndex());
			org.Row_Add();
			
			//(*mTableau)[mTableau->GetLastRowIndex()][i] = T(-1)*m[index];
			//org[org.GetLastRowIndex()][i] = T(-1)*m[index];
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex()] = T(-1)*m[index];
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()] = T(-1)*m[index];

			for ( UINT i = 0; i < mTableau->GetColumnSize(); i++ )
			{
				(*mTableau)[mTableau->GetLastRowIndex()][i] = ((*mTableau)[index][i] - floor((*mTableau)[index][i]))*T(-1);
				org[org.GetLastRowIndex()][i] = (org[index][i] - floor(org[index][i]))*T(-1);
			}
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex() - 1] = 1;
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()-1] = 1;
						
			vInList->Resize(mTableau->GetRowSize()-1);
			vOutList->Resize(mTableau->GetRowSize()-1);
			cut_in.Resize(org.GetRowSize()-1);
			cut_out.Resize(org.GetRowSize()-1);

			(*vInList)[vInList->GetSize()-1] = mTableau->GetColumnSize();
			vOutList = vInList;
			cut_in[cut_in.GetSize()-1] = org.GetColumnSize();
			cut_out = cut_in;

			CMatrix_SimplexDual(mTableau,minimize,vInList,vOutList,nLastInput,numSteps,res);
			//CMatrix_SimplexDual(mTableau,true,vInList,vOutList,nLastInput,numSteps,res);
			if (*res != SX_OPTIM)
			{
				break;
			}
			mTableau->PrintfToConsole();

			if(mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&s))
			{
				cout << " ===========================OPTIM INTEGER========================== "<<endl;
				mTableau->PrintfToConsole();
				vInList->PrintToConsole();
				break;
			}
			else
			{
				//mTableau->Equals(org);
			}
		}while(true);
	}
}
template < typename T > void CMatrix_IntegerProgramming(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, CARRAY_UINT* vAList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE t, LPVOID out_list)
{
	//if t = null, then the tableau is already optimized and skips the switch options
	bool proc = true;
	CQTimer_ p_time;
	CQTimer_ cut_time;
	double i_time = 0;
	
	if ( t == NULL )
	{
		*res = SX_OPTIM;
	}
	else
	{
		*res = SX_NOT_SOLUTION;
	}
	UINT iSRow;
	UINT g = 0;
	CArray_<T> m;
	CMatrix_<T> objFunction = CMatrix_<T>(1,mTableau->GetColumnSize());
	for (register UINT i = 0; i <= mTableau->GetLastColumnIndex(); i++)
	{
		objFunction[0][i] = (*mTableau)[0][i];
	}
	
	if ( t != NULL )
	{
		switch(*t)
		{
		case 1:
		CMatrix_SimplexCanonical(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res, out_list);
			break;
		case 2:
		for (register UINT i = 0; i < (*vAList)[0]; i++)
		{
			(*mTableau)[0][i] = 0;
		}
		for (register UINT i = (*vAList)[0]; i < mTableau->GetLastColumnIndex(); i++)
		{
			(*mTableau)[0][i] = -1;
		}
		CMatrix_Simplex_2Phases(mTableau, &objFunction, minimize, vAList, vInList, vOutList, nLastInput, numSteps, res, out_list);
			break;
		case 4:
			CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res, out_list);
			break;
		}
	}
	WCHAR msg[255] = {0};

	if ( *res == SX_OPTIM )
	{
		StringCchPrintf(msg, 255*sizeof(WCHAR), L"Beggining Cutting Planes");
		i_time = p_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,0,i_time,true);
		cout << " ============== Beggining Cutting Planes ============== "<< endl;
		mTableau->PrintfToConsole();
		cout << " ====================================================== " << endl;
		CMatrix_<T> org(*mTableau);
		CArray_<T> m;
		CARRAY_UINT cut_in(*vInList);
		CARRAY_UINT cut_out(*vOutList);
		CARRAY_UINT past_ind;
		UINT o_size = cut_in.GetSize();

		do
		{
			m.Resize(mTableau->GetRowSize());
			m[0] = mTableau->Z;
				for ( UINT j = 1; j < m.GetSize(); j++ )
				{
					m[j] = (*mTableau)[j][mTableau->GetLastColumnIndex()] - floor( (*mTableau)[j][mTableau->GetLastColumnIndex()] );
				}
			T value = 0;
			UINT index = 0;
     		m.GetMaxAbsoluteValue(&value,&index);
			past_ind.Add(index);
			
			cout << " ================= Inserting Plane index : "<<index<< " ============== " << endl;
			StringCchPrintf(msg, 255*sizeof(WCHAR), L"Inserting Plane Index");
			i_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,index,0,0,i_time,false);
		
			mTableau->Column_Insert(mTableau->GetLastColumnIndex());
			mTableau->Row_Add();
			org.Column_Insert(org.GetLastColumnIndex());
			org.Row_Add();
			
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex()] = T(-1)*m[index];
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()] = T(-1)*m[index];

			for ( UINT i = 0; i < mTableau->GetColumnSize(); i++ )
			{
				(*mTableau)[mTableau->GetLastRowIndex()][i] = ((*mTableau)[index][i] - floor((*mTableau)[index][i]))*T(-1);
				org[org.GetLastRowIndex()][i] = (org[index][i] - floor(org[index][i]))*T(-1);
			}
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex() - 1] = 1;
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()-1] = 1;
						
			vInList->Resize(mTableau->GetRowSize()-1);
			vOutList->Resize(mTableau->GetRowSize()-1);
			cut_in.Resize(org.GetRowSize()-1);
			cut_out.Resize(org.GetRowSize()-1);

			(*vInList)[vInList->GetSize()-1] = mTableau->GetColumnSize();
			vOutList = vInList;
			cut_in[cut_in.GetSize()-1] = org.GetColumnSize();
			cut_out = cut_in;

			CMatrix_SimplexDual(mTableau,minimize,vInList,vOutList,nLastInput,numSteps,res,out_list);
			mTableau->PrintfToConsole();

			UINT s = 0;

			if(mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&s))
			{
				cout << " ===========================OPTIM INTEGER========================== "<<endl;
				mTableau->PrintfToConsole();
				StringCchPrintf(msg, 255*sizeof(WCHAR), L"OPTIM INTEGER");
				i_time = p_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,0,i_time,true);
				break;
			}
			else if (*res != SX_OPTIM)
			{
				break;
			}
		}while(true);
	}
}
template < typename T > void Integer_(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, CARRAY_UINT* vAList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE t)
{
	//if t = null, then the tableau is already optimized and skips the switch options
	bool proc = true;
	
	if ( t == NULL )
	{
		*res = SX_OPTIM;
	}
	else
	{
		*res = SX_NOT_SOLUTION;
	}
	UINT iSRow;
	UINT g = 0;
	CArray_<T> m;
	CMatrix_<T> objFunction = CMatrix_<T>(1,mTableau->GetColumnSize());
	for (register UINT i = 0; i <= mTableau->GetLastColumnIndex(); i++)
	{
		objFunction[0][i] = (*mTableau)[0][i];
	}
	
	if ( t != NULL )
	{
		switch(*t)
		{
		case 1:
		CMatrix_SimplexCanonical(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
			break;
		case 2:
		for (register UINT i = 0; i < (*vAList)[0]; i++)
		{
			(*mTableau)[0][i] = 0;
		}
		for (register UINT i = (*vAList)[0]; i < mTableau->GetLastColumnIndex(); i++)
		{
			(*mTableau)[0][i] = -1;
		}
		CMatrix_Simplex_2Phases(mTableau, &objFunction, minimize, vAList, vInList, vOutList, nLastInput, numSteps, res);
			break;
		case 4:
			CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
			break;
		}
	}
	if ( *res == SX_OPTIM )
	{
		cout << " ============== Beggining Cutting Planes ============== "<< endl;
		mTableau->PrintfToConsole();
		cout << " ====================================================== " << endl;
		CMatrix_<T> org(*mTableau);
		CArray_<T> m;
		CARRAY_UINT cut_in(*vInList);
		CARRAY_UINT cut_out(*vOutList);
		CARRAY_UINT past_ind;
		UINT o_size = cut_in.GetSize();

	//	for ( UINT i = 0; i <= 1 ; i++ )//Ciclo de iteraciones
		do
		{
			m.Resize(mTableau->GetRowSize());
			//if ( proc )
			//{
			m[0] = mTableau->Z;
				for ( UINT j = 1; j < m.GetSize(); j++ )
				{
					///*if ( !past_ind.Find(j) )
					//{*/
						//cout << " !ind.Find("<<j<<") = "<<past_ind.Find(j)<<endl;
						m[j] = (*mTableau)[j][mTableau->GetLastColumnIndex()] - floor( (*mTableau)[j][mTableau->GetLastColumnIndex()] );
					//}
					//else
					//{
					////	cout << " m["<<j<<"] = 0 " << endl;
					//	m[j] = 0;
					//}
				}
			//	proc = false;
			//}
			T value = 0;
			UINT index = 0;
			//m.GetMaxValue(&value,&index);
			m.GetMaxAbsoluteValue(&value,&index);
			past_ind.Add(index);
			cout <<" ----------------ind---------------- " << endl;
		//	past_ind.PrintToConsole();
			cout <<" ----------------------------------- " << endl;
			cout << endl;
			cout <<" -----------------m----------------- " << endl;
		//	m.PrintToConsole();
			cout << " index " << index << " value " << value << endl;
			cout <<" ----------------------------------- " << endl;
			cout << endl;
			//
			////----------------------------------------------TRUCO
			//index = 2;
			////----------------------------------------------
			cout << " ================= Inserting Plane index : "<<index<< " ============== " << endl;
		
			mTableau->Column_Insert(mTableau->GetLastColumnIndex());
			mTableau->Row_Add();
			org.Column_Insert(org.GetLastColumnIndex());
			org.Row_Add();
			
			//(*mTableau)[mTableau->GetLastRowIndex()][i] = T(-1)*m[index];
			//org[org.GetLastRowIndex()][i] = T(-1)*m[index];
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex()] = T(-1)*m[index];
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()] = T(-1)*m[index];

			for ( UINT i = 0; i < mTableau->GetColumnSize(); i++ )
			{
				(*mTableau)[mTableau->GetLastRowIndex()][i] = ((*mTableau)[index][i] - floor((*mTableau)[index][i]))*T(-1);
				org[org.GetLastRowIndex()][i] = (org[index][i] - floor(org[index][i]))*T(-1);
			}
			(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex() - 1] = 1;
			org[org.GetLastRowIndex()][org.GetLastColumnIndex()-1] = 1;
						
			vInList->Resize(mTableau->GetRowSize()-1);
			vOutList->Resize(mTableau->GetRowSize()-1);
			cut_in.Resize(org.GetRowSize()-1);
			cut_out.Resize(org.GetRowSize()-1);

			(*vInList)[vInList->GetSize()-1] = mTableau->GetColumnSize();
			vOutList = vInList;
			cut_in[cut_in.GetSize()-1] = org.GetColumnSize();
			cut_out = cut_in;

			CMatrix_SimplexDual(mTableau,minimize,vInList,vOutList,nLastInput,numSteps,res);
			mTableau->PrintfToConsole();

			UINT s = 0;

			if(mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&s))
			{
				cout << " ===========================OPTIM INTEGER========================== "<<endl;
				mTableau->PrintfToConsole();
				break;
			}
			else if (*res != SX_OPTIM)
			{
				break;
			}
		}while(true);
	}
}
#endif
