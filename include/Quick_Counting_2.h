#pragma once
#include "Common_.h"
#include "CArray_2.h"
#include "GenerarUCasilla.h"
#include "CFloat_.h"
#define NUMPASOS 1
typedef enum CHOSEN_FORMULA
{
	SMALL_FORMULA = 0,
	BIG_FORMULA = 1,
}FORMULA;

struct INTERVALOS
{
	CFPM_DOUBLE int_inf;
	CFPM_DOUBLE int_sup;
	CFPM_DOUBLE porcentaje;
};
inline double log2(double x)
{
  static const double d = 1.0/log(2.0);
  return log(x)*d;
}
UINT Bounded_Random(double A, double B)
{
	//Sleep(1);
	srand((UINT)time(0));
	double r = ((double)rand()/RAND_MAX);
	return (UINT)(((double)(B - A) * r) + A);
}
void Arrange_in_classes(CArray_< CArray_< UINT > >* booths_per_class, CArray_<casilla>* Booths, double* limit, int* class_number);
bool Check_Empty_Classes(CArray_< CArray_< UINT > > *Booth);
UINT Lineal_Congruence(int z, int c, int m);
double Chi_Square(UINT clases);
///////////////////////////////////////////////////
void Arrange_in_classes(CArray_< CArray_< UINT > >* clases, CArray_<UINT>* Casillas_vivas, double* limite, int* num_Clases);
bool Check_Empty_Classes2(CArray_< CArray_< UINT > > *Booth);
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV,CStringW* msg);
void Sample_mean(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas);
void Sample_Standard_Deviation(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* stdd_list,CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas);
void Confidence_Interval(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas);
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, int TClases, CArray_<UINT>* SimCV);
bool Full_Quick_CountingC(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula);
void Quick_Counting_NOCHI(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, int TClases, CArray_<UINT>* SimCV);
bool Quick_Counting_Higher(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV,CStringW* msg);
//////////////////////////////////////////////////////////
//FUNCIONES DE GENE
void Quick_Counting_()
{
	CArray_<UINT> casillas(1000);
	CArray_<CArray_<UINT>> clases_casillas;
	
	for ( UINT i = 0; i < casillas.GetSize(); i++ )
	{
		casillas[i] = i;
	}
	//Prep simulation at time T
	srand((UINT)time(0));
	UINT delete_booth = (UINT)(((double)rand()/(double)RAND_MAX)*1000);
	cout << " delete_booth " << delete_booth << endl;
	UINT fixed_delete = delete_booth;

	for ( UINT i = 0; i < fixed_delete; i++ )
	{
		delete_booth = (UINT)(((double)rand()/(double)RAND_MAX)*1000);
		casillas.Remove(delete_booth);
	}
	
	//Cycle?


	UINT CasillasVivas = casillas.GetSize();
	cout << " Casillas vivas " << CasillasVivas << endl;

	int NoClases = 0;
	double Altura = 0;
	NoClases = (int)ceil(log2((CasillasVivas)+1));
	cout << " NoClases " << NoClases << endl;
	if ( NoClases <= 4)
	{
		cout << "No hay datos suficientes para hacer el conteo " << endl;
		return;
	}
	double delimitador_clase = (double)casillas[casillas.GetSize()-1]/(double)NoClases; 
	cout << " delimitador clase " << delimitador_clase << endl;

	//Arrange Data in its respective Class
	for ( UINT i = 0; i < (UINT)NoClases; i++ )
	{
		clases_casillas.Add();
		for ( UINT j = 0; j < casillas.GetSize(); j++ )
		{
			if ((casillas[j] > (i)*delimitador_clase) && (casillas[j] < (i+1)*delimitador_clase))
			{
				clases_casillas[i].Add(casillas[j]);
			}
		}
		cout << " clases_casillas "<<i<<" size " << clases_casillas[i].GetSize() << endl;;
	}
	//Check for empty classes
	for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
	{
		if ( clases_casillas[i].GetSize() == 0 )
		{
			clases_casillas.Remove(i);
		}
	}


	//Mean for class height
	Altura = CasillasVivas / NoClases;
	//cout << " Altura " << Altura << endl;
	UINT rand_rem = 0;

	double chi_sum = 0;
	CArray_<long double> Chi_Square(6);
	Chi_Square[0] = 0.711;
	Chi_Square[1] = 1.145;
	Chi_Square[2] = 1.635;
	Chi_Square[3] = 2.167;
	Chi_Square[4] = 2.733;
	Chi_Square[5] = 3.325;

	
	for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
	{
		chi_sum += (pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura);
	}
	cout << " chi_suma  " << chi_sum << endl;
	cout << " chi_tabla " << Chi_Square[5] << endl;



	if ( chi_sum > Chi_Square[5] )
	{
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			if ( clases_casillas[i].GetSize() > Altura )
			{
				do
				{
					rand_rem = Bounded_Random(0,clases_casillas[i].GetSize());
					clases_casillas[i].Remove(rand_rem);	
				}while(clases_casillas[i].GetSize()>Altura);
			}
		}
	}
	else
	{
		return;
	}

	
	for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
	{
		cout << " Size " << clases_casillas[i].GetSize() << endl;
	}

	
	//Intercambiar... primero se encuentra CHI^2 y despues se recorta la altura de la clase

	//Modificar para que cambie el tama�o de clases
}


bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* total_casillas, FORMULA* Formula)
{
	UINT CasillasVivas = (*total_casillas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	//for ( UINT i = 0; i < 3; i ++ )
	/*
	Se debe hacer un arreglo de UINT que simule las casillas vivas. si se remueve una casilla de alguna clase 
	se debe remover del arreglo de UINT
	*/
	do
	{
		//Choosing the right formula
		if ( *Formula == SMALL_FORMULA )
		{
			NoClases = (int)ceil(log2(CasillasVivas)+1);
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			cout << "No hay datos suficientes para hacer el conteo " << endl;
			return false;
		}
		double delimitador_clase = (double)CasillasVivas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,Casillas,&delimitador_clase,&NoClases);
		
		//Check for empty classes
		/*
		Primero se debe quitar una clase, recalcular el nuevo ancho de clase y despu�s volver a revisar
		si hay clases vac�as(G)
		*/
		if (!Check_Empty_Classes(&clases_casillas))//falta reducir el n�mero de clases(G)
		{
			//recalcular delimitador_clase(G)
			do
			{
				Arrange_in_classes(&clases_casillas,Casillas,&delimitador_clase,&NoClases);
				if ( clases_casillas == 4 )
				{
					break;
				}
			}while(!Check_Empty_Classes(&clases_casillas));
		}
		if ( clases_casillas.GetSize() == 4 )
		{
			cout << " No hay informacion suficiente para el conteo " << endl;
			return false;
		}

		//Mean for class height
		Altura = CasillasVivas / NoClases;
		double chi_sum = 0;
		

		
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum += (pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura);
		}

		UINT rand_rem = 0;

		if ( chi_sum > Chi_Square((UINT)NoClases-1) )
		{
			for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
			{
				if ( clases_casillas[i].GetSize() > Altura )
				{
					do
					{
						//rand_rem = Bounded_Random(0,clases_casillas[i].GetSize());
						//rand_rem = Lineal_Congruence((i+1)*3,i,clases_casillas[i].GetSize()-1);
						rand_rem = Lineal_Congruence(i,Bounded_Random(rand_rem,clases_casillas[i].GetSize()-1),clases_casillas[i].GetSize()-1);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
				}
			}
		}
		else//agregar condicion chi_sum == chi_ant(G)
		{
			if ( chi_sum < Chi_Square((UINT)NoClases-1) )
			{
				return true;
			}
		}
		
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			s+= clases_casillas[i].GetSize() ;
		}	
		CasillasVivas = s;	
	}while(true);
}
void Arrange_in_classes(CArray_< CArray_< UINT > >* booths_per_class, CArray_<casilla>* Booths, double* limit, int* class_number)
{
	for ( UINT i = 0; i < (UINT)(*class_number); i++ )
	{
		booths_per_class->Add();
		for ( UINT j = 0; j < Booths->GetSize(); j++ )
		{
			if (( (*Booths)[j].ncasilla > (i)*(*limit) ) && ( (*Booths)[j].ncasilla < (i+1)*(*limit)))
			{
				(*booths_per_class)[i].Add((*Booths)[j].ncasilla);
			}
		}
	}
}
bool Check_Empty_Classes(CArray_< CArray_< UINT > > *Booth)
{
	for (UINT i = 0; i < Booth->GetSize(); i++ )
	{
		if ((*Booth)[i].GetSize() == 0 )
		{
			Booth->Remove(i);
		}
		if ( Booth->GetSize() < 4 )
		{
			return false;
		}
	}
	return true;
}
void Sample_mean(CArray_< casilla >* Sample, CArray_< double >* mean_list)
{
	/*Debe pedir el arreglo que simula las casillas vivas
	y en base a eso trabajar
	*/
	double pan_mean = 0;
	double prd_mean = 0;
	double psd_mean = 0;
	double pt_mean = 0;
	double pvem_mean = 0;
	double upc_mean = 0;
	double conv_mean = 0;
	double null_mean = 0;
	double total_votos = 0;
	
	mean_list->RemoveAll();

	for ( UINT i = 0; i < Sample->GetSize(); i++ )
	{
		conv_mean += (double)(*Sample)[i].CONV;
		null_mean += (double)(*Sample)[i].NULOS;
		pan_mean += (double)(*Sample)[i].PAN;
		prd_mean += (double)(*Sample)[i].PRD;
		psd_mean += (double)(*Sample)[i].PSD;
		pvem_mean += (double)(*Sample)[i].PVEM;
		pt_mean += (double)(*Sample)[i].PT;
		upc_mean += (double)(*Sample)[i].PRI;
		total_votos += (double)(*Sample)[i].total_votos;
	}
		
	mean_list->Add(pan_mean/total_votos);
	mean_list->Add(prd_mean/total_votos);
	mean_list->Add(psd_mean/total_votos);
	mean_list->Add(pt_mean/total_votos);
	mean_list->Add(pvem_mean/total_votos);
	mean_list->Add(upc_mean/total_votos);
	mean_list->Add(conv_mean/total_votos);
	mean_list->Add(null_mean/total_votos);


}
void Sample_Standard_Deviation(CArray_< casilla >* Sample, CArray_< double >* stdd_list)
{
	/*Debe pedir el arreglo que simula las casillas vivas
	y en base a eso trabajar
	*/
	//To fill the stdd_list list with the mean of each part first
	Sample_mean(Sample, stdd_list);
	//Then apply the Sample Std Deviation for further analysis
	double pan_stdd = 0;
	double prd_stdd = 0;
	double psd_stdd = 0;
	double pt_stdd = 0;
	double pvem_stdd = 0;
	double upc_stdd = 0;
	double conv_stdd = 0;
	double null_stdd = 0;
	double total_votos = 0;

	for ( UINT i = 0; i < Sample->GetSize(); i++ )
	{
		pan_stdd += pow( ((double)(*Sample)[i].PAN)/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[0], 2.0 );
		prd_stdd += pow( (double)(*Sample)[i].PRD/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[1], 2.0 );
		psd_stdd += pow( (double)(*Sample)[i].PSD/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[2], 2.0 );
		pt_stdd += pow( (double)(*Sample)[i].PT/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[3], 2.0 );
		pvem_stdd += pow( (double)(*Sample)[i].PVEM/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[4], 2.0 );
		upc_stdd += pow( (double)(*Sample)[i].PRI/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[5], 2.0 );
		conv_stdd += pow( (double)(*Sample)[i].CONV/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[6], 2.0 );
		null_stdd += pow( (double)(*Sample)[i].NULOS/((double)(*Sample)[i].total_votos) - (double)(*stdd_list)[7], 2.0 );
		total_votos += (*Sample)[i].total_votos;
	}
	stdd_list->RemoveAll();
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(pan_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(prd_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(psd_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(pt_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(pvem_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(upc_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(conv_stdd/(total_votos-1.0),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[stdd_list->GetSize()-1] = pow(null_stdd/(total_votos-1.0),1.0/2.0);
}
void Confidence_Interval(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals)
{
	CArray_<double> mean_list;
	CArray_<double> stdd_list;
	Sample_mean(Sample,&mean_list);
	Sample_Standard_Deviation(Sample,&stdd_list);
	
	Intervals->RemoveAll();
	for ( UINT i = 0; i < mean_list.GetSize(); i++ ) 
	{
		Intervals->Add();
		(*Intervals)[Intervals->GetSize()-1].int_inf = mean_list[i]-1.96*stdd_list[i];
		(*Intervals)[Intervals->GetSize()-1].int_sup = mean_list[i]+1.96*stdd_list[i];
		(*Intervals)[Intervals->GetSize()-1].porcentaje = mean_list[i];
	}

}

bool Full_Quick_Counting(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula,CStringW* msg,UINT corte)
{
	CArray_<casilla> Casillas;
	CArray_< CArray_< UINT > > SimCv;
	UINT Casillas_size = 0;
	WCHAR fname[255]={0};
	//GetCasillasp9(file_name,L"|",&Casillas,&Casillas_size);

	//GetCasillas(file_name,L"|",&Casillas,&Casillas_size);
	GetCasillasp2006(file_name,L"|",&Casillas,&Casillas_size);
    //Quick_Counting debe pedir adem�s el n�mero total de casillas(G)
	//Debe pedir un puntero a un array que simule casillas vivas(G)
	for(register UINT j = 0; j < NUMPASOS; ++j)
	{
		SimCv.Add();
		for(UINT i = 0; i < Casillas.GetSize();++i)
		{
			if(Casillas[i].total_votos > 0)
			{
				SimCv[j].Add();
				SimCv[j].GetLastValue() = i;
			}
		}
	}
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i){
		Sleep(10);
		if ( !Quick_Counting_(&Casillas,&Casillas_size,Casillas.GetSize(), Formula,&(SimCv[i]),msg) )
		{
			return false;
		}
	}
	/*
	
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i)
	{
		StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\CasillasVivas\\Corte%dCasillasVivas%d.txt",corte+1,i+1);
		Filecas.open(fname);
		for(register UINT j = 0; j < SimCv[i].GetSize(); ++j)
		{
			Filecas<<(SimCv[i])[j]<<endl;
		}
		Filecas.close();
	}*/
	Confidence_Interval(&Casillas,Intervals,&SimCv);
	//Casillas.RemoveAll();
	SimCv.RemoveAll();
	Casillas.RemoveAll();
	return true;
}
bool Full_Quick_CountingHigher(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula,CStringW* msg,UINT corte)
{
	CArray_<casilla> Casillas;
	CArray_< CArray_< UINT > > SimCv;
	UINT Casillas_size = 0;
	WCHAR fname[255]={0};
	//GetCasillasp9(file_name,L"|",&Casillas,&Casillas_size);

	//GetCasillas(file_name,L"|",&Casillas,&Casillas_size);
	GetCasillasp2006(file_name,L"|",&Casillas,&Casillas_size);
    //Quick_Counting debe pedir además el número total de casillas(G)
	//Debe pedir un puntero a un array que simule casillas vivas(G)
	for(register UINT j = 0; j < NUMPASOS; ++j)
	{
		SimCv.Add();
		for(UINT i = 0; i < Casillas.GetSize();++i)
		{
			if(Casillas[i].total_votos > 0)
			{
				SimCv[j].Add();
				SimCv[j].GetLastValue() = i;
			}
		}
	}
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i){
		Sleep(10);
		if ( !Quick_Counting_Higher(&Casillas,&Casillas_size,Casillas.GetSize(), Formula,&(SimCv[i]),msg) )
		{
			return false;
		}
	}
	/*
	
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i)
	{
		StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\CasillasVivas\\Corte%dCasillasVivas%d.txt",corte+1,i+1);
		Filecas.open(fname);
		for(register UINT j = 0; j < SimCv[i].GetSize(); ++j)
		{
			Filecas<<(SimCv[i])[j]<<endl;
		}
		Filecas.close();
	}*/
	Confidence_Interval(&Casillas,Intervals,&SimCv);
	//Casillas.RemoveAll();

	SimCv.RemoveAll();
	Casillas.RemoveAll();
	return true;
}
void funcionconfor(CArray_< CArray_< UINT > >* SimCv,CArray_<casilla>* Casillas)
{
	for(register UINT j = 0; j < NUMPASOS; ++j)
		{
			SimCv->Add();
			for(UINT i = 0; i < Casillas->GetSize();++i)
			{
				if((*Casillas)[i].total_votos > 0)
				{
					(*SimCv)[j].Add();
					(*SimCv)[j].GetLastValue() = i;
				}
			}
		}
}
bool Full_Quick_CountingC(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula,CStringW savefile_name)
{
	CArray_<casilla> Casillas;
	CArray_< CArray_< UINT > > SimCv;
	UINT Casillas_size = 0;
	ofstream Filecas;
	WCHAR fname[255]={0};
	//GetCasillasp9(file_name,L"|",&Casillas,&Casillas_size);

	//GetCasillas(file_name,L"|",&Casillas,&Casillas_size);
	GetCasillasp2006(file_name,L"|",&Casillas,&Casillas_size);
    
	int start, end;
	start = 1;//desde donde comenzar
	end = 200;//tan grande como se quiera probar 
	
	CArray_<INTERVALOS> IntervalsAux;
	bool fin = false;
	//StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\Intervalos.txt");
	Filecas.open(savefile_name);
	for(register int TClases =start; TClases < end; ++TClases)
	{
	
		funcionconfor(&SimCv,&Casillas);
		Quick_Counting_NOCHI(&Casillas,&Casillas_size,Casillas.GetSize(),10*TClases,&(SimCv[0]));
		Confidence_Interval(&Casillas,&IntervalsAux,&SimCv);
		
		Filecas<<endl<<"Intervalos para "<<10*TClases<<" clases"<<endl;
		cout<<"*******"<<endl;
		cout<<endl<<"Intervalos para "<<10*TClases<<" clases"<<endl;
		cout<<"*******"<<endl;

		for(register UINT j = 0; j < IntervalsAux.GetSize(); ++j)
		{
			Filecas<<IntervalsAux[j].porcentaje.GetValue()<<" "<<IntervalsAux[j].int_inf.GetValue()<<" "<<IntervalsAux[j].int_sup.GetValue()<<endl;
		}
		
		//Casillas.RemoveAll();
		SimCv.RemoveAll();
		IntervalsAux.RemoveAll();

	}
	SimCv.RemoveAll();
	IntervalsAux.RemoveAll();
	Filecas.close();
	Casillas.RemoveAll();
	return true;
}
UINT Lineal_Congruence(int z, int c, int m)
{
	return ((m-1)*z+c)%m;

}
double Chi_Square(UINT clases)
{
	//CArray_<long double> Chi(101);
	switch(clases)
	{
	case 0:
		return 1000;//0.00393;
		break;
	case 1:
		return 1000;//0.103;
		break;
	case 2:
		return 1000;//0.352;
		break;
	case 3:
		return 1000;//0.711;
		break;
	case 4:
		return 1000;//1.145;
		break;
	case 5:
		return 1000;//1.635;
		break;
	case 6:
		return 1000;//2.167;
		break;
	case 7:
		return 1000;//2.733;
		break;
	case 8:
		return 1000;//3.325;
		break;
	case 9:
		return 1000;//3.940;
		break;
	case 10:
		return 1000;//4.575;
		break;
	case 11:
		return 1000;//5.226;
		break;
	case 12:
		return 1000;//5.892;
		break;
	case 13:
		return 1000;//6.571;
		break;
	case 14:
		return 1000;//7.261;
		break;
	case 15:
		return 1000;//7.962;
		break;
	case 16:
		return 1000;//8.672;
		break;
	case 17:
		return 1000;//9.930;
		break;
	case 18:
		return 1000;//10.117;
		break;
	case 19:
		return 1000;//10.851;
		break;
	case 20:
		return 1000;//11.591;
		break;
	case 21:
		return 1000;//12.338;
		break;
	case 22:
		return 1000;//13.091;
		break;
	case 23:
		return 1000;//13.484;
		break;
	case 24:
		return 1000;//14.611;
		break;
	case 25:
		return 1000;//15.379;
		break;
	case 26:
		return 1000;//16.151;
		break;
	case 27:
		return 1000;//16.928;
		break;
	case 28:
		return 1000;//17.708;
		break;
	case 100:
		return 1000;//77.929;
		break;
	default:
		break;
	}
	
	
	
	
	
	if ( clases >= 29 && clases < 40)
	{
		return 1000;//18.493;
	}
	if ( clases >= 40 && clases < 50)
	{
		return 1000;//26.509;
	}
	if ( clases >= 50 && clases < 60)
	{
		return 1000;//34.764;
	}
	if ( clases >= 60 && clases < 70)
	{
		return 1000;//43.118;
	}
	if ( clases >= 70 && clases < 80)
	{
		return 1000;//51.739;
	}
	if ( clases >= 80 && clases < 90)
	{
		return 1000;//60.391;
	}
	if ( clases >= 90 && clases < 10000)
	{
		return 10000;//69.126;
	}
}
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV,CStringW* msg)
{
	UINT CasillasVivas = (*casillas_vivas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	double chi_sum = 0;
	double chi_sum_ant = 0;
	
	//for ( UINT i = 0; i < 3; i ++ )
	/*
	Se debe hacer un arreglo de UINT que simule las casillas vivas. si se remueve una casilla de alguna clase 
	se debe remover del arreglo de UINT
	*/
	do
	{
		CasillasVivas = SimCV->GetSize();
		//Choosing the right formula
		if ( *Formula == SMALL_FORMULA )
		{
			NoClases = (int)ceil(log2(CasillasVivas)+1);
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			*msg = L"No hay datos suficientes para hacer el conteo ";
			return false;
		}
		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
		
		//Check for empty classes
		/*
		Primero se debe quitar una clase, recalcular el nuevo ancho de clase y despu�s volver a revisar
		si hay clases vac�as(G)
		*/
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el n�mero de clases(G)
		{
			NoClases -= 1;
			delimitador_clase = (double)total_Casillas/(double)NoClases;
			clases_casillas.RemoveAll();
			Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
			if ( NoClases <= 4 )
			{
				*msg = L"No hay datos suficientes para hacer el conteo ";
				return false;
			}
		} //ESTO ELIMINA LAS CLASES VACIAS
		

		//Mean for class height
		Altura = CasillasVivas / NoClases;
		chi_sum = 0;
		

		
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum += (pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura);
		}
		if(chi_sum == chi_sum_ant)
		{
			WCHAR chistring[50]={0};
			StringCchPrintf(chistring,50*sizeof(WCHAR),L"%.15f,%.15f",chi_sum,chi_sum_ant);
			*msg = L"Chi = Chi anterior";
			*msg += chistring;
			free(chistring);
			return false;
		}

		UINT rand_rem = 0;
		UINT aux_rem;

		if ( chi_sum > Chi_Square((UINT)NoClases-1) )
		{
			chi_sum_ant = chi_sum;
			for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
			{
				if ( clases_casillas[i].GetSize() > Altura )
				{
					do
					{
						rand_rem = 0;
						//rand_rem = Bounded_Random(0,clases_casillas[i].GetSize());
						//rand_rem = Lineal_Congruence((i+1)*3,i,clases_casillas[i].GetSize()-1);
						rand_rem = Lineal_Congruence(i,Bounded_Random(rand_rem,clases_casillas[i].GetSize()-1),clases_casillas[i].GetSize()-1);
						SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
						SimCV->Remove(aux_rem);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
				}
			}
		}
		else//agregar condicion chi_sum == chi_ant(G)
		{
			/*ofstream f1;
			f1.open(L"C:\\resultadosCD.txt");
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<(*SimCV)[i]<<endl;
			}
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<"PRI:"<<(*Casillas)[(*SimCV)[i]].PRI<<" PAN:"<<(*Casillas)[(*SimCV)[i]].PAN<<" PRD:"<<(*Casillas)[(*SimCV)[i]].PRD<<" PT:"<<(*Casillas)[(*SimCV)[i]].PT<<endl;
			}*/
			return true;
		}
	}while(true);
}
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, int TClases, CArray_<UINT>* SimCV)
{
	UINT CasillasVivas = (*casillas_vivas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	double chi_sum = 0;
	double chi_sum_ant = 0;
	
	//for ( UINT i = 0; i < 3; i ++ )
	/*
	Se debe hacer un arreglo de UINT que simule las casillas vivas. si se remueve una casilla de alguna clase 
	se debe remover del arreglo de UINT
	*/
	do
	{
		CasillasVivas = SimCV->GetSize();
		//Choosing the right formula
		/*if ( *Formula == SMALL_FORMULA )
		{
			NoClases = (int)ceil(log2(CasillasVivas)+1);
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}*/
		NoClases = TClases;
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			//*msg = L"No hay datos suficientes para hacer el conteo ";
			return false;
		}
		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
		
		//Check for empty classes
		/*
		Primero se debe quitar una clase, recalcular el nuevo ancho de clase y después volver a revisar
		si hay clases vacías(G)
		*/
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el número de clases(G)
		{
			NoClases -= 1;
			delimitador_clase = (double)total_Casillas/(double)NoClases;
			clases_casillas.RemoveAll();
			Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
			if ( NoClases <= 4 )
			{
				//*msg = L"No hay datos suficientes para hacer el conteo ";
				return false;
			}
		}
		

		//Mean for class height
		Altura = CasillasVivas / NoClases;
		chi_sum = 0;
		

		
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum += (pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura);
		}
		if(chi_sum == chi_sum_ant)
		{
			//WCHAR chistring[50]={0};
			//StringCchPrintf(chistring,50*sizeof(WCHAR),L"%.15f,%.15f",chi_sum,chi_sum_ant);
			//*msg = L"Chi = Chi anterior";
			//*msg += chistring;
			//free(chistring);
			return false;
		}

		UINT rand_rem = 0;
		UINT aux_rem;

		if ( chi_sum > Chi_Square((UINT)NoClases-1) )
		{
			chi_sum_ant = chi_sum;
			for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
			{
				if ( clases_casillas[i].GetSize() > Altura )
				{
					do
					{
						rand_rem = 0;
						//rand_rem = Bounded_Random(0,clases_casillas[i].GetSize());
						//rand_rem = Lineal_Congruence((i+1)*3,i,clases_casillas[i].GetSize()-1);
						rand_rem = Lineal_Congruence(i,Bounded_Random(rand_rem,clases_casillas[i].GetSize()-1),clases_casillas[i].GetSize()-1);
						SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
						SimCV->Remove(aux_rem);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
				}
			}
		}
		else//agregar condicion chi_sum == chi_ant(G)
		{
			/*ofstream f1;
			f1.open(L"C:\\resultadosCD.txt");
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<(*SimCV)[i]<<endl;
			}
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<"PRI:"<<(*Casillas)[(*SimCV)[i]].PRI<<" PAN:"<<(*Casillas)[(*SimCV)[i]].PAN<<" PRD:"<<(*Casillas)[(*SimCV)[i]].PRD<<" PT:"<<(*Casillas)[(*SimCV)[i]].PT<<endl;
			}*/
			return true;
		}
	}while(true);
}
void Arrange_in_classes(CArray_< CArray_< UINT > >* clases, CArray_<UINT>* Casillas_vivas, double* limite, int* num_Clases)
{
	for ( UINT i = 0; i < (UINT)(*num_Clases); i++ )
	{
		clases->Add();
		for ( UINT j = 0; j < Casillas_vivas->GetSize(); j++ )
		{
			if ((*Casillas_vivas)[j] > (i)*(*limite) && ( (*Casillas_vivas)[j] < (i+1)*(*limite)))
			{
				(*clases)[i].Add((*Casillas_vivas)[j]);
			}
		}
	}
}
bool Check_Empty_Classes2(CArray_< CArray_< UINT > > *Booth)
{
	for (UINT i = 0; i < Booth->GetSize(); i++ )
	{
		if ((*Booth)[i].GetSize() == 0 )
		{
			return true;
		}
	}
	return false;
}
void Sample_mean(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas)
{
	/*Debe pedir el arreglo que simula las casillas vivas
	y en base a eso trabajar
	*/
	CArray_<CFPM_DOUBLE> pan_mean;
	CArray_<CFPM_DOUBLE> prd_mean;
	CArray_<CFPM_DOUBLE> panal_mean;
	CArray_<CFPM_DOUBLE> psd_mean;
	CArray_<CFPM_DOUBLE> pt_mean;
	CArray_<CFPM_DOUBLE> pvem_mean;
	CArray_<CFPM_DOUBLE> upc_mean;
	CArray_<CFPM_DOUBLE> conv_mean;
	CArray_<CFPM_DOUBLE> null_mean;
	CArray_<CFPM_DOUBLE> noreg_mean;
	CArray_<CFPM_DOUBLE> total_votos;
	
	mean_list->RemoveAll();

	for ( UINT j = 0; j < Listas->GetSize(); j++ )
	{
		conv_mean.Add();
		null_mean.Add();
		pan_mean.Add();
		prd_mean.Add();
		psd_mean.Add();
		pvem_mean.Add();
		pt_mean.Add();
		upc_mean.Add();
		panal_mean.Add();
		noreg_mean.Add();
		total_votos.Add();
		for ( UINT i = 0; i < (*Listas)[j].GetSize(); i++ )
		{
		conv_mean[j] = conv_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].CONV;
		null_mean[j] = null_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].NULOS;
		pan_mean[j] = pan_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PAN;
		prd_mean[j] = prd_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PRD;
		psd_mean[j] = psd_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PSD;
		pvem_mean[j] = pvem_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PVEM;
		pt_mean[j] = pt_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PT;
		upc_mean[j] = upc_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PRI;
		panal_mean[j] = panal_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].PANAL;
		noreg_mean[j] = noreg_mean[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].NOREG;
		total_votos[j] = total_votos[j].GetValue() + (double)(*Sample)[((*Listas)[j])[i]].total_votos;
		}
	}
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		mean_list->Add();
		(*mean_list)[0] = 0;
		(*mean_list)[1] = 0;
		(*mean_list)[2] = 0;
		(*mean_list)[3] = 0;
		(*mean_list)[4] = 0;
		(*mean_list)[5] = 0;
		(*mean_list)[6] = 0;
		(*mean_list)[7] = 0;
		(*mean_list)[8] = 0;
		(*mean_list)[9] = 0;
		CStringW msg;
	for ( UINT j = 0; j < total_votos.GetSize(); j++ )
	{
		(*mean_list)[0] = (*mean_list)[0] + (upc_mean[j]/total_votos[j]);
		(*mean_list)[1] = (*mean_list)[1] + (pan_mean[j]/total_votos[j]);
		(*mean_list)[2] = (*mean_list)[2] + (prd_mean[j]/total_votos[j]);
		(*mean_list)[3] = (*mean_list)[3] + (pt_mean[j]/total_votos[j]);
		(*mean_list)[4] = (*mean_list)[4] + (pvem_mean[j]/total_votos[j]);
		(*mean_list)[5] = (*mean_list)[5] + (psd_mean[j]/total_votos[j]);
		(*mean_list)[6] = (*mean_list)[6] + (conv_mean[j]/total_votos[j]);
		(*mean_list)[7] = (*mean_list)[7] + (panal_mean[j]/total_votos[j]);
		(*mean_list)[8] = (*mean_list)[8] + (noreg_mean[j]/total_votos[j]);
		(*mean_list)[9] = (*mean_list)[9] + (null_mean[j]/total_votos[j]);
	}
	(*mean_list)[0] /= double(total_votos.GetSize());
	(*mean_list)[1] /= double(total_votos.GetSize());
	(*mean_list)[2] /= double(total_votos.GetSize());
	(*mean_list)[3] /= double(total_votos.GetSize());
	(*mean_list)[4] /= double(total_votos.GetSize());
	(*mean_list)[5] /= double(total_votos.GetSize());
	(*mean_list)[6] /= double(total_votos.GetSize());
	(*mean_list)[7] /= double(total_votos.GetSize());
	(*mean_list)[8] /= double(total_votos.GetSize());
	(*mean_list)[9] /= double(total_votos.GetSize());
}
void Sample_Standard_Deviation(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* stdd_list,CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas)
{
	/*Debe pedir el arreglo que simula las casillas vivas
	y en base a eso trabajar
	*/
	//To fill the stdd_list list with the mean of each part first
	//Then apply the Sample Std Deviation for further analysis
	CFPM_DOUBLE pan_stdd = 0;
	CFPM_DOUBLE prd_stdd = 0;
	CFPM_DOUBLE psd_stdd = 0;
	CFPM_DOUBLE pt_stdd = 0;
	CFPM_DOUBLE pvem_stdd = 0;
	CFPM_DOUBLE upc_stdd = 0;
	CFPM_DOUBLE conv_stdd = 0;
	CFPM_DOUBLE panal_stdd = 0;
	CFPM_DOUBLE noreg_stdd = 0;
	CFPM_DOUBLE null_stdd = 0;
	CFPM_DOUBLE total_votos = 0;

	for ( UINT j = 0; j < Listas->GetSize(); j++ )
	{
		for ( UINT k = 0; k < (*Listas)[j].GetSize(); k++ )
		{
			upc_stdd = upc_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PRI/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[0], 2.0 );
			pan_stdd = pan_stdd.GetValue() + pow( ((double)(*Sample)[((*Listas)[j])[k]].PAN)/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[1], 2.0 );
			prd_stdd = prd_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PRD/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[2], 2.0 );
			pt_stdd = pt_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PT/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[3], 2.0 );
			pvem_stdd = pvem_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PVEM/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[4], 2.0 );
			psd_stdd = psd_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PSD/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[5], 2.0 );
			conv_stdd = conv_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].CONV/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[6], 2.0 );
			panal_stdd = panal_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].PANAL/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[7], 2.0 );
			noreg_stdd = noreg_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].NOREG/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[8], 2.0 );
			null_stdd = null_stdd.GetValue() + pow( (double)(*Sample)[((*Listas)[j])[k]].NULOS/((double)(*Sample)[((*Listas)[j])[k]].total_votos) - (double)(*mean_list)[9], 2.0 );
			total_votos = total_votos.GetValue() + (double)(*Sample)[((*Listas)[j])[k]].total_votos;
		}
	}
	stdd_list->RemoveAll();
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(upc_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(pan_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(prd_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(pt_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(pvem_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(psd_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(conv_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(panal_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(noreg_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = pow(null_stdd.GetValue()/(total_votos.GetValue()-1.0f),1.0/2.0);
}
void Confidence_Interval(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas)
{
	CArray_<CFPM_DOUBLE> mean_list;
	CArray_<CFPM_DOUBLE> stdd_list;
	Sample_mean(Sample,&mean_list,Listas);
	Sample_Standard_Deviation(Sample,&stdd_list,&mean_list,Listas);
	
	Intervals->RemoveAll();
	for ( UINT i = 0; i < mean_list.GetSize(); i++ ) 
	{
		Intervals->Add();
		(*Intervals)[i].int_inf = mean_list[i].GetValue() - 1.96*stdd_list[i].GetValue();
		(*Intervals)[i].int_sup = mean_list[i].GetValue() + 1.96*stdd_list[i].GetValue();
		(*Intervals)[i].porcentaje = mean_list[i].GetValue();
	}

}
void Quick_Counting_NOCHI(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, int TClases, CArray_<UINT>* SimCV)
{
	UINT CasillasVivas = (*casillas_vivas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	//for ( UINT i = 0; i < 3; i ++ )
	/*
	Se debe hacer un arreglo de UINT que simule las casillas vivas. si se remueve una casilla de alguna clase 
	se debe remover del arreglo de UINT
	*/
		CasillasVivas = SimCV->GetSize();
		NoClases = TClases;
		CArray_< CArray_<UINT> > clases_casillas;

		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
		
		
		//Mean for class height
		Altura = CasillasVivas / NoClases;
		UINT rand_rem = 0;
		UINT aux_rem;

		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			if ( clases_casillas[i].GetSize() > Altura )
			{
				do
				{
					rand_rem = 0;
					rand_rem = Lineal_Congruence(i,Bounded_Random(rand_rem,clases_casillas[i].GetSize()-1),clases_casillas[i].GetSize()-1);
					SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
					SimCV->Remove(aux_rem);
					clases_casillas[i].Remove(rand_rem);	
				}while(clases_casillas[i].GetSize()>Altura);
			}
		}
}
bool Quick_Counting_Higher(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV,CStringW* msg)
{
	UINT CasillasVivas = (*casillas_vivas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	double Altura2 = 0;
	UINT s = 0;	
	double chi_sum = 0;
	double chi_sum_ant = 0;
	
	//for ( UINT i = 0; i < 3; i ++ )
	/*
	Se debe hacer un arreglo de UINT que simule las casillas vivas. si se remueve una casilla de alguna clase 
	se debe remover del arreglo de UINT
	*/
	do
	{
		CasillasVivas = SimCV->GetSize();
		//Choosing the right formula
		if ( *Formula == SMALL_FORMULA )
		{
			NoClases = (int)ceil(log2(CasillasVivas)+1);
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			*msg = L"No hay datos suficientes para hacer el conteo ";
			return false;
		}
		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
		
		//Check for empty classes
		/*
		Primero se debe quitar una clase, recalcular el nuevo ancho de clase y despu�s volver a revisar
		si hay clases vac�as(G)
		*/
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el n�mero de clases(G)
		{
			NoClases -= 1;
			delimitador_clase = (double)total_Casillas/(double)NoClases;
			clases_casillas.RemoveAll();
			Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
			if ( NoClases <= 4 )
			{
				*msg = L"No hay datos suficientes para hacer el conteo ";
				return false;
			}
		} //ESTO ELIMINA LAS CLASES VACIAS
		

		//Mean for class height

		//Altura = CasillasVivas / NoClases;
		UINT i;
		Altura = 0;
		Altura2 = 0;
		for(i = 0; i < clases_casillas.GetSize();++i)
		{
			if(Altura < clases_casillas[i].GetSize())
				Altura = clases_casillas[i].GetSize();
		}

		for(i = 0; i < clases_casillas.GetSize();++i)
		{
			if(Altura2 < clases_casillas[i].GetSize())
				if(clases_casillas[i].GetSize() < Altura)
					Altura2 = clases_casillas[i].GetSize();
		}
		

		chi_sum = 0;
		for ( i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum += (pow((Altura2-(double)clases_casillas[i].GetSize()),2.0))/(Altura2);
		}
		if(chi_sum == chi_sum_ant)
		{
			WCHAR chistring[50]={0};
			StringCchPrintf(chistring,50*sizeof(WCHAR),L"%.15f,%.15f",chi_sum,chi_sum_ant);
			*msg = L"Chi = Chi anterior";
			*msg += chistring;
			free(chistring);
			return false;
		}

		UINT rand_rem = 0;
		UINT aux_rem;

		if ( chi_sum > Chi_Square((UINT)NoClases-1) )
		{
			chi_sum_ant = chi_sum;
			for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
			{
				if ( clases_casillas[i].GetSize() > Altura2 )
				{
					do
					{
						rand_rem = 0;
						//rand_rem = Bounded_Random(0,clases_casillas[i].GetSize());
						//rand_rem = Lineal_Congruence((i+1)*3,i,clases_casillas[i].GetSize()-1);
						rand_rem = Lineal_Congruence(i,Bounded_Random(rand_rem,clases_casillas[i].GetSize()-1),clases_casillas[i].GetSize()-1);
						SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
						SimCV->Remove(aux_rem);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
				}
			}
		}
		else//agregar condicion chi_sum == chi_ant(G)
		{
			/*ofstream f1;
			f1.open(L"C:\\resultadosCD.txt");
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<(*SimCV)[i]<<endl;
			}
			for(register UINT i = 0; i < SimCV->GetSize(); ++i)
			{
				f1<<"PRI:"<<(*Casillas)[(*SimCV)[i]].PRI<<" PAN:"<<(*Casillas)[(*SimCV)[i]].PAN<<" PRD:"<<(*Casillas)[(*SimCV)[i]].PRD<<" PT:"<<(*Casillas)[(*SimCV)[i]].PT<<endl;
			}*/
			return true;
		}
	}while(true);
}