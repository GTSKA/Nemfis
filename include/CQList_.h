#ifndef CQLIST_H
#define CQLIST_H

#define CQLIST_DEBUG_CONSOLE
#define CQLIST_VERSION 1.0

#include "CArray_.h"


UINT iMin=0, iMax=0, iMed;


template <typename T> class CQList_ 
{
public:
	
	CQList_(bool dir);
	CQList_(const CArray_<T>* obj, bool dir);
	CQList_(const CQList_& obj, bool dir);
	virtual ~CQList_(void);

	void Add(const T &value);
	void Remove(UINT index);
	void PrintToConsole();
	T GetMaxValue();
	T GetMinValue();
	T& GetDataList(UINT index);
	void SetData(T& value, UINT index);
	UINT GetLastIndex();
	UINT GetSize();

	T operator [] (const UINT index)
	{
		assert( index >= 0 && index < m_dataList.m_nSize); 
		return m_dataList[m_indexList[index]];
	}

protected:

	CARRAY_UINT m_indexList;
	CArray_<T> m_dataList;
	bool m_direction;

private:

	void Sort(const T* value, UINT iMinA, UINT iMaxA);

};

template <typename T>CQList_<T>::CQList_(bool dir) 
{
	m_direction = dir;
}
template <typename T>CQList_<T>::CQList_(const CQList_& obj, bool dir)
{
	m_direction = dir;
	m_dataList = obj.m_dataList;
	m_indexList = obj.m_indexList;
}
template <typename T>CQList_<T>::CQList_(const CArray_<T>* obj, bool dir)
{
	m_direction = dir;
	for(register int i=0; i<obj->m_nSize; i++)
	{
		m_dataList.Add((*obj)[i]);
		Sort( &(*obj)[i], iMin,iMax);		
	}
}
template <typename T> CQList_<T>::~CQList_()
{

}
template <typename T> void CQList_<T>::Add(const T &value)
{
	m_dataList.Add(value);
	Sort( &value, iMin,iMax);
}

template <typename T> void CQList_<T>::PrintToConsole()
{
	for(UINT i=0; i<m_indexList.m_nSize; i++)
	{
		cout << m_dataList[m_indexList[i]] << endl;
	}
}

template <typename T> void CQList_<T>::Sort(const T* value, UINT iMinA, UINT iMaxA)
{
	if(!m_direction)
	{
		bool c=true;
		if(m_indexList.m_nSize==0)
		{
			m_indexList.Add(UINT(0));
		}
		else
		{
			if(*value>=m_dataList[m_indexList[iMinA]])
			{
				m_indexList.Insert(iMinA,m_dataList.m_nSize-1);
				iMax++;
				c=false;
			}
			
			if(*value<=m_dataList[m_indexList[iMaxA]] && c==true)
			{
				if(iMaxA==m_dataList.m_nSize-2)
				{
					m_indexList.Add(m_dataList.m_nSize-1);
				}
				else
				{
					m_indexList.Insert(iMaxA, m_dataList.m_nSize-1);
				}
				iMax++;
			}
			if(*value<m_dataList[m_indexList[iMinA]] && *value>m_dataList[m_indexList[iMaxA]])
			{
				if(*value>m_dataList[m_indexList[int((iMinA+iMaxA)/2)]])
				{
					Sort(value,iMinA,int((iMinA+iMaxA)/2));
				}
				else
				{
					Sort(value,int((iMinA+iMaxA)/2)+1,iMaxA);
				}

			}	
		}
	}
	else
	{
		bool c=true;
		if(m_indexList.m_nSize==0)
		{
			m_indexList.Add(UINT(0));
		}
		else
		{
			if(*value<=m_dataList[m_indexList[iMinA]])
			{
				m_indexList.Insert(iMinA,m_dataList.m_nSize-1);
				iMax++;
				c=false;
			}
			
			if(*value>=m_dataList[m_indexList[iMaxA]] && c==true)
			{
				if(iMaxA==m_dataList.m_nSize-2)
				{
					m_indexList.Add(m_dataList.m_nSize-1);
				}
				else
				{
					m_indexList.Insert(iMaxA, m_dataList.m_nSize-1);
				}
				iMax++;
			}
			if(*value>m_dataList[m_indexList[iMinA]] && *value<m_dataList[m_indexList[iMaxA]])
			{
				if(*value<m_dataList[m_indexList[int((iMinA+iMaxA)/2)]])
				{
					Sort(value,iMinA,int((iMinA+iMaxA)/2));
				}
				else
				{
					Sort(value,int((iMinA+iMaxA)/2)+1,iMaxA);
				}

			}	
		}
	}
}

template <typename T> UINT CQList_<T>::GetSize()
{
	return m_indexList.m_nSize;
}

template <typename T> T& CQList_<T>::GetDataList(UINT index)
{
	return m_dataList[index];
}

template <typename T> UINT CQList_<T>::GetLastIndex()
{
	return m_dataList.m_nSize -1;
}

template <typename T> T CQList_<T>::GetMinValue()
{
	if(m_indexList.m_nSize>=0)
	{
		return m_dataList[m_indexList[0]];
	}
}

template <typename T> T CQList_<T>::GetMaxValue()
{
	if(m_indexList.m_nSize>=0)
	{
		return m_dataList[m_indexList[m_indexList.m_nSize-1]];
	}
}

template <typename T> void CQList_<T>::Remove(UINT index)
{
	for(register int i=0; i<m_indexList.m_nSize; i++)
	{
		if(m_indexList[i]>m_indexList[index])
		{
			m_indexList[i]--;
		}
	}
	m_dataList.Remove(m_indexList[index]);
	m_indexList.Remove(index);
}

template <typename T> void CQList_<T>::SetData(T &value, UINT index)
{
	for(register int i=0; i<m_indexList.m_nSize; i++)
	{
		if(m_indexList[i]>index)
		{
			m_indexList[i]--;
		}
	}
	m_dataList.Remove(m_indexList[index]);
	m_indexList.Remove(index);

	m_dataList.Add(&value);
	sort( &value, iMin,iMax);
	
}



#endif //CQLIST_H