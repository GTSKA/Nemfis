

template < typename T > void Matrix_IntegerProgramming(CMatrix_<T>* mTableau, UINT *Vin, UINT *Vout, LPSIMPLEX_RESULT Res)
{
	*Res = SX_NOT_SOLUTION;
	UINT iSysTest = mTableau->nRows - 1;

	////Firs we make the dual simplex procdure
	Matrix_DualSimplex(mTableau, Vin, Vout, Res);

	/*cout << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	mTableau->PrintToConsole();
	cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;*/

	if(*Res == SX_OPTIM)
	{
		//Make acopy of the origianl matrix...
		CMatrix_<T> save;

		//--------------------------------------------------------
		//save = *mTableau;

		save.SetSize(mTableau->nRows, mTableau->nCols);
		for(UINT i=0; i<save.nRows; i++)
		{
			for(UINT j=0; j<save.nCols; j++)
			{
				T value = mTableau->Matrix[i][j];
				save.Matrix[i][j] = value;
			}
		}

	/*	cout << endl;
		cout << "Dual Optimized Tableau" << endl;
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		mTableau->PrintToConsole();
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		cout << endl;*/

		//----------------------------------------------------------
		T fValue;
		T nValue;
		T floorValue;

		UINT step = 0;

		for(UINT iS = 1;  iS<=iSysTest; iS++)
		{
			if(!mTableau->TestInteger(iS, mTableau->nLastColumn, &fValue, &nValue))
			{
				//We found a not integer value... 
				//Resize the tableau and fill the new row...
				mTableau->Resize(mTableau->nRows+1, mTableau->nCols+1);

				//Nor Move the B column...
				for(UINT i=0; i<=mTableau->nLastRow; i++)
				{
					mTableau->Matrix[i][mTableau->nLastColumn] = mTableau->Matrix[i][mTableau->nLastColumn - 1];
					mTableau->Matrix[i][mTableau->nLastColumn - 1] = 0;
				}

				//Fill the new tableau,...
				for(UINT i=1; i<= mTableau->nLastColumn - 2; i++)
				{
					if(!mTableau->TestInteger(iS,i,&fValue, &nValue))
					{
						floorValue = mTableau->Floor(iS,i);
						//cout << "VALUE[" << iS << "][" << i << "] = " << mTableau->Matrix[iS][i] << " FLOOR VALUE: " << floorValue << endl;
						mTableau->Matrix[mTableau->nLastRow][i] = -(mTableau->Matrix[iS][i] - floorValue);
					}
				}
				mTableau->Matrix[mTableau->nLastRow][mTableau->nLastColumn-1] = 1;

				//For the B column
				floorValue = mTableau->Floor(iS,mTableau->nLastColumn);
				//cout << "B VALUE: " << mTableau->Matrix[iS][mTableau->nLastColumn] << " FLOOR VALUE: " << floorValue << endl;
				mTableau->Matrix[mTableau->nLastRow][mTableau->nLastColumn] = -(mTableau->Matrix[iS][mTableau->nLastColumn] - floorValue);

				//Now we make the Dual Simplex Procedure
					//Now Make the Dual Simplex
			/*	cout << endl;
				cout << "Making the dual simplex" << endl;
				cout << endl;

				cout << endl;
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				mTableau->PrintToConsole();
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;*/

				Matrix_DualSimplex(mTableau, Vin, Vout, Res);

				/*cout << endl;
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				mTableau->PrintToConsole();
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;*/

				/*cout << endl;
				cout << "Saved Matrix" << endl;
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				save.PrintToConsole();
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;*/
			}

		/*	cout << endl;
			cout << "iS = " << iS << endl;
			cout << endl;
			cout << "Intermadiate Tableau" << endl;
			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
			mTableau->PrintToConsole();
			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;*/

			UINT iSRow;

			//If all the Entrys in the B column are integers...
			/*BOOL bIntegerB0 = mTableau->TestInteger(0, mTableau->nLastColumn, &fValue, &nValue);
			BOOL bIntegerB = mTableau->Column_TestInteger( mTableau->nLastColumn, 1, mTableau->nLastRow, &iSRow);*/

			BOOL bIntegerB = mTableau->Column_TestInteger( mTableau->nLastColumn, &iSRow);
			//cout << "B = " << bIntegerB <<  endl;
			
			if( bIntegerB && *Res == SX_OPTIM )
			{
				return;
			}
			else
			{
				//The sistem is not feasible, not bouded or not optimized
				//Restore the original matrix and try with another line...
				//*mTableau = save;
				mTableau->SetSize(save.nRows, save.nCols);
				for(UINT i=0; i<mTableau->nRows; i++)
				{
					for(UINT j=0; j<mTableau->nCols; j++)
					{
						T value = save.Matrix[i][j];
						mTableau->Matrix[i][j] = value;
					}
				}

			/*	cout << endl;
				cout << "Restored Tableau" << endl;
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				mTableau->PrintToConsole();
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				cout << endl;*/
			}
		}
	}
}

