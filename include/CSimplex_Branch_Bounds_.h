#include "CMatrix_.h"
#include "CSimplex_Canonical_.h"
#include "CSimplex_2Phases_.h"
#include "CSimplex_Dual_.h"

template <typename T> void CMatrix_Branch_and_Bounds(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	CMatrix_<T> tab1 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
	CMatrix_<T> tab2 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
		
	CMatrix_<T> tabaux = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
	
	CARRAY_UINT vInList1;
	CARRAY_UINT vInList2;
	vInList1 = (*vInList);
	vInList2 = (*vInList);
	CARRAY_UINT vOutList1;
	CARRAY_UINT vOutList2;
	vOutList1 = vInList1;
	vOutList2 = vOutList2;
	UINT row = 0;
	UINT m = 0;
		
	for ( register UINT i = 0; i < mTableau->GetRowSize(); i++)
	{
		for ( register UINT j = 0; j < mTableau->GetColumnSize(); j++)
		{
			tab1[i][j] = (*mTableau)[i][j];
			tab2[i][j] = (*mTableau)[i][j];	
			tabaux[i][j] = (*mTableau)[i][j];
		}
	}
	
	CMatrix_SimplexCanonical(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
	
	mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&row);

	tab1.Row_Add();
	tab1.Column_Insert(mTableau->GetLastColumnIndex());
	tab2.Row_Add();
	tab2.Column_Insert(mTableau->GetLastColumnIndex());
	tab2.Column_Insert(mTableau->GetLastColumnIndex());
	
	tab1[tab1.GetLastRowIndex()][(*vInList)[row-1]] = 1;
	tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()-1] = 1;
	tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = (int)(*mTableau)[row][mTableau->GetLastColumnIndex()];

	tab2[tab2.GetLastRowIndex()][(*vInList)[row-1]] = 1;
	tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-2] = -1;
	tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-1] = 1;
	//tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-1] = -1;
	tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = (int)(*mTableau)[row][mTableau->GetLastColumnIndex()]+1;
	CMatrix_<T> obj = CMatrix_<T>(1, tab2.GetColumnSize());
	for (register UINT i = 0; i < tab2.GetColumnSize(); i++)
	{
		obj[0][i] = tab2[0][i];
	} 
	CARRAY_UINT va = CARRAY_UINT(1);
	va[0] = (*vInList)[vInList->GetSize()-1]+1;
	
	vInList1.Resize(tab1.GetRowSize()-1);
	vInList2.Resize(tab2.GetRowSize()-2);
	//vInList2.Resize(tab2.GetRowSize()-1);

	vOutList1.Resize(tab1.GetRowSize()-1);
	vOutList2.Resize(tab2.GetRowSize()-2);
	//vOutList2.Resize(tab2.GetRowSize()-1);
	
	vInList1[vInList1.GetSize()-1] = tab1.GetLastColumnIndex()-1;
	vInList2[vInList2.GetSize()-1] = tab2.GetLastColumnIndex()-1;
	
	CMatrix_SimplexCanonical(&tab1, minimize, &vInList1, &vOutList1, nLastInput, numSteps, res);
	//CMatrix_SimplexCanonical(&tab2, minimize, &vInList2, &vOutList2, nLastInput, numSteps, res);
	CMatrix_Simplex_2Phases(&tab2, &obj, minimize, &va, &vInList2, &vOutList2, nLastInput, numSteps, res);
	//CMatrix_SimplexDual(&tab2, minimize, &vInList2, &vOutList2, nLastInput, numSteps, res);


	cout << "tab1[0][tab1.GetLastColumnIndex()] " << tab1[0][tab1.GetLastColumnIndex()] << endl;
	cout << "tab2[0][tab1.GetLastColumnIndex()] " << tab2[0][tab2.GetLastColumnIndex()] << endl;
	
	if ( tab1[0][tab1.GetLastColumnIndex()] < tab2[0][tab2.GetLastColumnIndex()] )
	{
		tabaux.Resize(tab2.GetRowSize(),tab2.GetColumnSize());
		
		for ( register UINT i = 0; i < tabaux.GetRowSize(); i++)
		{
			for ( register UINT j = 0; j < tabaux.GetColumnSize(); j++)
			{
				tabaux[i][j] = tab2[i][j];
			}
		}
	}
	else
	{
		tabaux.Resize(tab1.GetRowSize(),tab1.GetColumnSize());

		for ( register UINT i = 0; i < tabaux.GetRowSize(); i++)
		{
			for ( register UINT j = 0; j < tabaux.GetColumnSize(); j++)
			{
				tabaux[i][j] = tab1[i][j];
			}
		}
	}
	//tabaux.PrintToConsole();
	
	
}