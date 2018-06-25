#include "CMatrix_.h"
#include "CSimplex_Canonical_.h"
#include "CSimplex_2Phases_.h"
#include "CSimplex_Dual_.h"
#include "CFloat_.h"

#define BB_DEBUG_CONSOLE


template <typename T> void CMatrix_Branch_and_Bounds(CMatrix_<T>* mTableau, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res)
{
	CMatrix_<T> tab1 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
	CMatrix_<T> tab2 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
		
	CMatrix_<T> tabaux = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
	CMatrix_<T> tabaux1 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());
	CMatrix_<T> tabaux2 = CMatrix_<T>(mTableau->GetRowSize(),mTableau->GetColumnSize());

	CARRAY_UINT vInList1;
	CARRAY_UINT vInList2;
	CARRAY_UINT vInListaux;
	vInList1 = (*vInList);
	vInList2 = (*vInList);
	vInListaux = (*vInList);
	CARRAY_UINT vOutList1;
	CARRAY_UINT vOutList2;
	CARRAY_UINT vOutListaux;
	vOutList2 = vInList2;
	vOutList1 = vInList1;
	vOutListaux = vInListaux;
	UINT row = 0;
	UINT rowd = 0;
	UINT is = 0;
	UINT m = 0;
	CARRAY_UINT va;
	UINT g = 0;

	for ( register UINT i = 0; i < mTableau->GetRowSize(); i++)
	{
		for ( register UINT j = 0; j < mTableau->GetColumnSize(); j++)
		{
			tab1[i][j] = (*mTableau)[i][j];
			tab2[i][j] = (*mTableau)[i][j];	
			tabaux[i][j] = (*mTableau)[i][j];
		}
	}
		//tabaux siempre es el original pero con el nuevo corte
	//for ( register UINT i = 0; i<2; i++)
	T v = 0;
	do
	{
		CMatrix_SimplexCanonical(&tabaux, minimize, &vInListaux, &vOutListaux, nLastInput, numSteps, res);
		//tabaux.Column_IsInteger(tabaux.GetLastColumnIndex(),1,tabaux.GetLastRowIndex(),&row);
		//tabaux.Column_GetMaxNonIntegerValue(tabaux.GetLastColumnIndex(),1,tabaux.GetLastRowIndex(),&row);

		row = 1+(int)(((double)rand()/(double)RAND_MAX) * (double)(tabaux.GetLastRowIndex()));
		
		//if row is integer then generate row again
	
		tab1.Row_Add();
		tab1.Column_Insert(tab1.GetLastColumnIndex());
		tab2.Row_Add();
		tab2.Column_Insert(tab2.GetLastColumnIndex());
		tab2.Column_Insert(tab2.GetLastColumnIndex());
									
		tab1[tab1.GetLastRowIndex()][vInListaux[row-1]] = 1;
		tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()-1] = 1;
		tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = (int)(tabaux[row][tabaux.GetLastColumnIndex()]);
		
		tab2[tab2.GetLastRowIndex()][vInListaux[row-1]] = 1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-2] = -1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-1] = 1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = (int)(tabaux[row][tabaux.GetLastColumnIndex()]+T(1));

		
		CMatrix_<T> obj = CMatrix_<T>(1, tab2.GetColumnSize());
		va.Add(tab2.GetColumnSize()-2);

		///////////////////////////// Tableaux con cortes
		tabaux1.Resize(tab1.GetRowSize(),tab1.GetColumnSize());
		tabaux2.Resize(tab2.GetRowSize(),tab2.GetColumnSize());
		for ( register UINT i = 0; i < tab1.GetRowSize(); i++)
		{	
			for ( register UINT j = 0; j < tab1.GetColumnSize(); j++)
			{
				tabaux1[i][j] = tab1[i][j];
			}
		}
		for ( register UINT i = 0; i < tab2.GetRowSize(); i++)
		{	
			for ( register UINT j = 0; j < tab2.GetColumnSize(); j++)
			{
				tabaux2[i][j] = tab2[i][j];
			}
		}
		//cout << " TABAUX 1 " << endl;
		//tabaux1.PrintToConsole();
		//cout << " TABAUX 2 " << endl;
		//tabaux2.PrintToConsole();   
		//////////////////////////////////
				
		for (register UINT i = 0; i < tab2.GetColumnSize(); i++)
		{
			obj[0][i] = tab2[0][i];
			tab2[0][i] = 0;
		}
		for (register UINT i = 0; i < va.GetSize(); i++)
		{
			tab2[0][va[i]] = -1;
		}
	
		vInList1.RemoveAll();
		vInList2.RemoveAll();
		vOutList1.RemoveAll();
		vOutList2.RemoveAll();

		vInList1 = vInListaux;
		vInList2 = vInListaux;

		vInList1.Resize(tab1.GetRowSize()-1);
		vInList2.Resize(tab2.GetRowSize()-1);

		vOutList1.Resize(tab1.GetRowSize()-1);
		vOutList2.Resize(tab2.GetRowSize()-1);
		
		vInList1[vInList1.GetSize()-1] = tab1.GetLastColumnIndex()-1;
		vInList2[vInList2.GetSize()-1] = tab2.GetLastColumnIndex()-2;

		CMatrix_SimplexCanonical(&tab1, minimize, &vInList1, &vOutList1, nLastInput, numSteps, res);
		CMatrix_Simplex_2Phases(&tab2, &obj, minimize, &va, &vInList2, &vOutList2, nLastInput, numSteps, res);
		
		if ( tab1.Column_IsInteger(tab1.GetLastColumnIndex(),1,tab1.GetLastRowIndex(),&rowd) || tab2.Column_IsInteger(tab2.GetLastColumnIndex(),1,tab2.GetLastRowIndex(),&rowd) )
		{
			cout << " TAB 1 "<< endl;
			tab1.PrintfToConsole();
			cout << endl << " TAB 2 "<< endl;
			tab2.PrintfToConsole();
			break;
		}
		
		if ( tab1[0][tab1.GetLastColumnIndex()] < tab2[0][tab2.GetLastColumnIndex()] )
		{
			tabaux.Resize(tab2.GetRowSize(),tab2.GetColumnSize());
			vInListaux.Resize(tab2.GetColumnSize()-1);
			vOutListaux.Resize(tab2.GetColumnSize()-1);

			vInListaux = vInList2;
			vOutListaux = vOutList2;
			
			for ( register UINT i = 0; i < tabaux.GetRowSize(); i++)
			{
				for ( register UINT j = 0; j < tabaux.GetColumnSize(); j++)
				{
					tabaux[i][j] = tabaux2[i][j];
				}
			}
			///////////////////// hacer nuevo tab2
			for ( register UINT i = 0; i < tab2.GetRowSize(); i++)
			{
				for ( register UINT j = 0; j < tab2.GetColumnSize(); j++)
				{
					tab2[i][j] = tabaux[i][j];
				}
			}
	//		cout << " TABAUX 2 dentro de if " << endl;
	//		tabaux2.PrintToConsole();
			/////////////////////////
		}
		else
		{
			tabaux.Resize(tabaux1.GetRowSize(),tabaux1.GetColumnSize());
			vInListaux.Resize(tabaux1.GetColumnSize()-1);
			vOutListaux.Resize(tabaux1.GetColumnSize()-1);	

			vInListaux = vInList1;
			vOutListaux = vOutList1;

			for ( register UINT i = 0; i < tabaux.GetRowSize(); i++)
			{
				for ( register UINT j = 0; j < tabaux.GetColumnSize(); j++)
				{
					tabaux[i][j] = tabaux1[i][j];
				}
			}
			///////////////////// hacer nuevo tab1
			for ( register UINT i = 0; i < tab1.GetRowSize(); i++)
			{
				for ( register UINT j = 0; j < tab1.GetColumnSize(); j++)
				{
					tab1[i][j] = tabaux[i][j];
				}
			}
	//		cout << " TABAUX  dentro if " << endl;
	//		tabaux.PrintToConsole();
			/////////////////////////
		}
		//cout << " RESULTADO " << endl;
		//tabaux.PrintToConsole();
		//cout << endl << endl;
		g++;
	}while(!tabaux.Column_IsInteger(tabaux.GetLastColumnIndex(),1,tabaux.GetLastRowIndex(),&row));
	int i = 0;
	/*do
	{
		cout << " tabaux["<<i<<"]["<<tabaux.GetLastColumnIndex()<<"]" << tabaux[i][tabaux.GetLastColumnIndex()] << endl;
		i++;
	}while(i<tabaux.GetRowSize());
	vInListaux.PrintToConsole();
	cout << endl;*/
	cout << " IS INTEGER? " << tabaux.Column_IsInteger(tabaux.GetLastColumnIndex(),1,tabaux.GetLastRowIndex(),&row) << endl;
	cout << " ======================== OPTIM INTEGER ========================= " << endl;
	tab1.PrintfToConsole();	
	for ( UINT i = 0; i < vInList2.GetSize(); i++ )
	{
		cout << "V["<<vInList1[i]<<"] = "<<tab1[i][tab1.GetLastColumnIndex()]<<endl;
	}
}

