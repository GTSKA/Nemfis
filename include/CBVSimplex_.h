#ifndef CVBSIMPLEX_H
#define CVBSIMPLEX_H

#define CVBSIMPLEX_DEBUG_CONSOLE
#define CVBSIMPLEX_VERSION 1.0

#include "CArray_.h"
#include "CMatrix_.h"

template <typename T> class Bound_
{
public:
	UINT m_index;
	bool m_lower;
	T m_uBound;
	T m_lBound;
	T value;
	
	Bound_(const UINT n)
	{
	}

	Bound_(const Bound_ &nBound)
	{
	}
		 
	Bound_()
	{
		m_lower = true;
	}

	};

template <typename T> void CMatrix_CBVSimplex(CMatrix_<T> *bvTableau, CArray_<T> *cList, CArray_<Bound_<T>> *Bounds, bool minimize, CARRAY_UINT *vIn, CARRAY_UINT *vOut,UINT *lastInput, CARRAY_UINT *indexList, UINT *nSteps, LPSIMPLEX_RESULT res)
{
	T maxvalue, max1 = 0, max2 = 0;
	UINT kindex,k1 = 0 ,k2 = 0;
	T minvalgam1=0, minvalgam2 = 0;
	UINT rindex = 0, r1 = 0, r2 = 0;
	T deltamin;
	UINT deltaindex;
	CArray_<T> delta = CArray_<T>(3);
	CARRAY_UINT inferiores = CARRAY_UINT();
	CARRAY_UINT superiores = CARRAY_UINT();
	*nSteps = 0;
	*res = SX_NO_OPTIM;
	//Evaluacion Inicial con cotas inferiores
	CArray_<T> lower = CArray_<T>(bvTableau->GetRowSize()); // definir tamaño?
	#ifdef CVBSIMPLEX_DEBUG_CONSOLE
	cout << endl;
	cout << "=======================================================================================================================" << endl;
	cout << "BEGINNING THE BOUNDED VARIABLES SIMPLEX PROCEDURE, "; 
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
	//desde aqui
	bvTableau->PrintToConsole();
	cout<<endl;
	if ( minimize )
	{
		//inicializacion de indices
		for(register UINT i = 0; i < bvTableau->GetColumnSize()-1 ; i++)
		{
			(*Bounds)[i].m_index = i;
		}
		for(register UINT i = 0; i < lower.GetSize(); i++)
		{
			lower[i] = (*Bounds)[i].m_lBound;
		}
		for(register UINT i = 0; i <= bvTableau->GetLastRowIndex(); i++)
		{
			bvTableau->Row_Evaluate(i,0,bvTableau->GetLastColumnIndex() - 1,&lower,&maxvalue);
			bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] = bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]-maxvalue;
		}
		do
		{
			if(bvTableau->Column_IsNegative(bvTableau->GetLastColumnIndex(),1,bvTableau->GetLastRowIndex()))
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
			inferiores.RemoveAll();
			superiores.RemoveAll();
			//Criterio de terminacion
			for (register UINT i = 0; i < Bounds->GetSize(); i++)
			{
				if(!(*Bounds)[i].m_lower)
				{
					inferiores.Add(i)/*[i] = (*Bounds)[i].m_index*/;
				}
				else 
				{
					superiores.Add(i)/*[i] = (*Bounds)[i].m_index*/;
				}
			}
				
			//Maximo de zj-cj
			bvTableau->Row_GetMaxValue(UINT(0),&inferiores,&k1,&max1);
			bvTableau->Row_GetMaxValue(UINT(0),&superiores,&k2,&max2);
			
			if((max1 <= 0) && (max2 >=0))
			{
				#ifdef CVBSIMPLEX_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << bvTableau->Matrix[i + 1][bvTableau->GetLastColumnIndex()] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				*res = SX_OPTIM;
				#endif
				break;
				
			}
			else if ( max1 > 0 )
			{
				maxvalue = max1;
				kindex = k1;
			}
			else
			{
				maxvalue = max2;
				kindex = k2;
			}
			//Condicion que revisa si la variable esta en su cota inferior o en su cota superior
			if( !(*Bounds)[kindex].m_lower)
			{
			// gamma para variables en su cota inferior
			Gammal1(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r1 , &minvalgam1 );
			delta[0] = minvalgam1;
			Gammal2(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r2 , &minvalgam2 );
			delta[1] = minvalgam2;
			delta[2] = (*Bounds)[kindex].m_uBound - (*Bounds)[kindex].m_lBound;
			delta.GetMinValue(&deltamin, &deltaindex);
			
			}
			else
			{	
			// gamma para variables en su cota superior
			Gammau1(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r1 , &minvalgam1 );
			delta[0] = minvalgam1;
			Gammau2(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r2 , &minvalgam2 );
			delta[1] = minvalgam2;
			delta[2] = (*Bounds)[kindex].m_uBound - (*Bounds)[kindex].m_lBound;
			delta.GetMinValue(&deltamin, &deltaindex);
			
			}
			if ( deltamin == delta[0] ) 
			{
				rindex = r1;
			}
			else if ( deltamin == delta[1] )
			{
				rindex = r2;
			}
			//Actualizacion Z
			bvTableau->Matrix[0][bvTableau->GetLastColumnIndex()] = bvTableau->Matrix[0][bvTableau->GetLastColumnIndex()] - (maxvalue * deltamin);
			//actualizacion b
			for (register UINT i = 1; i <= bvTableau->GetLastRowIndex() ; i++)
			{
				bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] -= (deltamin * bvTableau->Matrix[i][kindex]);
					
    			if((bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) == (*Bounds)[(*vIn)[i-1]].m_uBound)
				{
					(*Bounds)[(*vIn)[i-1]].m_lower = !(*Bounds)[(*vIn)[i-1]].m_lower;
				}
			}

			if((deltamin == delta[0] || deltamin == delta[1]) && (deltamin != INFINITE_MAX))
			{
				(*Bounds)[kindex].value = (*Bounds)[kindex].m_lBound + deltamin;
				(*vOut)[rindex-1] = (*vIn)[rindex-1];
				(*vIn)[rindex-1] = kindex;
			}
			else if (deltamin == delta[2] && deltamin != INFINITE_MAX)
			{
				(*Bounds)[kindex].m_lower = !(*Bounds)[kindex].m_lower;
				(*Bounds)[kindex].value = (*Bounds)[kindex].m_lBound + deltamin;
			}
			else
			{
			#ifdef CVBSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "=======================================================================================================================" << endl;
			cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
			cout << "=======================================================================================================================" << endl;
			cout << endl;
			*res = SX_DUAL_NOT_BOUNDED;
			#endif
			}
			//actualizacion elemento b r-esimo
			bvTableau->Matrix[rindex][bvTableau->GetLastColumnIndex()]=(*Bounds)[kindex].value;
			//pivoteo
			bvTableau->Row_Pivot(rindex,kindex,rindex,bvTableau->GetLastColumnIndex());
			(*nSteps)++;
			*lastInput = kindex;
			#ifdef CVBSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << rindex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			bvTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
			cout<<endl;
		}while(!bvTableau->Column_IsNegative(bvTableau->GetLastColumnIndex(),1,bvTableau->GetLastRowIndex()));		
	}
	else
	{		
		for(register UINT i = 0; i < cList->GetSize() ; i++)
		{
			(*cList)[i] = (-1) * (*cList)[i];
		}
		//inicializacion de indices
		for(register UINT i = 0; i < bvTableau->GetColumnSize()-1 ; i++)
		{
			(*Bounds)[i].m_index = i;
		}
		for(register UINT i = 0; i < lower.GetSize(); i++)
		{
			lower[i] = (*Bounds)[i].m_lBound;
		}
		for(register UINT i = 0; i <= bvTableau->GetLastRowIndex(); i++)
		{
			bvTableau->Row_Evaluate(i,0,bvTableau->GetLastColumnIndex() - 1,&lower,&maxvalue);
			bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] = bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]-maxvalue;
		}
		do
		{
			if(bvTableau->Column_IsNegative(bvTableau->GetLastColumnIndex(),1,bvTableau->GetLastRowIndex()))
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
			inferiores.RemoveAll();
			superiores.RemoveAll();
			//Criterio de terminacion
			for (register UINT i = 0; i < Bounds->GetSize(); i++)
			{
				if(!(*Bounds)[i].m_lower)
				{
					inferiores.Add(i)/*[i] = (*Bounds)[i].m_index*/;
				}
				else 
				{
					superiores.Add(i)/*[i] = (*Bounds)[i].m_index*/;
				}
			}
				
			//Maximo de zj-cj
			bvTableau->Row_GetMaxValue(UINT(0),&inferiores,&k1,&max1);
			bvTableau->Row_GetMaxValue(UINT(0),&superiores,&k2,&max2);
			
			if((max1 <= 0) && (max2 >=0))
			{
				#ifdef CVBSIMPLEX_DEBUG_CONSOLE
				cout << "Resulting variables:" << endl;
				cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
				for(register UINT i = 0; i<vIn->GetSize(); i++)
				{
					cout << "V[" << (*vIn)[i] << "] = " << bvTableau->Matrix[i + 1][bvTableau->GetLastColumnIndex()] << endl;
				}
				cout << endl;
				cout << "=======================================================================================================================" << endl;
				cout << "Finishing Simplex Procedure, Result: OPTIM" << endl;
				cout << "=======================================================================================================================" << endl;
				cout << endl;
				*res = SX_OPTIM;
				#endif
				break;
				
			}
			else if ( max1 > 0 )
			{
				maxvalue = max1;
				kindex = k1;
			}
			else
			{
				maxvalue = max2;
				kindex = k2;
			}
			//Condicion que revisa si la variable esta en su cota inferior o en su cota superior
			if( !(*Bounds)[kindex].m_lower)
			{
			// gamma para variables en su cota inferior
			Gammal1(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r1 , &minvalgam1 );
			delta[0] = minvalgam1;
			Gammal2(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r2 , &minvalgam2 );
			delta[1] = minvalgam2;
			delta[2] = (*Bounds)[kindex].m_uBound - (*Bounds)[kindex].m_lBound;
			delta.GetMinValue(&deltamin, &deltaindex);
			
			}
			else
			{	
			// gamma para variables en su cota superior
			Gammau1(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r1 , &minvalgam1 );
			delta[0] = minvalgam1;
			Gammau2(bvTableau , Bounds , vIn , &kindex , UINT(1) , bvTableau->GetLastRowIndex() , &r2 , &minvalgam2 );
			delta[1] = minvalgam2;
			delta[2] = (*Bounds)[kindex].m_uBound - (*Bounds)[kindex].m_lBound;
			delta.GetMinValue(&deltamin, &deltaindex);
			
			}
			if ( deltamin == delta[0] ) 
			{
				rindex = r1;
			}
			else if ( deltamin == delta[1] )
			{
				rindex = r2;
			}
			//Actualizacion Z
			bvTableau->Matrix[0][bvTableau->GetLastColumnIndex()] = bvTableau->Matrix[0][bvTableau->GetLastColumnIndex()] - (maxvalue * deltamin);
			//actualizacion b
			for (register UINT i = 1; i <= bvTableau->GetLastRowIndex() ; i++)
			{
				bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] -= (deltamin * bvTableau->Matrix[i][kindex]);
					
    			if((bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) == (*Bounds)[(*vIn)[i-1]].m_uBound)
				{
					(*Bounds)[(*vIn)[i-1]].m_lower = !(*Bounds)[(*vIn)[i-1]].m_lower;
				}
			}

			if((deltamin == delta[0] || deltamin == delta[1]) && (deltamin != INFINITE_MAX))
			{
				(*Bounds)[kindex].value = (*Bounds)[kindex].m_lBound + deltamin;
				(*vOut)[rindex-1] = (*vIn)[rindex-1];
				(*vIn)[rindex-1] = kindex;
			}
			else if (deltamin == delta[2] && deltamin != INFINITE_MAX)
			{
				(*Bounds)[kindex].m_lower = !(*Bounds)[kindex].m_lower;
				(*Bounds)[kindex].value = (*Bounds)[kindex].m_lBound + deltamin;
			}
			else
			{
			#ifdef CVBSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "=======================================================================================================================" << endl;
			cout << "Finishing Simplex Procedure, Result: NOT BOUNDED" << endl;
			cout << "=======================================================================================================================" << endl;
			cout << endl;
			*res = SX_DUAL_NOT_BOUNDED;
			#endif
			}
			//actualizacion elemento b r-esimo
			bvTableau->Matrix[rindex][bvTableau->GetLastColumnIndex()]=(*Bounds)[kindex].value;
			//pivoteo
			bvTableau->Row_Pivot(rindex,kindex,rindex,bvTableau->GetLastColumnIndex());
			(*nSteps)++;
			*lastInput = kindex;
			#ifdef CVBSIMPLEX_DEBUG_CONSOLE
			cout << endl;
			cout << "Simplex Step: " << *nSteps << " Pivot( " << rindex << " , " << kindex << " )" << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			bvTableau->PrintToConsole();
			cout << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;
			#endif
			cout<<endl;
		}while(!bvTableau->Column_IsNegative(bvTableau->GetLastColumnIndex(),1,bvTableau->GetLastRowIndex()));		
	}
}



