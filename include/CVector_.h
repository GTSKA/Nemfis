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
	int y=0;
	for (register int i = 0; i<m_nSize; i++)
	{
		y=i+1;
		if(m_array[i]>0)
		{
			continue;
		}
		if(m_array[i]<0)
		{
			return false;
		}
	}
	if(y = m_nSize);
	{
		return true;
	}
}
template <typename T> bool CVector_<T>::IsNegative()
{
	int y = 0;
	for (register int i = 0; i<m_nSize; i++)
	{
		y=i+1;
		if(m_array[i]>0)
		{
			return false;
		}
		if(m_array[i]<0)
		{
			continue;
		}
	}
	if(y = m_nSize)
	{
		return true;
	}
}
template <typename T> void CVector_<T>::KProduct(CArray_<T> *v, T *k)
{
	v->RemoveAll();
	for(register int i = 0; i<v->GetSize(); i++)
	{
		(*v)[i] = (*k)*m_array[i];
	}
}
template <typename T> void CVector_<T>::KProduct(CArray_<T> v, T k)
{
	v.RemoveAll();
	for(register int i = 0; i<v.GetSize(); i++)
	{
		v[i]=k*m_array[i];
	}
}
template <typename T> T CVector_<T>::DotProduct(CArray_<T> *u, T *r)
{
    *r = 0;
	if ( u->GetSize() > m_nSize || u->GetSize()< m_nSize )
	{
		cout<<"Producto imposible";
	}
	else
	{
		for (register int i=0; i<u->GetSize(); i++)
		{
			(*r) = (*r) + (*u)[i]*(m_array[i]);
		}
	}
	return *r;
}
template <typename T> T CVector_<T>::DotProduct(CArray_<T> u, T r)
{
    r = 0;
	if ( u.GetSize() > m_nSize || u.GetSize()< m_nSize )
	{
		cout<<"Producto imposible";
	}
	else
	{
		for (register int i=0; i<u.GetSize(); i++)
		{
			r += u[i]*(m_array[i]);
		}
	}
	return r;
}
template <typename T> bool CVector_<T>::IsLexPositive(UINT *index, T *Value)
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if(m_array[i]!=0)
		{
			if(m_array[i]>0)
			{
				*index = i;
				*Value = m_array[i];
				return true;
     		}
			if(m_array[i]<0)
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
		if(m_array[i]!=0)
		{
			if(m_array[i]>0)
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
		if(m_array[i]!=0)
		{
			if(m_array[i]<0)
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
		if(m_array[i]!=0)
		{
			if(m_array[i]<0)
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
		if(m_array[i]!=0)
		{
			return false;
		    break;
		}
		x=i+1;
	}
	if(x=m_nSize)
	{
		return true;
	}
}


#endif