#ifndef CSimplexDual_H
#define CSimplexDual_H

#include "CMatrix_.h"
#include "CArray_.h"
#include "CSimplex_Canonical_.h"
#include "BCGPGridCtrl.h"
#include "CTimer_.h"	

#define SPLX_DUAL_VERSION	1.1
#define SPLX_DUAL_DEBUG_CONSOLE

//template <typename T> struct CSimplex_DataMsg_
//{
//	CBCGPGridCtrl* m_gridMsg;
//	void SendMsg(UINT nSteps, 
//		WCHAR* msg,
//		UINT xIn,
//		UINT xOut,
//		T z_value,
//		double e_time,
//		bool Only_Message = false);
//};
//template <typename T> void CSimplex_DataMsg_<T>::SendMsg(UINT nSteps, WCHAR *msg, UINT xIn, UINT xOut, T z_value, double e_time, bool Only_Message = false)
//{
//	m_gridMsg->AddRow(false);
//	if ( !Only_Message )
//	{	
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->m_strFormatDouble = "%.10lf";
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(0)->SetValue(nSteps,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(1)->SetValue(msg,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(2)->SetValue(xIn,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(3)->SetValue(xOut,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(4)->SetValue(z_value,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(5)->SetValue(e_time,FALSE);
//	}
//	else
//	{
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(0)->SetValue(L"-",FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(1)->SetValue(msg,FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(2)->SetValue(L"-",FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(3)->SetValue(L"-",FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(4)->SetValue(L"-",FALSE);
//	m_gridMsg->GetRow(m_gridMsg->GetRowCount()-1)->GetItem(5)->SetValue(L"-",FALSE);
//	}
//	m_gridMsg->AdjustLayout();
//}

template <typename T> void CMatrix_SimplexDual(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	#ifdef SPLX_DUAL_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE DUAL SIMPLEX PROCEDURE, "; 
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
	cout << "Tableau:" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	mTableau->PrintfToConsole();
	#endif

	UINT mRowIndex = 0;
	T value = mTableau->Z;
	
	
	wchar_t* x = L"ho.txt";
	bool min = true;

	SIMPLEX_PROCEDURE_TYPE u = SX_PROC_DUALSIMPLEX;
	//CMatrix_SaveTableauData(x,  mTableau, &min, &u, vInList, vOutList, res);
	
	//Asume a not solution system
	//------------------------------------------------------------------------------------------------------------------------------------------------
	*res = SX_NO_OPTIM;
	
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		//for ( register UINT i = 0; i < 22; i++ )
		{
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex
				-1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				
			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			//&& value <=0);
			if(mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;
				bStop = true;
				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
			}
			else
			{
				//Get the min value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMinValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				
				//Aplly the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;
					bStop = true;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUNDED" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
				}
				else
				{
					//printf("\n\nmTableau[%d][%d] = %0.32lf\n\n ",mRowIndex,*nLastInput,(*mTableau)[mRowIndex][*nLastInput]);
					//printf("\n\n  b[%d][%d] = %0.32lf\n\n ",mRowIndex,mTableau->m_nLastColumnIndex,(*mTableau)[mRowIndex][mTableau->m_nLastColumnIndex]);

					mTableau->Row_Pivot(mRowIndex, *nLastInput);

					//printf("\n\n  b[%d][%d] = %0.32lf\n\n ",mRowIndex,mTableau->m_nLastColumnIndex,(*mTableau)[mRowIndex][mTableau->m_nLastColumnIndex]);

					(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
					(*vInList)[mRowIndex - 1] = *nLastInput;

					(*numSteps)++;

					

					//if ( *numSteps >= 18 && *numSteps <= 21 )
					{
						#ifdef SPLX_DUAL_DEBUG_CONSOLE
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
		}while(!bStop);
	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex -1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			
			if(mTableau->Column_IsNegativeZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
			}
			else
			{
				//Get the max value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMaxValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				

				//Aplly the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;
					bStop = true;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUDED" << endl;
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

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					mTableau->PrintToConsole();
					cout << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					#endif
				}
			}
		}while(!bStop);
	}
}

template <typename T> void CMatrix_SimplexDual(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{

	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0; 

	p_time.Start();
	t_time.Start();
	
	
#ifdef SPLX_DUAL_DEBUG_CONSOLE
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
	cout << "Tableau:" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	mTableau->PrintToConsole();
	#endif

	WCHAR msg[255]={0};
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(0,msg,0,0,0,0);

	UINT mRowIndex = 0;
	T value = mTableau->Z;	

	//Asume a not solution system
	//------------------------------------------------------------------------------------------------------------------------------------------------
	*res = SX_NO_OPTIM;
	
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		//for ( int i = 0; i < 1000; i++ )
		{
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			//&& value <=0);
			if(mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;
				bStop = true;
				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
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
				//Get the min value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMinValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				
				//Aplly the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;
					bStop = true;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUNDED" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
					e_time = t_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

				}
				else
				{
					mTableau->Row_Pivot(mRowIndex, *nLastInput);
		
					(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
					(*vInList)[mRowIndex - 1] = *nLastInput;
					(*numSteps)++;

					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
					e_time = p_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					mTableau->PrintToConsole();
					cout << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					#endif
				}
			}
		}while(!bStop);

	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		bool bStop = false;
		do
		{
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex -1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			
			if(mTableau->Column_IsNegativeZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;
				bStop = true;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
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
				//Get the max value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMaxValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				//Apply the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;
					bStop = true;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUNDED" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
					e_time = t_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{				

					mTableau->Row_Pivot(mRowIndex, *nLastInput);
		
					(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
					(*vInList)[mRowIndex - 1] = *nLastInput;

					(*numSteps)++;

					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
					e_time = p_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
	
					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					mTableau->PrintToConsole();
					cout << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					#endif
				}
			}
		}while(!bStop);
	}
}
template <typename T> void CMatrix_SimplexDualP(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{

	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0; 

	p_time.Start();
	t_time.Start();
	
	
#ifdef SPLX_DUAL_DEBUG_CONSOLE
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
	cout << "Tableau:" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	mTableau->PrintToConsole();
	#endif

	WCHAR msg[255]={0};
	if(*numSteps==0)
	{
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(0,msg,0,0,0,0);
	//Asume a not solution system
	//------------------------------------------------------------------------------------------------------------------------------------------------
	*res = SX_NO_OPTIM;
	}

	UINT mRowIndex = 0;
	T value = mTableau->Z;	

	
	
	if(minimize)
	{
		//MINIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;
				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			//&& value <=0);
			if(mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;
				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
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
				//Get the min value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMinValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				
				//Aplly the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUNDED" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
					e_time = t_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

				}
				else
				{
					mTableau->Row_Pivot(mRowIndex, *nLastInput);
		
					(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
					(*vInList)[mRowIndex - 1] = *nLastInput;
					(*numSteps)++;

					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
					e_time = p_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					mTableau->PrintToConsole();
					cout << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					#endif
				}
			}
		

	}
	else
	{
		//MAXIMIZE
		//------------------------------------------------------------------------------------------------------------------------------------------------
		
			//Feasibility conditions
			//------------------------------------------------------------------------------------------------------------------------------------------------
			mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex -1, nLastInput, &value); 
			if(value>T(0))
			{
				*res = SX_DUAL_NOT_FEASIBLE;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif

				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
			}
			//Chek if all the values in the B column, are greater than or equal to zero
			//------------------------------------------------------------------------------------------------------------------------------------------------
			//mTableau->Row_GetMaxValue(0, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value);
			
			if(mTableau->Column_IsNegativeZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
			{
				*res = SX_OPTIM;

				#ifdef SPLX_DUAL_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vInList->GetSize(); i++)
				{
					cout << "V[" << (*vInList)[i] << "] = " << mTableau->Matrix[i + 1][mTableau->m_nLastColumnIndex] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Dual Simplex Procedure, Result: OPTIM" << endl;
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
				//Get the max value for B column
				//------------------------------------------------------------------------------------------------------------------------------------------------
				mTableau->Column_GetMaxValue(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex, &mRowIndex, &value);

				//Apply the lexicographic rule
				//------------------------------------------------------------------------------------------------------------------------------------------------
				if(!mTableau->Row_LexicographicalRule_Min_Dual(0, mRowIndex, 0, mTableau->m_nLastColumnIndex-1, nLastInput, &value))
				{
					*res = SX_DUAL_NOT_BOUNDED;

					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "=======================================================================================================================" << endl;
					cout << "Finishing Dual Simplex Procedure, Result: DUAL NOT BOUNDED" << endl;
					cout << "=======================================================================================================================" << endl;
					cout << endl;
					#endif
					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
					e_time = t_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
				}
				else
				{				

					mTableau->Row_Pivot(mRowIndex, *nLastInput);
		
					(*vOutList)[mRowIndex - 1] = (*vInList)[mRowIndex - 1];
					(*vInList)[mRowIndex - 1] = *nLastInput;

					(*numSteps)++;

					WCHAR msg[255]={0};
					StringCchPrintf(msg,255*sizeof(WCHAR),L"Pivoting");
					e_time = p_time.GetElapsedTime();
					((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,mRowIndex,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
	
					#ifdef SPLX_DUAL_DEBUG_CONSOLE
					cout << endl;
					cout << "Simplex Step: " << *numSteps << " Pivot( " << mRowIndex << " , " << *nLastInput << " )" << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					mTableau->PrintToConsole();
					cout << endl;
					cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
					cout << endl;
					#endif
				}
			}
		
	}
}




template <typename T> void CMatrix_MakeDualRandomTableau(UINT nR, UINT nV, CMatrix_<T>* tableau, CARRAY_UINT* vInList)
{
	
	//First resize the tableau,...
	CMatrix_Random<T>(tableau, -100, 0, nR, nV);

	//Identity Matrix
	CMatrix_<T> I;
	CMatrix_Identity(&I, nR);
	I.Row_Insert(0);

	Sleep(500);

	//Z Row
	CMatrix_<T> Z;
	CMatrix_Random<T>(&Z, -100, 0, 1, nV);

	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, -200, 0, nR + 1, 1);
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
template <typename T> void CMatrix_MakeDualRandomTableau(UINT nR, UINT nV, CMatrix_<T>* tableau, CARRAY_UINT* vInList, T min_A, T max_A, T min_Z, T max_Z, T min_b, T max_b)
{
	
	//First resize the tableau,...
	CMatrix_Random<T>(tableau, min_A, max_A, nR, nV);

	//Identity Matrix
	CMatrix_<T> I;
	CMatrix_Identity(&I, nR);
	I.Row_Insert(0);

	Sleep(500);

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

#endif