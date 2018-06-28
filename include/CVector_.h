#ifndef CVECTOR_H
#define CVECTOR_H

#include "Common_.h"
#include "CArray_.h"

template <typename T> class CVector_ : public CArray_<T>
{
public: 
	CVector_();
	CVector_(int n);
	~CVector_();
	bool IsPositive();
	bool IsNegative();
	void KProduct(CArray_<T> *v, T *k);
	void KProduct(CArray_<T> v, T k);
	T DotProduct(CArray_<T> *u, T *r);
	T DotProduct(CArray_<T> u, T r);
	bool IsLexPositive();
	bool IsLexPositive(UINT *index, T *Value);
	bool IsLexNegative();
	bool IsLexNegative(UINT *index, T *Value);
	bool IsZero();


	CVector_& operator =(CVector_& vec);
	CVector_ operator +(CVector_& vec);
	CVector_& operator +=(CVector_& vec);
	CVector_ operator -();
	CVector_ operator -(CVector_& vec);
	CVector_& operator -=(CVector_& vec);
	CVector_ operator *(T k);
	CVector_& operator *=(T k);
	CVector_ operator /(T k);
	CVector_& operator /=(T k);

	
	bool operator < ( const CVector_ &ObjRef )
	{
		T ac;
		for (UINT i = 0; i<ObjRef.m_nSize; i++)
		{
			ac=m_array[i]-ObjRef.m_array[i];
			if(ac<0)
				{
					return true;
				}
			else
			{
				if (ac>0)
					{
						return false;
					}
				else
				{
					return false;
				}
			}
			ac = 0;
		}
	}
	bool operator > ( const CVector_ &ObjRef)
	{	
		T ac;
		for(UINT i = 0; i<ObjRef.m_nSize; i++)
		{
			ac = m_array[i] - ObjRef.m_array[i];
			if (ac > 0)
			{
				return true;
			}
			else if (ac < 0)
			{
				return false;
			}
			else 
			{
				return false;
			}
		}
	}

	//Identity value for the elements this vector
	T I;

	//Zero value for the elements this vector
	T Z;

	//Negative value for the elements in the vector
	T N;
	
};

typedef CVector_<float> CVECTOR_FLOAT;
typedef CVector_<double> CVECTOR_DOUBLE;

template <typename T> CVector_<T>::CVector_()
{
	I = T(1.0);
	Z = (T)0;
	N = (T)(-1);
}
template <typename T> CVector_<T>::CVector_(int n) : CArray_(n)
{
	I = T(1.0);
	Z = (T)0;
	N = (T)(-1);
}
template <typename T> CVector_<T>::~CVector_()
{
}
template <typename T> bool CVector_<T>::IsPositive()
{
	int y = 0;
	for (register int i = 0; i<m_nSize; i++)
	{
		y = i + 1;
		if (m_array[i]>0)
		{
			continue;
		}
		if (m_array[i]<0)
		{
			return false;
		}
	}
	if (y = m_nSize);
	{
		return true;
	}
}
template <typename T> bool CVector_<T>::IsNegative()
{
	int y = 0;
	for (register int i = 0; i<m_nSize; i++)
	{
		y = i + 1;
		if (m_array[i]>0)
		{
			return false;
		}
		if (m_array[i]<0)
		{
			continue;
		}
	}
	if (y = m_nSize)
	{
		return true;
	}
}
template <typename T> void CVector_<T>::KProduct(CArray_<T> *v, T *k)
{
	v->RemoveAll();
	for (register int i = 0; i<v->GetSize(); i++)
	{
		(*v)[i] = (*k)*m_array[i];
	}
}
template <typename T> void CVector_<T>::KProduct(CArray_<T> v, T k)
{
	v.RemoveAll();
	for (register int i = 0; i<v.GetSize(); i++)
	{
		v[i] = k*m_array[i];
	}
}
template <typename T> T CVector_<T>::DotProduct(CArray_<T> *u, T *r)
{
	*r = 0;
	if (u->GetSize() > m_nSize || u->GetSize()< m_nSize)
	{
		cout << "Producto imposible";
	}
	else
	{
		for (register int i = 0; i<u->GetSize(); i++)
		{
			(*r) = (*r) + (*u)[i] * (m_array[i]);
		}
	}
	return *r;
}
template <typename T> T CVector_<T>::DotProduct(CArray_<T> u, T r)
{
	r = 0;
	if (u.GetSize() > m_nSize || u.GetSize()< m_nSize)
	{
		cout << "Producto imposible";
	}
	else
	{
		for (register int i = 0; i<u.GetSize(); i++)
		{
			r += u[i] * (m_array[i]);
		}
	}
	return r;
}
template <typename T> bool CVector_<T>::IsLexPositive(UINT *index, T *Value)
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i] != 0)
		{
			if (m_array[i]>0)
			{
				*index = i;
				*Value = m_array[i];
				return true;
			}
			if (m_array[i]<0)
			{
				return false;
			}
		}
	}
}
template <typename T> bool CVector_<T>::IsLexPositive()
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i] != 0)
		{
			if (m_array[i]>0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
template <typename T> bool CVector_<T>::IsLexNegative(UINT *index, T *Value)
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i] != 0)
		{
			if (m_array[i]<0)
			{
				*index = i;
				*Value = m_array[i];
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
template <typename T> bool CVector_<T>::IsLexNegative()
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i] != 0)
		{
			if (m_array[i]<0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
template <typename T> bool CVector_<T>::IsZero()
{
	int x;
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i] != 0)
		{
			return false;
			break;
		}
		x = i + 1;
	}
	if (x = m_nSize)
	{
		return true;
	}
}

