#include "CArray_.h"

template <typename T>ARRAY_GROUP<T>::~ARRAY_GROUP(void)
{
	//Dispose();
}



template <typename T>CArray_<T>::CArray_(void)
{
	m_nSize = 0;
	m_array = NULL;
	m_bUseDestructor = false;
}

template <typename T>CArray_<T>::CArray_(const CArray_<T> &nCArray)
{
	if (nCArray.m_nSize >= 1)
	{
		//Free the memory for the array
		m_array = NULL;
		m_nSize = nCArray.m_nSize;

		try
		{
			m_array = (T*)calloc(m_nSize, m_nSize * sizeof(T));
		}
		catch (char* error)
		{
#ifdef _CMP_VST_WIN
			::MessageBoxA(NULL, error, "Error", 0);
#endif
		}

#ifdef _CMP_VST_WIN
		CopyMemory(m_array, nCArray.m_array, nCArray.m_nSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
		memcpy(m_array, nCArray.m_array, nCArray.m_nSize * sizeof(T));
#endif

		m_bUseDestructor = nCArray.m_bUseDestructor;
	}
	else
	{
		Dispose();
	}
}

template <typename T>CArray_<T>::CArray_(const UINT nSize)
{
	m_nSize = nSize;
	m_array = (T*)calloc(m_nSize, m_nSize * sizeof(T));
	m_bUseDestructor = false;
}
template <typename T>CArray_<T>::CArray_(T *mDataArray, const UINT nDataSize)
{
	m_nSize = nDataSize;
	m_array = (T*)calloc(m_nSize, m_nSize * sizeof(T));
	m_bUseDestructor = false;

#ifdef _CMP_VST_WIN
	CopyMemory(m_array, mDataArray, nDataSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
	memcpy(m_array, mDataArray, nDataSize * sizeof(T));
#endif
}
template <typename T>CArray_<T>::~CArray_(void)
{
	Dispose();
}

template <typename T> void CArray_<T>::Dispose()
{
	if (m_array != NULL)
	{
		//First we call the destructor for the data objects...
		if (m_bUseDestructor)
		{
			for (UINT i = 0; i<m_nSize; i++)
			{
				m_array[i].~T();
			}
		}
		free(m_array);
		m_array = NULL;
	}
}
template <typename T> bool CArray_<T>::SetSize_Private(UINT nNewSize)
{
	if ((nNewSize < 0 || (nNewSize >(INT_MAX / sizeof(T)))))
	{
		//Do nothing...
#ifdef 	CARRAY_DEBUG_CONSOLE
#endif
		return false;
	}
	else if (nNewSize == 0)
	{
		if (m_nSize != 0)
		{
#ifdef 	CARRAY_DEBUG_CONSOLE
#endif

			//First we call the destructor for the data objects...
			if (m_bUseDestructor)
			{
				for (UINT i = 0; i<m_nSize; i++)
				{
					m_array[i].~T();
				}
			}

			//Free the memory for the array
			free(m_array);
			m_array = NULL;
			m_nSize = 0;
			return true;
		}
		else
		{
			return true;
		}
	}
	else if (nNewSize < m_nSize)
	{
#ifdef 	CARRAY_DEBUG_CONSOLE
		/*cout << endl;
		cout << " (nNewSize < m_nSize) " << endl;
		cout << endl;*/
#endif

		////Erase the rest of the elements in the array,...
		//First we call the destructor for the data objects...
		if (m_bUseDestructor)
		{
			for (UINT i = nNewSize; i<m_nSize; i++)
			{
				m_array[i].~T();
			}
		}
		//Now resize the array...
		m_array = (T*)realloc(m_array, nNewSize * sizeof(T));
		if (m_array == NULL)
		{
			return false;
		}
		m_nSize = nNewSize;
		return true;
	}
	else if (nNewSize > m_nSize)
	{
#ifdef 	CARRAY_DEBUG_CONSOLE
		/*cout << endl;
		cout << " (nNewSize > m_nSize) " << endl;
		cout << endl;*/
#endif

		//Resize the memory buffer
		m_array = (T*)realloc(m_array, nNewSize * sizeof(T));
		if (m_array == NULL)
		{
			return false;
		}

		//Make and initialize the new elements
		for (UINT i = m_nSize; i<nNewSize; i++)
		{
			::new (&m_array[i]) T;
			//m_array[i] = T();
		}
		m_nSize = nNewSize;

		return true;
	}
	return false;
}
template <typename T> void CArray_<T>::GetData(T *mDataArray)
{
	if (mDataArray != NULL)
	{
#ifdef _CMP_VST_WIN
		CopyMemory(mDataArray, m_array, m_nSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
		memcpy(mDataArray, m_array, m_nSize * sizeof(T));
#endif
	}
}
template <typename T> void CArray_<T>::Add()
{
	m_array = (T*)realloc(m_array, (m_nSize + 1) * sizeof(T));
	if (m_array != NULL)
	{
		::new (&m_array[m_nSize]) T;
		//m_array[m_nSize] = T();
		m_nSize++;
	}
}
template <typename T> void CArray_<T>::Add(const T &value)
{
	if (m_nSize == 0)
	{
		m_array = (T*)calloc(m_nSize + 1, m_nSize + 1 * sizeof(T));
		if (m_array != NULL)
		{
			m_array[m_nSize] = value;
			m_nSize++;
		}
	}
	else
	{
		m_array = (T*)realloc(m_array, (m_nSize + 1) * sizeof(T));
		if (m_array != NULL)
		{
			::new (&m_array[m_nSize]) T;
			m_array[m_nSize] = value;
			m_nSize++;
		}
	}
}


template <typename T> void CArray_<T>::Add(T *mDataArray, const UINT nDataSize)
{
	if (SetSize_Private(m_nSize + nDataSize))
	{
#ifdef _CMP_VST_WIN
		CopyMemory(&m_array[m_nSize - nDataSize], mDataArray, nDataSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
		memcpy(&m_array[m_nSize - nDataSize], mDataArray, nDataSize * sizeof(T));
#endif
	}
}
template <typename T> void CArray_<T>::Add(CArray_<T> *mDataArray)
{
	if (SetSize_Private(m_nSize + mDataArray->m_nSize))
	{
#ifdef _CMP_VST_WIN
		CopyMemory(&m_array[m_nSize - mDataArray->m_nSize], mDataArray->m_array, mDataArray->m_nSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
		memcpy(&m_array[m_nSize - mDataArray->m_nSize], mDataArray->m_array, mDataArray->m_nSize * sizeof(T));
#endif
	}
}

template <typename T> void CArray_<T>::Insert(const UINT nIndex, const T &value)
{
	if (nIndex >= 0)
	{
		if (nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			SetSize_Private(m_nSize + 1);

			//Move the elements of the array...
#ifdef _CMP_VST_WIN
			MoveMemory(&m_array[nIndex + 1], &m_array[nIndex], ((m_nSize - 1) - nIndex) * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
			memmove(&m_array[nIndex + 1], &m_array[nIndex], ((m_nSize - 1) - nIndex) * sizeof(T));
#endif

			//Construct the new element
			//::new (&m_array[nIndex]) T;

			//Assign
			m_array[nIndex] = value;
		}
	}
}
template <typename T> void CArray_<T>::Insert(const UINT nIndex, T *mDataArray, const UINT nDataSize)
{
	if (nIndex >= 0)
	{
		if (nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			if (SetSize_Private(m_nSize + nDataSize))
			{
#ifdef _CMP_VST_WIN
				//Move the elements of the array...
				MoveMemory(&m_array[nIndex + nDataSize], &m_array[nIndex], ((m_nSize - nDataSize) - nIndex) * sizeof(T));
				//Now we copy the buffer content to the array
				CopyMemory(&m_array[nIndex], mDataArray, nDataSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
				//Move the elements of the array...
				memmove(&m_array[nIndex + nDataSize], &m_array[nIndex], ((m_nSize - nDataSize) - nIndex) * sizeof(T));
				//Now we copy the buffer content to the array
				memcpy(&m_array[nIndex], mDataArray, nDataSize * sizeof(T));
#endif
			}
		}
	}
}
template <typename T> void CArray_<T>::Insert(const UINT nIndex, const CArray_<T> *mDataArray)
{
	if (nIndex >= 0)
	{
		if (nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			if (SetSize_Private(m_nSize + mDataArray->m_nSize))
			{
#ifdef _CMP_VST_WIN
				//Move the elements of the array...
				MoveMemory(&m_array[nIndex + mDataArray->m_nSize], &m_array[nIndex], ((m_nSize - mDataArray->m_nSize) - nIndex) * sizeof(T));
				//Now we copy the buffer content to the array
				CopyMemory(&m_array[nIndex], mDataArray, mDataArray->m_nSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
				//Move the elements of the array...
				memmove(&m_array[nIndex + mDataArray->m_nSize], &m_array[nIndex], ((m_nSize - mDataArray->m_nSize) - nIndex) * sizeof(T));
				//Now we copy the buffer content to the array
				memcpy(&m_array[nIndex], mDataArray, mDataArray->m_nSize * sizeof(T));
#endif
			}
		}
	}

}


template <typename T> bool CArray_<T>::Find(T sValue)
{
	for (UINT i = 0; i<m_nSize; i++)
	{
		if (m_array[i] == sValue)
		{
			return  true;
		}
	}
	return false;
}
template <typename T> bool CArray_<T>::FindFirst(T sValue, UINT *iIndex)
{
	*iIndex = -1;
	for (UINT i = 0; i<m_nSize; i++)
	{
		if (m_array[i] == sValue)
		{
			*iIndex = i;
			return  true;
		}
	}
	return false;
}
template <typename T> bool CArray_<T>::FindLast(T sValue, UINT *iIndex)
{
	*iIndex = -1;
	for (UINT i = m_nSize - 1; i>0; i--)
	{
		if (m_array[i] == sValue)
		{
			*iIndex = i;
			return  true;
		}
	}
	return false;
}
template <typename T> bool CArray_<T>::FindAll(T sValue, CArray_<T>* indexList)
{
	//Clean the index list
	indexList->RemoveAll();
	bool res = false;

	for (UINT i = 0; i<m_nSize; i++)
	{
		if (m_array[i] == sValue)
		{
			res = true;
			indexList->Add(i);
		}
	}
	return res;
}



template <typename T> void CArray_<T>::GetMaxValue(T *value, UINT *index)
{
	*index = 0;
	*value = m_array[0];
	for (UINT i = 1; i<m_nSize; i++)
	{
		if (m_array[i]>*value)
		{
			*value = m_array[i];
			*index = i;
		}
	}
}
template <typename T> void CArray_<T>::GetMaxValue(UINT *index)
{
	T a;
	*index = 0;
	a = m_array[0];
	for (UINT i = 1; i<m_nSize; i++)
	{
		if (m_array[i]>a)
		{
			a = m_array[i];
			*index = i;
		}
	}
}
template <typename T> void CArray_<T>::GetMinValue(T *value, UINT *index)
{
	*index = 0;
	*value = m_array[0];
	for (UINT i = 1; i<m_nSize; i++)
	{
		if (m_array[i]<*value)
		{
			*value = m_array[i];
			*index = i;
		}
	}
}
template <typename T> void CArray_<T>::GetMinValue(UINT *index)
{
	T a;
	*index = 0;
	a = m_array[0];
	for (UINT i = 1; i<m_nSize; i++)
	{
		if (m_array[i]<a)
		{
			a = m_array[i];
			*index = i;
		}
	}
}
template <typename T> bool CArray_<T>::IsNegative()
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i]>0)
		{
			return false;
		}
	}
	return true;
}

template <typename T> bool CArray_<T>::IsPositive()
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if (m_array[i]<0)
		{
			return false;
		}
	}
	return true;
}
template <typename T> void CArray_<T>::PrintToConsole()
{
	for (register UINT i = 0; i<m_nSize; i++)
	{
		cout << m_array[i] << endl;
	}
}
template <typename T> void CArray_<T>::Remove(const UINT nIndex)
{
	if (nIndex >= 0)
	{
		if (nIndex < m_nSize)
		{
			if (m_bUseDestructor)
			{
				//Call the destructor for the element to be removed
				m_array[nIndex].~T();
			}
			//free(&m_array[nIndex]);

			//Compact the array and decrease the size
#ifdef _CMP_VST_WIN
			MoveMemory(&m_array[nIndex], &m_array[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
			memmove(&m_array[nIndex], &m_array[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
#endif

			SetSize_Private(m_nSize - 1);
		}
	}

}

template <typename T> void CArray_<T>::Remove(CArray_<UINT>* indexList)
{
	for (UINT i = 0; i<indexList->m_nSize; i++)
	{
		if (indexList->m_array[i] >= 0)
		{
			if (indexList->m_array[i] < m_nSize)
			{
				if (m_bUseDestructor)
				{
					//Call the destructor for the element to be removed
					m_array[indexList->m_array[i]].~T();
				}

#ifdef _CMP_VST_WIN
				//Compact the array and decrease the size
				MoveMemory(&m_array[indexList->m_array[i]],
					&m_array[indexList->m_array[i] + 1], (m_nSize - (indexList->m_array[i] + 1)) * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
				memmove(&m_array[indexList->m_array[i]],
					&m_array[indexList->m_array[i] + 1], (m_nSize - (indexList->m_array[i] + 1)) * sizeof(T));
#endif

				m_nSize--;
			}
		}

	}
}


template <typename T> void CArray_<T>::Resize(UINT newSize)
{
	SetSize_Private(newSize);
}

template <typename T> void CArray_<T>::Reverse()
{
	CArray_<T> q;
	while (m_nSize>0)
	{
		q.Add(m_array[m_nSize - 1]);
		Remove(m_nSize - 1);
	}
	*this = q;
}
template <typename T> T& CArray_<T>::GetLastValue()
{
	return m_array[m_nSize - 1];
}
template <typename T> void CArray_<T>::SetData(T *mDataArray, const UINT nDataSize)
{
	if (nDataSize > 0)
	{
		//First we try to release the old array...
		if (m_bUseDestructor)
		{
			for (UINT i = 0; i<m_nSize; i++)
			{
				m_array[i].~T();
			}
		}

		if (m_array != NULL)
		{
			free(m_array);
			m_array = NULL;
		}

		SetSize_Private(nDataSize);

#ifdef _CMP_VST_WIN
		CopyMemory(m_array, mDataArray, nDataSize * sizeof(T));
#endif

#ifdef _CMP_GCC_WIN
		memcpy(m_array, mDataArray, nDataSize * sizeof(T));
#endif
	}
}

template <typename T> void CArray_<T>::SubArray(UINT jMin, UINT jMax, CArray_<T>& obj)
{
	if (m_nSize<jMax)
	{
		obj.Resize(m_nSize - jMin + 1);
		for (UINT i = jMin; i<m_nSize; i++)
		{
			obj[i - jMin] = m_array[i];
		}
	}
	else
	{
		obj.Resize(jMax - jMin + 1);
		for (UINT i = jMin; i <= jMax; i++)
		{
			obj[i - jMin] = m_array[i];
		}
	}
}
template <typename T> void CArray_<T>::SubArray(CArray_<T>& indexList, CArray_<T>& obj)
{
	obj.Resize(indexList.m_nSize);
	for (register int i = 0; i<indexList.m_nSize; i++)
	{
		obj[i] = (m_array[indexList[i]]);
	}
}
template <typename T> void CArray_<T>::SubArray(UINT* indexList, UINT listSize, CArray_<T>& obj)
{
	obj.Resize(*indexList);
	for (UINT i = 0; i<listSize; i++)
	{
		obj[i] = m_array[indexList[i]];
	}
}


template <typename T> void CArray_<T>::Zero()
{
	ZeroMemory(m_array, m_nSize * sizeof(T));
}
template <typename T> bool CArray_<T>::IsContent(CArray_<T> *dataList)
{
	for (UINT i = 0; i<dataList->m_nSize; i++)
	{
		if (!Find((*dataList)[i]))
		{
			return false;
		}
	}
	return true;
}
template <typename T> void CArray_<T>::Interchange(UINT iPos, UINT jPos)
{
	T a;
	a = m_array[iPos];
	m_array[iPos] = m_array[jPos];
	m_array[jPos] = a;
}
template <typename T> void CArray_<T>::QuickSort_Ascending()
{
	_Qnum = 0;
	_QAs(0, m_nSize - 1);
}
template <typename T> void CArray_<T>::QuickSort_Ascending(CArray_<UINT> *a)
{
	_Qnum = 0;
	a->Resize(m_nSize);
	for (register UINT i = 0; i<m_nSize; i++)
	{
		(*a)[i] = i;
	}
	_QAsA(0, m_nSize - 1, a);
}
template <typename T> void CArray_<T>::Quicksort_Desending()
{
	_Qnum = 0;
	_QDes(0, m_nSize - 1);
}


template <typename T> void CArray_<T>::Quicksort_Desending(CArray_<UINT> *a)
{
	_Qnum = 0;
	a->Resize(m_nSize);
	for (register UINT i = 0; i<m_nSize; i++)
	{
		(*a)[i] = i;
	}
	_QDesA(0, m_nSize - 1, a);
}
//--------------------------------------------------------------------------------------------------------------------------------------
//Static Functions
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T> void CArray_MinSet(const CArray_<T>* a, CArray_<T>* r)
{
	int j = 0, k, h = 0;
	k = (*a).m_nSize;
	CArray_<T> b;
	*r = *a;
	while (j<k)
	{
		(*r).FindAll((*r)[h], &b);
		for (UINT i = b.m_nSize; i>1; i--)
		{
			(*r).Remove(b[i - 1]);
		}
		j = j + b.m_nSize;
		h++;
	}
}

template <typename T> void CArray_Union_Min(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r)
{
	bool t;
	CArray_MinSet(a, r);
	for (UINT i = 0; i<(*b).m_nSize; i++)
	{
		if (!r->Find((*b)[i]))
		{
			r->Add((*b)[i]);
		}
	}
}

template <typename T> void CArray_Union_Max(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r)
{
	bool t;
	(*r) = (*a);
	for (UINT i = 0; i<(*b).m_nSize; i++)
	{
		r->Add((*b)[i]);
	}
}

template <typename T> void CArray_Intersection(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r)
{
	CArray_MinSet(a, r);
	int k = 0;
	bool bFind = false;

	for (UINT i = 0; i <= r->m_nSize; i++)
	{
		bFind = false;
		for (UINT j = 0; j<b->m_nSize; j++)
		{
			if ((*b)[j] == (*r)[k])
			{
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			r->Remove(k);
		}
		else
		{
			k++;
		}
	}
}

template <typename T> void CArray_Diferencia_Min(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r)
{
	CArray_MinSet(a, r);
	UINT c = 0;
	for (UINT i = 0; i<b->m_nSize; i++)
	{
		if (r->FindFirst((*b)[i], &c))
		{
			r->Remove(c);
		}
	}
}

template <typename T> void CArray_Diferencia_Max(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r)
{
	*r = *a;
	UINT c = 0;
	for (UINT i = 0; i<b->m_nSize; i++)
	{
		if (r->FindFirst((*b)[i], &c))
		{
			while (r->FindFirst((*b)[i], &c))
			{
				r->Remove(c);
			}
		}
	}
}

template <typename T> void CArray_RandomFill(CArray_<T> *obj, T a, T b)
{
	float t;
	srand(time(NULL));
	for (register int i = 0; i<obj->m_nSize; i++)
	{
		t = float(rand()) / float(RAND_MAX);
		(*obj)[i] = a + T(int(T(t)*(b - a)));
	}
}
template <typename T> void CArray_RandomFill(CArray_<T> *obj, T a, T b, UINT seed)
{
	float t;
	srand(seed);
	for (register int i = 0; i<obj->m_nSize; i++)
	{
		t = float(rand()) / float(RAND_MAX);
		(*obj)[i] = a + int(T(t)*(b - a));
	}
}
template <typename T> void CArray_RandomFill(CArray_<T> *obj, UINT n, T a, T b)
{
	float t;
	(*obj).Resize(n);
	srand(time(NULL));
	for (register int i = 0; i<obj->m_nSize; i++)
	{
		t = float(rand()) / float(RAND_MAX);
		(*obj)[i] = a + int(t*(b - a));
	}
}
template <typename T> void CArray_RandomFill(CArray_<T> *obj, UINT n, UINT seed, T a, T b)
{
	float t;
	srand(seed);
	(*obj).Resize(n);
	for (register int i = 0; i<obj->m_nSize; i++)
	{
		t = float(rand()) / float(RAND_MAX);
		(*obj)[i] = a + int(t*(b - a));
	}
}
template <typename T> void CArray_GetDecreasingSets(CArray_< CArray_<UINT> > *sets, UINT numElements, UINT longString)
{
	CArray_<UINT> a;
	if (numElements >= longString)
	{

		for (register UINT k = 0; k<(1 + numElements - longString); k++)
		{
			a.Add();
			a.GetLastValue() = numElements - k;
			CArray_GetDecreasingSetsAux< UINT >(sets, &longString, &a, longString - 1);
			a.RemoveAll();
		}
	}

}
template <typename T> void CArray_GetDecreasingSetsAux(CArray_< CArray_<UINT> > *sets, UINT *longString, CArray_<UINT> *a, int falta)
{
	UINT b, c;
	if (falta > 0)
	{
		b = a->GetSize();
		c = falta;
		for (register UINT j = (*a)[a->GetSize() - 1] - 1; j>0; j--)
		{
			a->Add();
			a->GetLastValue() = j;
			if (a->GetSize() == *longString)
			{
				sets->Add(*a);
			}
			falta--;
			//cout<<j<<endl;
			CArray_GetDecreasingSetsAux< UINT >(sets, longString, a, falta);
			a->Resize(b);
			falta = c;
		}
	}
}

template <typename T> bool CArray_AscendingSet(CArray_<UINT> *mArray, UINT size, UINT maxValue)
{
	for (register UINT i = size; i>0; i--)
	{
		if ((*mArray)[i - 1]<maxValue - size + i - 1)
		{
			(*mArray)[i - 1]++;
			return true;
		}
	}
	return false;
}

template <typename T> void CArray_<T>::GetMaxAbsoluteValue(T* value, UINT *index)
{
	*index = 0;
	*value = m_array[0];
	for (register UINT i = 1; i < m_nSize; i++)
	{
		//Castrin Estricto  

		if (fabs((long double)(m_array[i])) > fabs((long double)(*value)))
		{
			*index = i;
			*value = m_array[i];
		}
	}
}
#ifdef CUDA_PROJECT
//Copy Array info to the Device
//---------------------------------
//-a: Index List
//-getset: indicates if get or set info
//---------------------------------
template <typename T> void CArray_<T>::CUDA_memcpy(T** a, bool getset)
{
	if (*a == NULL)
	{
		cutilSafeCall(cudaMalloc((void**)a, (m_nSize) * sizeof(T)));
	}
	if (getset)
	{
		cutilSafeCall(cudaMemcpy(*a, m_array, m_nSize * sizeof(T), cudaMemcpyHostToDevice));
	}
	else
	{
		cutilSafeCall(cudaMemcpy(m_array, *a, m_nSize * sizeof(T), cudaMemcpyDeviceToHost));
	}
}
#endif
template <typename T> bool CArray_WarrantlessSearch(CArray_<CArray_<T>> mnArray, CArray_<T> mArray)
{
	bool sT;

	for (register UINT i = 0; i<mnArray.GetSize(); i++)
	{
		for (register UINT j = 0; j<mArray.GetSize(); j++)
		{
			sT = false;
			if (mArray.GetSize() != mnArray[i].GetSize())
			{
				break;
			}
			for (register UINT k = 0; k<mnArray[i].GetSize(); k++)
			{
				if (mArray[j] == mnArray[i][k])
				{
					sT = true;
				}
			}
			if (sT == false)
			{
				break;
			}
		}
		if (sT == true)
		{
			return true;
		}
	}
	return false;

}


template <typename T> bool CArray_WarrantlessSearch(CArray_<CArray_<T>> mnArray, CArray_<T> mArray, UINT n)
{
	bool sT;

	for (register UINT i = 0; i<mnArray.GetSize(); i++)
	{
		if (i != n)
		{
			for (register UINT j = 0; j<mArray.GetSize(); j++)
			{
				sT = false;
				if (mArray.GetSize() != mnArray[i].GetSize())
				{
					break;
				}
				for (register UINT k = 0; k<mnArray[i].GetSize(); k++)
				{
					if (mArray[j] == mnArray[i][k])
					{
						sT = true;
					}
				}
				if (sT == false)
				{
					break;
				}
			}
			if (sT == true)
			{
				return true;
			}
		}
	}
	return false;

}
