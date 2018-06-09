#ifndef CSRSIMPLEX_H
#define CSRSIMPLEX_H

//#define CSRSIMPLEX_DEBUG_CONSOLE
#define CSRSIMPLEX_VERSION 1.0

#include "CMatrix_.h"
#include "CTimer_.h"
#include "CSimplex_Canonical_.h"

//SRSIMPLEX 
//Applies Revised Simplex Procedure to sRTableau
//----------------------------------------------
//-sRTableau: Simplex Revised Tableau Matrix.

//-cList: Objective Function values.
//-A: Pure Restrictions Matrix.
//-minimize: logic choice, if it's false, maximizes.
//-vIn: Basis variables entrance.
//-vOut: Basis variables that change while procedure is done.
//-nLastInput: Last column variable to insert in the basis.
//-nSteps: number of iterations.
//-res: Standard enumeration of the result
template <typename T> void CMatrix_SRSimplex(CMatrix_<T>* sRTableau, CArray_<T>* cList, CMatrix_<T>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res)
{
	sRTableau->Column_Add();
	CArray_<T> zc = CArray_<T>(A->GetColumnSize() + sRTableau->GetColumnSize() - 2);
	CMatrix_<T> original;
	CMatrix_Identity(&original,sRTableau->GetColumnSize() - 2 );
	UINT kindex;
	UINT minIndex = 0;
	T minValue = 0;
	*nSteps = 0;
	CARRAY_UINT priority;
	*res = SX_NO_OPTIM;
	#ifdef CSRSIMPLEX_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE REVISED SIMPLEX PROCEDURE, "; 
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

		if ( minimize )
		{

		do{
			zc.Zero();
			//Testing if the vector b implicit in sRTableau is feasible
			if(sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()))
			{
				*res = SX_NOT_FEASIBLE;
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				break;
			}
			//finding zj-cj parameters

			for ( register UINT j = 0; j < A->GetColumnSize(); j++)
			{
				for ( register int k = 0 ; k < sRTableau->GetColumnSize() - 2 ; k++)
				{

					zc[j] += (sRTableau->Matrix[0][k] * A->Matrix[k][j]);
				}
				zc[j] -= (*cList)[j];
			}
			// revisar 
			for ( register UINT k = 0 ; k < original.GetRowSize() ; k++)
			{
				for ( register UINT j = 0 ; j < zc.GetSize() - A->GetColumnSize() ; j++)
				{
					zc[k + A->GetColumnSize()] += ((*sRTableau)[0][j] * original[k][j]) ;
				}
			}
			//termina revisar
			kindex = 0;
			//finding the maximum of zj-cj
			for (register int i = 1; i< zc.GetSize(); i++)
			{
				if (zc[kindex]<zc[i]) 
				{
					kindex = i;
				}
			}
			//Optimality Test
			if ( zc[kindex] <= 0 )
			{
				*res = SX_OPTIM;
				
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << sRTableau->Matrix[i + 1][sRTableau->GetLastColumnIndex() - 1] <<endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				break;
			}
			//Inserting the yk vector at the last column
			sRTableau->Matrix[0][sRTableau->GetLastColumnIndex()] = zc[kindex];
			for ( register int i = 1; i < sRTableau->GetRowSize(); i++)
			{
				sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] = 0;
				for ( register int j = 0; j < sRTableau->GetColumnSize() - 2; j++)
				{
					sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] += (sRTableau->Matrix[i][j]*A->Matrix[j][kindex]);
				}
			}
			//Boundary Test
			if(sRTableau->Column_IsNegativeZero(sRTableau->GetLastColumnIndex(), 1, sRTableau->GetLastRowIndex()))
			{
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				break;
			}
			
			//Finding pivot candidate
			sRTableau->Column_LexicographicalRule_MinPositive(sRTableau->GetLastColumnIndex()-1,sRTableau->GetLastColumnIndex(),UINT(1),sRTableau->GetLastRowIndex(),&priority,&minIndex, &minValue);
			//Pivoting...
			sRTableau->Row_Pivot(minIndex,sRTableau->GetLastColumnIndex());
			//Defining Basis Variables 
			(*vOut)[minIndex -1] = (*vIn)[minIndex -1];
			(*vIn)[minIndex -1] = kindex;
			//Number of iterations
			(*nSteps)++;
			//Last column to insert in the basis
			(* nLastInput) = kindex;
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << minIndex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			sRTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
		}while(!sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex())); 
	}

	else
	{
		
	do{
		zc.Zero();
		//Testing if the vector b implicit in sRTableau is feasible
		if(sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()))
		{
			*res = SX_NOT_FEASIBLE;
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
			#endif
			break;
		}
		//finding zj-cj parameters
		for ( register UINT j = 0; j < A->GetColumnSize(); j++)
		{
			for ( register int k = 0 ; k < sRTableau->GetColumnSize() - 2 ; k++)
			{
				zc[j] += (sRTableau->Matrix[0][k] * A->Matrix[k][j]);
			}
			zc[j] -= (*cList)[j];
		}
		kindex = 0;
		//finding the maximum of zj-cj
		for (register int i = 1; i< zc.GetSize(); i++)
		{
			if (zc[kindex]>zc[i]) 
			{
				kindex = i;
			}
		}
		//Optimality Test
		if ( zc[kindex] >= 0 )
		{
			*res = SX_OPTIM;
			
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << sRTableau->Matrix[i + 1][sRTableau->GetLastColumnIndex() - 1] <<endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
			#endif

			break;
		}
		//Inserting the yk vector at the last column
		sRTableau->Matrix[0][sRTableau->GetLastColumnIndex()] = zc[kindex];
		for ( register int i = 1; i < sRTableau->GetRowSize(); i++)
		{
			sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] = 0;
			for ( register int j = 0; j < sRTableau->GetColumnSize()-2; j++)
			{
				sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] += (sRTableau->Matrix[i][j]*A->Matrix[j][kindex]);
			}
		}
		//Boundary Test
		if(sRTableau->Column_IsNegativeZero(sRTableau->GetLastColumnIndex(), 1, sRTableau->GetLastRowIndex()))
			{
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				break;
			}
		//Finding pivot candidate
		sRTableau->Column_LexicographicalRule_MinPositive(sRTableau->GetLastColumnIndex()-1,sRTableau->GetLastColumnIndex(),UINT(1), sRTableau->GetLastRowIndex(),&priority,&minIndex, &minValue);
		//Pivoting...
		sRTableau->Row_Pivot(minIndex,sRTableau->GetLastColumnIndex());
		//Defining Basis Variables 
		(*vOut)[minIndex -1] = (*vIn)[minIndex -1];
		(*vIn)[minIndex -1] = kindex;
		//Number of iterations
		(*nSteps)++;
		//Last column to insert in the basis
		(* nLastInput) = kindex;
		#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << minIndex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			sRTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
		//Number of iterations
		(*nSteps)++;
		//Last column to insert in the basis
		(* nLastInput) = kindex;
	}while(!sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()));

	}
}