template <typename T> CVector_<T>& CVector_<T>::operator =(CVector_& vec)
{
	if(this->GetSize() == vec.GetSize())
		for (register int i = 0; i < this->GetSize(); i++)
			m_array[i] = vec[i];
	return *this;
}
template <typename T> CVector_<T> CVector_<T>::operator +(CVector_& vec)
{
	CVector_<T> res(this->GetSize());
	if (GetSize() == vec.GetSize())
	{
		for (register int i = 0; i < this->GetSize(); i++)
			res[i] = m_array[i] + vec[i];
	}
	return res;
}
template <typename T> CVector_<T>& CVector_<T>::operator +=(CVector_& vec)
{
	if (this->GetSize() == vec.GetSize())
		m_array[i] += vec[i];
	return *this;
}
template <typename T> CVector_<T> CVector_<T>::operator -()
{
	CVector_<T> res(this->GetSize());
	for (register int i = 0; i < this->GetSize(); i++)
		res[i] = m_array[i] * N;
	return res;
}
template <typename T> CVector_<T> CVector_<T>::operator -(CVector_& vec)
{
	CVector_<T> res(this->GetSize());
	if (this->GetSize() == vec.GetSize())
	{
		for (register int i = 0; i < this->GetSize(); i++)
			res[i] = m_array[i] - vec[i];
	}
	return res;
}
template <typename T> CVector_<T>& CVector_<T>::operator -=(CVector_& vec)
{
	if (this->GetSize() == vec.GetSize())
		m_array[i] -= vec[i];
	return *this;
}
template <typename T> CVector_<T> CVector_<T>::operator *(T k)
{
	CVector_<T> res(this->GetSize());
	for (register int i = 0; i < this->GetSize(); i++)
		res[i] = m_array[i] * k;
	return res;
}
template <typename T> CVector_<T>& CVector_<T>::operator *=(T k)
{
	for (register int i = 0; i < this->GetSize(); i++)
		m_array[i] *= k;
	return *this;
}
template <typename T> CVector_<T> CVector_<T>::operator /(T k)
{
	CVector_<T> res(this->GetSize());
	for (register int i = 0; i < this->GetSize(); i++)
		res[i] = m_array[i] / k;
	return res;
}
template <typename T> CVector_<T>& CVector_<T>::operator /=(T k)
{
	for (register int i = 0; i < this->GetSize(); i++)
		m_array[i] /= k;
	return *this;
}

/////////////////////////////////////////////////////////////////
////////////CVECTOR_FLOAT2
//////////////////////////////////////////////////////////////
class CVECTOR_FLOAT2 : public CVECTOR_FLOAT
{
public:
	CVECTOR_FLOAT2();
	float x();
	float y();

	const CVECTOR_FLOAT2& operator=(CVECTOR_FLOAT2& obj)
	{
		m_array[0] = obj[0]; m_array[1] = obj[1];
		return *this;
	}

	const CVECTOR_FLOAT2& operator=(CVECTOR_FLOAT& obj)
	{
		
		if (obj.GetSize() >= GetSize())
		{
			m_array[0] = obj[0];
			m_array[1] = obj[1];
		}
		return *this;
	}

