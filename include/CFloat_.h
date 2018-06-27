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


#endif