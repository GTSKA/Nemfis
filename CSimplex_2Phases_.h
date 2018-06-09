#ifndef CSimplex_2Phases_H
#define CSimplex_2Phases_H

#include "CMatrix_.h"
#include "CArray_.h"
#include "CSimplex_Canonical_.h"
#include "CTimer_.h"

#define SPLX_2PHASES_VERSION	1.0
#define SPLX_2PHASES_DEBUG_CONSOLE

template <typename U> void CMatrix_Simplex_2Phases(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	//Variables
	UINT iSrow = 0;
	U value = (U)0;

	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	//Make and fill the Q List
	CARRAY_UINT qList;
	vAList->GetMinValue(&iSrow); 
	for(UINT i=0; i<(*vAList)[iSrow]; i++)
	{
		qList.Add(i);
	}

	//Array list of the artificial values in the base...
	CARRAY_UINT vAList_Base;

	//Flag for dual not bounded
	bool bDualNotBounded = false;

	//Number of Va entrys in the base after the phase 1.2....
	UINT numVa = 0;


	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE 2 PHASES SIMPLEX PROCEDURE, "; 
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
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

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
	}

	//Making the phase 1, deleting the zero entrys in the Z-row, for the variables in the base 
	//---------------------------------------------------------------------------------------------------------------------------------------
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			mTableau->Row_OpInnerAdd(0, iSrow, (mTableau->Matrix[iSrow][(*vAList)[i]]));
		}
	}

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Phase 1.1. Deleting the -1 Entrys,..." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	
	//Making the phase 1.2, calling the simplex procedure,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	CMatrix_SimplexCanonical(mTableau, true, vInList, vOutList, nLastInput, numSteps, res);

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE

	char msgRes[MAX_PATH] = {0};
	SPLXRES_to_STR(res, msgRes);

	cout << endl;
	cout << "Phase 1.2. Simplex Procedure Result: " << msgRes << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

	//There are no optim solutions, now quit!!!,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	if(*res != SX_OPTIM)
	{
		return;
	}

	//Now set the objective function,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	mTableau->SetValue(0, 0, objFunction);

	//Proced to make the simplex algorithm only for the qList index group,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Update the objective function,...." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	#endif
	
	//Make a count of the Va variables in the base,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			if(mTableau->Matrix[iSrow][mTableau->m_nLastColumnIndex] != U(0))
			{
				bDualNotBounded = true;
				break;
			}
			else
			{
				qList.Add((*vAList)[i]);
			}
			numVa++;
		}
	}

	if(bDualNotBounded)
	{
		#ifdef SPLX_2PHASES_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: DUAL NOT BOUNDED   ---------" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif

		*res = SX_DUAL_NOT_BOUNDED;
		return;
	}
	else
	{
		//Sub Case B1, there are no one artificial value in the base....
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		if(numVa == 0)
		{
			bool bStop = false;

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "=======================================================================================================================" << endl;
			cout << "Sub Case B1" << endl;
			cout << "=======================================================================================================================" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			

			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "qList Size = " << qList.GetSize() << endl;
			if(qList.GetSize() > 0)
			{
				cout << "qList Elemnts: " << endl;
				for(register UINT i=0; i<qList.GetSize(); i++)
				{
					cout << "qList[" << i << "] = " << qList[i] << endl;
				}
			}
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			//Prepare the simplex,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			for(UINT i=0; i<vInList->GetSize() ; i++)
			{
				if(mTableau->Matrix[0][(*vInList)[i]] != U(0))
				{
					//Make the entrance zero
					//Check the i index,...
					if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
					{
						value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
						mTableau->Row_OpInnerAdd(0, iSrow, value);
					}
				}
			}

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "Setting-up the simplex tableau" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			#endif

			*res = SX_NO_OPTIM;

			//Beginning the simplex loop
			//---------------------------------------------------------------------------------------------------------------------------------------
			if(minimize)
			{
				//Minimize
				//---------------------------------------------------------------------------------------------------------------------------------------
				do
				{
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_2PHASES_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMaxValue(0, &qList, nLastInput, &value);
						if(value <= U(0))
						{
							#ifdef SPLX_2PHASES_DEBUG_CONSOLE
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

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL, 
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
				//Maximize
				//---------------------------------------------------------------------------------------------------------------------------------------
				do
				{
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMinValue(0, &qList, nLastInput, &value);
						if(value >= U(0))
						{ 
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

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL,
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
		//Sub Case B2, we found ta last one artifical value in the base...
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		else if(numVa > 0)
		{
			
		}
	}
}



template <typename U> void CMatrix_Simplex_2Phases(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	//Variables
	UINT iSrow = 0;
	T value = (T)0;
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0;

	p_time.Start();
	t_time.Start();

	
	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	//Make and fill the Q List
	CARRAY_UINT qList;
	vAList->GetMinValue(&iSrow); 
	for(UINT i=0; i<(*vAList)[iSrow]; i++)
	{
		qList.Add(i);
	}

	//Array list of the artificial values in the base...
	CARRAY_UINT vAList_Base;

	//Flag for dual not bounded
	bool bDualNotBounded = false;

	//Number of Va entrys in the base after the phase 1.2....
	UINT numVa = 0;
	
	WCHAR msg[255]={0};
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	e_time = t_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE 2 PHASES SIMPLEX PROCEDURE, "; 
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
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

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
		StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES NOT FEASIBLE");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
	}


	//Making the phase 1, deleting the zero entrys in the Z-row, for the variables in the base 
	//---------------------------------------------------------------------------------------------------------------------------------------
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			mTableau->Row_OpInnerAdd(0, iSrow, (mTableau->Matrix[iSrow][(*vAList)[i]]));
		}
	}

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Phase 1.1. Deleting the -1 Entrys,..." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Deleting the -1 Entries");
	e_time = p_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);

	
	//Making the phase 1.2, calling the simplex procedure,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	CMatrix_SimplexCanonical(mTableau, true, vInList, vOutList, nLastInput, numSteps, res, out_list);

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE

	char msgRes[MAX_PATH] = {0};
	SPLXRES_to_STR(res, msgRes);

	cout << endl;
	cout << "Phase 1.2. Simplex Procedure Result: " << msgRes << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

	//There are no optim solutions, now quit!!!,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	if(*res != SX_OPTIM)
	{
		return;
	}

	//Now set the objective function,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	mTableau->SetValue(0, 0, objFunction);

	//Proced to make the simplex algorithm only for the qList index group,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Update the objective function,...." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	#endif
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Updating the objective function");
	e_time = p_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);

	
	//Make a count of the Va variables in the base,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			if(mTableau->Matrix[iSrow][mTableau->m_nLastColumnIndex] != T(0))
			{
				bDualNotBounded = true;
				break;
			}
			else
			{
				qList.Add((*vAList)[i]);
			}
			numVa++;
		}
	}


	if(bDualNotBounded)
	{
		#ifdef SPLX_2PHASES_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: DUAL NOT BOUNDED   ---------" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif

		StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES: DUAL NOT BOUNDED");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);


		*res = SX_DUAL_NOT_BOUNDED;
		return;
	}
	else
	{
		//Sub Case B1, there are no one artificial value in the base....
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		if(numVa == 0)
		{
			bool bStop = false;

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "=======================================================================================================================" << endl;
			cout << "Sub Case B1" << endl;
			cout << "=======================================================================================================================" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			StringCchPrintf(msg,255*sizeof(WCHAR),L"Sub-Case B1");
			e_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "qList Size = " << qList.GetSize() << endl;
			if(qList.GetSize() > 0)
			{
				cout << "qList Elemnts: " << endl;
				for(register UINT i=0; i<qList.GetSize(); i++)
				{
					cout << "qList[" << i << "] = " << qList[i] << endl;
				}
			}
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			//Prepare the simplex,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			for(UINT i=0; i<vInList->GetSize() ; i++)
			{
				if(mTableau->Matrix[0][(*vInList)[i]] != T(0))
				{
					//Make the entrance zero
					//Check the i index,...
					if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
					{
						value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
						mTableau->Row_OpInnerAdd(0, iSrow, value);
					}
				}
			}

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "Setting-up the simplex tableau" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			#endif

			StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting up the simplex tableau");
			e_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


			*res = SX_NO_OPTIM;

			//Beginning the simplex loop
			//---------------------------------------------------------------------------------------------------------------------------------------
			if(minimize)
			{
				//Minimize
				//---------------------------------------------------------------------------------------------------------------------------------------
				do
				{
				
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_2PHASES_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif

						StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
						


					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMaxValue(0, &qList, nLastInput, &value);
						if(value <= T(0))
						{
							#ifdef SPLX_2PHASES_DEBUG_CONSOLE
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
							
							StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
							e_time = t_time.GetElapsedTime();
							((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

							#endif

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL, 
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif

								StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
								e_time = t_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

						
								StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

								e_time = p_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
				//Maximize
				//---------------------------------------------------------------------------------------------------------------------------------------
				do
				{
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					
						StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES NOT FEASIBLE");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMinValue(0, &qList, nLastInput, &value);
						if(value >= T(0))
						{ 
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
							
							StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
							e_time = t_time.GetElapsedTime();
							((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL,
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif
								
								StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
								e_time = t_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

								StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

								e_time = p_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
		//Sub Case B2, we found ta last one artifical value in the base...
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		else if(numVa > 0)
		{
			
		}
	}
}
template <typename U> void CMatrix_Simplex_2PhasesP0(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	//Variables
	UINT iSrow = 0;
	T value = (T)0;
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0;

	p_time.Start();
	t_time.Start();

	
	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	//Make and fill the Q List
	CARRAY_UINT qList;
	vAList->GetMinValue(&iSrow); 
	for(UINT i=0; i<(*vAList)[iSrow]; i++)
	{
		qList.Add(i);
	}

	//Array list of the artificial values in the base...
	CARRAY_UINT vAList_Base;

	//Flag for dual not bounded
	bool bDualNotBounded = false;

	//Number of Va entrys in the base after the phase 1.2....
	UINT numVa = 0;
	
	WCHAR msg[255]={0};
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	e_time = t_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE 2 PHASES SIMPLEX PROCEDURE, "; 
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
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

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
		StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES NOT FEASIBLE");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
	}


	//Making the phase 1, deleting the zero entrys in the Z-row, for the variables in the base 
	//---------------------------------------------------------------------------------------------------------------------------------------
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			mTableau->Row_OpInnerAdd(0, iSrow, (mTableau->Matrix[iSrow][(*vAList)[i]]));
		}
	}

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Phase 1.1. Deleting the -1 Entrys,..." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Deleting the -1 Entries");
	e_time = p_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);

	
	//Making the phase 1.2, calling the simplex procedure,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	CMatrix_SimplexCanonical(mTableau, true, vInList, vOutList, nLastInput, numSteps, res, out_list);

	#ifdef SPLX_2PHASES_DEBUG_CONSOLE

	char msgRes[MAX_PATH] = {0};
	SPLXRES_to_STR(res, msgRes);

	cout << endl;
	cout << "Phase 1.2. Simplex Procedure Result: " << msgRes << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	#endif

	//There are no optim solutions, now quit!!!,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	if(*res != SX_OPTIM)
	{
		return;
	}

	//Now set the objective function,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	mTableau->SetValue(0, 0, objFunction);

	//Proced to make the simplex algorithm only for the qList index group,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	#ifdef SPLX_2PHASES_DEBUG_CONSOLE
	cout << endl;
	cout << "Update the objective function,...." << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;
	mTableau->PrintfToConsole();
	cout << endl;
	#endif
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Updating the objective function");
	e_time = p_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);

	
	//Make a count of the Va variables in the base,....
	//---------------------------------------------------------------------------------------------------------------------------------------
	
	for(UINT i=0; i<vAList->GetSize(); i++)
	{
		if(mTableau->Column_IsCanonical((*vAList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
		{
			if(mTableau->Matrix[iSrow][mTableau->m_nLastColumnIndex] != T(0))
			{
				bDualNotBounded = true;
				break;
			}
			else
			{
				qList.Add((*vAList)[i]);
			}
			numVa++;
		}
	}


	if(bDualNotBounded)
	{
		#ifdef SPLX_2PHASES_DEBUG_CONSOLE
		cout << endl;
		cout << "=======================================================================================================================" << endl;
		cout << "Finishing Simplex Procedure, Result: DUAL NOT BOUNDED   ---------" << endl;
		cout << "=======================================================================================================================" << endl;
		cout << endl;
		#endif

		StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES: DUAL NOT BOUNDED");
		e_time = t_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);


		*res = SX_DUAL_NOT_BOUNDED;
		return;
	}
	else
	{
		//Sub Case B1, there are no one artificial value in the base....
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		if(numVa == 0)
		{
			bool bStop = false;

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "=======================================================================================================================" << endl;
			cout << "Sub Case B1" << endl;
			cout << "=======================================================================================================================" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			StringCchPrintf(msg,255*sizeof(WCHAR),L"Sub-Case B1");
			e_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "qList Size = " << qList.GetSize() << endl;
			if(qList.GetSize() > 0)
			{
				cout << "qList Elemnts: " << endl;
				for(register UINT i=0; i<qList.GetSize(); i++)
				{
					cout << "qList[" << i << "] = " << qList[i] << endl;
				}
			}
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			#endif

			//Prepare the simplex,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			for(UINT i=0; i<vInList->GetSize() ; i++)
			{
				if(mTableau->Matrix[0][(*vInList)[i]] != T(0))
				{
					//Make the entrance zero
					//Check the i index,...
					if(mTableau->Column_IsCanonical((*vInList)[i], 1, mTableau->m_nLastRowIndex, &iSrow))
					{
						value = (mTableau->N * mTableau->Matrix[0][(*vInList)[i]]);
						mTableau->Row_OpInnerAdd(0, iSrow, value);
					}
				}
			}

			//Proced to make the simplex algorithm only for the qList index group,....
			//---------------------------------------------------------------------------------------------------------------------------------------
			#ifdef SPLX_2PHASES_DEBUG_CONSOLE
			cout << endl;
			cout << "Setting-up the simplex tableau" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			mTableau->PrintfToConsole();
			cout << endl;
			#endif

			StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting up the simplex tableau");
			e_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,0,0,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time,true);


			*res = SX_NO_OPTIM;


		//Sub Case B2, we found ta last one artifical value in the base...
		//-----------------------------------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------------------------------------
		else if(numVa > 0)
		{
			
		}
		}
}}

template <typename U> void CMatrix_Simplex_2PhasesP(CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, bool minimize, CARRAY_UINT* vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	//Variables
	UINT iSrow = 0;
	T value = (T)0;
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0;

	p_time.Start();
	t_time.Start();

	
	//Soupouse a not solution sistem
	*res = SX_NOT_SOLUTION;

	//------------------------------------------------------------------------------------------------------
	if(minimize)
	{
				//Minimize
				//---------------------------------------------------------------------------------------------------------------------------------------
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_2PHASES_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif

						StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT FEASIBLE");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
						


					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMaxValue(0, &qList, nLastInput, &value);
						if(value <= T(0))
						{
							#ifdef SPLX_2PHASES_DEBUG_CONSOLE
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
							
							StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
							e_time = t_time.GetElapsedTime();
							((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

							#endif

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL, 
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif

								StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
								e_time = t_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

						
								StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

								e_time = p_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

								#ifdef SPLX_2PHASES_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
				//Maximize
				//---------------------------------------------------------------------------------------------------------------------------------------
					//Check for feasibility conditions
					//------------------------------------------------------------------------------------------------------------------------------------------------
					if(!mTableau->Column_IsPositiveZero(mTableau->m_nLastColumnIndex, 1, mTableau->m_nLastRowIndex))
					{
						*res = SX_DUAL_NOT_FEASIBLE;
						bStop = true;

						#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
						cout << endl;
						cout << "=======================================================================================================================" << endl;
						cout << "Finishing Simplex Procedure, Result: DUAL NOT FEASIBLE" << endl;
						cout << "=======================================================================================================================" << endl;
						cout << endl;
						#endif
					
						StringCchPrintf(msg,255*sizeof(WCHAR),L"2 PHASES NOT FEASIBLE");
						e_time = t_time.GetElapsedTime();
						((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
					}
					else
					{

						//Get the max index,...
						//---------------------------------------------------------------------------------------------------------------------------------------
						mTableau->Row_GetMinValue(0, &qList, nLastInput, &value);
						if(value >= T(0))
						{ 
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
							
							StringCchPrintf(msg,255*sizeof(WCHAR),L"OPTIM");
							e_time = t_time.GetElapsedTime();
							((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

							bStop = true;
							*res = SX_OPTIM;
						}
						else
						{
							if(!mTableau->Column_LexicographicalRule_MinPositive(mTableau->m_nLastColumnIndex, *nLastInput, 
							1, mTableau->m_nLastRowIndex, NULL,
							&iSrow, &value))
							{
								*res = SX_DUAL_NOT_BOUNDED;
								bStop = true;

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "=======================================================================================================================" << endl;
								cout << "Finishing Simplex Procedure, Index Column: " << *nLastInput << " ,Result: DUAL NOT BOUNDED" << endl;
								cout << "=======================================================================================================================" << endl;
								cout << endl;
								#endif
								
								StringCchPrintf(msg,255*sizeof(WCHAR),L"DUAL NOT BOUNDED");
								e_time = t_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);
							}
							else
							{
								mTableau->Row_Pivot(iSrow, *nLastInput);
								(*vOutList)[iSrow - 1] = (*vInList)[iSrow - 1];
								(*vInList)[iSrow - 1] = *nLastInput;
								
								(*numSteps)++;

								StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");

								e_time = p_time.GetElapsedTime();
								((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*numSteps,msg,*nLastInput,iSrow,(*mTableau)[0][mTableau->GetLastColumnIndex()],e_time);

								#ifdef SPLX_CANONICAL_DEBUG_CONSOLE
								cout << endl;
								cout << "Simplex Step: " << *numSteps << " Pivot( " << iSrow << " , " << *nLastInput << " )" << endl;
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
}



template <typename U> void CMatrix_Make2PhasesRandom(UINT nR, UINT nV, UINT nA, CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, CARRAY_UINT* vAList, CARRAY_UINT* vInList)
{
	//nA = number of artificial variables
	
	//First resize the tableau,...
	CMatrix_Random<T>(mTableau, -100, 500, nR, nV);
	objFunction->Resize(1,nV);
	vAList->Resize(nA);
	UINT n = 0;
	
	//Identity Matrix
	CMatrix_<T> I;
	//CMatrix_Identity(&I, nR);
	I.Resize(nR,nR);
	for ( register UINT i = 0; i < I.GetColumnSize(); i++ )
	{
		if ( n < nA)
		{
			I[i][i] = -1;
			n++;
		}
		else
		{
			I[i][i] = 1;			
		}		
	}
	//I.Row_Insert(0);


	//Artificial Matrix;
	CMatrix_<T> Art;
	Art.Resize(I.GetRowSize(),nA);
	for ( register UINT i = 0; i < nA; i++ )
	{
		Art[1+i][i] = 1;
	}

	//Z Row
	CMatrix_<T> Z;
	//CMatrix_Random<T>(&Z, -100, 100, 1, nV+nA);
	Z.Resize(1,nV+nA);
	
	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, 0, 200, nR /*+ 1*/ , 1);
	//B.Matrix[0][0] = 0;

	UINT m = mTableau->GetColumnSize();
	mTableau->Join_Right(&I);
	mTableau->Join_Right(&Art);
	mTableau->Join_Right(&B);
	mTableau->Join_Up(&Z);

	for ( register UINT i = nV + I.GetColumnSize(); i < mTableau->GetColumnSize()-1; i++ )
	{
		(*mTableau)[0][i] = (mTableau->N);
	}

	vInList->Resize(mTableau->GetRowSize()-1);
	for(UINT i=0; i<vInList->GetSize(); i++)
	{
		(*vInList)[i] = m + i;
	}
	for (UINT i = 0; i < vAList->GetSize(); i++)
	{
		(*vAList)[i] = (*vInList)[vInList->GetSize()-1] + 1 + i;
		(*vInList)[i] = (*vAList)[i];
	}
	CMatrix_Random<T>(objFunction, -100, 100, 1, objFunction->GetColumnSize());
}
template <typename U> void CMatrix_Make2PhasesRandom(UINT nR, UINT nV, UINT nA, CMatrix_<U>* mTableau, CMatrix_<U>* objFunction, CARRAY_UINT* vAList, CARRAY_UINT* vInList, U A_Min, U A_Max, U Z_Min, U Z_Max, U B_Min, U B_Max)
{
	//nA = number of artificial variables
	
	//First resize the tableau,...
	CMatrix_Random<T>(mTableau, A_Min, A_Max, nR, nV);
	objFunction->Resize(1,nV);
	vAList->Resize(nA);
	UINT n = 0;
	
	//Identity Matrix
	CMatrix_<T> I;
	//CMatrix_Identity(&I, nR);
	I.Resize(nR,nR);
	for ( register UINT i = 0; i < I.GetColumnSize(); i++ )
	{
		if ( n < nA)
		{
			I[i][i] = -1;
			n++;
		}
		else
		{
			I[i][i] = 1;			
		}		
	}
	//I.Row_Insert(0);


	//Artificial Matrix;
	CMatrix_<T> Art;
	Art.Resize(I.GetRowSize(),nA);
	for ( register UINT i = 0; i < nA; i++ )
	{
		Art[1+i][i] = 1;
	}

	//Z Row
	CMatrix_<T> Z;
	//CMatrix_Random<T>(&Z, Z_Min, Z_Max, 1, nV+nA);
	Z.Resize(1,nV+nA);
	
	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, B_Min, B_Max, nR , 1);
	//B.Matrix[0][0] = 0;

	UINT m = mTableau->GetColumnSize();
	mTableau->Join_Right(&I);
	mTableau->Join_Right(&Art);
	mTableau->Join_Right(&B);
	mTableau->Join_Up(&Z);

	for ( register UINT i = nV + I.GetColumnSize(); i < mTableau->GetColumnSize()-1; i++ )
	{
		(*mTableau)[0][i] = (mTableau->N);
	}

	vInList->Resize(mTableau->GetRowSize()-1);
	for(UINT i=0; i<vInList->GetSize(); i++)
	{
		(*vInList)[i] = m + i;
	}
	for (UINT i = 0; i < vAList->GetSize(); i++)
	{
		(*vAList)[i] = (*vInList)[vInList->GetSize()-1] + 1 + i;
		(*vInList)[i] = (*vAList)[i];
	}
	CMatrix_Random<T>(objFunction, -100, 100, 1, objFunction->GetColumnSize());
}
#endif