	float Length();
	CVECTOR_FLOAT2& Normalize();

};
CVECTOR_FLOAT2::CVECTOR_FLOAT2() : CVECTOR_FLOAT(2)
{}
float CVECTOR_FLOAT2::x()
{
	return m_array[0];
}
float CVECTOR_FLOAT2::y()
{
	return m_array[1];
}
float CVECTOR_FLOAT2::Length()
{
	return sqrtf(m_array[0] * m_array[0] + m_array[1] * m_array[1]);
}
CVECTOR_FLOAT2& CVECTOR_FLOAT2::Normalize()
{
	float lenInv = 1.0f / Length();
	m_array[0] *= lenInv;
	m_array[1] *= lenInv;

	return *this;
}


/////////////////////////////////////////////////////////////////
////////////CVECTOR_FLOAT3
//////////////////////////////////////////////////////////////

class CVECTOR_FLOAT3 : public CVECTOR_FLOAT
{
public:
	CVECTOR_FLOAT3();
	float x();
	float y();
	float z();

	const CVECTOR_FLOAT3& operator=(CVECTOR_FLOAT3& obj)
	{
		m_array[0] = obj[0]; m_array[1] = obj[1]; m_array[2] = obj[2];
		return *this;
	}
	const CVECTOR_FLOAT3& operator=(CVECTOR_FLOAT& obj)
	{
		if (obj.GetSize() >= GetSize())
		{
			m_array[0] = obj[0];
			m_array[1] = obj[1];
			m_array[2] = obj[2];
		}
		return *this;
	}
	float Length();
	CVECTOR_FLOAT3& Normalize();
	
};

CVECTOR_FLOAT3::CVECTOR_FLOAT3() : CVECTOR_FLOAT(3)
{}
float CVECTOR_FLOAT3::x()
{
	return m_array[0];
}
float CVECTOR_FLOAT3::y()
{
	return m_array[1];
}
float CVECTOR_FLOAT3::z()
{
	return m_array[2];
}

CVECTOR_FLOAT3 operator*(CVECTOR_FLOAT3 V1, const CVECTOR_FLOAT3 &V2)
{
	CVECTOR_FLOAT3 res = CVECTOR_FLOAT3();
	res[0] = V1[1] * V2[2] - V1[2] * V2[1];
	res[1] = -1.0f*(V1[0] * V2[2] - V1[2] * V2[0]);
	res[2] = V1[0] * V2[1] - V1[1] * V2[0];
	return res;
}

float CVECTOR_FLOAT3::Length()
{
	return sqrtf(m_array[0] * m_array[0] + m_array[1] * m_array[1] + m_array[2] * m_array[2]);
}
CVECTOR_FLOAT3& CVECTOR_FLOAT3::Normalize()
{
	float lenInv = 1.0f / Length();
	m_array[0] *= lenInv;
	m_array[1] *= lenInv;
	m_array[2] *= lenInv;

	return *this;
}

//////////////////////////////////////////////////////////////////
////////////////CVECTOR_FLOAT4
/////////////////////////////////////////////////////////////////

class CVECTOR_FLOAT4 : public CVECTOR_FLOAT
{
public:
	CVECTOR_FLOAT4();
	float x();
	float y();
	float z();
	float w();
	const CVECTOR_FLOAT4& operator=(CVECTOR_FLOAT4& obj)
	{
		m_array[0] = obj[0]; m_array[1] = obj[1]; m_array[2] = obj[2]; m_array[3] = obj[3];
		return *this;
	}
	const CVECTOR_FLOAT4& operator=(CVECTOR_FLOAT& obj)
	{
		if (obj.GetSize() >= GetSize())
		{
			m_array[0] = obj[0];
			m_array[1] = obj[1];
			m_array[2] = obj[2];
			m_array[3] = obj[3];
		}
		return *this;
	}
	float Length();
	CVECTOR_FLOAT4& Normalize();
	
};
CVECTOR_FLOAT4::CVECTOR_FLOAT4() : CVECTOR_FLOAT(4)
{}
float CVECTOR_FLOAT4::x()
{
	return m_array[0];
}
float CVECTOR_FLOAT4::y()
{
	return m_array[1];
}
float CVECTOR_FLOAT4::z()
{
	return m_array[2];
}
float CVECTOR_FLOAT4::w()
{
	return m_array[3];
}
float CVECTOR_FLOAT4::Length()
{
	return sqrtf(m_array[0] * m_array[0] + m_array[1] * m_array[1] + m_array[2] * m_array[2] + m_array[3] * m_array[3]);
}
CVECTOR_FLOAT4& CVECTOR_FLOAT4::Normalize()
{
	float lenInv = 1.0f / Length();
	m_array[0] *= lenInv;
	m_array[1] *= lenInv;
	m_array[2] *= lenInv;
	m_array[3] *= lenInv;

	return *this;
}
#endif