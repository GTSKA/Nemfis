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

	
	CVector_& operator - (const CVector_ &ObjRef)
	{		
		for (UINT i = 0; i<ObjRef.m_nSize; i++)
		{
			m_array[i]=m_array[i]-ObjRef.m_array[i];
		}
		return *this;
	}
	
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
	
};

typedef CVector_<float> CVECTOR_FLOAT;
typedef CVector_<double> CVECTOR_DOUBLE;

class CVECTOR_FLOAT3 : public CVECTOR_FLOAT
{
public:
	CVECTOR_FLOAT3();
	float x();
	float y();
	float z();
	
};
class CVECTOR_FLOAT4 : public CVECTOR_FLOAT
{
public:
	CVECTOR_FLOAT4();
	float x();
	float y();
	float z();
	float w();
	
};
CVECTOR_FLOAT3 operator*(CVECTOR_FLOAT3 V1, const CVECTOR_FLOAT3 &V2);
#endif