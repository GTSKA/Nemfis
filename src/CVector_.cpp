#include "CVector_.h"
#include "CArray_.h"
template <typename T> CVector_<T>::CVector_()
{
}
template <typename T> CVector_<T>::CVector_(int n) : CArray_(n)
{
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



/////////////////////////////////////////////////////////////////
////////////CVECTOR_FLOAT3
//////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////
////////////////CVECTOR_FLOAT4
/////////////////////////////////////////////////////////////////
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