template <typename T> bool Gammal1(CMatrix_<T>* bvTableau, CArray_<Bound_<T>>* Bounds, CARRAY_UINT* vIn, UINT *Col, UINT iRow, UINT fRow, UINT *minindex, T *minvalue)
{
	T c;
	UINT i;
	bool process = false ;
    for (i = iRow; i <= fRow; i++)
	{
		if(bvTableau->Matrix[i][*Col]>T(0))
		{
			if(!process)
			{
				*minvalue = ( bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] - (*Bounds)[(*vIn)[i-1]].m_lBound ) / bvTableau->Matrix[i][*Col];
				*minindex = i ;
				process = true;
			}
			else
			{
				c = ( bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] - (*Bounds)[(*vIn)[i-1]].m_lBound ) / bvTableau->Matrix[i][*Col];
				if(c < *minvalue && c > T(0))
				{
					*minvalue = c ;
					*minindex = i ;
				}
			}
		}
	}
	if(!process)
	{
		*minvalue = INFINITE_MAX;
		*minindex = fRow;
	}
	return process;
}
template <typename T> bool Gammal2(CMatrix_<T>* bvTableau, CArray_<Bound_<T>>* Bounds, CARRAY_UINT* vIn, UINT *Col, UINT iRow, UINT fRow, UINT *minindex, T *minvalue)
{
	T c;
	bool process = false ;
	for ( register UINT i = iRow; i <= fRow; i++)
	{
		if(bvTableau->Matrix[i][*Col]<T(0))
		{
			if(!process)
			{
				*minvalue = ((*Bounds)[(*vIn)[i-1]].m_uBound - bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) / -(bvTableau->Matrix[i][*Col]);
				*minindex = i ;
				process = true;
			}
			else
			{
				c = ((*Bounds)[(*vIn)[i-1]].m_uBound - bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) / -(bvTableau->Matrix[i][*Col]);
				if(c > *minvalue && c < T(0))
				{
					*minvalue = c;
					*minindex = i ;
				}
			}
		}
	
	}
	if(!process)
	{
		*minvalue = INFINITE_MAX;
		*minindex = fRow;
	}
	return process;
}
template <typename T> bool Gammau1(CMatrix_<T>* bvTableau, CArray_<Bound_<T>>* Bounds, CARRAY_UINT* vIn, UINT *Col, UINT iRow, UINT fRow, UINT *minindex, T *minvalue)
{
	T c;
	bool process = false ;
	for ( register UINT i = iRow; i <= fRow; i++)
	{
		if(bvTableau->Matrix[i][*Col]<T(0))
		{
			if(!process)
			{
				*minvalue = ( bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] - (*Bounds)[(*vIn)[i-1]].m_lBound ) / -bvTableau->Matrix[i][*Col];
				process = true;
			}
			else
			{
				c = ( bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()] - (*Bounds)[(*vIn)[i-1]].m_lBound ) / -bvTableau->Matrix[i][*Col];
				if(c < *minvalue)
				{
					*minvalue = c;
					*minindex = i;
				}
			}
		}
	}
	if(!process)
	{
		*minvalue = INFINITE_MAX;
	}
	return process;
}
template <typename T> bool Gammau2(CMatrix_<T>* bvTableau, CArray_<Bound_<T>>* Bounds, CARRAY_UINT* vIn, UINT *Col, UINT iRow, UINT fRow, UINT *minindex, T *minvalue)
{
	T c;
	bool process = false ;
	for ( register UINT i = iRow; i <= fRow; i++)
	{
		if(bvTableau->Matrix[i][*Col]>T(0))
		{
			if(!process)
			{
				*minvalue = ((*Bounds)[(*vIn)[i-1]].m_uBound - bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) / (bvTableau->Matrix[i][*Col]);
				process = true;
			}
			else
			{
				c = ((*Bounds)[(*vIn)[i-1]].m_uBound - bvTableau->Matrix[i][bvTableau->GetLastColumnIndex()]) / (bvTableau->Matrix[i][*Col]);
				if(c < *minvalue)
				{
					*minvalue = c;
					*minindex = i;
				}
			}
		}
	}
	if(!process)
	{
		*minvalue = INFINITE_MAX;
	}
	return process;
	//hasta aaca
}
#endif
