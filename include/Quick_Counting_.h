#pragma once
#include "Common_.h"
#include "CArray_.h"
#include "GenerarUCasilla.h"
#include "CFloat_.h"
#define NUMPASOS 10
int PASOS;
float raiz;
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
	srand((UINT)time(0));
	double r = ((double)rand()/RAND_MAX);
	return (UINT)(((double)(B - A) * r) + A);
}
UINT Bounded_Random(UINT B)
{
	return rand()%B;
}
void Arrange_in_classes(CArray_< CArray_< UINT > >* booths_per_class, CArray_<casilla>* Booths, double* limit, int* class_number);
bool Check_Empty_Classes(CArray_< CArray_< UINT > > *Booth);
UINT Lineal_Congruence(int z, int c, int m);
double Chi_Square(UINT clases);
///////////////////////////////////////////////////
void Arrange_in_classes(CArray_< CArray_< UINT > >* clases, CArray_<UINT>* Casillas_vivas, double* limite, int* num_Clases);
void Arrange_in_classesDiputados(CArray_< CArray_< UINT > >* clases, CArray_<UINT>* Casillas_vivas, double* limite, int* num_Clases,CArray_<casilla>* Casillas);
bool Check_Empty_Classes2(CArray_< CArray_< UINT > > *Booth);
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV,CStringW* msg);
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV);
bool Quick_Counting_Diputados(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV);
void Sample_mean(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas);
void Sample_Standard_Deviation(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* stdd_list,CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas);
void Confidence_Interval(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas);
void Confidence_IntervalDiputados(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas);
bool Full_Quick_Counting(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula);
bool Full_Quick_CountingDiputados(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula);
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
	
	cout << " Altura " << Altura << endl;
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
					rand_rem = Bounded_Random(clases_casillas[i].GetSize());
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

	//Modificar para que cambie el tamaño de clases
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
			//NoClases = (int)ceil(log2(CasillasVivas)+1);
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
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
		Primero se debe quitar una clase, recalcular el nuevo ancho de clase y después volver a revisar
		si hay clases vacías(G)
		*/
		if (!Check_Empty_Classes(&clases_casillas))//falta reducir el número de clases(G)
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
						rand_rem = rand_rem = Bounded_Random(clases_casillas[i].GetSize());
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
	UINT totalvotosvivos;
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
		if ( !Quick_Counting_(&Casillas,&Casillas_size,Casillas.GetSize(), Formula,&(SimCv[i]),msg) )
		{
			return false;
		}
	}
	Confidence_Interval(&Casillas,Intervals,&SimCv);
	ofstream Filecas;
	WCHAR fname[255]={0};
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i)
	{
		StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\CasillasVivas\\Corte%dCasillasVivas%d.txt",corte+1,i+1);
		Filecas.open(fname);
		Filecas<<"Intervalos de confianza"<<endl;
		for(register UINT i = 0; i < Intervals->GetSize(); ++i)
		{
			Filecas<<(*Intervals)[i].porcentaje<<","<<(*Intervals)[i].int_inf<<","<<(*Intervals)[i].int_sup<<endl;
		}
		Filecas<<"clases vivas:"<<endl;
		for(register UINT j = 0; j < SimCv[i].GetSize(); ++j)
		{
			Filecas<<(SimCv[i])[j]<<endl;
		}
		Filecas.close();
		Filecas.clear();
		
	}
	
	Casillas.RemoveAll();
	SimCv.RemoveAll();
	return true;
}
bool Full_Quick_Counting(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula)
{
	srand((UINT)time(0));
	CArray_<casilla> Casillas;
	CArray_< CArray_< UINT > > SimCv;
	UINT Casillas_size = 0;
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
		if ( !Quick_Counting_(&Casillas,&Casillas_size,Casillas.GetSize(), Formula,&(SimCv[i])) )
		{
			return false;
		}
	}
	Confidence_Interval(&Casillas,Intervals,&SimCv);
	ofstream Filecas;
	WCHAR fname[255]={0};
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i)
	{
		StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\CasillasVivas\\CasillasVivas%d.txt",i+1);
		Filecas.open(fname);
		Filecas<<"Intervalos de confianza"<<endl;
		for(register UINT i = 0; i < Intervals->GetSize(); ++i)
		{
			Filecas<<(*Intervals)[i].porcentaje<<","<<(*Intervals)[i].int_inf<<","<<(*Intervals)[i].int_sup<<endl;
		}
		Filecas<<"clases vivas:"<<endl;
		for(register UINT j = 0; j < SimCv[i].GetSize(); ++j)
		{
			Filecas<<(SimCv[i])[j]<<endl;
		}
		Filecas.close();
		Filecas.clear();
		
	}
	
	Casillas.RemoveAll();
	SimCv.RemoveAll();
	return true;
}
bool Full_Quick_CountingDiputados(CStringW file_name, CArray_<INTERVALOS>* Intervals, FORMULA *Formula)
{
	srand((UINT)time(0));
	CArray_<casilla> Casillas;
	CArray_< CArray_< UINT > > SimCv;
	CArray_<UINT> Casillas_size;
	UINT estado;
	UINT distrito;
	estado = 0;
	distrito = 0;
	UINT casillasvsize;
	//GetCasillasp9(file_name,L"|",&Casillas,&Casillas_size);
	//GetCasillas(file_name,L"|",&Casillas,&Casillas_size);
	GetCasillascDistritoyEstado(file_name,L"|",&Casillas);
    //Quick_Counting debe pedir además el número total de casillas(G)
	//Debe pedir un puntero a un array que simule casillas vivas(G)
	for(register UINT j = 0; j < Casillas.GetSize(); ++j)
	{
		while(true)
		{
			if(estado != Casillas[j].estado || distrito != Casillas[j].distrito)
			{
				estado = Casillas[j].estado;
				distrito = Casillas[j].distrito;
				SimCv.Add();
			}
			SimCv[SimCv.GetSize() -1].Add(j);
			j++;
		}
	}
	LeerCasillasDistrito(&Casillas_size);
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i){
		Sleep(10);
		if ( !Quick_Counting_Diputados(&Casillas,&casillasvsize,Casillas_size[i], Formula,&(SimCv[i])) )
		{
			return false;
		}
	}
	Confidence_IntervalDiputados(&Casillas,Intervals,&SimCv);
	ofstream Filecas;
	WCHAR fname[255]={0};
	for(register UINT i = 0; i < SimCv.GetSize(); ++ i)
	{
		StringCchPrintf(fname,255*sizeof(WCHAR),L"C:\\CasillasVivas\\CasillasVivas%d.txt",i+1);
		Filecas.open(fname);
		Filecas<<"Intervalos de confianza"<<endl;
		for(register UINT i = 0; i < Intervals->GetSize(); ++i)
		{
			Filecas<<(*Intervals)[i].porcentaje<<","<<(*Intervals)[i].int_inf<<","<<(*Intervals)[i].int_sup<<endl;
		}
		Filecas<<"clases vivas:"<<endl;
		for(register UINT j = 0; j < SimCv[i].GetSize(); ++j)
		{
			Filecas<<(SimCv[i])[j]<<endl;
		}
		Filecas.close();
		Filecas.clear();
		
	}
	
	Casillas.RemoveAll();
	SimCv.RemoveAll();
	return true;
}
UINT Lineal_Congruence(int z, int c, int m)
{
	return ((m-1)*z+c)%m;

}
double Chi_Square(UINT clases)
{
	CArray_<long double> Chi(501);
	Chi[0]=1.57079633522479E-08;
	Chi[1]=0.00393214000053241;
	Chi[2]=0.00393214000053241;
	Chi[3]=0.102586588821673;
	Chi[4]=0.351846318405446;
	Chi[5]=0.710723022815976;
	Chi[6]=1.14547622756411;
	Chi[7]=1.63538289543692;
	Chi[8]=2.16734991862191;
	Chi[9]=2.73263679805379;
	Chi[10]=3.32511286376367;
	Chi[11]=3.94029914479582;
	Chi[12]=4.57481310792578;
	Chi[13]=5.22602949964436;
	Chi[14]=5.89186436818766;
	Chi[15]=6.57063145693823;
	Chi[16]=7.26094395538054;
	Chi[17]=7.96164563237117;
	Chi[18]=8.67176032514028;
	Chi[19]=9.39045512627611;
	Chi[20]=10.1170131499475;
	Chi[21]=10.850811548496;
	Chi[22]=11.5913052679392;
	Chi[23]=12.3380146806528;
	Chi[24]=13.0905143570209;
	Chi[25]=13.8484250929664;
	Chi[26]=14.6114077456671;
	Chi[27]=15.3791567495346;
	Chi[28]=16.1513961031099;
	Chi[29]=16.9278751458134;
	Chi[30]=17.7083663348919;
	Chi[31]=18.492661205064;
	Chi[32]=19.2805688799913;
	Chi[33]=20.0719135963331;
	Chi[34]=20.8665341783284;
	Chi[35]=21.664280975174;
	Chi[36]=22.4650155828339;
	Chi[37]=23.2686091712924;
	Chi[38]=24.0749427653717;
	Chi[39]=24.8839046652958;
	Chi[40]=25.6953907757982;
	Chi[41]=26.5093036928853;
	Chi[42]=27.3255516853696;
	Chi[43]=28.144049779757;
	Chi[44]=28.9647170380122;
	Chi[45]=29.787477555272;
	Chi[46]=30.6122593557674;
	Chi[47]=31.4389955369119;
	Chi[48]=32.267621874375;
	Chi[49]=33.0980778648586;
	Chi[50]=33.9303061646553;
	Chi[51]=34.7642519316135;
	Chi[52]=35.5998642491653;
	Chi[53]=36.4370936232161;
	Chi[54]=37.2758932780959;
	Chi[55]=38.1162186502034;
	Chi[56]=38.9580272443809;
	Chi[57]=39.8012779664127;
	Chi[58]=40.6459330379886;
	Chi[59]=41.4919549731578;
	Chi[60]=42.3393083310146;
	Chi[61]=43.1879591761042;
	Chi[62]=44.0378749904588;
	Chi[63]=44.88902397835;
	Chi[64]=45.7413773372217;
	Chi[65]=46.5949058151843;
	Chi[66]=47.4495818045603;
	Chi[67]=48.3053787620308;
	Chi[68]=49.1622711521039;
	Chi[69]=50.0202337315022;
	Chi[70]=50.879244045313;
	Chi[71]=51.739278708264;
	Chi[72]=52.6003157210907;
	Chi[73]=53.4623338609165;
	Chi[74]=54.3253126434554;
	Chi[75]=55.1892316034768;
	Chi[76]=56.0540728975333;
	Chi[77]=56.9198174569952;
	Chi[78]=57.7864474170326;
	Chi[79]=58.6539454941076;
	Chi[80]=59.5222949598554;
	Chi[81]=60.3914796164545;
	Chi[82]=61.2614829999208;
	Chi[83]=62.1322913495154;
	Chi[84]=63.0038892426084;
	Chi[85]=63.8762623827167;
	Chi[86]=64.7493968997385;
	Chi[87]=65.6232793323748;
	Chi[88]=66.4978966114714;
	Chi[89]=67.3732352099252;
	Chi[90]=68.2492843637813;
	Chi[91]=69.1260313455161;
	Chi[92]=70.0034645036843;
	Chi[93]=70.8815725075782;
	Chi[94]=71.7603443350124;
	Chi[95]=72.6397692606994;
	Chi[96]=73.5198359781178;
	Chi[97]=74.4005359602217;
	Chi[98]=75.281858529132;
	Chi[99]=76.1637940409574;
	Chi[100]=77.0463330982985;
	Chi[101]=77.9294665415248;
	Chi[102]=78.8131854404442;
	Chi[103]=79.697481086342;
	Chi[104]=80.5823440187202;
	Chi[105]=81.4677677818236;
	Chi[106]=82.3537434119224;
	Chi[107]=83.2402630128521;
	Chi[108]=84.1273188750263;
	Chi[109]=85.014903469326;
	Chi[110]=85.9030094412462;
	Chi[111]=86.791628660291;
	Chi[112]=87.6807559038772;
	Chi[113]=88.570383447127;
	Chi[114]=89.4605045796798;
	Chi[115]=90.3511127398999;
	Chi[116]=91.2422015103055;
	Chi[117]=92.1337646131781;
	Chi[118]=93.0257959063409;
	Chi[119]=93.9182893791019;
	Chi[120]=94.8112380708859;
	Chi[121]=95.7046382826751;
	Chi[122]=96.5984833857671;
	Chi[123]=97.4927678574425;
	Chi[124]=98.3874862883456;
	Chi[125]=99.2826333792626;
	Chi[126]=100.178203938003;
	Chi[127]=101.074192876407;
	Chi[128]=101.970594114125;
	Chi[129]=102.86740485921;
	Chi[130]=103.764619309162;
	Chi[131]=104.662232763702;
	Chi[132]=105.560240613092;
	Chi[133]=106.458638335714;
	Chi[134]=107.357421495735;
	Chi[135]=108.256585740841;
	Chi[136]=109.156126800061;
	Chi[137]=110.056039310786;
	Chi[138]=110.956321410702;
	Chi[139]=111.856967973482;
	Chi[140]=112.757975032825;
	Chi[141]=113.659338693883;
	Chi[142]=114.561055131473;
	Chi[143]=115.463120588344;
	Chi[144]=116.365531373505;
	Chi[145]=117.268283860603;
	Chi[146]=118.171373264516;
	Chi[147]=119.074798439812;
	Chi[148]=119.978554805189;
	Chi[149]=120.882638977367;
	Chi[150]=121.787047630302;
	Chi[151]=122.691777493847;
	Chi[152]=123.596825352444;
	Chi[153]=124.50218804386;
	Chi[154]=125.407862457954;
	Chi[155]=126.313845535491;
	Chi[156]=127.22013293527;
	Chi[157]=128.126724271313;
	Chi[158]=129.033615382269;
	Chi[159]=129.940803401064;
	Chi[160]=130.848285506061;
	Chi[161]=131.756058920057;
	Chi[162]=132.664120909312;
	Chi[163]=133.572468782602;
	Chi[164]=134.481099890299;
	Chi[165]=135.390010292497;
	Chi[166]=136.299199998411;
	Chi[167]=137.208665226302;
	Chi[168]=138.118403484066;
	Chi[169]=139.028412316962;
	Chi[170]=139.938689306834;
	Chi[171]=140.849232071351;
	Chi[172]=141.760038263267;
	Chi[173]=142.671105569713;
	Chi[174]=143.58243171148;
	Chi[175]=144.494014442356;
	Chi[176]=145.405850077651;
	Chi[177]=146.317939290703;
	Chi[178]=147.230278541459;
	Chi[179]=148.142865709036;
	Chi[180]=149.055698702379;
	Chi[181]=149.968775459686;
	Chi[182]=150.882093947827;
	Chi[183]=151.795652161803;
	Chi[184]=152.709448124194;
	Chi[185]=153.623479884637;
	Chi[186]=154.537744017634;
	Chi[187]=155.452241545736;
	Chi[188]=156.366969174277;
	Chi[189]=157.281925055959;
	Chi[190]=158.197107368087;
	Chi[191]=159.112514312109;
	Chi[192]=160.028144113172;
	Chi[193]=160.943995019686;
	Chi[194]=161.860065302907;
	Chi[195]=162.776353256513;
	Chi[196]=163.692857196207;
	Chi[197]=164.609573870446;
	Chi[198]=165.526504732938;
	Chi[199]=166.443646653321;
	Chi[200]=167.360998031405;
	Chi[201]=168.278557287133;
	Chi[202]=169.196322860224;
	Chi[203]=170.114293209834;
	Chi[204]=171.032466814214;
	Chi[205]=171.950842170384;
	Chi[206]=172.869417793809;
	Chi[207]=173.788192218088;
	Chi[208]=174.707162344204;
	Chi[209]=175.62632996091;
	Chi[210]=176.545692081731;
	Chi[211]=177.465247309672;
	Chi[212]=178.384994264392;
	Chi[213]=179.304931581928;
	Chi[214]=180.225057914423;
	Chi[215]=181.145371929865;
	Chi[216]=182.06587231182;
	Chi[217]=182.986557759191;
	Chi[218]=183.907426985955;
	Chi[219]=184.828477033332;
	Chi[220]=185.749709941317;
	Chi[221]=186.671122855705;
	Chi[222]=187.59271454833;
	Chi[223]=188.514483804938;
	Chi[224]=189.436429424967;
	Chi[225]=190.358550221329;
	Chi[226]=191.280845020208;
	Chi[227]=192.203312660849;
	Chi[228]=193.125951995354;
	Chi[229]=194.04876188848;
	Chi[230]=194.971741217458;
	Chi[231]=195.894887093946;
	Chi[232]=196.818201896981;
	Chi[233]=197.741682837631;
	Chi[234]=198.665328841042;
	Chi[235]=199.589138843902;
	Chi[236]=200.513111794285;
	Chi[237]=201.43724665146;
	Chi[238]=202.36154238574;
	Chi[239]=203.285997978321;
	Chi[240]=204.210612421103;
	Chi[241]=205.135384716556;
	Chi[242]=206.060313877554;
	Chi[243]=206.985397083804;
	Chi[244]=207.910636978431;
	Chi[245]=208.836030835521;
	Chi[246]=209.761577708018;
	Chi[247]=210.687276658542;
	Chi[248]=211.613126759248;
	Chi[249]=212.539127091704;
	Chi[250]=213.465276746741;
	Chi[251]=214.391574824331;
	Chi[252]=215.318020433466;
	Chi[253]=216.244612692015;
	Chi[254]=217.171350726616;
	Chi[255]=218.098233672553;
	Chi[256]=219.025258713361;
	Chi[257]=219.952428844441;
	Chi[258]=220.879741340864;
	Chi[259]=221.807195371247;
	Chi[260]=222.734790112274;
	Chi[261]=223.662524748587;
	Chi[262]=224.590398472679;
	Chi[263]=225.518410484786;
	Chi[264]=226.446559992787;
	Chi[265]=227.374846212099;
	Chi[266]=228.303268365581;
	Chi[267]=229.231825683437;
	Chi[268]=230.16051542543;
	Chi[269]=231.089340717;
	Chi[270]=232.018298904334;
	Chi[271]=232.947389245993;
	Chi[272]=233.876611007417;
	Chi[273]=234.805963460829;
	Chi[274]=235.735445885145;
	Chi[275]=236.665057565903;
	Chi[276]=237.594797795159;
	Chi[277]=238.524665871422;
	Chi[278]=239.454661099554;
	Chi[279]=240.384782790709;
	Chi[280]=241.31503026224;
	Chi[281]=242.245400791467;
	Chi[282]=243.17589772686;
	Chi[283]=244.106518428974;
	Chi[284]=245.037262239168;
	Chi[285]=245.968128504614;
	Chi[286]=246.899116578245;
	Chi[287]=247.830225818681;
	Chi[288]=248.761455590146;
	Chi[289]=249.692805262416;
	Chi[290]=250.624274210744;
	Chi[291]=251.555861815796;
	Chi[292]=252.48756746358;
	Chi[293]=253.419390545389;
	Chi[294]=254.351330457737;
	Chi[295]=255.28338443894;
	Chi[296]=256.215556148849;
	Chi[297]=257.14784290747;
	Chi[298]=258.080244131533;
	Chi[299]=259.012759242684;
	Chi[300]=259.945387667426;
	Chi[301]=260.878128837062;
	Chi[302]=261.810982187643;
	Chi[303]=262.743947159906;
	Chi[304]=263.677023199233;
	Chi[305]=264.610209755576;
	Chi[306]=265.543506283426;
	Chi[307]=266.476912241748;
	Chi[308]=267.410424908628;
	Chi[309]=268.344048051242;
	Chi[310]=269.277779025679;
	Chi[311]=270.211617308271;
	Chi[312]=271.145562379573;
	Chi[313]=272.079613724336;
	Chi[314]=273.013770831427;
	Chi[315]=273.948033193812;
	Chi[316]=274.882400308496;
	Chi[317]=275.816871676479;
	Chi[318]=276.751446802721;
	Chi[319]=277.686125196082;
	Chi[320]=278.620906369297;
	Chi[321]=279.555789838918;
	Chi[322]=280.490772869472;
	Chi[323]=281.425859426496;
	Chi[324]=282.361046850331;
	Chi[325]=283.296334672379;
	Chi[326]=284.231722427669;
	Chi[327]=285.167209654798;
	Chi[328]=286.102795895924;
	Chi[329]=287.038480696691;
	Chi[330]=287.974263606226;
	Chi[331]=288.910144177073;
	Chi[332]=289.846121965182;
	Chi[333]=290.782196529855;
	Chi[334]=291.718367433718;
	Chi[335]=292.654634242685;
	Chi[336]=293.590996525922;
	Chi[337]=294.527451483084;
	Chi[338]=295.464003364939;
	Chi[339]=296.400649446051;
	Chi[340]=297.337389308236;
	Chi[341]=298.274222536391;
	Chi[342]=299.211148718483;
	Chi[343]=300.148167445489;
	Chi[344]=301.085278311391;
	Chi[345]=302.022480913129;
	Chi[346]=302.959774850575;
	Chi[347]=303.89715972651;
	Chi[348]=304.834635146581;
	Chi[349]=305.772200719285;
	Chi[350]=306.709856055939;
	Chi[351]=307.647600770635;
	Chi[352]=308.585432013797;
	Chi[353]=309.523354268141;
	Chi[354]=310.461364757667;
	Chi[355]=311.399463107326;
	Chi[356]=312.337648944719;
	Chi[357]=313.275921900075;
	Chi[358]=314.21428160622;
	Chi[359]=315.152727698553;
	Chi[360]=316.091259815027;
	Chi[361]=317.029877596115;
	Chi[362]=317.968580684797;
	Chi[363]=318.907368726522;
	Chi[364]=319.846241369195;
	Chi[365]=320.785198263139;
	Chi[366]=321.7242390611;
	Chi[367]=322.663360880887;
	Chi[368]=323.602568385869;
	Chi[369]=324.541858765749;
	Chi[370]=325.481231682627;
	Chi[371]=326.420686800898;
	Chi[372]=327.360223787227;
	Chi[373]=328.299842310528;
	Chi[374]=329.239542041942;
	Chi[375]=330.179322654816;
	Chi[376]=331.119183824688;
	Chi[377]=332.05912522926;
	Chi[378]=332.999146548371;
	Chi[379]=333.939247463999;
	Chi[380]=334.87942766022;
	Chi[381]=335.819686823206;
	Chi[382]=336.760022054948;
	Chi[383]=337.700438150976;
	Chi[384]=338.640932283154;
	Chi[385]=339.58150414579;
	Chi[386]=340.52215343517;
	Chi[387]=341.462879849562;
	Chi[388]=342.40368308919;
	Chi[389]=343.344562856218;
	Chi[390]=344.285518854726;
	Chi[391]=345.226550790702;
	Chi[392]=346.167658372023;
	Chi[393]=347.108841308437;
	Chi[394]=348.050099311537;
	Chi[395]=348.991432094766;
	Chi[396]=349.932839373371;
	Chi[397]=350.874320864424;
	Chi[398]=351.815873606844;
	Chi[399]=352.757502614397;
	Chi[400]=353.699204994885;
	Chi[401]=354.640980472418;
	Chi[402]=355.582828772817;
	Chi[403]=356.524749623618;
	Chi[404]=357.46674275406;
	Chi[405]=358.408807895066;
	Chi[406]=359.350944779208;
	Chi[407]=360.293153140736;
	Chi[408]=361.235432715516;
	Chi[409]=362.177783241059;
	Chi[410]=363.12020445647;
	Chi[411]=364.062696102457;
	Chi[412]=365.005257921312;
	Chi[413]=365.947889656896;
	Chi[414]=366.890588303058;
	Chi[415]=367.833359044157;
	Chi[416]=368.776198941568;
	Chi[417]=369.719107745287;
	Chi[418]=370.662085206797;
	Chi[419]=371.605131079082;
	Chi[420]=372.548245116612;
	Chi[421]=373.491427075317;
	Chi[422]=374.434676712583;
	Chi[423]=375.377993787241;
	Chi[424]=376.321378059542;
	Chi[425]=377.264829291165;
	Chi[426]=378.208347245199;
	Chi[427]=379.151931686123;
	Chi[428]=380.0955823798;
	Chi[429]=381.039299093465;
	Chi[430]=381.983078793684;
	Chi[431]=382.926926790084;
	Chi[432]=383.870840115095;
	Chi[433]=384.814818541321;
	Chi[434]=385.758861842675;
	Chi[435]=386.702969794385;
	Chi[436]=387.647142172971;
	Chi[437]=388.591378756232;
	Chi[438]=389.535679323254;
	Chi[439]=390.480043654388;
	Chi[440]=391.42447153123;
	Chi[441]=392.368962736633;
	Chi[442]=393.313517054678;
	Chi[443]=394.258134270672;
	Chi[444]=395.202814171145;
	Chi[445]=396.147556543823;
	Chi[446]=397.092361177639;
	Chi[447]=398.037224967438;
	Chi[448]=398.982153431183;
	Chi[449]=399.927143528777;
	Chi[450]=400.872195053835;
	Chi[451]=401.817307801112;
	Chi[452]=402.7624815665;
	Chi[453]=403.707716147043;
	Chi[454]=404.653011340889;
	Chi[455]=405.598366947311;
	Chi[456]=406.543782766686;
	Chi[457]=407.489258600491;
	Chi[458]=408.434794251288;
	Chi[459]=409.380389522728;
	Chi[460]=410.326044219532;
	Chi[461]=411.27175814748;
	Chi[462]=412.217531113423;
	Chi[463]=413.163362925244;
	Chi[464]=414.109250424669;
	Chi[465]=415.055199293121;
	Chi[466]=416.001206436252;
	Chi[467]=416.947271666049;
	Chi[468]=417.893394795527;
	Chi[469]=418.839575638685;
	Chi[470]=419.785814010529;
	Chi[471]=420.732109727036;
	Chi[472]=421.678462605189;
	Chi[473]=422.624872462923;
	Chi[474]=423.571339119141;
	Chi[475]=424.517862393722;
	Chi[476]=425.464442107461;
	Chi[477]=426.411078082127;
	Chi[478]=427.35777014043;
	Chi[479]=428.304518105986;
	Chi[480]=429.25132180335;
	Chi[481]=430.198178039286;
	Chi[482]=431.145092615846;
	Chi[483]=432.092062402315;
	Chi[484]=433.03908722686;
	Chi[485]=433.986166918537;
	Chi[486]=434.933301307297;
	Chi[487]=435.880490223941;
	Chi[488]=436.827733500179;
	Chi[489]=437.775030968554;
	Chi[490]=438.722382462484;
	Chi[491]=439.669787816246;
	Chi[492]=440.617246864935;
	Chi[493]=441.564759444529;
	Chi[494]=442.512325391794;
	Chi[495]=443.459944544366;
	Chi[496]=444.407616740685;
	Chi[497]=445.355341820002;
	Chi[498]=446.30311962238;
	Chi[499]=447.250946877608;
	Chi[500]=448.198829588343;		
	return Chi[clases];
	
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
			//NoClases = (int)ceil(log2(CasillasVivas)+1);
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
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
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el n?mero de clases(G)
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
						rand_rem = Bounded_Random(clases_casillas[i].GetSize());
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
			PASOS = NoClases;
			return true;
		}
	}while(true);
}
bool Quick_Counting_(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV)
{
	UINT CasillasVivas = (*casillas_vivas);
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	CFPM_DOUBLE chi_sum = 0;
	CFPM_DOUBLE chi_sum_ant = 0;
	
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
			//NoClases = (int)ceil(log2(CasillasVivas)+1);
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			cout<<"No hay datos suficientes para hacer el conteo ";
			return false;
		}
		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el n?mero de clases(G)
		{
			NoClases -= 1;
			delimitador_clase = (double)total_Casillas/(double)NoClases;
			clases_casillas.RemoveAll();
			Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
			if ( NoClases <= 4 )
			{
				cout<<"No hay datos suficientes para hacer el conteo ";
				return false;
			}
		} //ESTO ELIMINA LAS CLASES VACIAS
		

		//Mean for class height
		Altura = CasillasVivas / NoClases;
		chi_sum = 0;			
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum = chi_sum + CFPM_DOUBLE((pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura));
		}
		if(chi_sum.GetValue() == chi_sum_ant.GetValue())
		{
			cout<<"Chi = Chi anterior";
			return false;
		}

		UINT rand_rem = 0;
		UINT aux_rem;

		if ( chi_sum.GetValue() > Chi_Square((UINT)NoClases) )
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
						rand_rem = Bounded_Random(clases_casillas[i].GetSize());;
						SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
						SimCV->Remove(aux_rem);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
					printf("Clase %d: Casillasvivas: %d\n",i,clases_casillas[i].GetSize());
				}
			}
			printf("tamaño de casillas total: %d\n",SimCV->GetSize());
			printf("chisum: %g Chicuadrada: %g",chi_sum.GetValue(),Chi_Square((UINT)NoClases));
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
			printf("chisum: %g Chicuadrada: %g\n",chi_sum.GetValue(),Chi_Square((UINT)NoClases));
			PASOS = NoClases;
			return true;
		}
	}while(true);
}
bool Quick_Counting_Diputados(CArray_<casilla>* Casillas, UINT* casillas_vivas, UINT total_Casillas, FORMULA* Formula, CArray_<UINT>* SimCV)
{
	UINT CasillasVivas = SimCV->GetSize();;
	int NoClases = 0;
	double Altura = 0;//Debe ser entera(G)
	UINT s = 0;	
	CFPM_DOUBLE chi_sum = 0;
	CFPM_DOUBLE chi_sum_ant = 0;
	
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
			//NoClases = (int)ceil(log2(CasillasVivas)+1);
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
		}
		else
		{
			NoClases = (int)ceil(1.2185*pow((double)CasillasVivas,0.4554));
			
		}
		CArray_< CArray_<UINT> > clases_casillas;
		if ( NoClases <= 4)
		{
			cout<<"No hay datos suficientes para hacer el conteo ";
			return false;
		}
		double delimitador_clase = (double)total_Casillas/(double)NoClases;//casillas_totales / NoClases(G) 
		
		//Arrange Data in its respective Class
		Arrange_in_classesDiputados(&clases_casillas,SimCV,&delimitador_clase,&NoClases,Casillas);
		while (Check_Empty_Classes2(&clases_casillas))//falta reducir el n?mero de clases(G)
		{
			NoClases -= 1;
			delimitador_clase = (double)total_Casillas/(double)NoClases;
			clases_casillas.RemoveAll();
			Arrange_in_classes(&clases_casillas,SimCV,&delimitador_clase,&NoClases);
			if ( NoClases <= 4 )
			{
				cout<<"No hay datos suficientes para hacer el conteo ";
				return false;
			}
		} //ESTO ELIMINA LAS CLASES VACIAS
		

		//Mean for class height
		Altura = CasillasVivas / NoClases;
		chi_sum = 0;			
		for ( UINT i = 0; i < clases_casillas.GetSize(); i++ )
		{
			chi_sum = chi_sum + CFPM_DOUBLE((pow((Altura-(double)clases_casillas[i].GetSize()),2.0))/(Altura));
		}
		if(chi_sum.GetValue() == chi_sum_ant.GetValue())
		{
			cout<<"Chi = Chi anterior";
			return false;
		}

		UINT rand_rem = 0;
		UINT aux_rem;

		if ( chi_sum.GetValue() > Chi_Square((UINT)NoClases) )
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
						rand_rem = Bounded_Random(clases_casillas[i].GetSize());;
						SimCV->FindFirst((clases_casillas[i])[rand_rem],&aux_rem);
						SimCV->Remove(aux_rem);
						clases_casillas[i].Remove(rand_rem);	
					}while(clases_casillas[i].GetSize()>Altura);
					printf("Clase %d: Casillasvivas: %d\n",i,clases_casillas[i].GetSize());
				}
			}
			printf("tamaño de casillas total: %d\n",SimCV->GetSize());
			printf("chisum: %g Chicuadrada: %g",chi_sum.GetValue(),Chi_Square((UINT)NoClases));
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
			printf("chisum: %g Chicuadrada: %g\n",chi_sum.GetValue(),Chi_Square((UINT)NoClases));
			PASOS = NoClases;
			return true;
		}
	}while(true);
}
void Arrange_in_classesDiputados(CArray_< CArray_< UINT > >* clases, CArray_<UINT>* Casillas_vivas, double* limite, int* num_Clases,CArray_<casilla>* Casillas)
{
	for ( UINT i = 0; i < (UINT)(*num_Clases); i++ )
	{
		clases->Add();
		for ( UINT j = 0; j < Casillas_vivas->GetSize(); j++ )
		{
			if ((*Casillas)[(*Casillas_vivas)[j]].ncasilla > (i)*(*limite) && ( (*Casillas_vivas)[j] < (i+1)*(*limite)))
			{
				(*clases)[i].Add((*Casillas_vivas)[j]);
			}
		}
	}
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
	(*mean_list)[0] /= double(Listas->GetSize());
	(*mean_list)[1] /= double(Listas->GetSize());
	(*mean_list)[2] /= double(Listas->GetSize());
	(*mean_list)[3] /= double(Listas->GetSize());
	(*mean_list)[4] /= double(Listas->GetSize());
	(*mean_list)[5] /= double(Listas->GetSize());
	(*mean_list)[6] /= double(Listas->GetSize());
	(*mean_list)[7] /= double(Listas->GetSize());
	(*mean_list)[8] /= double(Listas->GetSize());
	(*mean_list)[9] /= double(Listas->GetSize());
}
void Sample_meanDiputados(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas)
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
			for(UINT k = 0; k <10;k++)
			{
				mean_list->Add();
			}
			(*mean_list)[mean_list->GetSize()-1] = null_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-2] = noreg_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-3] = panal_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-4] = conv_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-5] = psd_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-6] = pvem_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-7] = pt_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-8] = prd_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-9] = pan_mean[j]/total_votos[j];
			(*mean_list)[mean_list->GetSize()-10] = upc_mean[j]/total_votos[j];
		}
	}
}
void Sample_Standard_Deviation(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* stdd_list,CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas)//este tambièn
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
	stdd_list->RemoveAll();
	CArray_< CFPM_DOUBLE > stdd_listaux;
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
		}
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(upc_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(pan_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(prd_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(pt_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(pvem_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(psd_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(conv_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(panal_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(noreg_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
		stdd_listaux.Add();
		stdd_listaux[(stdd_list->GetSize())-1] = sqrt(null_stdd.GetValue()/(double)(((*Listas)[j]).GetSize()-1));///(total_votos.GetValue()));
	}
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	stdd_list->Add();
	(*stdd_list)[(stdd_list->GetSize())-1] = 0.0f;
	for ( UINT j = 0; j < Listas->GetSize(); j++ )
	{
		(*stdd_list)[(stdd_list->GetSize())-10] += stdd_listaux[j*10 + 0];
		(*stdd_list)[(stdd_list->GetSize())-9] += stdd_listaux[j*10 + 1];
		(*stdd_list)[(stdd_list->GetSize())-8] += stdd_listaux[j*10 + 2];
		(*stdd_list)[(stdd_list->GetSize())-7] += stdd_listaux[j*10 + 3];
		(*stdd_list)[(stdd_list->GetSize())-6] += stdd_listaux[j*10 + 4];
		(*stdd_list)[(stdd_list->GetSize())-5] += stdd_listaux[j*10 + 5];
		(*stdd_list)[(stdd_list->GetSize())-4] += stdd_listaux[j*10 + 6];
		(*stdd_list)[(stdd_list->GetSize())-3] += stdd_listaux[j*10 + 7];
		(*stdd_list)[(stdd_list->GetSize())-2] += stdd_listaux[j*10 + 8];
		(*stdd_list)[(stdd_list->GetSize())-1] += stdd_listaux[j*10 + 9];
	}
	(*stdd_list)[(stdd_list->GetSize())-10] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-9] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-8] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-7] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-6] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-5] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-4] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-3] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-2] /= Listas->GetSize();
	(*stdd_list)[(stdd_list->GetSize())-1] /= Listas->GetSize();
}
void Sample_Standard_DeviationDiputados(CArray_< casilla >* Sample, CArray_< CFPM_DOUBLE >* stdd_list,CArray_< CFPM_DOUBLE >* mean_list,CArray_< CArray_< UINT > >* Listas)//este tambièn
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
	stdd_list->RemoveAll();
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
		}
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(upc_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(pan_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(prd_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(pt_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(pvem_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(psd_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(conv_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(panal_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(noreg_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));
			stdd_list->Add();
			(*stdd_list)[(stdd_list->GetSize())-1] = sqrt(null_stdd.GetValue()/(double)((*Listas)[j]).GetSize());///(total_votos.GetValue()));

	}
}
void Confidence_Interval(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas)//este es
{
	CArray_<CFPM_DOUBLE> mean_list;
	CArray_<CFPM_DOUBLE> stdd_list;
	Sample_mean(Sample,&mean_list,Listas);
	Sample_Standard_Deviation(Sample,&stdd_list,&mean_list,Listas);
	
	Intervals->RemoveAll();
	raiz=sqrtf(float(/*(*Listas)[0].GetSize()));//*/PASOS));
	for ( UINT i = 0; i < mean_list.GetSize(); i++ ) 
	{
		Intervals->Add();
		(*Intervals)[i].int_inf = mean_list[i].GetValue() - (1.96*stdd_list[i].GetValue());
		(*Intervals)[i].int_sup = mean_list[i].GetValue() + (1.96*stdd_list[i].GetValue());
		(*Intervals)[i].porcentaje = mean_list[i].GetValue();
	}

}
void Confidence_IntervalDiputados(CArray_< casilla >* Sample, CArray_<INTERVALOS>* Intervals,CArray_< CArray_< UINT > >* Listas)
{
	CArray_<CFPM_DOUBLE> mean_list;
	CArray_<CFPM_DOUBLE> stdd_list;
	Sample_meanDiputados(Sample,&mean_list,Listas);
	Sample_Standard_DeviationDiputados(Sample,&stdd_list,&mean_list,Listas);
	
	Intervals->RemoveAll();
	raiz=sqrtf(float(/*(*Listas)[0].GetSize()));//*/PASOS));
	for ( UINT i = 0; i < mean_list.GetSize(); i++ ) 
	{
		Intervals->Add();
		(*Intervals)[i].int_inf = mean_list[i].GetValue() - (1.96*stdd_list[i].GetValue());
		(*Intervals)[i].int_sup = mean_list[i].GetValue() + (1.96*stdd_list[i].GetValue());
		(*Intervals)[i].porcentaje = mean_list[i].GetValue();
	}
}