template <typename T> void BrBo(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT *vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE type)
{
	//General purpose variables
	CARRAY_UINT ex_list;
	UINT crec = 1;
	for ( UINT i = 0 ; i < (mTableau->GetColumnSize () - mTableau->GetRowSize()) ; i++ )
	{
		ex_list.Add(i);
	}
	bool proc1 = true;
	bool proc2 = false;
	bool proc_I = false;
	bool proc_II = false;
	CMatrix_<T> original_tableau(*mTableau);
	original_tableau.Column_Remove(original_tableau.GetLastColumnIndex());

	CMatrix_<T> Slack(mTableau->GetRowSize(),1);
	CMatrix_<T> A_Slack(mTableau->GetRowSize(),1);

	
	CMatrix_<T> Artificial(mTableau->GetRowSize(),1);
	CMatrix_<T> b(mTableau->GetRowSize(),1);
	for (int i = 0; i < b.GetRowSize(); i++)
	{
		b[i][0] = (*mTableau)[i][mTableau->GetLastColumnIndex()];
	}

	CARRAY_UINT original_in1(*vInList);
	CARRAY_UINT original_in2(*vInList);
	T max_val = mTableau->Z;
	T ind_val = mTableau->Z;
	UINT max_ind = 0;
	UINT ind_find = 0;
	UINT aux_ind = 0;
	UINT spoil = 0;

	T max_val1 = T(0);
	UINT max_ind1 = 0;
	T max_val2 = T(0);
	UINT max_ind2 = 0;
	T b1 = T(0);
	T b2 = T(0);

	
	//1st branch variables
	CMatrix_<T> tab1(*mTableau);
	CARRAY_UINT in1(*vInList);
	CARRAY_UINT out1;
	SIMPLEX_RESULT r1;
	SIMPLEX_PROCEDURE_TYPE t1 = (*type);
	CARRAY_UINT art1;
	CMatrix_<T> obj1;
	
	if ( (*type) == SX_PROC_2PHASES	 )
	{
		art1 = (*vAList);
		obj1.Equals(*objFunction);
	}
	
	//2nd branch variables
	CMatrix_<T> tab2(*mTableau);
	CARRAY_UINT in2(*vInList);
	CARRAY_UINT out2;
	SIMPLEX_RESULT r2;
	SIMPLEX_PROCEDURE_TYPE t2= SX_PROC_2PHASES;
	CARRAY_UINT art2(*vAList);
	CMatrix_<T> obj2(*objFunction);
	
//	tab1.Column_Remove(tab1.GetLastColumnIndex());
//	tab2.Column_Remove(tab2.GetLastColumnIndex());

	if (objFunction->GetColumnSize() == 1)
	{
		obj2.Resize(1,tab2.GetColumnSize());
		for ( int i = 0; i < obj2.GetColumnSize(); i++ )
		{
			obj2[0][i] = (*mTableau)[0][i];
		}
	}

	Solve(mTableau, objFunction, minimize, vInList, vOutList, vAList, nLastInput, numSteps, res, type);

	//cycle start
	//In this method, original tableau stays the same as the input tableau, the slack and artificial variables are saved in diferent arrays
	for ( int j = 0; j < 2; j++ ) //3
	{
		getchar();
		cout << " ======================== PASO " << j << " ========================== " << endl;
		cout << " ========================================================== " << endl;
		if (proc1) 
		{
			//Choosing the variable to cut
			mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
		}
		cout << " max_ind " << max_ind << endl;
		
		if (proc1)
		{
			tab1.Row_Add();
			tab1.Column_Insert(tab1.GetLastColumnIndex());
			tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()-1] = tab1.I;
			tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] );
			tab1[tab1.GetLastRowIndex()][(*vInList)[max_ind]] = tab1.I;

			Slack.Row_Add();
			Slack[Slack.GetLastRowIndex()][Slack.GetLastColumnIndex()] = Slack.I;
			b1 = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] );
			cout << " Slack i " << endl;
			Slack.PrintfToConsole();
			cout << endl;
		}
		if (proc2)
		{
			t1 = SX_PROC_2PHASES;
		}
		if (proc1)
		{
			tab2.Row_Add();
			tab2.Column_Insert(tab2.GetLastColumnIndex());
			tab2.Column_Insert(tab2.GetLastColumnIndex());
			tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-2] = -1;
			tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-1] = 1;
			tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] ) + tab2.I;
			tab2[tab2.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
			for ( int i = 0; i < tab2.GetColumnSize(); i++ )
			{
				tab2[0][i] = tab2.Z;
			}
			art2.Add(tab2.GetLastColumnIndex()-1);
			tab2[0][art2[0]] = -1;

			A_Slack.Row_Add();
			A_Slack[A_Slack.GetLastRowIndex()][A_Slack.GetLastColumnIndex()] = Slack.N;
			Artificial.Row_Add();
			Artificial[0][0] = Artificial.N;
			Artificial[Artificial.GetLastRowIndex()][0] = Artificial.I;
			
			b2 = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] ) + mTableau->I;

			cout << " A_Slack  " << endl;
			A_Slack.PrintfToConsole();
			cout << endl;
		}
		if (proc2)
		{

		}

		//---------------
		in1.RemoveAll();
		//---------------
		in1.Resize(tab1.GetRowSize()-1);
		//Counter to restablish the original in-base1 variables list
		for ( int i = in1.GetSize(); i > 0; i-- )
		{
			in1[in1.GetSize()-i] = tab1.GetLastColumnIndex() - i;
		}
		cout << " in1 " << endl;
		in1.PrintToConsole();
		out1 = in1;
		
		//---------------
		in2.RemoveAll();
		//---------------
		in2.Resize(tab2.GetRowSize()-1);
		//Counter to restablish the original in-base1 variables list		
		for ( int i = in2.GetSize(); i > 0; i-- )
		{
			in2[in2.GetSize()-i] = tab2.GetLastColumnIndex() - i;
		}
		cout << " in2 " << endl;
		in2.PrintToConsole();
		out2 = in2;

		cout << " art1 " << endl;
		art1.PrintToConsole();
		cout << " art2 " << endl;
		art2.PrintToConsole();

		//At this point is needed a logical expression for the tableau solve type


		//If the input tableau solve type is Dual Simplex, there's no need to use Two-Phase Method
		//so the artificial array is not needed neither the objective function needs to be changed for
		//the second phase minimize function

		cout << " type1 " << t1;
		cout << " type2 " << t2;

		Solve(&tab1,&obj1,minimize,&in1,&out1,&art1,nLastInput,numSteps,&r1,&t1);
		Solve(&tab2,&obj2,minimize,&in2,&out2,&art2,nLastInput,numSteps,&r2,&t2);

		if ( minimize )
		{
		}
		else
		{
			if ( tab1[0][tab1.GetLastColumnIndex()] >= tab2[0][tab2.GetLastColumnIndex()] )
			{
				cout << " Branch Xbr <= [Xbr] Chosen " << endl;

				original_tableau.Row_Add();
				original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = original_tableau.I;
				
				mTableau->Equals(tab1);
				(*vInList) = in1;

				if ( mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&spoil) )
				{
					cout << " optim " << endl;
					break;
				}

				mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
				cout << " max ind " << max_ind << endl;
				mTableau->PrintfToConsole();
				cout << endl;
				cout << " resulting var " << endl;
				for ( UINT i = 0; i < vInList->GetSize(); i++ )
				{
					cout << "X["<<(*vInList)[i]<<"] = " << (*mTableau)[i+1][mTableau->GetLastColumnIndex()] << endl;
				}

				tab1.Equals(original_tableau);
				tab2.Equals(original_tableau);

				if ( proc1 )
				{
					Slack.Column_Remove(1);
					b.Resize(b.GetRowSize()+1,1);
					b[b.GetLastRowIndex()][0] = b1;
				}
				proc1 = false;
				
				
				Slack.Row_Add();
				Slack.Column_Add();
				b.Resize(b.GetRowSize()+1,1);

				Add_LastSlack(&Slack,true);

				b1 = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] );
				b[b.GetLastRowIndex()][0] = b1;
				
				tab1.Join_Right(&Slack);
				tab1.Join_Right(&Artificial);
				tab1.Join_Right(&b);
				tab1[tab1.GetLastRowIndex()][in2[max_ind]] = tab1.I;

				art1.RemoveAll();
				if ( proc_II )
				{
					for ( int i = 0 ; i < Artificial.GetColumnSize() ; i++ )
					{
						art1.Add(tab1.GetLastColumnIndex() - 1 - i);
					}
				}

				Turn_LastSlack(&Slack);

				b2 = floor( (*mTableau)[max_ind +1][mTableau->GetLastColumnIndex()] ) + mTableau->I;


				b[b.GetLastRowIndex()][0] = b2;

				if (proc_II)
				{
					Artificial.Column_Remove(Artificial.GetLastColumnIndex());
					Artificial.Row_Add();
				}
								
				Add_LastArtificial(&Artificial);

				tab2.Join_Right(&Slack);
				tab2.Join_Right(&Artificial);
				tab2.Join_Right(&b);
				tab2[tab2.GetLastRowIndex()][in2[max_ind]] = tab2.I;

				Turn_LastSlack(&Slack);

				art2.RemoveAll();
				for ( int i = 0 ; i < Artificial.GetColumnSize() ; i++ )
				{
					art2.Add(tab2.GetLastColumnIndex() - 1 - i);
				}
					

				for ( int i = 0; i < original_tableau.GetColumnSize(); i++ )
				{
					if ( proc_II )
					{
						tab1[0][i] = tab1.Z;
					}
					tab2[0][i] = tab2.Z;
				}


				cout << " TAB1 " << endl;
				tab1.PrintfToConsole();
				cout << " TAB2 " << endl;
				tab2.PrintfToConsole();
				

			}
			else
			{
				cout << " Branch Xbr >= [Xbr] + 1 Chosen " << endl;
				original_tableau.Row_Add();
				original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = original_tableau.I;
				
				mTableau->Equals(tab1);
				(*vInList) = in1;

				if ( mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(),1,mTableau->GetLastRowIndex(),&spoil) )
				{
					cout << " optim " << endl;
					break;
				}
     
				mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
				cout << " max ind " << max_ind << endl;

				mTableau->PrintfToConsole();
				cout << endl;
				cout << " resulting var " << endl;
				for ( UINT i = 0; i < vInList->GetSize(); i++ )
				{
					cout << "X["<<(*vInList)[i]<<"] = " << (*mTableau)[i+1][mTableau->GetLastColumnIndex()] << endl;
				}
				tab1.Equals(original_tableau);
				tab2.Equals(original_tableau);

				//Remove from tab1 the objective function because at least one artificial variable is now in the basis
				if ( proc1 ) 
				{
					obj1.Resize(1,tab1.GetColumnSize());
				}
				for ( int i = 0; i < obj1.GetColumnSize(); i++ )
				{
					obj1[0][i] = tab1[0][i];
				}
				for ( int i = 0; i < original_tableau.GetColumnSize(); i++ )
				{
					tab1[0][i] = tab1.Z;
					tab2[0][i] = tab2.Z;
				}

				//cout << " Tab1 " << endl;
				//tab1.PrintfToConsole();
				//cout << " tab2 " << endl;
				//tab2.PrintfToConsole();

				//cout << " Slack " << endl;
				//Slack.PrintfToConsole();
				//cout << " Artificial " << endl;
				//Artificial.PrintfToConsole();
				//
				if ( proc1 )
				{
					Slack.Column_Remove(UINT(0));
					b.Resize(b.GetRowSize()+1,1);
					b[b.GetLastRowIndex()][0] = b2;
				}
				proc1 = false;

				Slack.Row_Add();
				Slack.Column_Add();
				b.Resize(b.GetRowSize()+1,1);

				Add_LastSlack(&Slack,true);

				b1 = floor( (*mTableau)[max_ind + 1][mTableau->GetLastColumnIndex()] );

			/*	cout << " Slack M1 " << endl;
				Slack.PrintfToConsole();*/

				
				b[b.GetLastRowIndex()][0] = b1;
			/*	cout << " b = " << b1 << endl;
				b.PrintfToConsole();*/

				tab1.Join_Right(&Slack);
				tab1.Join_Right(&Artificial);
				tab1.Join_Right(&b);
				tab1[tab1.GetLastRowIndex()][in2[max_ind]] = tab1.I;

				art1.RemoveAll();
				for ( int i = 0 ; i < Artificial.GetColumnSize() ; i++ )
				{
					art1.Add(tab1.GetLastColumnIndex() - 1 - i);
				}

				Turn_LastSlack(&Slack);

				/*cout << " tab1 mod " << endl;
				tab1.PrintfToConsole();*/

				b2 = floor( (*mTableau)[max_ind +1][mTableau->GetLastColumnIndex()] ) + mTableau->I;


			/*	cout << " Slack M2 " << endl;
				Slack.PrintfToConsole();*/
				b[b.GetLastRowIndex()][0] = b2;
				/*cout << " b2 = " << b2 << endl;
				b.PrintfToConsole();*/

				Artificial.Column_Add();
				Artificial.Row_Add();
				
				Add_LastArtificial(&Artificial);

				tab2.Join_Right(&Slack);
				tab2.Join_Right(&Artificial);
				tab2.Join_Right(&b);
				tab2[tab2.GetLastRowIndex()][in2[max_ind]] = tab2.I;

				Turn_LastSlack(&Slack);

				//cout << " Artificial 2 " << endl;
				//Artificial.PrintfToConsole();

				//cout << " Tab2 mod " << endl;
				//tab2.PrintfToConsole();

				//cout << " art1 " << endl;
				//art1.PrintToConsole();
				//cout << " art2 " << endl;
				//art2.PrintToConsole();

				art2.RemoveAll();
				for ( int i = 0 ; i < Artificial.GetColumnSize() ; i++ )
				{
					art2.Add(tab2.GetLastColumnIndex() - 1 - i);
				}
				
			/*	cout << " new Art1 " << endl;
				art1.PrintToConsole();
				cout << " new Art2 " << endl;
				art2.PrintToConsole();*/

				cout << " tab1 " << endl;
				tab1.PrintfToConsole();
				cout << " tab2 " << endl;
				tab2.PrintfToConsole();

				proc2 = true;
				proc_II = true;
				Slack.PrintfToConsole();

				
	
			}
		}
	}	
}
template <typename T> void Branch_Bounds(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT *vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE type, bool optimized)
{
	//General purpose variables
	CARRAY_UINT ex_list;
	UINT crec = 1;
	for ( UINT i = 0 ; i < (mTableau->GetColumnSize () - mTableau->GetRowSize()) ; i++ )
	{
		ex_list.Add(i);
	}
	bool proc1 = true;
	bool proc2 = true;
	bool proc3 = true;
	bool proc4 = true;
	CMatrix_<T> original_tableau(*mTableau);
	CARRAY_UINT original_in1(*vInList);
	CARRAY_UINT original_in2(*vInList);
	T max_val = mTableau->Z;
	T ind_val = mTableau->Z;
	UINT max_ind = 0;
	UINT ind_find = 0;
	UINT aux_ind = 0;
	UINT spoil = 0;

	T max_val1 = T(0);
	UINT max_ind1 = 0;
	T max_val2 = T(0);
	UINT max_ind2 = 0;

	
	//1st branch variables
	CMatrix_<T> tab1(*mTableau);
	CARRAY_UINT in1(*vInList);
	CARRAY_UINT out1;
	SIMPLEX_RESULT r1;
	SIMPLEX_PROCEDURE_TYPE t1 = (*type);
	CARRAY_UINT art1;
	CMatrix_<T> obj1;
	if ( (*type) == SX_PROC_2PHASES	 )
	{
		art1 = (*vAList);
		obj1.Equals(*objFunction);
	}
	
	//2nd branch variables
	CMatrix_<T> tab2(*mTableau);
	CARRAY_UINT in2(*vInList);
	CARRAY_UINT out2;
	SIMPLEX_RESULT r2;
	SIMPLEX_PROCEDURE_TYPE t2= SX_PROC_2PHASES;
	CARRAY_UINT art2(*vAList);
	CMatrix_<T> obj2(*objFunction);

	
	UINT a = (*vInList)[0];
	UINT b = (*vInList)[vInList->GetSize()-1];

	if( !optimized )
	{
		Solve(mTableau, objFunction, minimize, vInList, vOutList, vAList, nLastInput, numSteps, res, type);
	}

	//mTableau->Row_DriebeekRule_Max_Penalization(0, mTableau->GetLastColumnIndex() - 1, ex_list, &max_val, &max_ind);
	//choosing variable to cut
	//cycle start
	for ( int i = 0; i < 2; i++ ) //3
	{
		cout << " ======================== PASO " << i << " ========================== " << endl;
		cout << " ========================================================== " << endl;
		if (proc1) 
		{
			//Choosing the variable to cut
			mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
		}
		cout << " max_ind " << max_ind << endl;

		//Creating branch Xbr <= [Xbr]
		tab1.Row_Add();
		tab1.Column_Insert(tab1.GetLastColumnIndex());
		tab1[tab1.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
		tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()-1] = 1;
		tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) ;

		original_tableau.Row_Add();
		original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
		original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
		original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()-1] = 1;
		original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]);

		
		in1.Resize(tab1.GetRowSize()-1);
		//Counter to restablish the original in-base1 variables list
		for ( int i = in1.GetSize(); i > 0; i-- )
		{
			in1[in1.GetSize()-i] = tab1.GetLastColumnIndex() - i;
		}
		out1 = in1;

		cout << " tab1 " << endl;
		tab1.PrintfToConsole();

		cout << " in1 " << endl;
		in1.PrintToConsole();
		
		//Creating branch Xbr <= [Xbr]+1

		tab2.Row_Add();
		tab2.Column_Insert(tab2.GetLastColumnIndex());
		tab2.Column_Insert(tab2.GetLastColumnIndex()-crec);
		tab2[tab2.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-(crec+1)] = -1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()-1] = 1;
		tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) + tab2.I ;

		original_tableau.Row_Add();
		original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
		original_tableau.Column_Insert(original_tableau.GetLastColumnIndex()-crec);
		original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
		original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()-(crec+1)] = -1;
		original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()-1] = 1;
		original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) + original_tableau.I ;;
		
		art2.Add(tab2.GetLastColumnIndex()-1);
		//tab2[0][art2[art2.GetSize()-1]] = -1;
		obj2.Resize(1,tab2.GetColumnSize());


		in2.Resize(tab2.GetRowSize()-1);
		//Counter to restablish the original in-base1 variables list		
		for ( int i = in2.GetSize(); i > 0; i-- )
		{
			in2[in2.GetSize()-i] = tab2.GetLastColumnIndex() - i;
		}
		out2 = in2;

		if ( proc1 )
		{
			if ( t1 != SX_PROC_2PHASES )
			{
				for ( UINT i = 0; i < tab2.GetColumnSize(); i++ )
				{
					obj2[0][i] = tab2[0][i];
					if ( art2.Find(i) )
					{
						tab2[0][i] = tab2.N;
					}
					else
					{
						tab2[0][i] = tab2.Z;
					}
				}
			}
			proc1 = false;
		}
		
		cout << " tab2 " << endl;
		tab2.PrintfToConsole();
		cout << " in2 " << endl; 
		in2.PrintToConsole();

		cout << " original " << endl;
		original_tableau.PrintfToConsole();

		
		//Now we solve tab1 and tab2 for choosing the better constraint
		Solve(&tab1,&obj1,minimize,&in1,&out1,&art1,nLastInput,numSteps,&r1,&t1);
		if ( r1 == SX_OPTIM )
		{
			tab1.Row_DriebeekRule_Max_Penalization(0,tab1.GetLastColumnIndex()-1,ex_list, in1, &max_val1, &max_ind1);
		}
		cout << " solved tab1 " << endl;
		tab1.PrintfToConsole();
		cout << " in1.GetSize() = "  << in1.GetSize() << endl;
		Solve(&tab2,&obj2,minimize,&in2,&out2,&art2,nLastInput,numSteps,&r2,&t2);
		if ( r2 == SX_OPTIM )
		{
			tab2.Row_DriebeekRule_Max_Penalization(0,tab2.GetLastColumnIndex()-1,ex_list, in2, &max_val2, &max_ind2);
		}
		cout << " solved tab2 " << endl;
		tab2.PrintfToConsole();

		//Now we choose the better tableau for further analysis

		if ( minimize )
		{
			if ( tab1[0][tab1.GetLastColumnIndex()] <= tab2[0][tab2.GetLastColumnIndex()] || (r1 == SX_OPTIM && r2 != SX_OPTIM) )
			{	
				proc1 = false;
				mTableau->Equals(tab1);
				(*vInList) = in1;
				(*vOutList) = out1;

			}
			else if ( tab1[0][tab1.GetLastColumnIndex()] > tab2[0][tab2.GetLastColumnIndex()] || (r1 != SX_OPTIM && r2 == SX_OPTIM) )
			{
				proc1 = false;
				mTableau->Equals(tab2);
				(*vInList) = in2;
				(*vOutList) = out2;				
			}
		}
		else
		{
			if ( tab1[0][tab1.GetLastColumnIndex()] >= tab2[0][tab2.GetLastColumnIndex()] || (r1 == SX_OPTIM && r2 != SX_OPTIM) )
			{	
				cout << " Branch Xbr <= [Xbr] Chosen " << endl;
				proc1 = false;
				mTableau->Equals(tab1);
				(*vInList) = in1;
				(*vOutList) = out1;

				//Need to remove the branch Xbr >= [Xbr]+1
				original_tableau.Row_Remove(original_tableau.GetLastRowIndex());
				original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
				original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
				cout << " original en if " << endl;
				original_tableau.PrintfToConsole();
				tab1.Equals(original_tableau);
				tab2.Equals(original_tableau);
				art2 = art1;		
				max_val = max_val1;
				max_ind = max_ind1;
			}
			else if ( tab1[0][tab1.GetLastColumnIndex()] < tab2[0][tab2.GetLastColumnIndex()] || (r1 != SX_OPTIM && r2 == SX_OPTIM) )
			{
				cout << " Branch Xbr >= [Xbr] + 1 Chosen " << endl;
				proc1 = false;
				mTableau->Equals(tab2);
				(*vInList) = in2;
				(*vOutList) = out2;

				//Need to remove the branch Xbr <= [Xbr]
				original_tableau.Row_Remove(original_tableau.GetLastRowIndex()-1);
				original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-3);
				tab1.Equals(original_tableau);
				tab2.Equals(original_tableau);
				art1 = art2;
				crec++;
			}

		}
		if ( mTableau->Column_IsInteger(mTableau->GetLastColumnIndex(), 1, mTableau->GetLastRowIndex(), &spoil) )
		{
			cout << " Resulting Variables " <<endl;
			for ( int i = 0; i < vInList->GetSize(); i++ )
			{
				cout <<" X["<<(*vInList)[i]<<"] = " << (*mTableau)[i+1][mTableau->GetLastColumnIndex()] << endl;
			}				
			break;
		}
	}
}
template <typename T> void BB(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT *vAList, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE type)
{
	//General purpose variables
	CARRAY_UINT ex_list;
	UINT crec = 1;
	//si hay variables artificiales, hay q cambiar el criterio de busqueda de variables originales 
	for ( UINT i = 0 ; i < (mTableau->GetColumnSize () - mTableau->GetRowSize()) ; i++ )
	{
		ex_list.Add(i);
	}
	cout << "EXLIST " << endl;
	ex_list.PrintToConsole();

	bool proc1 = true;
	bool proc2 = false;
	bool proc_I = false;
	bool proc_II = false;
	bool b_1 = false;
	bool b_2 = false;
	CMatrix_<T> original_tableau(*mTableau);

	CARRAY_UINT original_in(*vInList);
	CARRAY_UINT original_in1(*vInList);
	CARRAY_UINT original_in2(*vInList);

	T max_val = mTableau->Z;
	T ind_val = mTableau->Z;
	UINT max_ind = 0;
	UINT ind_find = 0;
	UINT aux_ind = 0;
	UINT spoil = 0;

	T max_val1 = T(0);
	UINT max_ind1 = 0;
	T max_val2 = T(0);
	UINT max_ind2 = 0;
	T b1 = T(0);
	T b2 = T(0);

	
	//1st branch variables
	CMatrix_<T> tab1(*mTableau);
	CARRAY_UINT in1(*vInList);
	CARRAY_UINT out1;
	SIMPLEX_RESULT r1;
	SIMPLEX_PROCEDURE_TYPE t1 = (*type);
	CARRAY_UINT art1;
	CMatrix_<T> obj1;
	
	if ( (*type) == SX_PROC_2PHASES	 )
	{
		art1 = (*vAList);
		obj1.Equals(*objFunction);
	}
	
	//2nd branch variables
	CMatrix_<T> tab2(*mTableau);
	CARRAY_UINT in2(*vInList);
	CARRAY_UINT out2;
	SIMPLEX_RESULT r2;
	SIMPLEX_PROCEDURE_TYPE t2= SX_PROC_2PHASES;
	CARRAY_UINT art2(*vAList);
	CMatrix_<T> obj2(*objFunction);
	
//	tab1.Column_Remove(tab1.GetLastColumnIndex());
//	tab2.Column_Remove(tab2.GetLastColumnIndex());

	if (objFunction->GetColumnSize() == 1)
	{
		obj2.Resize(1,tab2.GetColumnSize());
		for ( int i = 0; i < obj2.GetColumnSize(); i++ )
		{
			obj2[0][i] = (*mTableau)[0][i];
		}
	}

	//Step1: Solve Inicial Tableau
	Solve(mTableau, objFunction, minimize, vInList, vOutList, vAList, nLastInput, numSteps, res, type);
	mTableau->PrintfToConsole();
	cout << " vinList " << endl;
	vInList->PrintToConsole();	

	//cycle start
	for ( int i = 0; i < 9; i++ )
	{
		if ( *type != SX_PROC_DUALSIMPLEX )
		{
			//Step2: Choose Variable to Bound
			cout << " =============================== Step "<<i<<" ============================" << endl;
			//getchar();
			if ( proc1 ) 
			{
				mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
			}
			
			//Creating branch Xbr <= [Xbr]
			cout << " Creating Branch I Adding Xbr <= [Xbr] Restriction " << endl;
			tab1.Row_Add();
			tab1.Column_Insert(tab1.GetLastColumnIndex());
			tab1[tab1.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
			Add_Slack(&tab1, tab1.GetLastColumnIndex()-1, true);
			tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) ;

			original_tableau.Row_Add();
			original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
			original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
			Add_Slack(&original_tableau, original_tableau.GetLastColumnIndex()-1, true);
			original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]);
			
			//in1.Add(tab1.GetLastColumnIndex()-1);
			in1.Add();
			in1[in1.GetSize()-1] = tab1.GetLastColumnIndex()-1;
			
			original_in.Add(tab1.GetLastColumnIndex()-1);
			out1 = in1;

			cout << " Lineal Program of Branch I " << endl;
			tab1.PrintfToConsole();

			//cout << " in1 " << endl;
			//in1.PrintToConsole();

			//Creating branch Xbr >= [Xbr]+1
			cout << " Creating Branch II Adding Xbr >= [Xbr] + 1 Restriction " << endl;

			tab2.Row_Add();
			tab2.Column_Insert(tab2.GetLastColumnIndex());
			tab2.Column_Insert(tab2.GetLastColumnIndex());

			if ( proc1 && ((*type) != SX_PROC_2PHASES) ) 
			{
				Set_to_2Phases(&tab2, &obj2);
			}
			tab2[tab2.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
			Add_Slack(&tab2, tab2.GetLastColumnIndex()-2, false);
			Add_Artificial(&tab2, tab2.GetLastColumnIndex()-1);
			tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) + tab2.I ;

			original_tableau.Row_Add();
			original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
			original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
			original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = 1;
			Add_Slack(&original_tableau, original_tableau.GetLastColumnIndex()-2, false);
			Add_Artificial(&original_tableau, original_tableau.GetLastColumnIndex()-1);
			original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = floor((*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()]) + original_tableau.I ;;
			
			//art2.Add(tab2.GetLastColumnIndex()-1);
			art2.Add();
			art2[art2.GetSize()-1] = tab2.GetLastColumnIndex()-1;

			//in2.Add(tab2.GetLastColumnIndex()-1);
			in2.Add();
			in2[in2.GetSize()-1] = tab2.GetLastColumnIndex()-1;

			//original_in.Add(tab2.GetLastColumnIndex()-1);
			original_in.Add();
			original_in[original_in.GetSize()-1] = tab2.GetLastColumnIndex()-1;

			out2 = in2;
			
			cout << " Lineal Program with Branch II " << endl;
			tab2.PrintfToConsole();

			//cout << " in2 " << endl;
			//in2.PrintToConsole();
		
			//cout << " original tableau with both activities added " << endl;
			//original_tableau.PrintfToConsole();

			//cout << endl;

			//cout << " tab1.GetRowSize() " << tab1.GetRowSize() << " in1.GetSize() " << in1.GetSize() << endl;
			//cout << " out1.GetSize() " << out1.GetSize() << " art1.GetSize() " << art1.GetSize() << endl;
			cout << " X" << (*vInList)[max_ind] << " <= " << tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()]<< endl;
			//cout << " tab1.GetRowSize() " << tab1.GetRowSize() << endl;
			//cout << " obj1.GetColumnSize() " << obj1.GetColumnSize() << endl;
			//cout << " in1.GetSize() " << in1.GetSize() << endl;
			//cout << " out1.GetSize() " << out1.GetSize() << endl;
			//cout << " art1.GetSize() " << art1.GetSize() << endl;
			Solve(&tab1,&obj1,minimize,&in1,&out1,&art1,nLastInput,numSteps,&r1,&t1);
			cout << " Z1 = " << tab1[0][tab1.GetLastColumnIndex()] << endl;
		/*	cout << " RESULT " << r1 << " Z = " << tab1[0][tab1.GetLastColumnIndex()] << endl;
			for ( int i = 0; i < in1.GetSize(); i++ )
			{
				if ( r1 == SX_OPTIM )
				{
					cout << " x["<<in1[i]<<"] = "<<tab1[i+1][tab1.GetLastColumnIndex()] << endl;
				}
			}*/
			if ( r1 == SX_OPTIM ) 
			{
				if ( !tab1.Row_DriebeekRule_Max_Penalization(0,tab1.GetLastColumnIndex()-1,ex_list, in1, &max_val1, &max_ind1) )
				{
					cout << " optim 1" << endl;
					for ( int i = 0; i < in1.GetSize(); i++ )
					{
						cout << " x["<<in1[i]<<"] = "<<tab1[i+1][tab1.GetLastColumnIndex()] << endl;
					}
					cout << " Z = " << tab1[0][tab1.GetLastColumnIndex()] << endl;
					break;
				}
			}
			//tab1.PrintfToConsole();
			//in1.PrintToConsole();

			//cout << endl;
			cout << " X" << (*vInList)[max_ind] << " >= " << tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()]<< endl;
			//cout << " tab2.GetRowSize() " << tab2.GetRowSize() << " in2.GetSize() " << in2.GetSize() << endl;
			//cout << " out2.GetSize() " << out2.GetSize() << " art2.GetSize() " << art2.GetSize() << endl;
			//cout << " tab2.GetRowSize() " << tab2.GetRowSize() << endl;
			//cout << " obj2.GetColumnSize() " << obj2.GetColumnSize() << endl;
			//cout << " in2.GetSize() " << in2.GetSize() << endl;
			//cout << " out2.GetSize() " << out2.GetSize() << endl;
			//cout << " art2.GetSize() " << art2.GetSize() << endl;
			Solve(&tab2,&obj2,minimize,&in2,&out2,&art2,nLastInput,numSteps,&r2,&t2);
			cout << " Z2 = " << tab2[0][tab2.GetLastColumnIndex()] << endl;
		/*	cout << " RESULT " << r2 << " Z = " << tab2[0][tab2.GetLastColumnIndex()] << endl;
			for ( int i = 0; i < in2.GetSize(); i++ )
			{
				if ( r2 == SX_OPTIM )
				{
					cout << " x["<<in2[i]<<"] = "<<tab2[i+1][tab2.GetLastColumnIndex()] << endl;
				}
			}*/
			cout << endl;
			if ( r2 == SX_OPTIM ) 
			{
				if ( !tab2.Row_DriebeekRule_Max_Penalization(0,tab2.GetLastColumnIndex()-1,ex_list, in2, &max_val2, &max_ind2) )
				{
					cout << " optim 2" << endl;
					for ( int i = 0; i < in2.GetSize(); i++ )
					{
						if ( r2 == SX_OPTIM )
						{
							cout << " x["<<in2[i]<<"] = "<<tab2[i+1][tab2.GetLastColumnIndex()] << endl;
						}
					}
					cout << " Z = " << tab2[0][tab2.GetLastColumnIndex()] << endl;
					break;
				}
			}
			//tab2.PrintfToConsole();
			//in2.PrintToConsole();

			if ( minimize ) 
			{
				if( tab1[0][tab1.GetLastColumnIndex()] <= tab2[0][tab2.GetLastColumnIndex()] || ( r1 == SX_OPTIM && r2 != SX_OPTIM ) )
				{
					if ( tab1.Column_IsInteger(tab1.GetLastColumnIndex(),1,tab1.GetLastRowIndex(),&spoil) )
					{
						cout << " optim " << endl;
						break;
					}
					else
					{
						cout << " BRANCH Xbr <= [Xbr] " << endl;
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
						original_tableau.Row_Remove(original_tableau.GetLastRowIndex());
						original_in.Remove(original_in.GetSize()-1);
						
						
						art2.Remove(art2.GetSize()-1);
						max_ind = max_ind1;
						mTableau->Equals(tab1);
						(*vInList) = in1;
						in1 = original_in;
						in2 = in1;
						//cout << " original 1 " << endl;
						//original_tableau.PrintfToConsole();
						tab1.Equals(original_tableau);
						tab2.Equals(original_tableau);
						////////Set_to_2Phases(&tab2,&obj2);

						//cout << " obj1 " << endl;
						//obj1.PrintfToConsole();
						//cout << " obj2 " << endl;
						//obj2.PrintfToConsole();
					
						proc1 = false;
					}
				}
				else if ( tab1[0][tab1.GetLastColumnIndex()] > tab2[0][tab2.GetLastColumnIndex()] || ( r1 != SX_OPTIM && r2 != SX_OPTIM ) )
				{
					if ( tab2.Column_IsInteger(tab2.GetLastColumnIndex(),1,tab2.GetLastRowIndex(),&spoil) ) 
					{
						cout << " optim " << endl;
						break;
					}
					else
					{
						cout << " Branch Xbr >= [Xbr] + 1 chosen " << endl;
						Set_to_2Phases(&original_tableau,objFunction);
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-3);
						original_tableau.Row_Remove(original_tableau.GetLastRowIndex()-1);
						original_in.Remove(original_in.GetSize()-2);
						t1 = SX_PROC_2PHASES;
						
						art1 = art2;
						obj1.Equals(obj2);
						max_ind = max_ind2;
						mTableau->Equals(tab2);
						(*vInList) = in2;
						in2 = original_in;
						in1 = in2;
						tab1.Equals(original_tableau);
						tab2.Equals(original_tableau);
						//---
						obj1.Resize(1,original_tableau.GetColumnSize());
						obj2.Resize(1,original_tableau.GetColumnSize());
						//---

						//cout << " original 2 " << endl;
						//original_tableau.PrintfToConsole();
						//cout << " vinList " << endl;
						//vInList->PrintToConsole();
						//cout << " (*vInList)[max_ind] " << (*vInList)[max_ind] << endl;

						//cout << " obj1 " << endl;
						//obj1.PrintfToConsole();
						//cout << " obj2 " << endl;
						//obj2.PrintfToConsole();
						proc1 = false;
					}
				}
			}
			else
			{
				if ( (tab1[0][tab1.GetLastColumnIndex()] >= tab2[0][tab2.GetLastColumnIndex()]) && ( r1 == SX_OPTIM || r2 != SX_OPTIM ) )
				{
					if ( tab1.Column_IsInteger(tab1.GetLastColumnIndex(),1,tab1.GetLastRowIndex(),&spoil))
					{
						cout << " OPTIM INTEGER " << endl;
						break;
					}
					else
					{
						cout << " Branch Xbr <= [Xbr] chosen " << endl;
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-1);
						original_tableau.Row_Remove(original_tableau.GetLastRowIndex());
						original_in.Remove(original_in.GetSize()-1);
						
						art2.Remove(art2.GetSize()-1);
						max_ind = max_ind1;
						mTableau->Equals(tab1);
						(*vInList) = in1;
						in1 = original_in;
						in2 = in1;
						//cout << " original 1 " << endl;
						//original_tableau.PrintfToConsole();
						tab1.Equals(original_tableau);
						tab2.Equals(original_tableau);
						////////Set_to_2Phases(&tab2,&obj2);

						//cout << " obj1 " << endl;
						//obj1.PrintfToConsole();
						//cout << " obj2 " << endl;
						//obj2.PrintfToConsole();
					
						proc1 = false;
					}
				}
				else if ( (tab1[0][tab1.GetLastColumnIndex()] < tab2[0][tab2.GetLastColumnIndex()]) && ( r1 != SX_OPTIM || r2 == SX_OPTIM) )
				{
					if ( tab2.Column_IsInteger(tab2.GetLastColumnIndex(),1,tab2.GetLastRowIndex(),&spoil) )
					{
						cout << " OPTIM INTEGER " << endl;
						break;
					}
					else
					{
						cout << " Branch Xbr >= [Xbr] + 1 chosen " << endl;
						Set_to_2Phases(&original_tableau,objFunction);
						original_tableau.Column_Remove(original_tableau.GetLastColumnIndex()-3);
						original_tableau.Row_Remove(original_tableau.GetLastRowIndex()-1);
						original_in.Remove(original_in.GetSize()-2);
						t1 = SX_PROC_2PHASES;
						
						art1 = art2;
						obj1.Equals(obj2);
						max_ind = max_ind2;
						mTableau->Equals(tab2);
						(*vInList) = in2;
						in2 = original_in;
						in1 = in2;
						tab1.Equals(original_tableau);
						tab2.Equals(original_tableau);
						//---
						obj1.Resize(1,original_tableau.GetColumnSize());
						obj2.Resize(1,original_tableau.GetColumnSize());
						//---

						//cout << " original 2 " << endl;
						//original_tableau.PrintfToConsole();
						//cout << " vinList " << endl;
						//vInList->PrintToConsole();
						//cout << " (*vInList)[max_ind] " << (*vInList)[max_ind] << endl;

						//cout << " obj1 " << endl;
						//obj1.PrintfToConsole();
						//cout << " obj2 " << endl;
						//obj2.PrintfToConsole();
						proc1 = false;
					}
				}
			}
		}
		else
		{
			//Creating branch Xbr <= [Xbr]
			if ( proc1 )
			{
				mTableau->Row_DriebeekRule_Max_Penalization(0,mTableau->GetLastColumnIndex()-1,ex_list, *vInList, &max_val, &max_ind);	
			}

			tab1.Row_Add();
			tab1.Column_Insert(tab1.GetLastColumnIndex());
			tab1[tab1.GetLastRowIndex()][(*vInList)[max_ind]] = tab1.I;
			tab1[tab1.GetLastRowIndex()][tab1.GetLastColumnIndex()] = floor( (*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()] );
			in1.Add();
			in1[in1.GetSize()-1] = tab1.GetLastColumnIndex()-1;
			out1 = in1;
			
			original_tableau.Row_Add();
			original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
			original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = original_tableau.I;
			original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = floor( (*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()] );
			original_in.Add();
			original_in[original_in.GetSize()-1] = tab1.GetLastColumnIndex()-1;

			tab2.Row_Add();
			tab2.Column_Insert(tab2.GetLastColumnIndex());
			tab2[tab2.GetLastRowIndex()][(*vInList)[max_ind]] = tab1.N;
			tab2[tab2.GetLastRowIndex()][tab2.GetLastColumnIndex()] = (floor( (*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()] ) + mTableau->I ) * ( mTableau->N ) ;
			in2.Add();
			in2[in2.GetSize()-1] = tab2.GetLastColumnIndex() - 1;
			out2 = in2;

			original_tableau.Row_Add();
			original_tableau.Column_Insert(original_tableau.GetLastColumnIndex());
			original_tableau[original_tableau.GetLastRowIndex()][(*vInList)[max_ind]] = original_tableau.N;
			original_tableau[original_tableau.GetLastRowIndex()][original_tableau.GetLastColumnIndex()] = (floor( (*mTableau)[max_ind+1][mTableau->GetLastColumnIndex()] ) + mTableau->I ) * ( mTableau->N ) ;
			original_in.Add();
			original_in[original_in.GetSize()-1] = tab1.GetLastColumnIndex()-1;
			
			Solve(&tab1,&obj1,minimize,&in1,&out1,&art1,nLastInput,numSteps,&r1,&t1);
			tab1.Row_DriebeekRule_Max_Penalization(0,tab1.GetLastColumnIndex()-1,&max_val1,&max_ind1);
			Solve(&tab2,&obj2,minimize,&in2,&out2,&art2,nLastInput,numSteps,&r2,&t2);
			tab2.Row_DriebeekRule_Max_Penalization(0,tab2.GetLastColumnIndex()-1,&max_val2,&max_ind2);

			if (minimize)
			{
				if ( (tab1[0][tab1.GetLastColumnIndex()] <= tab2[0][tab2.GetLastColumnIndex()]) || ( r1 == SX_OPTIM && r2 != SX_OPTIM ) )
				{

				}
				else if ( (tab1[0][tab1.GetLastColumnIndex()] > tab2[0][tab2.GetLastColumnIndex()]) || ( r1 != SX_OPTIM && r2 == SX_OPTIM ) )
				{

				}
			}
			else
			{
				if ( (tab1[0][tab1.GetLastColumnIndex()] >= tab2[0][tab2.GetLastColumnIndex()]) || ( r1 == SX_OPTIM && r2 != SX_OPTIM ) )
				{

				}
				else if ( (tab1[0][tab1.GetLastColumnIndex()] < tab2[0][tab2.GetLastColumnIndex()]) || ( r1 != SX_OPTIM && r2 == SX_OPTIM ) )
				{

				}
			}				
		}
	}
}
template <typename T> void Solve(CMatrix_<T>* mTableau, CMatrix_<T>* objFunction, bool minimize, CARRAY_UINT* vInList, CARRAY_UINT* vOutList, CARRAY_UINT* vAList, UINT* nLastInput, UINT* numSteps, LPSIMPLEX_RESULT res, LPSIMPLEX_PROCEDURE_TYPE type)
{
	switch(*type)
	{
	case 1:
	CMatrix_SimplexCanonical(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
		break;
	case 2:
	CMatrix_Simplex_2Phases(mTableau, objFunction, minimize, vAList, vInList, vOutList, nLastInput, numSteps, res);
		break;
	case 4:
		CMatrix_SimplexDual(mTableau, minimize, vInList, vOutList, nLastInput, numSteps, res);
		break;
	}
}

template <typename T> void Add_LastSlack(CMatrix_<T>* Matrix, bool positive)
{
	for ( int i = 0; i < Matrix->GetRowSize() - 1 ; i++ )
	{
		(*Matrix)[i][Matrix->GetLastColumnIndex()] = Matrix->Z;
	}
	if(positive)
	{
		(*Matrix)[Matrix->GetLastRowIndex()][Matrix->GetLastColumnIndex()] = Matrix->I;
	}
	else
	{
		(*Matrix)[Matrix->GetLastRowIndex()][Matrix->GetLastColumnIndex()] = Matrix->N;
	}
}
template <typename T> void Add_Slack(CMatrix_<T>* Matrix, UINT iCol, bool positive)
{
	for ( int i = 0; i < Matrix->GetRowSize() - 1 ; i++ )
	{
		(*Matrix)[i][iCol] = Matrix->Z;
	}
	if(positive)
	{
		(*Matrix)[Matrix->GetLastRowIndex()][iCol] = Matrix->I;
	}
	else
	{
		(*Matrix)[Matrix->GetLastRowIndex()][iCol] = Matrix->N;
	}
}
template <typename T> void Add_LastArtificial(CMatrix_<T>* Matrix)
{
	(*Matrix)[0][Matrix->GetLastColumnIndex()] = Matrix->N;
	for ( int i = 1; i < Matrix->GetRowSize() - 1 ; i++ )
	{
		(*Matrix)[i][Matrix->GetLastColumnIndex()] = Matrix->Z;
	}
	(*Matrix)[Matrix->GetLastRowIndex()][Matrix->GetLastColumnIndex()] = Matrix->I;
	
}
template <typename T> void Add_Artificial(CMatrix_<T>* Matrix, UINT iCol)
{
	(*Matrix)[0][iCol] = Matrix->N;
	for ( int i = 1; i < Matrix->GetRowSize() - 1 ; i++ )
	{
		(*Matrix)[i][iCol] = Matrix->Z;
	}
	(*Matrix)[Matrix->GetLastRowIndex()][iCol] = Matrix->I;
	
}
template <typename T> void Turn_LastSlack(CMatrix_<T>* Matrix)
{
	UINT spoil = 0;
	Matrix->Column_IsCanonical(Matrix->GetLastColumnIndex(), 1, Matrix->GetLastRowIndex(), &spoil);
	(*Matrix)[spoil][Matrix->GetLastColumnIndex()] *= Matrix->N;
}
template <typename T> void Create_Branch_I(CMatrix_<T> original_tableau, CMatrix_<T> Slack_Matrix, CMatrix_<T> Artificial_Matrix, CMatrix_<T> b, CMatrix_<T> *Save_Matrix)
{
	Save_Matrix->Equals(original_tableau);
	Save_Matrix->Join_Right(&Slack_Matrix);
	Save_Matrix->Join_Right(&Artificial_Matrix);
	Save_Matrix->Join_Right(&b);
}
template <typename T> void Create_Branch_I(CMatrix_<T> original_tableau, CMatrix_<T> Slack_Matrix, CMatrix_<T> b, CMatrix_<T> *Save_Matrix)
{
	Save_Matrix->Equals(original_tableau);
	Save_Matrix->Join_Right(&Slack_Matrix);
	Save_Matrix->Join_Right(&b);
}
template <typename T> void Create_Branch_II(CMatrix_<T> original_tableau, CMatrix_<T> Slack_Matrix, CMatrix_<T> Artificial_Matrix, CMatrix_<T> b, CMatrix_<T> *Save_Matrix)
{
	CMatrix_<T> Artificial_Column(original_tableau.GetRowSize(), 1);
	Add_LastArtificial(&Artificial_Column);
	Save_Matrix->Equals(original_tableau);
	Save_Matrix->Join_Right(&Slack_Matrix);
	Save_Matrix->Join_Right(&Artificial_Matrix);
	Save_Matrix->Join_Right(&Artificial_Column);
	Save_Matrix->Join_Right(&b);
}
template <typename T> void Set_to_2Phases(CMatrix_<T> *mTableau, CMatrix_<T> *objective)
{
	UINT s = mTableau->GetColumnSize()-mTableau->GetRowSize();
	objective->Resize(1,mTableau->GetColumnSize());
	objective->SetValue(0,0,mTableau);
	for ( int i = 0; i <= s; i++ )
	{
		(*mTableau)[0][i] = mTableau->Z;
	}
}
