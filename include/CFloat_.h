#include "Common_.h"
#include "CMatrix_.h"

#ifndef CFLOAT_H
#define CFLOAT_H

template < typename T > class CFloat_PManager_  
{
public:

	CFloat_PManager_();

	CFloat_PManager_(T val);

	CFloat_PManager_(T val, T tolerance);
	
	bool IsZero();
	bool IsInteger();
	bool Equals(CFloat_PManager_<T> eq);

	void SetValue(T val);
	T GetValue();

	void SetInteger(T val);
	T GetInteger();

	void SetFloat(T val);
	T GetFloat();

	void Print();

	template < typename T > CFloat_PManager_ floor(CFloat_PManager_<T> fl);
	template < typename T > void ToIntegerMatrix(CMatrix_<T>* iMatrix, T* value, bool to_i = true);
			
	CFloat_PManager_ operator + (/*const*/ CFloat_PManager_& obj)
	{
		CFloat_PManager_<T> fa;
		
		IsZero();
		IsInteger();
		obj.IsZero();
		obj.IsInteger();
		
		fa.m_value = m_value + obj.m_value;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = min(m_tolerance, obj.m_tolerance);

		fa.IsZero();
		fa.IsInteger();

		return fa;
	}

	CFloat_PManager_ operator + (const long double obj)
	{
		CFloat_PManager_<T> fa;
		
		IsZero();
		IsInteger();
		
		fa.m_value = m_value + obj;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = m_tolerance;

		fa.IsZero();
		fa.IsInteger();

		return fa;
	}


	CFloat_PManager_ operator - (/*const*/ CFloat_PManager_& obj)
	{
		CFloat_PManager_<T> fa;

		IsZero();
		IsInteger();
		obj.IsZero();
		obj.IsInteger();

		fa.m_value = m_value - obj.m_value;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = min(m_tolerance, obj.m_tolerance);

		fa.IsZero();
		fa.IsInteger();

		return fa;
	}
	CFloat_PManager_ operator - (const int& obj)
	{
		CFloat_PManager_<T> fa;

		IsZero();
		IsInteger();

		fa.m_value = m_value - obj;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = m_tolerance;

		fa.IsZero();
		fa.IsInteger();

		return fa;
	}
	CFloat_PManager_ operator * (/*const*/ CFloat_PManager_& obj)
	{
		CFloat_PManager_<T> fa;
	
		IsZero();
		IsInteger();
		obj.IsZero();
		obj.IsInteger();

		fa.m_value = m_value * obj.m_value;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = min(m_tolerance, obj.m_tolerance);

		fa.IsZero();
		fa.IsInteger();

		return fa;
	} 	
	CFloat_PManager_ operator * (/*const*/ double obj)
	{
		CFloat_PManager_<T> fa;
	
		IsZero();
		IsInteger();

		fa.m_value = m_value * obj;
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = m_tolerance;

		fa.IsZero();
		fa.IsInteger();

		return fa;
	} 



	/*const*/ CFloat_PManager_  operator / (/*const*/ CFloat_PManager_& obj) 
	{
		assert(obj.m_value != 0);
		CFloat_PManager_<T> fa = *this;

		IsZero();
		IsInteger();
		obj.IsZero();
		obj.IsInteger();

		fa.m_value = long double(m_value) / long double(obj.m_value);
		fa.m_frac = modf(fa.m_value, &fa.m_int);
		fa.m_tolerance = min(m_tolerance, obj.m_tolerance);

		fa.IsZero();
		fa.IsInteger();

		return fa;
	}

	CFloat_PManager_ operator += (/*const*/ CFloat_PManager_& obj)
	{
		this->m_value += obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));
		
		this->IsZero();
		this->IsInteger();

		return *this;
	}
	CFloat_PManager_ operator -= (const CFloat_PManager_& obj)
	{
		this->m_value -= obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));
		
		this->IsZero();
		this->IsInteger();

		return *this;
	}
	CFloat_PManager_ operator *= (const CFloat_PManager_& obj)
	{
		this->m_value *= obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));

		this->IsZero();
		this->IsInteger();

		return *this;
	}

	CFloat_PManager_ operator /= (const CFloat_PManager_& obj)
	{
		assert(obj.m_value != T(0));

		this->m_value /= obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));

		this->IsZero();
		this->IsInteger();

		return *this;
	}

	CFloat_PManager_ operator = (/*const*/ CFloat_PManager_& obj)
	{
		this->m_value = obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));
		this->m_tolerance = obj.m_tolerance;

		this->IsZero();
		this->IsInteger();		

		return *this;		
	}
	
	CFloat_PManager_ operator = (const CFloat_PManager_& obj)
	{
		this->m_value = obj.m_value;
		this->m_frac = modf(this->m_value, &(this->m_int));
		this->m_tolerance = obj.m_tolerance;

		this->IsZero();
		this->IsInteger();		

		return *this;		
	}

	
	friend ostream& operator << (ostream &out, CFloat_PManager_<T>& obj)
	{
		out << obj.m_value;
		return out;
	}

	friend istream& operator >> (istream &in, CFloat_PManager_<T>& obj)
	{
		in >> obj.m_value;
		return in;
	}

	bool operator < (/*const*/ CFloat_PManager_& obj)
	{
		IsZero();
		IsInteger();
		obj.IsZero();
		obj.IsInteger();

		if ( this->m_value < obj.m_value )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator <= (const CFloat_PManager_& obj)
	{
		IsZero();
		IsInteger();

		if ( this->m_value <= obj.m_value )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator > (const CFloat_PManager_& obj)
	{
		IsZero();
		IsInteger();
		if ( this->m_value > obj.m_value )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator >= (const CFloat_PManager_& obj)
	{
		IsZero();
		IsInteger();
		if ( this->m_value >= obj.m_value )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator == (const CFloat_PManager_& obj)
	{
		if (this->m_value == obj.m_value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator != (const CFloat_PManager_& obj)
	{
		if (this->m_value != obj.m_value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	operator int() 
	{
		return m_int;
	}
	operator float()
	{
		return (float)m_value;
	}
	operator double()
	{
		return (double)m_value;
	}
	operator long double()
	{
		return (long double)m_value;
	}
	

protected:

	T m_value;
	T m_int;	
	T m_frac;
	T m_tolerance;

};


typedef CFloat_PManager_<long double> CFPM_LONGDOUBLE;
typedef CFloat_PManager_<double> CFPM_DOUBLE;
typedef CFloat_PManager_<float>	CFPM_FLOAT;

template < typename T > CFloat_PManager_<T>::CFloat_PManager_()
{
	m_value = 0;
	m_int = 0;
	m_frac = 0;
	m_tolerance = 0;
}
template < typename T > CFloat_PManager_<T>::CFloat_PManager_(T val)
{
	m_value = val;
	m_frac = modf(val,&m_int);
	
	if ( typeid(T) == typeid(long double) )
	{
	m_tolerance = DBL_EPSILON;
	}
	if ( typeid(T) == typeid(float) )
	{
	m_tolerance = FLT_EPSILON;
	}
	if ( typeid(T) == typeid(double) )
	{
	m_tolerance = DBL_EPSILON;
	}	
}
template < typename T > CFloat_PManager_<T>::CFloat_PManager_(T val, T tolerance)
{
	m_value = val;
	m_frac = modf(val,&m_int);
	m_tolerance = tolerance;
}
template < typename T > void CFloat_PManager_<T>::SetValue(T val)
{
	m_value = val;
	m_frac = modf(val,&m_int);
}
template < typename T > T CFloat_PManager_<T>::GetValue()
{
	return m_value;
}
template < typename T > void CFloat_PManager_<T>::SetInteger(T val)
{
	m_value = val + m_frac;
	m_frac = modf(val,&m_int);
}
template < typename T > T CFloat_PManager_<T>::GetInteger()
{
	return m_int;
}
template < typename T > void CFloat_PManager_<T>::SetFloat(T val)
{
	if ( val < 1 )
	{
		m_frac = val;
	}
}
template < typename T > T CFloat_PManager_<T>::GetFloat()
{
	return m_frac;
}

template < typename T > bool CFloat_PManager_<T>::IsZero()
{
	int fp = _fpclass(m_frac);
	if( m_int == 0 && m_frac != 0 )
	{
		if ( fp == _FPCLASS_NZ || fp == _FPCLASS_PZ )
		{
			SetValue(0);			
			return true;
		}
		if ( fp == _FPCLASS_NN || fp == _FPCLASS_PN )
		{
			if ( typeid(T) == typeid(double) || typeid(T) == typeid (long double) )
			{
				//if ( fabs(m_frac) < /*(9*pow(10.0,-14.0))*/(9*pow(10.0,-13.0)) )
				if ( fabs(m_frac) < /*(9*pow(10.0,-14.0))*/(9*pow(10.0,-12.0)) )
				{
					SetValue(0);
					return true;
				}
			}
			if ( typeid(T) == typeid(float) )
			{
				if ( fabs(m_frac) < (9*pow(10.0,-6.0)) )
				{
					SetValue(0);
					return true;
				}
			}
			return false;
		}
	}
	if ( m_int == 0 && m_frac == 0 )
	{
		SetValue(0);
		return true;
	}
	return false;
}
template < typename T > bool CFloat_PManager_<T>::IsInteger()
{
	if ( typeid(T) == typeid(long double) || typeid(T) == typeid (double) )
	{
		if ( m_frac != T(0))
		{
			if ( fabs(long double(m_frac)) < pow(10.0,-12.0) )
			{
				SetValue(m_int);
				return true;
			}
			if ( (fabs(long double(m_frac)) + pow(10.0,-12.0)) >= 1  )
			{
				if ( m_frac < 0 || m_int < 0)
				{
					SetValue(m_int - 1);
				}
				else
				{
					SetValue(m_int + 1);
				}
				return true;
			}
		}
	}
	if ( typeid(T) == typeid(float) )
	{
		if ( m_frac != 0 )
		{
			if ( fabs(m_frac) < pow(10.0,-6.0) )
			{
				SetValue(m_int);
				return true;
			}
			if ( (fabs(m_frac) + pow(10.0,-6.0)) >= 1  )
			{
				SetValue(m_int + 1);
				return true;
			}
		}
	}	
	return false;
}
template < typename T > bool CFloat_PManager_<T>::Equals(CFloat_PManager_<T> eq)
{
	if ( fabs(m_value - eq.m_value) < min(m_tolerance,eq.m_tolerance) )
	{
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > void CFloat_PManager_<T>::Print()
{
	printf(" GetValue() \t%0.32lf \n", GetValue());
	printf(" GetFloat() \t%0.32lf \n", GetFloat());
}

template < typename T > void CFloat_PManager_GCD( T a, T b, T *gcd, int *m, int *n)
{
	CFloat_PManager_<T> a1,b1,w=1,z,x,y,x1=0,y1=1,x0=1,y0=0,menos=-1;
	//int w=1;
	bool pasa=true,cambio,cambioA=false,cambioB=false;
	//int w=1,z,x,y,xl=0,yl=1,xo=1,yo=0;
	//CFloat_PManager_<T> w=CFloat_PManager_<T>(1);
	if(a<0)
	{
		a=-a;
		cambioA=true;
	}
    if(b<0)
	{
		b=-b;
		cambioB=true;
	}
	if(a!=0 && b!=0)
	{
		
		if(a>b)
		{
			a1.SetValue(a);
			b1.SetValue(b);
			cambio=true;
		}
		else 
		{
			if(b>a)	
			{
				a1.SetValue(b);
				b1.SetValue(a);
				cambio=false;
			}
			else
			{

				pasa=false;
			}
		}
		if(pasa)
		{
			while(w.GetValue()!=0)
			{
				/*a1.Print();
				b1.Print();*/
				z.SetInteger((a1/b1).GetInteger());
				/*cout<<"z=";
				z.Print();
				(a1/b1).Print();*/
				w.SetValue((((a1/b1).GetValue()-(a1/b1).GetInteger()))*b1.GetValue());
				//cout<<"w="<<w<<endl;
				x = ((z * x1)* menos) + x0;
				/*cout<<z<<" "<<x1<<" "<<x0<<" "<<menos<<endl;
				cout<<x<<endl;*/
				y =	((z * y1)* menos) + y0;
				/*cout<<z<<" "<<y1<<" "<<y0<<" "<<menos<<endl;
				cout<<y<<endl;*/

				x0=x1;
				y0=y1;
				x1=x;
				y1=y;
				a1=b1;
				b1=w;
				
			 }
			*gcd = a1.GetValue();
			if(cambio)
			{
				if(cambioA)
				{
					(*m)=-x0.GetInteger();					
				}
				else
				{
					(*m)=x0.GetInteger();
				}
				if(cambioB)
				{
					(*n)=-y0.GetInteger();
				}
				else
				{
					(*n)=y0.GetInteger();
				}
				if((*m)==0)
				{
					(*m)=1;
					(*n)=((*gcd)-a)/b;
					
					if(cambioA)
					{
						(*m)=-(*m);
					}
					if(cambioB)
					{
						(*n)=-(*n);
					}
				}
			}
			else
			{
				if(cambioA)
				{
					(*m)=-y0.GetInteger();
				}
				else
				{
					(*m)=y0.GetInteger();
				}
				if(cambioB)
				{
					
					(*n)=-x0.GetInteger();
				}
				else
				{
					(*n)=x0.GetInteger();
					
				}
				if((*n)==0)
				{
					(*n)=1;
					(*m)=((*gcd)-b)/a;
					if(cambioA)
					{
						(*n)=-(*n);
						
					}
					if(cambioB)
					{
						(*m)=-(*m);
					}
				}
			}
		}
		else
		{
			(*m)=1;
			(*n)=0;
			*gcd=a;
		}
	}
	else
	{
		if(a==0)
		{
			(*m)=0;
			(*n)=1;
			*gcd=b;
		}
		else
		{
			(*m)=1;
			(*n)=0;
			*gcd=a;
		}
	}
}
template < typename T > void CFloat_PManager_GCDA( T a, T b, int *p, int *q)
{
	CFloat_PManager_<T> p1,q1;
	if( (a>0 && b>0 && a>=b) || (a<0 && b<0 && -a>=-b) || (a<0 && b>0 && -a>b) || (a>0 && b<0 && a>-b) )
	{
		if( ((*q)>0 && a>0 && (*q)>a) || ((*q)<0 && a<0 && -(*q)>-a) || ((*q)<0 && a>0 && -(*q)>a) || ((*q)>0 && a<0 && (*q)>-a) )
		{
			(*p)=(*p)+(int)((*q)/a)*b;

			(*q)=(*q)-(int)((*q)/a)*b;
		}
	}
	else
	{
		if( ((*p)>0 && b>0 && (*p)>b) || ((*p)<0 && b<0 && -(*p)>-b) || ((*p)<0 && b>0 && -(*p)>b) || ((*p)>0 && b<0 && (*p)>-b) )
		{
			(*q)=(*q)+(int)((*p)/b)*a;

			(*p)=(*p)-(int)((*p)/b)*b;
		}
	}
}
template < typename T > CFloat_PManager_<T> floor(CFloat_PManager_<T> fl)
{
	CFloat_PManager_<T> Z = CFloat_PManager_<T>(0);
	if(fl.IsZero())
	{
		return Z;
	}
	if(fl.IsInteger())
	{
		return CFloat_PManager_<T>(fl.GetInteger());
	}
	else if( fl > Z )
	{
		return CFloat_PManager_<T>(fl.GetValue() - fl.GetFloat());
	}
	else if( fl < Z )
	{
		return CFloat_PManager_<T>(fl.GetInteger()-1);
	}
}
template < typename T > void ToIntegerMatrix(CMatrix_<T>* iMatrix, T* value, bool to_i = true)
{
	T i_mod = T(0);
	T i_cont = 1;
	int j = 0;
	*value = fabs( long double((*iMatrix)[0][0]) );
	//Finding Maximum Value
	for ( UINT i = 0; i < iMatrix->GetRowSize(); i++ )
	{
		for ( UINT k = 0; k < iMatrix->GetColumnSize(); k++ )
		{
			if ( fabs( long double((*iMatrix)[i][k]) ) > fabs(long double(*value)) )
			{
				*value = (*iMatrix)[i][k];
			}
		}
	}
	long double i_frac;
	long double i_int;
	long double val = long double(*value);
	do
	{
		i_frac = modf(val,&i_int);
		i_int *= 10;
		i_frac *= 10;
		val = i_frac;
		cout << " i_p1 " << i_frac << endl;
		cout << " j " << j << endl;
		j++;
	}while(i_frac != 0);
	

	for ( UINT i = 0; i < iMatrix->GetRowSize(); i++ )
	{
		for ( UINT k = 0; k < iMatrix->GetColumnSize(); k++ )
		{
			(*iMatrix)[i][k] *= (pow(10.0,14.0));
		}
	}		
	if ( to_i == false )
	{
		cout << " homies " << endl;
	}
}
#endif