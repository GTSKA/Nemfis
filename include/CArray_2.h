#ifndef CArray_H
#define CArray_H

#define CARRAY_DEBUG_CONSOLE
#define CARRAY_VERSION 1.63

#include "Common_.h"

//Class CArray, manage arrays and list data structurs of objects
//-----------------------------------------------------------------------------------------------------------------
template <typename T> class CArray_
{
public:
	
	CArray_(void);
	CArray_(const CArray_ &nCArray);
	CArray_(const UINT nSize );
	//Fill the array from a memory buffer
	//---------------------------------
	//mDataArray: Pointer to the data buffer
	//mDataSize: _Qnumber of elements in the data buffer
	//---------------------------------
	CArray_( T *mDataArray, const UINT nDataSize);
	virtual ~CArray_(void);

	virtual void Dispose();

	//Fill a data buffer whit the conten of the array
	//---------------------------------
	//-mDataArray: Buffer to fill with the elements of the array
	//---------------------------------
	void GetData(T *mDataArray);

	//Add a empty element in the array
	//---------------------------------
	void virtual Add();

	//Add a new element in the array
	//---------------------------------
	// - value: Element to add in the last array last position
	//---------------------------------
	void virtual Add(const T& value);

	//Add a buffer of elements to de array
	//---------------------------------
	// - mDataArray: Data array buffer
	// - nDataSize: _Qnumbers of elements of the data
	//---------------------------------
	void virtual Add(T *mDataArray, const UINT nDataSize);

	//Add a buffer of elements to de array
	//---------------------------------
	// - mDataArray: Data array buffer
	//---------------------------------
	void virtual Add(CArray_ *mDataArray);

	//Resize the _Qnumber of element of the array
	//---------------------------------
	// - nSize: New array size
	//---------------------------------
	void Resize(UINT nSize);

	//Insert a new element in the nIndex position
	//---------------------------------
	// - nIndex: Position to insert the new element
	// - value: New element to insert
	//---------------------------------
	void Insert(const UINT nIndex, const T& value);

	//Insert a buffer of elements in the nIndex position
	//---------------------------------
	// - nIndex: Position to insert the new element
	// - mDataArray: Buffer of data to insert
	// - nDataSize: _Qnumber of elements to insert
	//---------------------------------
	void Insert(const UINT nIndex, T *mDataArray, const UINT nDataSize);

	//Insert a CArray_ object in the nIndex position
	//---------------------------------
	// - nIndex: Position to insert the new element
	// - mDataArray: CArray_ objecto to insert
	//---------------------------------
	void Insert(const UINT nIndex, const CArray_ *mDataArray);

	//Remove the element of the array in the position, nIndex
	//---------------------------------
	// - nIndex: Position to insert the new element
	//---------------------------------
	void Remove(const UINT nIndex);

	//Remove all the elements in the index list
	//---------------------------------
	// - indexList: Set of index to remove
	//---------------------------------
	void Remove(CArray_<UINT>* indexList);

	//Remove all the elements in the array
	//---------------------------------
	void RemoveAll() { SetSize_Private(0); }

	//Reverse the array content
	//---------------------------------
	void Reverse();

	//Get the las data of the list
	//---------------------------------
	T& GetLastValue();

	//Get a sub array from the origina array between jMin and jMax
	//---------------------------------
	//- jMin index to begin the extration
	//- jMax index to end the extraction
	//---------------------------------
	void SubArray(UINT jMin, UINT jMax, CArray_& obj);

	//Get a sub array from the origina array, for the index list positions
	//---------------------------------
	//Guarda en obj un arreglo que consta de los elementos del arreglo original
	//cuyos indices son especificados en indexList
	//---------------------------------
	void SubArray(CArray_& indexList, CArray_& obj);

	//Get a sub array from the origina array, for the index list postions
	//---------------------------------
	//Guarda en obj un arreglo que consta de los elementos del arreglo original
	//cuyos indice son especificados en el arreglo indexList de tamaño listSize
	void SubArray(UINT* indexList, UINT listSize, CArray_& obj);

	//Guarda el valor maximo del arreglo en *value y el indice 
	//donde se encuentra tal valor en *index
	//---------------------------------------------
	void GetMaxValue(T *value, UINT *index);

	//Guarda el valor del indice donde se encuentra el elemento maximo del arreglo
	//------------------------------------------
	void GetMaxValue(UINT *index);

	//Guarda el valor minimo del arreglo en *value y el indice 
	//donde se encuentra tal valor en *index
	//---------------------------------------------
	void GetMinValue(T *value, UINT *index);

	//Guarda el valor del indice donde se encuentra el elemento minimo del arreglo
	//-----------------------------------
	void GetMinValue(UINT *index);
	
	//Return the size of the array
	//---------------------------------
	const UINT& GetSize() const { return  m_nSize; } 

	//Fill the array from a memory buffer
	//---------------------------------
	//-mDataArray: Pointer to the data buffer
	//-mDataSize: _Qnumber of elements in the data buffer
	//---------------------------------
	void SetData(T *mDataArray, const UINT nDataSize);



	//Enable or disable the use of destructors for the items in the array
	//---------------------------------
	//-bValue: Set the use of destructor for the array values
	//---------------------------------
	void SetDestructorValue(bool bValue) { m_bUseDestructor = bValue; }

	//Return a bool indicating if the array use destructor for the memory clean up
	//---------------------------------
	bool IsCallingDestructor() { return m_bUseDestructor; }
	
	//Set to zero all the elements in the array
	//------------------------------
	void Zero();

	//Print to console the content of the array
	//---------------------------------
	void PrintToConsole();
	//---------------------------------

	//Regresa true si todo el arreglo esta lleno de valores 
	//negativos y false y al menos unos es positivo 
	//------------------------------------------------
	bool IsNegative();

	//Regresa true si todo el arreglo esta lleno de valores 
	//positivos y false y al menos unos es negativo 
	//------------------------------------------------
	bool IsPositive();

	//Find the the firs entry, with a value: sValue
	//---------------------------------
	//-sValue: Value for search for
	//---------------------------------
	bool Find(T sValue);

	//Find the the firs entry, with value: sValue
	//---------------------------------
	//-sValue: Value for search for
	//-iIndex: Index of the firs element, with value: sValue
	//---------------------------------
	bool FindFirst(T sValue, UINT* iIndex );

	//Find the the last entry, with value: sValue
	//---------------------------------
	//-sValue: Value for search for
	//-iIndex: Index of the last element, with value: sValue
	//---------------------------------
	bool FindLast(T sValue, UINT* iIndex );

	//Find all the element wiht value: sValue, if sValues do not exist, indexList is empty
	//---------------------------------
	//-sValue: Value to search for
	//-iIndex: Index list wiht values 
	//---------------------------------
	bool FindAll(T sValue, CArray_<T>* indexList );

	
	//Test if dataList is contained array
	//---------------------------------
	//-dataList: Array to test for
	//---------------------------------
	bool IsContent(CArray_<T>* dataList);

	//Interchange the element in the position iPos by the element with index jPos
	//---------------------------------
	//-iPos: Index list ton interchange
	//-jPos: Index list ton interchange 
	//---------------------------------
	void Interchange(UINT iPos, UINT jPos);

	//Sort the array in asending mode
	//---------------------------------
	void QuickSort_Ascending();

	//Sort the array in asending mode for a index list
	//---------------------------------
	//-a: Index List
	//---------------------------------
	void QuickSort_Ascending(CArray_<UINT> *a);
	
	//Sort the array in desending mode
	//---------------------------------
	void Quicksort_Desending();

	//Sort the array in desending mode for a index list
	//---------------------------------
	//-a: Index List
	//---------------------------------
	void Quicksort_Desending(CArray_<UINT> *a);

	const T& operator [] (const UINT index) const
	{
		assert( index >= 0 && index < m_nSize); 
		return m_array[index];
	}
	
	T& operator [] (const UINT index)
	{
		assert( index >= 0 && index < m_nSize); 
		return m_array[index];
	}
	
	CArray_& operator = (const CArray_& obj) 
	{
		if(m_nSize == obj.m_nSize)
		{
			#ifdef _CMP_VST_WIN
			CopyMemory(&m_array[0], obj.m_array, obj.m_nSize * sizeof(T));
			#endif 

			#ifdef _CMP_GCC_WIN
			memcpy(&m_array[0], obj.m_array, obj.m_nSize * sizeof(T));
			#endif
		}
		else
		{
			if(SetSize_Private(obj.m_nSize))
			{
				#ifdef _CMP_VST_WIN
				CopyMemory(&m_array[0], obj.m_array, obj.m_nSize * sizeof(T));
				#endif 

				#ifdef _CMP_GCC_WIN
				memcpy(&m_array[0], obj.m_array, obj.m_nSize * sizeof(T));
				#endif
			}
		}
		return *this;
	}

	bool operator == (const CArray_& obj)
	{
		if(m_nSize==obj.m_nSize)
		{
			for(UINT i=0; i<m_nSize; i++)
			{
				if(m_array[i]!=obj[i])
				{
					return false;
					break;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator != (const CArray_& obj)
	{
		if(m_nSize==obj.m_nSize)
		{
			for(UINT i=0; i<m_nSize; i++)
			{
				if(m_array[i]!=obj[i])
				{
					return true;
					break;
				}
			}
			return false;
		}
		else
		{
			return true;
		}
	}

	CArray_& operator ++ ()
	{
		for(UINT i=0; i<m_nSize; i++)
		{
			m_array[i]++;
		}
		return *this;
	}

	CArray_& operator -- ()
	{
		for(UINT i=0; i<m_nSize; i++)
		{
			m_array[i]--;
		}
		return *this;
	}
	CArray_& operator += (CArray_& obj)
	{
		if(m_nSize > obj.m_nSize)
		{
			for(UINT i=0; i<obj.m_nSize; i++)
			{
				m_array[i]+=obj[i];
			}
		}
		else
		{
			for(UINT i=0; i<m_nSize; i++)
			{
				m_array[i]+=obj[i];
			}
		}
		return *this;
	}

	CArray_& operator -= (CArray_& obj)
	{
		if(m_nSize > obj.m_nSize)
		{
			for(UINT i=0; i<obj.m_nSize; i++)
			{
				m_array[i]-=obj[i];
			}
		}
		else
		{
			for(UINT i=0; i<m_nSize; i++)
			{
				m_array[i]-=obj[i];
			}
		}
		return *this;
	}

	#ifdef _CONSOLE_HANDLE
	friend ostream& operator << (ostream &out, CArray_<T> &cArray)
	{
		for(UINT i=0; i<cArray.m_nSize; i++)
		{
			out << cArray[i];
		}
		return out;
	}
	friend istream& operator >> (istream &in, CArray_<T> &cArray)
	{
		for(UINT i=0; i<cArray.m_nSize; i++)
		{
			in << cArray[i];
		}
		return in;
	}
	#endif

	
	//Friend functions
	//--------------------------------------------------------------------------------------------------------------------------------------
	#ifdef _CMP_VST_WIN
	template <typename T> friend void CArray_MinSet(const CArray_<T>* a, CArray_<T>* r);
	template <typename T> friend void CArray_Union_Min(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Union_Max(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Intersection(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b, UINT seed);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, T a, T b);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, UINT seed, T a, T b);
	template <typename T> friend void CArray_Diferencia_Min(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Diferencia_Max(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);
	#endif

	#ifdef _CMP_GCC_WIN //Not Windows Compiling

	template <typename T> friend void CArray_MinSet(const CArray_<T>* a, CArray_<T>* r);
	template <typename T> friend void CArray_Union_Min(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Union_Max(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Intersection(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b, UINT seed);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, T a, T b);
	template <typename T> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, UINT seed, T a, T b);
	template <typename T> friend void CArray_Diferencia_Min(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);
	template <typename T> friend void CArray_Diferencia_Max(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);

	/*friend void CArray_MinSet();
	friend void CArray_Union_Min();
	friend void CArray_Union_Max();
	friend void CArray_Intersection();
	friend void CArray_RandomFill();
	friend void CArray_RandomFill();
	friend void CArray_RandomFill();
	friend void CArray_RandomFill();
	friend void CArray_Diferencia_Min();
	friend void CArray_Diferencia_Max();*/

	#endif

	//Friend classes
	//--------------------------------------------------------------------------------------------------------------------------------------
	template <typename T> friend class CQList_;

protected:
	//Pointer to array buffer
	T* m_array;
	//Size of the array
	UINT m_nSize;
	//Flag for calling the destructor
	bool m_bUseDestructor;
	
	bool SetSize_Private(UINT nNewSize);

private:

	UINT _Qnum;
	void _QAs(UINT inicio, UINT fin)
	{
		int i=inicio,j=fin,k=int((fin+inicio)/2.0);
		T a;	
		if(_Qnum==0)
		{
			_QAs_x();
		}
		if((j-i)<=1)
		{
			if(j!=i)
			{
				if(m_array[inicio]>m_array[fin])
				{
					a=m_array[inicio];
					m_array[inicio]=m_array[fin];
					m_array[fin]=a;
					_Qnum++;
				}
			}
		}
		else
		{  
			while(i<k || j>k)
			{
				while(m_array[i]<=m_array[k] && i<k )
				{
					i++;
				}
				while(m_array[j]>=m_array[k] && j>k)
				{
					j--;
				}
				if(i==j)
				{
					break;	
				}				
				if(i<k && j>k)
				{
					Interchange(i,j);
					_Qnum++;
				}
				else
				{
					if(i==k)
					{
						a=m_array[j];
						Insert(k,a);
						Remove(j+1);
						k++;
						j++;
						_Qnum++;
					}
					if(j==k)
					{
						Insert(k+1,m_array[i]);
						Remove(i);
						k--;
						i--;
						_Qnum++;
					}
				}
				j--;
				i++;
			}
			_QAs(inicio,k);
			_QAs(k+1,fin);
		}       
	}
	void _QAs_x()
	{
		UINT i=0,j=0;
		while(i<m_nSize)
		{
			if(m_array[m_nSize-1]<=m_array[j])
			{
				Add(m_array[j]);
				Remove(j);
				j--;
			}
			i++;
			j++;
		}
	}


	void _QDes(UINT inicio, UINT fin)
{
	int i=inicio,j=fin,k=int((fin+inicio)/2.0);
	T a;	
	if(_Qnum==0)
	{
		_QDes_x();
	}
	if((j-i)<=1)
	{
		if(j!=i)
		{
			if(m_array[inicio]<m_array[fin])
			{
				a=m_array[inicio];
				m_array[inicio]=m_array[fin];
				m_array[fin]=a;
				_Qnum++;
			}
		}
	}
	else
	{  
		while(i<k || j>k)
		{
			while(m_array[i]>=m_array[k] && i<k )
			{
				i++;
			}
			while(m_array[j]<=m_array[k] && j>k)
			{
				j--;
			}
			if(i==j)
			{
				break;	
			}				
			if(i<k && j>k)
			{
				Interchange(i,j);
				_Qnum++;
			}
			else
			{
				if(i==k)
				{
					a=m_array[j];
					Insert(k,a);
					Remove(j+1);
					k++;
					j++;
					_Qnum++;
				}
				if(j==k)
				{
					Insert(k+1,m_array[i]);
					Remove(i);
					k--;
					i--;
					_Qnum++;
					}
			}
			j--;
			i++;
		}
		_QDes(inicio,k);
		_QDes(k+1,fin);
	}       
}

	void _QDes_x()
	{
		UINT i=0,j=0;
		while(i<m_nSize)
		{
			if(m_array[0]>=m_array[j])
			{
				Insert(0,m_array[j]);
				Remove(j+1);
				j--;
			}
			i++;
			j++;
		}
	}
	void _QAsA(UINT inicio, UINT fin, CArray_<UINT> *iOrder)
	{
		int i=inicio,j=fin,k=int((fin+inicio)/2.0);
		UINT a;	
		if(_Qnum==0)
		{
			_QAs_x_A(iOrder);
		}
		if((j-i)<=1)
		{
			if(j!=i)
			{
				if(m_array[(*iOrder)[inicio]]>m_array[(*iOrder)[fin]])
				{
					a=(*iOrder)[inicio];
					(*iOrder)[inicio]=(*iOrder)[fin];
					(*iOrder)[fin]=a;
					_Qnum++;
				}
			}
		}
		else
		{  
			while(i<k || j>k)
			{
				while(m_array[(*iOrder)[i]]<=m_array[(*iOrder)[k]] && i<k )
				{
					i++;
				}
				while(m_array[(*iOrder)[j]]>=m_array[(*iOrder)[k]] && j>k)
				{
					j--;
				}
				if(i==j)
				{
					break;	
				}				
				if(i<k && j>k)
				{
					a=(*iOrder)[i];
					(*iOrder)[i]=(*iOrder)[j];
					(*iOrder)[j]=a;
					//Interchange(i,j);
					_Qnum++;
				}
				else
				{
					if(i==k)
					{
						a=(*iOrder)[j];
						(*iOrder).Insert(k,a);
						(*iOrder).Remove(j+1);
						k++;
						j++;
						_Qnum++;
					}
					if(j==k)
					{
						(*iOrder).Insert(k+1,(*iOrder)[i]);
						(*iOrder).Remove(i);
						k--;
						i--;
						_Qnum++;
					}
				}
				j--;
				i++;
			}
			_QAsA(inicio,k,iOrder);
			_QAsA(k+1,fin,iOrder);
		}       
	}
	void _QAs_x_A(CArray_<UINT> *a)
	{
		UINT i=0,j=0;
		while(i<m_nSize)
		{
			if(m_array[(*a)[m_nSize-1]]<=m_array[(*a)[j]])
			{
				a->Add((*a)[j]);
				a->Remove(j);
				j--;
			}
			i++;
			j++;
		}
	}
	void _QDesA(UINT inicio, UINT fin, CArray_<UINT> *iOrder)
	{
		int i=inicio,j=fin,k=int((fin+inicio)/2.0);
		UINT a;	
		if(_Qnum==0)
		{
			_QDes_x_A(iOrder);
		}
		if((j-i)<=1)
		{
			if(j!=i)
			{
				if(m_array[(*iOrder)[inicio]]<m_array[(*iOrder)[fin]])
				{
					a=(*iOrder)[inicio];
					(*iOrder)[inicio]=(*iOrder)[fin];
					(*iOrder)[fin]=a;
					_Qnum++;
				}
			}
		}
		else
		{  
			while(i<k || j>k)
			{
				while(m_array[(*iOrder)[i]]>=m_array[(*iOrder)[k]] && i<k )
				{
					i++;
				}
				while(m_array[(*iOrder)[j]]<=m_array[(*iOrder)[k]] && j>k)
				{
					j--;
				}
				if(i==j)
				{
					break;	
				}				
				if(i<k && j>k)
				{
					a=(*iOrder)[i];
					(*iOrder)[i]=(*iOrder)[j];
					(*iOrder)[j]=a;
					//Interchange(i,j);
					_Qnum++;
				}
				else
				{
					if(i==k)
					{
						a=(*iOrder)[j];
						(*iOrder).Insert(k,a);
						(*iOrder).Remove(j+1);
						k++;
						j++;
						_Qnum++;
					}
					if(j==k)
					{
						(*iOrder).Insert(k+1,(*iOrder)[i]);
						(*iOrder).Remove(i);
						k--;
						i--;
						_Qnum++;

					}
				}
				j--;
				i++;
			}
			_QDesA(inicio,k,iOrder);
			_QDesA(k+1,fin,iOrder);
		}       
	}		
	void _QDes_x_A(CArray_<UINT> *a)
	{
		UINT i=0,j=0;
		while(i<m_nSize)
		{
			if(m_array[(*a)[0]]>=m_array[(*a)[j]])
			{
				a->Insert(0,(*a)[j]);
				a->Remove(j+1);
				j--;
			}
			i++;
			j++;
		}
	}
};


typedef CArray_<UINT> CARRAY_UINT;

template <typename T>CArray_<T>::CArray_(void)
{
	m_nSize = 0;
	m_array = NULL;
	m_bUseDestructor = false;
}

template <typename T>CArray_<T>::CArray_(const CArray_<T> &nCArray)
{
	if( nCArray.m_nSize >= 1)
	{
		//Free the memory for the array
		m_array = NULL;
		m_nSize = nCArray.m_nSize;

		try
		{
			m_array = (T*)calloc(m_nSize, m_nSize * sizeof(T));
		}
		catch(char* error)
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
	if(m_array != NULL)
	{
		//First we call the destructor for the data objects...
		if(m_bUseDestructor)
		{
			for(UINT i=0; i<m_nSize; i++)
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
	if((nNewSize < 0 || (nNewSize > (INT_MAX/sizeof(T)))))
	{
		//Do nothing...
		#ifdef 	CARRAY_DEBUG_CONSOLE
		#endif
		return false;
	}
	else if(nNewSize == 0)
	{
		if(m_nSize != 0)
		{
			#ifdef 	CARRAY_DEBUG_CONSOLE
			#endif

			//First we call the destructor for the data objects...
			if(m_bUseDestructor)
			{
				for(UINT i=0; i<m_nSize; i++)
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
	else if(nNewSize < m_nSize)
	{
		#ifdef 	CARRAY_DEBUG_CONSOLE
		/*cout << endl;
		cout << " (nNewSize < m_nSize) " << endl;
		cout << endl;*/
		#endif

		////Erase the rest of the elements in the array,...
		//First we call the destructor for the data objects...
		if(m_bUseDestructor)
		{
			for(UINT i=nNewSize; i<m_nSize; i++)
			{
				m_array[i].~T();
			}
		}
		//Now resize the array...
		m_array = (T*)realloc( m_array, nNewSize * sizeof(T));
		if(m_array == NULL)
		{
			return false;
		}
		m_nSize = nNewSize;
		return true;
	}
	else if(nNewSize > m_nSize)
	{
		#ifdef 	CARRAY_DEBUG_CONSOLE
		/*cout << endl;
		cout << " (nNewSize > m_nSize) " << endl;
		cout << endl;*/
		#endif
	
		//Resize the memory buffer
		m_array = (T*)realloc( m_array, nNewSize * sizeof(T));
		if(m_array == NULL)
		{
			return false;
		}

		//Make and initialize the new elements
		for(UINT i=m_nSize; i<nNewSize; i++)
		{
			::new (&m_array[i]) T;
			m_array[i] = T();
		}
		m_nSize = nNewSize;
		
		return true;
	}
	return false;
}

template <typename T> void CArray_<T>::GetData(T *mDataArray)
{
	if(mDataArray != NULL)
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
	if(m_nSize==0)
	{
		m_array = (T*)calloc(m_nSize + 1, m_nSize + 1 * sizeof(T));
		if(m_array != NULL)
		{
			::new (&m_array[m_nSize]) T;
			//m_array[m_nSize] = T();
			m_nSize++; 
		}
	}
	else
	{
		m_array = (T*)realloc( m_array, (m_nSize + 1) * sizeof(T));
		if(m_array != NULL)
		{
			::new (&m_array[m_nSize]) T;
			//m_array[m_nSize] = T();
			m_nSize++; 
		}
	}
}
template <typename T> void CArray_<T>::Add(const T &value)
{
	if(m_nSize == 0)
	{
		m_array = (T*)calloc(m_nSize + 1, m_nSize + 1 * sizeof(T));
		if(m_array != NULL)
		{
			m_array[m_nSize] = value;
			m_nSize++;
		}
	}
	else
	{
		m_array = (T*)realloc( m_array, (m_nSize + 1) * sizeof(T));
		if(m_array != NULL)
		{
			::new (&m_array[m_nSize]) T;
			m_array[m_nSize] = value;
			m_nSize++; 
		}
	}
}
template <typename T> void CArray_<T>::Add(T *mDataArray, const UINT nDataSize)
{
	if(SetSize_Private(m_nSize + nDataSize))
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
	if(SetSize_Private(m_nSize + mDataArray->m_nSize))
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
	if(nIndex >= 0)
	{
		if(nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			SetSize_Private(m_nSize +1);

			//Move the elements of the array...
			#ifdef _CMP_VST_WIN
			MoveMemory(&m_array[nIndex + 1], &m_array[nIndex], ((m_nSize-1) - nIndex) * sizeof(T));
			#endif

			#ifdef _CMP_GCC_WIN
			memmove(&m_array[nIndex + 1], &m_array[nIndex], ((m_nSize-1) - nIndex) * sizeof(T));
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
	if(nIndex >= 0)
	{
		if(nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			if(SetSize_Private(m_nSize + nDataSize))
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
	if(nIndex >= 0)
	{
		if(nIndex < m_nSize)
		{
			//GrowUp the array buffer...
			if(SetSize_Private(m_nSize + mDataArray->m_nSize))
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
	for(UINT i=0; i<m_nSize; i++)
	{
		if(m_array[i] == sValue)
		{
			return  true;
		}
	}
	return false;
}
template <typename T> bool CArray_<T>::FindFirst(T sValue, UINT *iIndex)
{
    *iIndex = -1;
    for(UINT i=0; i<m_nSize; i++)
	{
		if(m_array[i] == sValue)
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
    for(UINT i=m_nSize -1; i>0; i--)
	{
		if(m_array[i] == sValue)
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

	for(UINT i=0; i<m_nSize; i++)
	{
		if(m_array[i] == sValue)
		{
			res = true;
			indexList->Add(i);
		}
	}
	return res;
}



template <typename T> void CArray_<T>::GetMaxValue(T *value, UINT *index)
	{
		*index=0;
		*value=m_array[0];
		for(UINT i=1; i<m_nSize; i++)
		{
			if(m_array[i]>*value)
			{
				*value=m_array[i];
				*index=i;
			}
		}		
	}
template <typename T> void CArray_<T>::GetMaxValue( UINT *index)
	{
		T a;
		*index=0;
		a=m_array[0];
		for(UINT i=1; i<m_nSize; i++)
		{
			if(m_array[i]>a)
			{
				a=m_array[i];
				*index=i;
			}
		}		
	}
template <typename T> void CArray_<T>::GetMinValue(T *value, UINT *index)
{
	*index=0;
	*value=m_array[0];
	for(UINT i=1; i<m_nSize; i++)
	{
		if(m_array[i]<*value)
		{
			*value=m_array[i];
			*index=i;
		}
	}		
}
template <typename T> void CArray_<T>::GetMinValue(UINT *index)
{
	T a;
	*index=0;
	a=m_array[0];
	for(UINT i=1; i<m_nSize; i++)
	{
		if(m_array[i]<a)
		{
			a=m_array[i];
			*index=i;
		}
	}		
}
template <typename T> bool CArray_<T>::IsNegative()
{
	for (register int i = 0; i<m_nSize; i++)
	{
		if(m_array[i]>0)
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
		if(m_array[i]<0)
		{
			return false;
		}
	}
	return true;
}
template <typename T> void CArray_<T>::PrintToConsole()
{
	for(register UINT i=0; i<m_nSize; i++)
	{
		cout << m_array[i] << endl;
	}
}
template <typename T> void CArray_<T>::Remove(const UINT nIndex)
{
	if(nIndex >= 0)
	{
		if(nIndex < m_nSize)
		{
			if(m_bUseDestructor)
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
	for(UINT i=0; i<indexList->m_nSize; i++)
	{
		if(indexList->m_array[i] >= 0)
		{
			if(indexList->m_array[i] < m_nSize)
			{
				if(m_bUseDestructor)
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
	while(m_nSize>0)
	{
		q.Add(m_array[m_nSize-1]);
		Remove(m_nSize-1);
	}
	*this = q;
}
template <typename T> T& CArray_<T>::GetLastValue()
{
	return m_array[m_nSize-1];
}
template <typename T> void CArray_<T>::SetData(T *mDataArray, const UINT nDataSize)
{
	if(nDataSize > 0)
	{
		//First we try to release the old array...
		if(m_bUseDestructor)
		{
			for(UINT i=0; i<m_nSize; i++)
			{
				m_array[i].~T();
			}
		}

		if(m_array != NULL)
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
		if(m_nSize<jMax)
		{
			obj.Resize(m_nSize-jMin+1);
			for(UINT i=jMin; i<m_nSize; i++)
			{
				obj[i-jMin]=m_array[i];
			}
		}
		else
		{
			obj.Resize(jMax-jMin+1);
			for(UINT i=jMin; i<=jMax; i++)
			{
				obj[i-jMin]=m_array[i];
			}
		}
	}
template <typename T> void CArray_<T>::SubArray(CArray_<T>& indexList, CArray_<T>& obj)
{
	obj.Resize(indexList.m_nSize);
	for(register int i=0; i<indexList.m_nSize; i++)
	{
		obj[i]=(m_array[indexList[i]]);
	}
}
template <typename T> void CArray_<T>::SubArray(UINT* indexList, UINT listSize, CArray_<T>& obj)
{
	obj.Resize(*indexList);
	for(UINT i=0; i<listSize; i++)
	{
		obj[i]=m_array[indexList[i]];
	}
}


template <typename T> void CArray_<T>::Zero()
{
	ZeroMemory(m_array, m_nSize * sizeof(T));
}
template <typename T> bool CArray_<T>::IsContent(CArray_<T> *dataList)
{
	for(UINT i=0; i<dataList->m_nSize; i++)
	{
		if(!Find((*dataList)[i]))
		{
			return false;
		}
	}
	return true;
}
template <typename T> void CArray_<T>::Interchange(UINT iPos, UINT jPos)
{
	T a;
	a=m_array[iPos];
	m_array[iPos]=m_array[jPos];
	m_array[jPos]=a;
}
template <typename T> void CArray_<T>::QuickSort_Ascending()
{
	_Qnum = 0;
	_QAs(0,m_nSize-1);
}
template <typename T> void CArray_<T>::QuickSort_Ascending(CArray_<UINT> *a)
{
	_Qnum = 0;
	a->Resize(m_nSize);
	for(register UINT i=0; i<m_nSize; i++)
	{
		(*a)[i]=i;
	}
	_QAsA(0,m_nSize-1,a);
}
template <typename T> void CArray_<T>::Quicksort_Desending()
{
	_Qnum = 0;
	_QDes(0,m_nSize-1);
}



template <typename T> void CArray_<T>::Quicksort_Desending(CArray_<UINT> *a)
{
	_Qnum = 0;
	a->Resize(m_nSize);
	for(register UINT i=0; i<m_nSize; i++)
	{
		(*a)[i]=i;
	}
	_QDesA(0,m_nSize-1,a);
}
//--------------------------------------------------------------------------------------------------------------------------------------
//Static Functions
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T> void CArray_MinSet(const CArray_<T>* a, CArray_<T>* r)
{
	int j=0,k,h=0;
	k=(*a).m_nSize;
	CArray_<T> b;
	*r=*a;
	while(j<k)
	{
		(*r).FindAll((*r)[h],&b);
		for(UINT i=b.m_nSize;i>1;i--)
		{
			(*r).Remove(b[i-1]);
		}
		j=j+b.m_nSize;
		h++;
	}	
}

template <typename T> void CArray_Union_Min(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r)
{
	  bool t;
	  CArray_MinSet(a,r);
	  for(UINT i=0; i<(*b).m_nSize; i++)
	  {
		  if(!r->Find((*b)[i]))
		  {
			  r->Add((*b)[i]);
		  }
	  }
}

template <typename T> void CArray_Union_Max(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r)
{
	  bool t;
	  (*r)=(*a);
	  for(UINT i=0; i<(*b).m_nSize; i++)
	  {
		  r->Add((*b)[i]);
	  }
}

template <typename T> void CArray_Intersection(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r)
{
	CArray_MinSet(a,r);
	int k=0;
	bool bFind = false;
	
	for(UINT i=0; i<=r->m_nSize; i++)
	{
		bFind = false;		
		for(UINT j=0; j<b->m_nSize; j++)
		{
			if((*b)[j] == (*r)[k])
			{
				bFind = true;
				break;
			}
		}		
		if(!bFind)
		{
			r->Remove(k);
		}
		else
		{
			k++;			
		}
	}
}

template <typename T> void CArray_Diferencia_Min(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r)
{
	CArray_MinSet(a,r);
	UINT c=0;
	for(UINT i=0; i<b->m_nSize; i++)
	{
		if(r->FindFirst((*b)[i],&c))
		{
			r->Remove(c);
		}
	}
}

template <typename T> void CArray_Diferencia_Max(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r)
{
	*r=*a;
	UINT c=0;
	for(UINT i=0; i<b->m_nSize; i++)
	{
		if(r->FindFirst((*b)[i],&c))
		{
			while(r->FindFirst((*b)[i],&c))
			{
			r->Remove(c);
			}
		}
	}
}

template <typename T> void CArray_RandomFill( CArray_<T> *obj, T a, T b)
{
	float t;
	srand(time(NULL));
	for(register int i=0; i<obj->m_nSize; i++)
	{
		t=float(rand())/float(RAND_MAX);
		(*obj)[i]=a+int(t*(b-a));
	}
}
template <typename T> void CArray_RandomFill( CArray_<T> *obj, T a, T b, UINT seed)
{
	float t;
	srand(seed);
	for(register int i=0; i<obj->m_nSize; i++)
	{
		t=float(rand())/float(RAND_MAX);
		(*obj)[i]=a+int(t*(b-a));
	}
}
template <typename T> void CArray_RandomFill( CArray_<T> *obj, UINT n, T a, T b)
{
	float t;
	(*obj).Resize(n);
	srand(time(NULL));
	for(register int i=0; i<obj->m_nSize; i++)
	{
		t=float(rand())/float(RAND_MAX);
		(*obj)[i]=a+int(t*(b-a));
	}
}
template <typename T> void CArray_RandomFill( CArray_<T> *obj, UINT n, UINT seed, T a, T b)
{
	float t;
	srand(seed);
	(*obj).Resize(n);
	for(register int i=0; i<obj->m_nSize; i++)
	{
		t=float(rand())/float(RAND_MAX);
		(*obj)[i]=a+int(t*(b-a));
	}
}
template < typename T> void CArray_GetDecreasingSets(CArray_<CArray_<UINT> > *sets, UINT numElements, UINT longString)
{
	CArray_<UINT> a;
	if(numElements>=longString)
	{
		for(register UINT k=0; k<(1+numElements-longString); k++)
		{
			a.Add();
			a->GetLastValue() = numElements-k;
			CArray_GetDecreasingSetsAux(sets, &longString, &a, longString-1);
			a.RemoveAll();
		}
	}
	
}
template < typename T> void CArray_GetDecreasingSetsAux(CArray_<CArray_<UINT>> *sets, UINT *longString, CArray_<UINT> *a, int falta)
{
	UINT b,c;
	if(falta > 0)
	{
		b = a->GetSize();
		c=falta;
		for(register UINT j=(*a)[a->GetSize()-1]-1; j>0; j--)
		{
			a->Add();
			a->GetLastValue() = j;
			if(a->GetSize()==*longString)
			{
				 sets->Add(*a);
			}
			falta--;
			CArray_GetDecreasingSetsAux(sets, longString, a, falta);
			a->Resize(b);
			falta=c;
		}
	}
}
#endif