template <typename T> void CMatrix_SRSimplex(CMatrix_<T>* sRTableau, CArray_<T>* cList, CMatrix_<T>* A, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut, UINT* nLastInput, UINT *nSteps, LPSIMPLEX_RESULT res, LPVOID out_list)
{
	CQTimer_ p_time;
	CQTimer_ t_time;
	double e_time = 0;
	p_time.Start();
	t_time.Start();

	sRTableau->Column_Add();
	CArray_<T> zc = CArray_<T>(A->GetColumnSize() + sRTableau->GetColumnSize() - 2);
	CMatrix_<T> original;
	CMatrix_Identity(&original,sRTableau->GetColumnSize() - 2 );
	UINT kindex = 0;
	UINT minIndex = 0;
	T minValue = 0;
	*nSteps = 0;
	CARRAY_UINT priority;
	*res = SX_NO_OPTIM;
	#ifdef CSRSIMPLEX_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE REVISED SIMPLEX PROCEDURE, "; 
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

	WCHAR msg[255]={0};
	StringCchPrintf(msg,255*sizeof(WCHAR),L"Setting Up Tableau");
	e_time = t_time.GetElapsedTime();
	((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,0,0,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time,true);
	
		if ( minimize )
		{

		do{
			zc.Zero();
			//Testing if the vector b implicit in sRTableau is feasible
			if(sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()))
			{
				*res = SX_NOT_FEASIBLE;
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
				break;
			}
			//finding zj-cj parameters

			for ( register UINT j = 0; j < A->GetColumnSize(); j++)
			{
				for ( register int k = 0 ; k < sRTableau->GetColumnSize() - 2 ; k++)
				{

					zc[j] += (sRTableau->Matrix[0][k] * A->Matrix[k][j]);
				}
				zc[j] -= (*cList)[j];
			}
			// revisar 
			for ( register UINT k = 0 ; k < original.GetRowSize() ; k++)
			{
				for ( register UINT j = 0 ; j < zc.GetSize() - A->GetColumnSize() ; j++)
				{
					zc[k + A->GetColumnSize()] += ((*sRTableau)[0][j] * original[k][j]) ;
				}
			}
			//termina revisar
			kindex = 0;
			//finding the maximum of zj-cj
			for (register int i = 1; i< zc.GetSize(); i++)
			{
				if (zc[kindex]<zc[i]) 
				{
					kindex = i;
				}
			}
			//Optimality Test
			if ( zc[kindex] <= T(0) )
			{
				*res = SX_OPTIM;
				
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << sRTableau->Matrix[i + 1][sRTableau->GetLastColumnIndex() - 1] <<endl;
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
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
				break;
			}
			//Inserting the yk vector at the last column
			sRTableau->Matrix[0][sRTableau->GetLastColumnIndex()] = zc[kindex];
			for ( register int i = 1; i < sRTableau->GetRowSize(); i++)
			{
				sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] = 0;
				for ( register int j = 0; j < sRTableau->GetColumnSize() - 2; j++)
				{
					sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] += (sRTableau->Matrix[i][j]*A->Matrix[j][kindex]);
				}
			}
			//Boundary Test
			if(sRTableau->Column_IsNegativeZero(sRTableau->GetLastColumnIndex(), 1, sRTableau->GetLastRowIndex()))
			{
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif


				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
				break;
			}
			
			//Finding pivot candidate
			sRTableau->Column_LexicographicalRule_MinPositive(sRTableau->GetLastColumnIndex()-1,sRTableau->GetLastColumnIndex(),UINT(1),sRTableau->GetLastRowIndex(),&priority,&minIndex, &minValue);
			//Pivoting...
			sRTableau->Row_Pivot(minIndex,sRTableau->GetLastColumnIndex());
			//Defining Basis Variables 
			(*vOut)[minIndex -1] = (*vIn)[minIndex -1];
			(*vIn)[minIndex -1] = kindex;
			//Number of iterations
			(*nSteps)++;
			//Last column to insert in the basis
			(* nLastInput) = kindex;
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << minIndex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			sRTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
			WCHAR msg[25]={0};
			StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");
			e_time = p_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
		}while(!sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex())); 
	}

	else
	{
		
	do{
		zc.Zero();
		//Testing if the vector b implicit in sRTableau is feasible
		if(sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()))
		{
			*res = SX_NOT_FEASIBLE;
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT FEASIBLE" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
			#endif
			WCHAR msg[255]={0};
			StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT FEASIBLE");
			e_time = t_time.GetElapsedTime();
			((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
			break;
		}
		//finding zj-cj parameters
		for ( register UINT j = 0; j < A->GetColumnSize(); j++)
		{
			for ( register int k = 0 ; k < sRTableau->GetColumnSize() - 2 ; k++)
			{
				zc[j] += (sRTableau->Matrix[0][k] * A->Matrix[k][j]);
			}
			zc[j] -= (*cList)[j];
		}
		kindex = 0;
		//finding the maximum of zj-cj
		for (register int i = 1; i< zc.GetSize(); i++)
		{
			if (zc[kindex]>zc[i]) 
			{
				kindex = i;
			}
		}
		//Optimality Test
		if ( zc[kindex] >= T(0) )
		{
			*res = SX_OPTIM;
			
			#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << sRTableau->Matrix[i + 1][sRTableau->GetLastColumnIndex() - 1] <<endl;
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
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);

			break;
		}
		//Inserting the yk vector at the last column
		sRTableau->Matrix[0][sRTableau->GetLastColumnIndex()] = zc[kindex];
		for ( register int i = 1; i < sRTableau->GetRowSize(); i++)
		{
			sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] = 0;
			for ( register int j = 0; j < sRTableau->GetColumnSize()-2; j++)
			{
				sRTableau->Matrix[i][sRTableau->GetLastColumnIndex()] += (sRTableau->Matrix[i][j]*A->Matrix[j][kindex]);
			}
		}
		//Boundary Test
		if(sRTableau->Column_IsNegativeZero(sRTableau->GetLastColumnIndex(), 1, sRTableau->GetLastRowIndex()))
			{
				#ifdef CSRSIMPLEX_DEBUG_CONSOLE
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				#endif
				WCHAR msg[255]={0};
				StringCchPrintf(msg,255*sizeof(WCHAR),L"NOT BOUNDED");
				e_time = t_time.GetElapsedTime();
				((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
				break;
			}
		//Finding pivot candidate
		sRTableau->Column_LexicographicalRule_MinPositive(sRTableau->GetLastColumnIndex()-1,sRTableau->GetLastColumnIndex(),UINT(1), sRTableau->GetLastRowIndex(),&priority,&minIndex, &minValue);
		//Pivoting...
		sRTableau->Row_Pivot(minIndex,sRTableau->GetLastColumnIndex());
		//Defining Basis Variables 
		(*vOut)[minIndex -1] = (*vIn)[minIndex -1];
		(*vIn)[minIndex -1] = kindex;
		//Number of iterations
		(*nSteps)++;
		//Last column to insert in the basis
		(*nLastInput) = kindex;
		#ifdef CSRSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << minIndex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			sRTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
		/*//Number of iterations
		(*nSteps)++;
		//Last column to insert in the basis
		(* nLastInput) = kindex;*/

		WCHAR msg[25]={0};
		StringCchPrintf(msg,25*sizeof(WCHAR),L"Pivoting");
		e_time = p_time.GetElapsedTime();
		((CSimplex_DataMsg_<T>*)(out_list))->SendMsg(*nSteps,msg,*nLastInput,minIndex,(*sRTableau)[0][sRTableau->GetLastColumnIndex()-1],e_time);
	}while(!sRTableau->Column_IsNegative(sRTableau->GetLastColumnIndex()-1,1,sRTableau->GetLastRowIndex()));

	}
}



