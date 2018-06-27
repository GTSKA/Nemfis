#ifndef CArray_H
#define CArray_H

#define CARRAY_DEBUG_CONSOLE
#define CARRAY_VERSION 1.63

#include "Common_.h"
#ifdef CUDA_PROJECT
#include <cutil_inline.h>
#include <cuda_runtime_api.h>
#endif
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
	UINT GetSize() { return  m_nSize; } 

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

	void GetMaxAbsoluteValue(T* value, UINT *index);
	
	#ifdef CUDA_PROJECT
	//Copy Array info to the Device
	//---------------------------------
	//-a: Index List
	//-getset: indicates if get or set info
	//---------------------------------
	void CUDA_memcpy(T** a,bool getset = true);
	#endif

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

	template <typename U> friend void CArray_MinSet(const CArray_<T>* a, CArray_<T>* r);
	template <typename U> friend void CArray_Union_Min(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename U> friend void CArray_Union_Max(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename U> friend void CArray_Intersection(const CArray_<T>* a, const CArray_<T>* b, CArray_<T>* r);
	template <typename U> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b);
	template <typename U> friend void CArray_RandomFill( CArray_<T> *obj, T a, T b, UINT seed);
	template <typename U> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, T a, T b);
	template <typename U> friend void CArray_RandomFill( CArray_<T> *obj,UINT n, UINT seed, T a, T b);
	template <typename U> friend void CArray_Diferencia_Min(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);
	template <typename U> friend void CArray_Diferencia_Max(const CArray_<T>* a,const CArray_<T>* b, CArray_<T>* r);
	template <typename U> friend void CArray_GetDecreasingSetsAux( CArray_< CArray_<UINT> > *sets, UINT *longString, CArray_<UINT> *a, int falta);

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
	template <typename U> friend class CQList_;
	//template <typename V> friend class CARRAY_GROUP;

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

template <typename T> class ARRAY_GROUP
{
public:
	CArray_<T> arrayD;

	ARRAY_GROUP()
	{
	}

	
	ARRAY_GROUP(const CArray_<T>& arraY)
	{
		arrayD = arraY;
	}
	
	virtual ~ARRAY_GROUP(void);

	//virtual void Dispose();

	ARRAY_GROUP& operator = (const ARRAY_GROUP& obj)
	{
		arrayD = obj.arrayD;
		return *this;		
	}

	ARRAY_GROUP& operator = (const T& obj)
	{
		arrayD.RemoveAll();
		arrayD.Add(obj);
		return *this;
	}

	ARRAY_GROUP& operator += (ARRAY_GROUP& obj)
	{
		for(register UINT i=0; i < obj.arrayD.GetSize(); i++)
		{
			arrayD.Add(obj.arrayD[i]);
		}
		return *this;
	}
	ARRAY_GROUP& operator += (const T& obj) 
	{
		arrayD.Add(obj);
		return *this;
	}

	ARRAY_GROUP operator * (const ARRAY_GROUP& obj) 
	{
		ARRAY_GROUP<T> mCopy;
		for(register UINT i=0; i<arrayD.GetSize();i++)
		{
			mCopy.arrayD.Add(arrayD[obj.arrayD[i]-1]);
		}
		return mCopy;
	}


	bool operator == (const ARRAY_GROUP& obj) 
	{
		if(arrayD==obj.arrayD)
		{
			return true;
		}
		return false;
	}

	bool operator != (const ARRAY_GROUP& obj) 
	{
		if(arrayD!=obj.arrayD)
		{
			return true;
		}
		return false;
	}

	operator int()
	{
		for(register UINT i=0; i<arrayD.GetSize(); i++)
		{
			arrayD[i]=(T)(int)arrayD[i];
		}
		return *this;
	}


	friend ostream& operator << (ostream &out, ARRAY_GROUP &dArray)
	{
		for(register UINT i=0; i < dArray.arrayD.GetSize(); i++)
		{
			out << dArray.arrayD[i];
			if(i != dArray.arrayD.GetSize()-1)
			{
				out << ",";
			}
			else
			{
				out << " ";
			}
		}
		return out;
	}
};


#endif
