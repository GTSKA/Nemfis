#ifndef CSIMPLEX_INTEGER_H
#define CSIMPLEX_INTEGER_H


#include "CSimplex_Dual_.h"

#define INTEGER_SIMPLEX_DEBUG_CONSOLE


template < typename T > void CMatrix_IntegerProgramming(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	*res = SX_NOT_SOLUTION;
	UINT iSysTest = mTableau->GetRowSize();
	////First we make the dual simplex procdure
	CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
	if(*res == SX_OPTIM)
	{
		//Make acopy of the origianl matrix...
		CMatrix_<T> save;

		//--------------------------------------------------------
		//save = *mTableau;

		save.SetSize(mTableau->GetRowSize(), mTableau->GetColumnSize());
		for(UINT i=0; i<save.GetRowSize(); i++)
		{
			for(UINT j=0; j<save.GetColumnSize(); j++)
			{
				T value = (*mTableau)[i][j];
				save[i][j] = value;
			}
		}

		/*cout << endl;
		cout << "Dual Optimized Tableau" << endl;
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		mTableau->PrintToConsole();
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		cout << endl;*/

		//----------------------------------------------------------
		T fValue = 0;
		T nValue = 0;
		T floorValue = 0;
		UINT index = 0; 
		*numSteps = 0;
		for(UINT iS = 0;  iS < iSysTest ; iS++)
		{
			if(!mTableau->Column_IsInteger(iS, &index))
			{
				//We found a not integer value... 
				//Resize the tableau and fill the new row...
				mTableau->Resize(mTableau->GetRowSize()+1, mTableau->GetColumnSize()+1);

				//Nor Move the B column...
				for(UINT i=0; i<=mTableau->GetLastRowIndex(); i++)
				{
					(*mTableau)[i][mTableau->GetLastColumnIndex()] = (*mTableau)[i][mTableau->GetLastColumnIndex() - 1];
					(*mTableau)[i][mTableau->GetLastColumnIndex() - 1] = 0;
				}
	//			mTableau->PrintToConsole();
				//Fill the new tableau,...
				for(UINT i=1; i<= mTableau->GetLastColumnIndex() ; i++)
				{
					if(!mTableau->Column_IsInteger(i,&index))
					{
						/*floorValue = */mTableau->Floor(iS,i,&floorValue);
						(*mTableau)[mTableau->GetLastRowIndex()][i] = -((*mTableau)[iS][i] - floorValue) ;
					}
				}
				(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex()-1] = 1;
				// Resise the in & out arrays for dual-simplex
				vInList->Resize(mTableau->GetRowSize() - 1);
				vOutList->Resize(mTableau->GetRowSize() - 1);
				////For the B column
				//floorValue = mTableau->Floor(iS,mTableau->GetLastColumnIndex());
				////cout << "B VALUE: " << mTableau->Matrix[iS][mTableau->nLastColumn] << " FLOOR VALUE: " << floorValue << endl;
				//(*mTableau)[mTableau->GetLastRowIndex()][mTableau->GetLastColumnIndex()] = (-((*mTableau)[iS][mTableau->GetLastColumnIndex()] - floorValue));

				//Now Make the Dual Simplex
			/*	cout << endl;
				cout << "Making the dual simplex" << endl;
				cout << endl;

	//			cout << endl;
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			mTableau->PrintToConsole();
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			cout << endl;*/
				CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
	//			/*cout << endl;
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			cout << endl;*/

	//			/*cout << endl;
	//			cout << "Saved Matrix" << endl;
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			save.PrintToConsole();
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			cout << endl;*/
			}

	//	/*	cout << endl;
	//		cout << "iS = " << iS << endl;
			cout << endl;
	//		cout << "Intermadiate Tableau" << endl;
	//		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//		mTableau->PrintToConsole();
	//		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//		cout << endl;*/

			UINT iSRow = 0;

	//		//If all the Entrys in the B column are integers...
			BOOL bIntegerB = mTableau->Column_IsInteger( mTableau->GetLastColumnIndex(), 1, mTableau->GetLastRowIndex(), &iSRow);		
			cout << "bIntegerB " << bIntegerB << endl;

			if( bIntegerB && *res == SX_OPTIM )
			{
				return;
			}
			else
			{
				//The sistem is not feasible, not bouded or not optimized
				//Restore the original matrix and try with another line...
				//*mTableau = save;
				mTableau->SetSize(save.GetRowSize(), save.GetColumnSize());
				for(UINT i=0; i<mTableau->GetRowSize(); i++)
				{
					for(UINT j=0; j<mTableau->GetColumnSize(); j++)
					{
						T value = save.Matrix[i][j];
						(*mTableau)[i][j] = value;
					}
				}

	//		/*	cout << endl;
	//			cout << "Restored Tableau" << endl;
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			mTableau->PrintToConsole();
	//			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	//			cout << endl;*/
			}
		}
	}
}
#endif