template <typename T> void CMatrix_MakeRevisedRandomTableau(UINT nR, UINT nV, CMatrix_<T>* sRTableau, CArray_<T>* cList,CMatrix_<T>* A, CARRAY_UINT* vIn)
{
	//First resize the tableau,...
	CMatrix_Random<T>(A, -100, 500, nR, nV);

	//Identity Matrix
	//CMatrix_<T> I;
	CMatrix_Identity(sRTableau, nR);
	sRTableau->Row_Insert(0);
	
	//Objective Function
	cList->Resize(A->GetColumnSize());
	CArray_RandomFill<T>(cList, -100, 100);
	

	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, 0, 200, nR + 1, 1);
	B.Matrix[0][0] = 0;

	vIn->Resize(sRTableau->GetColumnSize());
	for(UINT i=0; i<vIn->GetSize(); i++)
	{
		(*vIn)[i] = vIn->GetSize() + i;
	}
	sRTableau->Join_Right(&B);
}
template <typename T> void CMatrix_MakeRevisedRandomTableau(UINT nR, UINT nV, CMatrix_<T>* sRTableau, CArray_<T>* cList,CMatrix_<T>* A, CARRAY_UINT* vIn, T c_Min, T c_Max, T A_Min, T A_Max, T B_Min, T B_Max)
{
	//First resize the tableau,...
	CMatrix_Random<T>(A, A_Min, A_Max, nR, nV);

	//Identity Matrix
	//CMatrix_<T> I;
	CMatrix_Identity(sRTableau, nR);
	sRTableau->Row_Insert(0);
	
	//Objective Function
	cList->Resize(A->GetColumnSize());
	CArray_RandomFill<T>(cList, c_Min, c_Max);
	

	//B Column
	CMatrix_<T> B;
	CMatrix_Random<T>(&B, B_Min, B_Max, nR + 1, 1);
	B.Matrix[0][0] = 0;

	vIn->Resize(sRTableau->GetColumnSize());
	for(UINT i=0; i<vIn->GetSize(); i++)
	{
		(*vIn)[i] = vIn->GetSize() + i;
	}
	sRTableau->Join_Right(&B);
}
#endif 