#include "CMatrix_.h"

template < typename T > CMatrix_<T>::CMatrix_()
{
	Matrix = NULL;

	m_bUseDestructor = false;

	I = T(1.0);
	Z = (T)0;
	N = (T)(-1);


	m_bFromFile = false;

	SetSize(1, 1);
}

template < typename T > CMatrix_<T>::CMatrix_(UINT n)
{
	Matrix = NULL;

	m_bUseDestructor = false;

	I = (T)1;
	Z = (T)0;
	N = (T)(-1);

	m_bFromFile = false;

	SetSize(n, n);
}

template < typename T > CMatrix_<T>::CMatrix_(UINT n, UINT m)
{
	Matrix = NULL;

	m_bUseDestructor = false;

	I = (T)1;
	Z = (T)0;
	N = (T)(-1);

	m_bFromFile = false;

	SetSize(n, m);
}

template < typename T > CMatrix_<T>::CMatrix_(const CMatrix_<T> &pCMatrix)
{
	Matrix = NULL;
	m_bUseDestructor = pCMatrix.m_bUseDestructor;
	m_bFromFile = pCMatrix.m_bFromFile;

	if (m_bFromFile)
	{

	}
	else
	{
		SetSize(pCMatrix.m_nRows, pCMatrix.m_nCols);
		for (UINT i = 0; i<m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(Matrix[i], pCMatrix.Matrix[i], pCMatrix.m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(Matrix[i], pCMatrix.Matrix[i], pCMatrix.m_nCols * sizeof(T));
#endif
		}
	}

	I = pCMatrix.I;
	Z = pCMatrix.Z;
	N = pCMatrix.N;
}

template < typename T > CMatrix_<T>::~CMatrix_()
{
	Dispose();
}
template < typename T > void CMatrix_<T>::SetSize(UINT n, UINT m)
{
	Dispose();

	m_nRows = n;
	m_nCols = m;

	m_nLastRowIndex = m_nRows - 1;
	m_nLastColumnIndex = m_nCols - 1;

	Matrix = (T**)calloc(m_nRows, sizeof(T*));
	for (UINT i = 0; i<m_nRows; i++)
	{
		Matrix[i] = (T*)calloc(m_nCols, sizeof(T));
	}
}
template < typename T > bool CMatrix_<T>::Resize(UINT newRowSize, UINT newColSize)
{
	bool res = false;
	if (newRowSize > 0 || newColSize > 0)
	{
		if (newRowSize == m_nRows && newColSize == m_nCols)
		{
			return true;
		}
		//Expanding the rows....
		if (newRowSize == 0)
		{
			newRowSize = m_nRows;
		}
		if (newColSize == 0)
		{
			newColSize = m_nCols;
		}

		if (newRowSize > m_nRows)
		{
			if ((Matrix = (T**)realloc(Matrix, newRowSize * sizeof(T*))) != NULL)
			{
				if (newColSize > m_nCols)
				{

					for (UINT i = 0; i<m_nRows; i++)
					{
						Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
					}
					for (UINT i = m_nRows; i<newRowSize; i++)
					{
						Matrix[i] = (T*)calloc(newColSize, sizeof(T));
					}
					for (register UINT i = 0; i < m_nRows; i++)
					{
						for (register UINT j = m_nCols; j < newColSize; j++)
						{
							::new (&Matrix[i][j]) T;
							Matrix[i][j] = T();
						}
					}
					m_nCols = newColSize;
					m_nLastColumnIndex = m_nCols - 1;
					res = true;
				}
				else if (newColSize < m_nCols)
				{
					for (UINT i = 0; i<m_nRows; i++)
					{
						if (m_bUseDestructor)
						{
							for (UINT j = newColSize; j<m_nCols; j++)
							{
								Matrix[i][j].~T();
							}
						}
						Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
					}
					for (UINT i = m_nRows; i<newRowSize; i++)
					{
						Matrix[i] = (T*)calloc(newColSize, sizeof(T));
					}
					for (register UINT i = 0; i < newRowSize; i++)
					{
						for (register UINT j = m_nCols; j < newColSize; j++)
						{
							::new (&Matrix[i][j]) T;
							Matrix[i][j] = T();
						}
					}
					m_nCols = newColSize;
					m_nLastColumnIndex = m_nCols - 1;
					res = true;
				}
				else
				{
					for (UINT i = m_nRows; i<newRowSize; i++)
					{
						Matrix[i] = (T*)calloc(m_nCols, sizeof(T));
					}
				}
				m_nRows = newRowSize;
				m_nLastRowIndex = m_nRows - 1;
				res = true;
			}
		}
		if (newRowSize < m_nRows)
		{
			if (m_bUseDestructor)
			{
				//Call the destructor for all the elements to be erased
				for (UINT i = newRowSize; i<m_nRows; i++)
				{
					for (UINT j = 0; j<m_nCols; j++)
					{
						Matrix[i][j].~T();
					}
				}
			}

			if ((Matrix = (T**)realloc(Matrix, newRowSize * sizeof(T*))) != NULL)
			{
				m_nRows = newRowSize;
				m_nLastRowIndex = m_nRows - 1;
				res = true;

				if (newColSize > m_nCols)
				{
					for (UINT i = 0; i<m_nRows; i++)
					{
						Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
					}
					for (register UINT i = 0; i < newRowSize; i++)
					{
						for (register UINT j = m_nCols; j < newColSize; j++)
						{
							::new (&Matrix[i][j]) T;
							Matrix[i][j] = T();
						}
					}
					m_nCols = newColSize;
					m_nLastColumnIndex = m_nCols - 1;
					res = true;
				}
				else if (newColSize < m_nCols)
				{
					for (UINT i = 0; i<m_nRows; i++)
					{
						if (m_bUseDestructor)
						{
							for (UINT j = newColSize; j<m_nCols; j++)
							{
								Matrix[i][j].~T();
							}
						}
						Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
					}

					m_nCols = newColSize;
					m_nLastColumnIndex = m_nCols - 1;
					res = true;
				}
				else
				{
					for (UINT i = m_nRows; i<newRowSize; i++)
					{
						Matrix[i] = (T*)calloc(m_nCols, sizeof(T));
					}
				}
			}
		}
		if (m_nRows == newRowSize)
		{
			if (newColSize > m_nCols)
			{
				for (UINT i = 0; i<m_nRows; i++)
				{
					Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
				}
				for (register UINT i = 0; i < m_nRows; i++)
				{
					for (register UINT j = m_nCols; j < newColSize; j++)
					{
						::new (&Matrix[i][j]) T;
						Matrix[i][j] = T();
					}
				}
				m_nCols = newColSize;
				m_nLastColumnIndex = m_nCols - 1;
				res = true;
			}
			else if (newColSize < m_nCols)
			{
				for (UINT i = 0; i<m_nRows; i++)
				{
					if (m_bUseDestructor)
					{
						for (UINT j = newColSize; j<m_nCols; j++)
						{
							Matrix[i][j].~T();
						}
					}
					Matrix[i] = (T*)realloc(Matrix[i], newColSize * sizeof(T));
				}
				m_nCols = newColSize;
				m_nLastColumnIndex = m_nCols - 1;
				res = true;
			}
		}
	}

	return res;
}

template < typename T > void CMatrix_<T>::Dispose()
{

	if (Matrix != NULL)
	{
		for (UINT i = 0; i<m_nRows; i++)
		{
			free(Matrix[i]);
		}

		free(Matrix);

		//delete[] Matrix;

		Matrix = NULL;
	}

}


template < typename T > void CMatrix_<T>::PrintToConsole()
{
	if (Matrix != NULL)
	{
		for (UINT i = 0; i<m_nRows; i++)
		{
			for (UINT j = 0; j<m_nCols; j++)
			{
				cout << Matrix[i][j] << "\t";
				//printf(" %lf \t ", Matrix[i][j]);
			}
			cout << endl;
		}
	}
}


template < typename T > void CMatrix_<T>::PrintfToConsole()
{
	char mat_string[255] = { 0 };

	if (Matrix != NULL)
	{
		for (UINT i = 0; i<m_nRows; i++)
		{
			for (UINT j = 0; j<m_nCols; j++)
			{
				long double k = long double(Matrix[i][j]);
				//printf(" %0.16lf\t", k);
				sprintf(mat_string, " %.16Lf", k);
				printf(" %s\t ", mat_string);
			}
			cout << endl;
		}
	}
}

template < typename T > UINT CMatrix_<T>::GetRowSize()
{
	return m_nRows;
}

template < typename T > UINT CMatrix_<T>::GetRowSize() const
{
	return m_nRows;
}


template < typename T > UINT CMatrix_<T>::GetColumnSize()
{
	return m_nCols;
}

template < typename T > UINT CMatrix_<T>::GetColumnSize() const
{
	return m_nCols;
}

template < typename T > UINT CMatrix_<T>::GetLastRowIndex()
{
	return m_nLastRowIndex;
}
template < typename T > UINT CMatrix_<T>::GetLastRowIndex() const
{
	return m_nLastRowIndex;
}
template < typename T > UINT CMatrix_<T>::GetLastColumnIndex()
{
	return m_nLastColumnIndex;
}
template < typename T > UINT CMatrix_<T>::GetLastColumnIndex() const
{
	return m_nLastColumnIndex;
}
template < typename T > void CMatrix_<T>::ToString(std::string *str, UINT iRow, UINT iCol)
{
	ostringstream oss;
	oss << Matrix[iRow][iCol];
	*str = oss.str();
}
template < typename T > void CMatrix_<T>::ToString(char *str, UINT iRow, UINT iCol)
{
	ostringstream oss;
	oss << Matrix[iRow][iCol];
#ifdef _CMP_VST_WIN
	CopyMemory(str, oss.str().data(), oss.str().length() * sizeof(char));
#endif
#ifdef _CMP_GCC_WIN
	memcpy(str, oss.str().data(), oss.str().length() * sizeof(char));
#endif
	str[oss.str().length()] = '\0';
}
template < typename T > void CMatrix_<T>::FromString(std::string *str, UINT iRow, UINT iCol)
{
	if (typeid(T) == typeid(unsigned int) || typeid(T) == typeid(int) || typeid(T) == typeid(short))
	{
		Matrix[iRow][iCol] = ::atoi(str->data());
	}
	else if (typeid(T) == typeid(long))
	{
		Matrix[iRow][iCol] = ::atol(str->data());
	}
	else if (typeid(T) == typeid(char))
	{
		//cout << "FLOAT Type - " << typeid(T).name() << endl;
		if (str->length() > 0)
		{
			Matrix[iRow][iCol] = (*str)[0];
		}
	}
	else
	{
	}
}
template < typename T > void CMatrix_<T>::SetValue(UINT iRow, UINT iCol, CMatrix_<T>* matValues)
{
	for (UINT i = iRow, j = 0; i<min((m_nRows - iRow), (matValues->m_nRows - iRow)); i++, j++)
	{
#ifdef _CMP_VST_WIN
		CopyMemory(&Matrix[i][iCol], &matValues->Matrix[j][0], min((m_nCols - iCol), (matValues->m_nCols - iCol)) * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
		memcpy(&Matrix[i][iCol], &matValues->Matrix[j][0], _min((m_nCols - iCol), (matValues->m_nCols - iCol)) * sizeof(T));
#endif
	}
}
template < typename T > void CMatrix_<T>::FromString(char* str, UINT iRow, UINT iCol)
{
	if (typeid(T) == typeid(unsigned int) || typeid(T) == typeid(int) || typeid(T) == typeid(short))
	{
		Matrix[iRow][iCol] = ::atoi(str);
	}
	else if (typeid(T) == typeid(long))
	{
		Matrix[iRow][iCol] = ::atol(str);
	}
	else if (typeid(T) == typeid(char))
	{
		//cout << "FLOAT Type - " << typeid(T).name() << endl;
		if (strlen(str) > 0)
		{
			Matrix[iRow][iCol] = str[0];
		}
	}
	else
	{
	}
}
template < typename T > void CMatrix_<T>::SaveToTextFile(char *fileName)
{
	ofstream fStream(fileName);
	if (fStream)
	{
		fStream << "#nRows " << m_nRows << endl;
		fStream << "#nCols " << m_nCols << endl;
		fStream << endl;

		for (UINT i = 0; i<m_nRows; i++)
		{
			for (UINT j = 0; j<m_nCols; j++)
			{
				fStream << Matrix[i][j] << " ";
			}
			fStream << endl;
		}
	}
	fStream.close();
}
template < typename T > void CMatrix_<T>::SaveToBinaryFile(char *fileName)
{
	ofstream oFileStream(fileName, ios::out | ios::binary);
	if (oFileStream)
	{
		oFileStream.write((char *)&m_nRows, sizeof(m_nRows));
		oFileStream.write((char *)&m_nCols, sizeof(m_nCols));
		for (UINT i = 0; i<m_nRows; i++)
		{
			oFileStream.write((char *)Matrix[i], m_nCols * sizeof(T));
		}
		//oFileStream << *this;

	}
	oFileStream.close();
}
template < typename T > void CMatrix_<T>::OpenFromTextFile(char *fileName)
{
	ifstream fStream(fileName);

	if (!fStream)
	{
		return;
	}

	UINT nRows = 0;
	UINT nCols = 0;

	// File input
	char strCommand[256] = { 0 };
	//for(UINT t=0; ;)
	do
	{
		fStream >> strCommand;
		//cout << strCommand << endl;
		if (!fStream)
		{
			break;
		}

		if (strcmp(strCommand, "#nRows") == 0)
		{
			fStream >> nRows;
			//cout << "nRows = " << nRows << endl; 
		}

		if (strcmp(strCommand, "#nCols") == 0)
		{
			fStream >> nCols;
			//cout << "nCols = " << nCols << endl; 
		}
	} while (nRows == 0 || nCols == 0);
	fStream.ignore();

	if (nRows > 0 && nCols > 0)
	{
		Resize(nRows, nCols);
		for (UINT i = 0; i<m_nRows; i++)
		{
			for (UINT j = 0; j<m_nCols; j++)
			{
				if (!fStream)
				{
					break;
				}
				fStream >> Matrix[i][j];
			}
		}
	}

	fStream.close();
}

template < typename T > void CMatrix_<T>::OpenFromTextFile(wchar_t *fileName)
{
	ifstream fStream(fileName);

	if (!fStream)
	{
		return;
	}

	UINT nRows = 0;
	UINT nCols = 0;

	// File input
	char strCommand[256] = { 0 };
	do
	{
		fStream >> strCommand;
		//cout << strCommand << endl;
		if (!fStream)
		{
			break;
		}

		if (strcmp(strCommand, "#nRows") == 0)
		{
			fStream >> nRows;
			//cout << "nRows = " << nRows << endl; 
		}

		if (strcmp(strCommand, "#nCols") == 0)
		{
			fStream >> nCols;
			//cout << "nCols = " << nCols << endl; 
		}
	} while (nRows == 0 || nCols == 0);
	fStream.ignore();

	if (nRows > 0 && nCols > 0)
	{
		Resize(nRows, nCols);
		for (UINT i = 0; i<m_nRows; i++)
		{
			for (UINT j = 0; j<m_nCols; j++)
			{
				if (!fStream)
				{
					break;
				}
				fStream >> Matrix[i][j];
			}
		}
	}

	fStream.close();
}
template < typename T > void CMatrix_<T>::OpenFromBinaryFile(char *fileName)
{
	ifstream iFileStream(fileName, ios::binary);
	if (iFileStream)
	{
		UINT numRows = 0;
		UINT numCols = 0;

		iFileStream.read((char *)&numRows, sizeof(numRows));
		iFileStream.read((char *)&numCols, sizeof(numCols));

		if (numRows > 0 && numCols > 0)
		{
			Resize(numRows, numCols);
			for (UINT i = 0; i<m_nRows; i++)
			{
				iFileStream.read((char *)Matrix[i], m_nCols * sizeof(T));
			}
		}
		//Resize(numRows, numCols);
		//iFileStream >> *this;
	}
	iFileStream.close();
}
template < typename T > bool CMatrix_<T>::Join_Right(const CMatrix_<T>* pCMatrix)
{
	UINT oldColSize = m_nCols;
	//First we growup the matrix
	if (Resize(max(m_nRows, pCMatrix->m_nRows), m_nCols + pCMatrix->m_nCols))
	{
		//Now we copy the new matrix in the group
		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][oldColSize],
				&pCMatrix->Matrix[i][0],
				pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][oldColSize], &pCMatrix->Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
		}
		return true;
	}
	else
	{
		return false;
	}

}
template < typename T > bool CMatrix_<T>::Join_RightExplicit(const CMatrix_<T>* pCMatrix)
{
	UINT oldColSize = m_nCols;
	//First we grow-up the matrix
	if (Resize(_max(m_nRows, pCMatrix->m_nRows), m_nCols + pCMatrix->m_nCols))
	{
		//Now we copy the new matrix in the group
		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
			for (UINT j = 0; j<pCMatrix->m_nCols; j++)
			{
				Matrix[i][m_nCols + j - pCMatrix->m_nCols] = (*pCMatrix)[i][j];
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}
template < typename T > bool CMatrix_<T>::Join_Left(const CMatrix_<T>* pCMatrix)
{
	UINT oldColSize = m_nCols;
	UINT oldRowSize = m_nRows;
	//First we grow-up the matrix
	if (Resize(_max(m_nRows, pCMatrix->m_nRows), m_nCols + pCMatrix->m_nCols))
	{
		for (register UINT i = 0; i</*pCMatrix->*/m_nRows; i++)
		{
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][m_nCols - oldColSize], &Matrix[i][0], oldColSize * sizeof(T));
#endif
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][m_nCols - oldColSize], &Matrix[i][0], oldColSize * sizeof(T));
#endif
		}
		for (register UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][0], &pCMatrix->Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][0], &pCMatrix->Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
		}
		if (oldRowSize > pCMatrix->m_nRows)
		{
			for (register UINT i = pCMatrix->m_nRows; i<m_nRows; i++)
			{
#ifdef _CMP_VST_WIN
				SecureZeroMemory(&Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
				memset(&Matrix[i][0], 0, pCMatrix->m_nCols * sizeof(T));
#endif
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Join_LeftExplicit(const CMatrix_<T>* pCMatrix)
{
	UINT oldColSize = m_nCols;
	UINT oldRowSize = m_nRows;
	//First we grow-up the matrix
	if (Resize(_max(m_nRows, pCMatrix->m_nRows), m_nCols + pCMatrix->m_nCols))
	{
		for (register UINT i = 0; i<m_nRows; i++)
		{
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][m_nCols - oldColSize], &Matrix[i][0], oldColSize * sizeof(T));
#endif
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][m_nCols - oldColSize], &Matrix[i][0], oldColSize * sizeof(T));
#endif
		}

		for (register UINT i = 0; i < pCMatrix->m_nRows; i++)
		{
			for (register UINT j = 0; j < pCMatrix->m_nCols; j++)
			{
				Matrix[i][j] = (*pCMatrix)[i][j];
			}
		}
		if (oldRowSize > pCMatrix->m_nRows)
		{
			for (register UINT i = pCMatrix->m_nRows; i<m_nRows; i++)
			{
#ifdef _CMP_VST_WIN
				SecureZeroMemory(&Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
				memset(&Matrix[i][0], 0, pCMatrix->m_nCols * sizeof(T));
#endif
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Join_Up(const CMatrix_<T>* pCMatrix)
{
	UINT oldRowSize = m_nRows;
	UINT oldColSize = m_nCols;

	//First we growup the matrix
	if (Resize(m_nRows + pCMatrix->m_nRows, max(m_nCols, pCMatrix->m_nCols)))
	{
		for (UINT i = m_nLastRowIndex; i >= pCMatrix->m_nRows; i--)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][0], &Matrix[i - pCMatrix->m_nRows][0], m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][0], &Matrix[i - pCMatrix->m_nRows][0], m_nCols * sizeof(T));
#endif
		}

		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][0],
				&pCMatrix->Matrix[i][0],
				pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][0], &pCMatrix->Matrix[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
		}
		if (pCMatrix->m_nCols < oldColSize)
		{
			for (register UINT i = pCMatrix->m_nCols; i<m_nCols; i++)
			{
				for (register UINT j = 0; j < pCMatrix->m_nRows; j++)
				{
#ifdef _CMP_VST_WIN
					SecureZeroMemory(&Matrix[j][i], sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
					memset(&Matrix[j][i], 0, sizeof(T));
#endif
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Join_UpExplicit(const CMatrix_<T>* pCMatrix)
{
	UINT oldRowSize = m_nRows;
	UINT oldColSize = m_nCols;

	//First we growup the matrix
	if (Resize(m_nRows + pCMatrix->m_nRows, _max(m_nCols, pCMatrix->m_nCols)))
	{
		for (UINT i = m_nLastRowIndex; i >= pCMatrix->m_nRows; i--)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][0], &Matrix[i - pCMatrix->m_nRows][0], m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][0], &Matrix[i - pCMatrix->m_nRows][0], m_nCols * sizeof(T));
#endif
		}

		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
			for (UINT j = 0; j < pCMatrix->m_nCols; j++)
			{
				Matrix[i][j] = (*pCMatrix)[i][j];
			}
		}
		if (pCMatrix->m_nCols < oldColSize)
		{
			for (register UINT i = pCMatrix->m_nCols; i<m_nCols; i++)
			{
				for (register UINT j = 0; j < pCMatrix->m_nRows; j++)
				{
#ifdef _CMP_VST_WIN
					SecureZeroMemory(&Matrix[j][i], sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
					memset(&Matrix[j][i], 0, sizeof(T));
#endif
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}
template < typename T > bool CMatrix_<T>::Join_Down(const CMatrix_<T>* pCMatrix)
{
	UINT oldRowSize = m_nRows;

	//First we grow-up the matrix
	if (Resize(m_nRows + pCMatrix->m_nRows, _max(m_nCols, pCMatrix->m_nCols)))
	{
		for (UINT i = oldRowSize; i < m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[i][0], &pCMatrix->Matrix[i - oldRowSize][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[i][0], &pCMatrix->Matrix[i - oldRowSize][0], pCMatrix->m_nCols * sizeof(T));
#endif
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Join_DownExplicit(const CMatrix_<T>* pCMatrix)
{
	UINT oldRowSize = m_nRows;

	if (Resize(m_nRows + pCMatrix->m_nRows, _max(m_nCols, pCMatrix->m_nCols)))
	{
		for (UINT i = 0; i < pCMatrix->m_nRows; i++)
		{
			for (UINT j = 0; j < pCMatrix->m_nRows; j++)
			{
				Matrix[i + oldRowSize][j] = (*pCMatrix)[i][j];
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Insert(const CMatrix_<T>* pCMatrix, UINT iRow, UINT iCol)
{
	bool res = false;
	UINT k = 0;
	UINT oldColSize = m_nCols;
	UINT oldRowSize = m_nRows;
	if (Resize(pCMatrix->m_nRows + m_nRows, pCMatrix->m_nCols + m_nCols))
	{
		for (UINT i = 0; i<oldRowSize - iRow; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[m_nRows - i - 1][0], &Matrix[oldRowSize - i - 1][0], m_nCols * sizeof(T));
			SecureZeroMemory(&Matrix[oldRowSize - i - 1][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[m_nRows - i - 1][0], &Matrix[oldRowSize - i - 1][0], m_nCols * sizeof(T));
			memset(&Matrix[oldRowSize - i - 1][0], 0, m_nCols * sizeof(T));
#endif
		}

		for (UINT i = 0; i<m_nRows; i++)
		{
			if (i<iRow || i>(iRow + pCMatrix->m_nRows) - 1)
			{
#ifdef _CMP_VST_WIN
				CopyMemory(&Matrix[i][iCol + pCMatrix->m_nCols], &Matrix[i][iCol], (oldColSize - iCol) * sizeof(T));
				SecureZeroMemory(&Matrix[i][iCol], (oldColSize - iCol) * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
				memcpy(&Matrix[i][iCol + pCMatrix->m_nCols], &Matrix[i][iCol], (oldColSize - iCol) * sizeof(T));
				memset(&Matrix[i][iCol]);
#endif
			}
		}
		for (UINT i = 0; i < pCMatrix->m_nRows; i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&Matrix[iRow + i][0], &(*pCMatrix)[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&Matrix[iRow + i][0], &(*pCMatrix)[i][0], pCMatrix->m_nCols * sizeof(T));
#endif
		}
	}
	return res;
}
template < typename T > void CMatrix_<T>::Floor(UINT iRow, UINT iCol, T* floorValue)
{
	*floorValue = (int)floor(DOUBLE_ZERO_TEST(Matrix[iRow][iCol]));
}
template < typename T > void CMatrix_<T>::Determinant(T *det)
{
	CArray_<bool> col = CArray_<bool>(this->GetRowSize());
	for (register UINT i = 0; i < this->GetRowSize(); ++i)
	{
		col[i] = true;
	}
	*det = this->detaux(&col, this);
}
template < typename T > bool CMatrix_<T>::Column_Add()
{
	return Resize(m_nRows, m_nCols + 1);
}
template < typename T > bool CMatrix_<T>::Column_Add(UINT numColumns)
{
	if (numColumns >0)
	{
		for (UINT i = 0; i<numColumns; i++)
		{
			if (!Resize(m_nRows, m_nCols + 1))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Column_Insert(UINT iCol)
{
	bool res = false;
	if (iCol >= 0)
	{
		if (iCol < m_nCols)
		{
			//First resize the matrix...
			if (Resize(0, m_nCols + 1))
			{
				//Now move the memory content to the new location
				for (UINT i = 0; i<m_nRows; i++)
				{
#ifdef _CMP_VST_WIN
					MoveMemory(&Matrix[i][iCol + 1], &Matrix[i][iCol], ((m_nCols - 1) - iCol) * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
					memmove(&Matrix[i][iCol + 1], &Matrix[i][iCol], ((m_nCols - 1) - iCol) * sizeof(T));
#endif
					Matrix[i][iCol] = Z;
				}
			}
			else
			{
				res = false;
			}
		}
	}
	return res;
}


template < typename T > bool CMatrix_<T>::Column_Remove(UINT iCol)
{
	if (iCol >= 0)
	{
		if ((m_nCols == 1) && (iCol == 0))
		{

			for (UINT i = 0; i < m_nRows; i++)
			{
				Matrix[i][iCol].~T();
				memset(&Matrix[i][0], 0, m_nCols * sizeof(T));
			}

		}
		if (iCol < m_nCols)
		{
			//Call the destructor for all the elements in the column
			for (register UINT i = 0; i < m_nRows; i++)
			{
				if (m_bUseDestructor)
				{
					Matrix[i][iCol].~T();
					memmove(&Matrix[i][iCol], &Matrix[i][iCol + 1], (m_nCols - (iCol + 1)) * sizeof(T));
				}
			}
			for (register UINT j = 0; j < m_nRows; j++)
			{
				memcpy(&Matrix[j][iCol], &Matrix[j][iCol + 1], (m_nCols - (iCol + 1)) * sizeof(T));
			}
			Resize(0, m_nCols - 1);
			return true;
		}
		return false;
	}
	return false;
}
template < typename T > bool CMatrix_<T>::Column_Remove(CARRAY_UINT *indexlist)
{
	CARRAY_UINT ind_aux(indexlist->GetSize());
	indexlist->Quicksort_Desending(&ind_aux);

	for (register UINT i = 0; i < ind_aux.GetSize(); i++)
	{
		Column_Remove((*indexlist)[ind_aux[i]]);
	}
	return true;

}
template < typename T > void CMatrix_<T>::Column_OpScMultiply(UINT iCol, T sC)
{
	if (sC != Z)
	{
		for (UINT i = 0; i < m_nRows; i++)
		{
			Matrix[i][iCol] *= sC;
		}
	}
}
template < typename T > void CMatrix_<T>::Column_OpScMultiply(UINT iCol, UINT xRow, T sC)
{
	if (sC != Z)
	{
		for (UINT i = 0; i<m_nRows; i++)
		{
			if (i != xRow)
			{
				Matrix[i][iCol] *= sC;
			}
		}
	}
}
template < typename T > void CMatrix_<T>::Column_OpInterchange(UINT iCol, UINT jCol)
{
	for (register UINT i = 0; i<m_nRows; i++)
	{
		T v = Matrix[i][iCol];
		Matrix[i][iCol] = Matrix[i][jCol];
		Matrix[i][jCol] = v;
	}
}
template < typename T > void CMatrix_<T>::Column_OpInnerAdd(UINT iCol, UINT jCol, T C)
{
	for (UINT i = 0; i<m_nRows; i++)
	{
		Matrix[i][iCol] += (C*Matrix[i][jCol]);
	}
}
template < typename T > void CMatrix_<T>::Column_OpInnerAdd(UINT iCol, UINT jCol, UINT xRow, T C)
{
	for (UINT i = 0; i<m_nRows; i++)
	{
		if (i != xRow)
		{
			Matrix[i][iCol] += (C* DOUBLE_ZERO_TEST(Matrix[i][jCol]));
		}
	}
}

template < typename T > void CMatrix_<T>::Column_Pivot(UINT iRow, UINT iCol)
{
	T v;

	//Make to I the Matrix[iRow][iCol]
	if (Matrix[iRow][iCol] != I)
	{
		//v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
		//v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
		v = I / Matrix[iRow][iCol];
		Column_OpScMultiply(iCol, v);
	}

	//Now for each row,.. make an internal operation...
	for (UINT i = 0; i<m_nCols; i++)
	{
		if (i != iCol)
		{
			//v = -Matrix[i][iCol];
			v = T(-1)*(Matrix[iCol][i]);
			Column_OpInnerAdd(i, iCol, v);
		}
	}
}
template < typename T > void CMatrix_<T>::Column_Pivot(UINT iRow, UINT iCol, UINT xCol)
{
	//Make to I the Matrix[iRow][iCol]
	T v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
	Col_OpScMultiply(iCol, v);

	//Now for each row,.. make an internal operation...
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (i != iCol && i != xCol)
		{
			v = -Matrix[iRow][i];
			Column_OpInnerAdd(i, iCol, v);
		}
	}

}
template < typename T > void CMatrix_<T>::Column_Pivot(UINT iRow, UINT iCol, UINT xCol, UINT xRow)
{
	//Make to I the Matrix[iRow][iCol]
	T v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
	Column_OpScMultiply(iCol, xRow, v);

	//Now for each row,.. make an internal operation...
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (i != iCol && i != xCol)
		{
			v = -Matrix[iRow][i];
			Column_OpInnerAdd(i, iCol, v);
		}
	}

}
template < typename T > void CMatrix_<T>::Column_Partial_Pivot(UINT iRow, UINT iCol, bool right)
{
	T v;

	if ((Matrix[iRow][iCol] != I) && (Matrix[iRow][iCol] != Z))
	{
		v = I / Matrix[iRow][iCol];
		Column_OpScMultiply(iCol, v);
	}

	if (right)
	{
		for (UINT i = iCol + 1; i<m_nCols; i++)
		{
			if (i != iCol)
			{
				v = -Matrix[iCol][i];
				Column_OpInnerAdd(i, iCol, v);
			}
		}
	}
	else
	{
		for (UINT i = 0; i<iCol; i++)
		{
			if (i != iCol)
			{
				v = -Matrix[iCol][i];
				Column_OpInnerAdd(i, iCol, v);
			}
		}
	}
}
template < typename T > bool CMatrix_<T>::Column_IsCanonical(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* iSRow)
{
	bool res = true;
	UINT nI = 0;
	for (UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] != I && /*DOUBLE_ZERO_TEST*/(Matrix[i][iCol]) != Z)
		{
			res = false;
			break;
		}
		else if (Matrix[i][iCol] == I)
		{
			*iSRow = i;
			nI++;
			if (nI>1)
			{
				res = false;
				break;
			}
		}
	}
	if (nI == 0)
	{
		res = false;
	}
	return res;
}
template < typename T > bool CMatrix_<T>::Column_IsArtificial(UINT iCol, UINT* iSRow)
{
	bool res = true;
	UINT nI = 0;
	if (Matrix[0][iCol] == N)
	{
		for (UINT i = 1; i < m_nRows; i++)
		{
			if ((Matrix[i][iCol] == I) || (Matrix[i][iCol] == Z))
			{
				if (Matrix[i][iCol] == I)
				{
					*iSRow = i;
					nI++;
					if (nI>1)
					{
						res = false;
						break;
					}
					else
					{
						res = true;
					}
				}
			}
			else
			{
				res = false;
				break;
			}
		}
	}
	else
	{
		res = false;
	}
	return res;
}
template < typename T > bool CMatrix_<T>::Column_IsInteger(UINT iCol, UINT* iSRow)
{
	bool res = true;
	long double n;

	for (register UINT i = 0; i<m_nRows; i++)
	{
		if (DOUBLE_ZERO_TEST(modfl(Matrix[i][iCol], &n)) != 0)
			//if(EPS_DIST_TEST(Matrix[i][iCol]) !=  int(Matrix[i][iCol]))
		{
			res = false;
			*iSRow = i;
			break;
		}
	}
	return res;
}
template < typename T > bool CMatrix_<T>::Column_IsInteger(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* iSRow)
{
	bool res = true;

	for (register UINT i = iRowMin; i <= iRowMax; i++)
	{
		//if(Matrix[i][iCol] != T(floor(Matrix[i][iCol])) )
		if (Matrix[i][iCol] != T(int(Matrix[i][iCol])))
		{
			res = false;
			*iSRow = i;
			break;
		}
	}
	return res;
}
template < typename T > bool CMatrix_<T>::Column_IsInteger(UINT iCol, CARRAY_UINT nRows, UINT* iSRow)
{
	bool res = true;

	for (register UINT i = 0; i < nRows.GetSize(); i++)
	{
		if (Matrix[nRows[i]][iCol] != T(int(Matrix[nRows[i]][iCol])))
		{
			res = false;
			*iSRow = nRows[i];
			break;
		}
	}
	return res;
}

template < typename T > void CMatrix_<T>::Column_GetMinValue(UINT iCol, UINT* index, T* value)
{
	*index = 0;
	for (register UINT i = 0; i<m_nRows; i++)
	{
		if (Matrix[i][iCol] < Matrix[*index][iCol])
		{
			*index = i;
		}
	}
	*value = Matrix[*index][iCol];
}
template < typename T > void CMatrix_<T>::Column_GetMinValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* index, T* value)
{
	*index = iRowMin;
	for (register UINT i = iRowMin + 1; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] < Matrix[*index][iCol])
		{
			*index = i;
		}
	}
}
template < typename T > void CMatrix_<T>::Column_GetMinValue(UINT iCol, CARRAY_UINT indexList, UINT* index, T* value)
{
	if (indexList != NULL)
	{
		*index = indexList[0];
		for (register UINT i = 1; i<indexList.GetSize(); i++)
		{
			if (Matrix[indexList[i]][iCol] < Matrix[*index][iCol])
			{
				*index = indexList[i];
			}
		}
		*value = Matrix[*index][iCol];
	}
}
template < typename T > void CMatrix_<T>::Column_GetMaxValue(UINT iCol, UINT* index, T* value)
{
	*index = 0;
	for (register UINT i = 0; i<m_nRows; i++)
	{
		if (Matrix[i][iCol] > Matrix[*index][iCol])
		{
			*index = i;
		}
	}
	*value = Matrix[*index][iCol];
}
template < typename T > void CMatrix_<T>::Column_GetMaxValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT* index, T* value)
{
	*index = iRowMin;
	for (register UINT i = iRowMin + 1; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] > Matrix[*index][iCol])
		{
			*index = i;
		}
	}
	*value = Matrix[*index][iCol];
}
template < typename T > void CMatrix_<T>::Column_GetMaxNonIntegerValue(UINT iCol, UINT iRowMin, UINT iRowMax, UINT *index)
{
	int o = 0;
	for (register UINT i = iRowMin + 1; i <= iRowMax; i++)
	{
		//if( (int(Matrix[i][iCol]) - Matrix[i][iCol]) < 0.0000000000001 )
		if (Matrix[i][iCol] != floor(Matrix[i][iCol]))
		{
			*index = i;
			break;
		}
	}
	for (register UINT i = (*index) + 1; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] != floor(Matrix[i][iCol]))
		{
			if (Matrix[i][iCol] > Matrix[*index][iCol])
			{
				*index = i;
			}
		}
	}
}
template < typename T > void CMatrix_<T>::Column_GetMaxValue(UINT iCol, CARRAY_UINT indexList, UINT* index, T* value)
{
	if (indexList != NULL)
	{
		*index = indexList[0];
		for (register UINT i = 1; i<indexList.GetSize(); i++)
		{
			if (Matrix[indexList[i]][iCol] > Matrix[*index][iCol])
			{
				*index = indexList[i];
			}
		}
		*value = Matrix[*index][iCol];
	}
}
template < typename T > bool CMatrix_<T>::Column_IsNegative(UINT iCol, UINT iRowMin, UINT iRowMax)
{
	for (register UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] >= T(0))
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Column_IsNegativeZero(UINT iCol, UINT iRowMin, UINT iRowMax)
{
	for (UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol]>Z)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Column_IsPositive(UINT iCol, UINT iRowMin, UINT iRowMax)
{
	for (UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol] <= T(0))
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Column_IsPositiveZero(UINT iCol, UINT iRowMin, UINT iRowMax)
{
	for (UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (Matrix[i][iCol]<Z)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Column_MakeQuotientRule_MinPositive(UINT iCol, UINT jCol, UINT iRowMin, UINT iRowMax, UINT* minIndex, T* minValue)
{
	T c;
	UINT k;

	for (k = iRowMin; k <= iRowMax; k++)
	{
		if (Matrix[k][jCol] > Z && Matrix[k][iCol] > Z)
		{
			if (k == iRowMax)
			{
				*minIndex = iRowMax;
				*minValue = Matrix[k][iCol] / Matrix[k][jCol];
				return false;
			}
			else
			{
				break;
			}
		}
		else if (k == (iRowMax))
		{
			return false;
		}
	}

	*minIndex = k;
	*minValue = DOUBLE_ZERO_TEST(Matrix[k][iCol] / Matrix[k][jCol]);

	for (UINT i = k + 1; i <= iRowMax; i++)
	{
		if (Matrix[i][jCol] > Z && Matrix[i][iCol] > Z)
		{
			c = DOUBLE_ZERO_TEST(Matrix[i][iCol] / Matrix[i][jCol]);
			if (c < *minValue && c > Z)
			{
				*minValue = c;
				*minIndex = i;
			}
		}
	}
	return true;
}

template < typename T > bool CMatrix_<T>::Column_LexicographicalRule_MinPositive(UINT iCol, UINT jCol, UINT iRowMin, UINT iRowMax, CARRAY_UINT *priorityIndex, UINT *minIndex, T *minValue)
{
	bool res = false;

	T value = T(0);
	*minIndex = 0;
	CARRAY_UINT IList;

	//First we search in the artifical values
	if (priorityIndex != NULL)
	{
		for (UINT i = 0; i<priorityIndex->GetSize(); i++)
		{
			if (Matrix[(*priorityIndex)[i]][jCol] > Z)
			{
				if (!res)
				{
					*minValue = Matrix[(*priorityIndex)[i]][iCol] / Matrix[(*priorityIndex)[i]][jCol];
					*minIndex = (*priorityIndex)[i];
					res = true;

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add((*priorityIndex)[i]);
				}
				else
				{
					value = Matrix[(*priorityIndex)[i]][iCol] / Matrix[(*priorityIndex)[i]][jCol];
					if (value < *minValue)
					{
						*minValue = value;
						*minIndex = i;

						//We have found a smallest element, we clean the list and add the new element...
						IList.RemoveAll();
						IList.Add((*priorityIndex)[i]);
					}
					if (value == *minValue && IList.GetSize() > 0)
					{
						//We have foun a similiar value we must addit to the list...
						IList.Add((*priorityIndex)[i]);
					}
				}
			}
		}
	}

	//Now search for the rest of the matrix...
	for (register UINT i = iRowMin; i <= iRowMax; i++)
	{
		//Matrix[k][iCol]/Matrix[k][jCol];
		if (Matrix[i][jCol] > Z)
		{
			if (!res)
			{
				*minValue = Matrix[i][iCol] / Matrix[i][jCol];
				*minIndex = i;
				res = true;

				//We have found a smallest element, we clean the list and add the new element...
				IList.RemoveAll();
				IList.Add(i);
			}
			else
			{
				value = Matrix[i][iCol] / Matrix[i][jCol];
				if (value < *minValue)
				{
					*minValue = value;
					*minIndex = i;

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add(i);
				}
				if (value == *minValue && IList.GetSize() > 0)
				{
					//We have foun a similiar value we must addit to the list...
					IList.Add(i);
				}
			}
		}
	}

	//Make the lexicografic rule,...
	if (IList.GetSize() > 1)
	{
		bool rB = false;
		UINT t = 0;

		//For each column...
		for (register UINT i = 0; i<m_nLastColumnIndex; i++)
		{
			t = 0;	//Number of couples values....
					//For each row..
			for (register UINT j = 0; j<IList.GetSize(); j++)
			{
				if (i != jCol)
				{
					if (!rB)
					{
						*minValue = Matrix[IList[j]][i] / Matrix[IList[j]][jCol];
						*minIndex = IList[j];
						rB = true;
					}
					else
					{
						value = Matrix[IList[j]][i] / Matrix[IList[j]][jCol];
						if (value < *minValue)
						{
							*minValue = value;
							*minIndex = IList[j];
							t = 0;
						}
						if (value == *minValue)
						{
							t++;
						}
					}
				}
			}
			if (t == 0)
			{
				break;
			}
		}
	}
	return res;
}
template < typename T > bool CMatrix_<T>::Column_LexicographicalRule_MinPositive(UINT iCol, UINT jCol, CARRAY_UINT* rowIndexList, CARRAY_UINT *priorityIndex, UINT* minIndex, T* minValue)
{
	bool res = false;

	T value = T(0);
	*minIndex = 0;
	CARRAY_UINT IList;

	//First we search in the artifical values
	if (priorityIndex != NULL)
	{
		for (UINT i = 0; i<priorityIndex->GetSize(); i++)
		{
			if (Matrix[(*priorityIndex)[i]][jCol] > Z)
			{
				if (!res)
				{
					*minValue = Matrix[(*priorityIndex)[i]][iCol] / Matrix[(*priorityIndex)[i]][jCol];
					*minIndex = (*priorityIndex)[i];
					res = true;

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add((*priorityIndex)[i]);
				}
				else
				{
					value = Matrix[(*priorityIndex)[i]][iCol] / Matrix[(*priorityIndex)[i]][jCol];
					if (value < *minValue)
					{
						*minValue = value;
						*minIndex = i;

						//We have found a smallest element, we clean the list and add the new element...
						IList.RemoveAll();
						IList.Add((*priorityIndex)[i]);
					}
					if (value == *minValue && IList.GetSize() > 0)
					{
						//We have foun a similiar value we must addit to the list...
						IList.Add((*priorityIndex)[i]);
					}
				}
			}
		}
	}

	for (register UINT i = 0; i<rowIndexList->GetSize(); i++)
	{
		//Matrix[k][iCol]/Matrix[k][jCol];
		if (Matrix[(*rowIndexList)[i]][jCol] > Z)
		{
			if (!res)
			{
				*minValue = Matrix[(*rowIndexList)[i]][iCol] / Matrix[(*rowIndexList)[i]][jCol];
				*minIndex = (*rowIndexList)[i];
				res = true;

				//We have found a smallest element, we clean the list and add the new element...
				IList.RemoveAll();
				IList.Add((*rowIndexList)[i]);
			}
			else
			{
				value = Matrix[(*rowIndexList)[i]][iCol] / Matrix[(*rowIndexList)[i]][jCol];
				if (value < *minValue)
				{
					*minValue = value;
					*minIndex = (*rowIndexList)[i];

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add((*rowIndexList)[i]);
				}
				if (value == *minValue && IList.GetSize() > 0)
				{
					//We have foun a similiar value we must addit to the list...
					IList.Add((*rowIndexList)[i]);
				}
			}
		}
	}

	//Make the lexicografic rule,...
	if (IList.GetSize() > 1)
	{
		bool rB = false;
		UINT t = 0;

		//For each column...
		for (register UINT i = 0; i<m_nLastColumnIndex; i++)
		{
			t = 0;	//Number of couples values....
					//For each row..
			for (register UINT j = 0; j<IList.GetSize(); j++)
			{
				if (i != jCol)
				{
					if (!rB)
					{
						*minValue = Matrix[IList[j]][i] / Matrix[IList[j]][jCol];
						*minIndex = IList[j];
						rB = true;
					}
					else
					{
						value = Matrix[IList[j]][i] / Matrix[IList[j]][jCol];
						if (value < *minValue)
						{
							*minValue = value;
							*minIndex = IList[j];
							t = 0;
						}
						if (value == *minValue)
						{
							t++;
						}
					}
				}
			}
			if (t == 0)
			{
				break;
			}
		}
	}
	return res;
}

template < typename T > void CMatrix_<T>::Column_Evaluate(UINT iCol, UINT iRowMin, UINT iRowMax, CArray_<T>* cList, T* value)
{
	*value = 0;
	for (register UINT i = iRowMin; i <= iRowMax; i++)
	{
		if (i < cList->GetSize())
		{
			*value += (*cList)[i] * Matrix[i][iCol];
		}
	}
}
template < typename T > void CMatrix_<T>::Column_Evaluate(UINT iCol, CARRAY_UINT* indexList, CArray_<T> *cList, T *value)
{
	*value = 0;
	for (register UINT i = 0; i<indexList->GetSize(); i++)
	{
		if ((*indexList)[i] < m_nRows)
		{
			*value += (*cList)[(*indexList)[i]] * Matrix[(*indexList)[i]][iCol];
		}
	}
}
template < typename T > bool CMatrix_<T>::Row_Add()
{
	return Resize(m_nRows + 1, 0);
}
template < typename T > bool CMatrix_<T>::Row_Add(UINT numRows)
{
	if (numRows >0)
	{
		for (UINT i = 0; i<numRows; i++)
		{
			if (!Resize(m_nRows + 1, 0))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > bool CMatrix_<T>::Row_Insert(UINT iRow)
{
	bool res = false;
	//First we resize the matrix
	if (iRow >= 0)
	{
		if (iRow < m_nRows)
		{
			if (Resize(m_nRows + 1, m_nCols))
			{
				//Now we move the rows in the matrix...
				for (UINT i = m_nLastRowIndex; i>iRow; i--)
				{
#ifdef _CMP_VST_WIN
					CopyMemory(Matrix[i], Matrix[i - 1], m_nCols * sizeof(T));
#endif 
#ifdef _CMP_GCC_WIN
					memcpy(Matrix[i], Matrix[i - 1], m_nCols * sizeof(T));
#endif	
				}
				free(Matrix[iRow]);
				Matrix[iRow] = (T*)calloc(m_nCols, sizeof(T));
			}
		}
	}
	return res;
}
template < typename T > void CMatrix_<T>::Row_Memcpy(T* arr, UINT row, UINT start, bool getset)
{
	if (getset)
	{
		memcpy(arr, Matrix[row] + start, (m_nCols - start) * sizeof(T));
	}
	else
	{
		memcpy(Matrix[row] + start, arr, (m_nCols - start) * sizeof(T));
	}
}
#ifdef CUDA_PROJECT
template < typename T > void CMatrix_<T>::Row_Memcpy_CUDA(T* arr, UINT row, UINT start, bool getset/*true = get//false = set*/)
{
	T* aux;
	aux = Matrix[row] + start;
	if (getset)
	{
		cutilSafeCall(cudaMemcpy(arr, aux, (m_nCols - start) * sizeof(T), cudaMemcpyHostToDevice));
	}
	else
	{
		cutilSafeCall(cudaMemcpy(aux, arr, (m_nCols - start) * sizeof(T), cudaMemcpyDeviceToHost));
	}
}
#endif
template < typename T > bool CMatrix_<T>::Row_Remove(UINT iRow)
{
	if (iRow >= 0)
	{
		if (iRow < m_nRows)
		{
			if (m_bUseDestructor)
			{
				//Call the destructor for all the elements in the row...
				for (register UINT i = 0; i<m_nCols; i++)
				{
					Matrix[iRow][i].~T();
				}
			}
			//Free the row,...
			free(Matrix[iRow]);

			//Now move the memory...
#ifdef _CMP_VST_WIN
			MoveMemory(&Matrix[iRow], &Matrix[iRow + 1], (m_nRows - (iRow + 1)) * sizeof(T*));
#endif

#ifdef _CMP_GCC_WIN
			memmove(&Matrix[iRow], &Matrix[iRow + 1], (m_nRows - (iRow + 1)) * sizeof(T));
#endif

			Resize(m_nRows - 1, 0);

			return true;
		}
		return false;
	}
	return false;
}
template < typename T > bool CMatrix_<T>::Row_Remove(CARRAY_UINT *indexList)
{
	CARRAY_UINT ind_aux(indexList->GetSize());
	indexList->Quicksort_Desending(&ind_aux);

	for (register UINT i = 0; i < ind_aux.GetSize(); i++)
	{
		Row_Remove((*indexList)[ind_aux[i]]);
	}
	return true;
}
template < typename T > void CMatrix_<T>::Row_OpScMultiply(UINT iRow, T sC)
{
	if (sC != Z)
	{
		for (UINT i = 0; i<m_nCols; i++)
		{
			Matrix[iRow][i] *= sC;
		}
	}
}
template < typename T > void CMatrix_<T>::Row_OpScMultiply(UINT iRow, UINT xCol, T sC)
{
	if (sC != Z)
	{
		for (UINT i = 0; i<m_nCols; i++)
		{
			if (i != xCol)
			{
				Matrix[iRow][i] *= sC;
			}
		}
	}
}
template < typename T > void CMatrix_<T>::Row_OpInterchange(UINT iRow, UINT jRow)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		T v = Matrix[iRow][i];
		Matrix[iRow][i] = Matrix[jRow][i];
		Matrix[jRow][i] = v;
	}
}

template < typename T > void CMatrix_<T>::Row_OpInnerAdd(UINT iRow, UINT jRow, T C)
{
	for (UINT i = 0; i<m_nCols; i++)
	{
		//Matrix[iRow][i] += ((C*Matrix[jRow][i]));
		Matrix[iRow][i] = Matrix[iRow][i] + ((C*Matrix[jRow][i]));

	}
}
template < typename T > void CMatrix_<T>::Row_OpInnerAdd(UINT iRow, UINT jRow, UINT xCol, T C)
{
	for (UINT i = 0; i<m_nCols; i++)
	{
		if (i != xCol)
		{
			//Matrix[iRow][i] += (C* DOUBLE_ZERO_TEST(Matrix[jRow][i]));
			Matrix[iRow][i] = Matrix[iRow][i] + (C* DOUBLE_ZERO_TEST(Matrix[jRow][i]));

		}
	}
}
template < typename T > void CMatrix_<T>::Row_Pivot(UINT iRow, UINT iCol)
{
	T v;

	//Make to I the Matrix[iRow][iCol]
	if (Matrix[iRow][iCol] != I)
	{
		//v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
		v = /*DOUBLE_ZERO_TEST*/(I / Matrix[iRow][iCol]);
		Row_OpScMultiply(iRow, v);
	}

	//Now for each row,.. make an internal operation...
	for (UINT i = 0; i<m_nRows; i++)
	{
		if (i != iRow)
		{
			v = T(-1)*(Matrix[i][iCol]);
			Row_OpInnerAdd(i, iRow, v);
		}
	}
}
template < typename T > void CMatrix_<T>::Row_Pivot(UINT iRow, UINT iCol, UINT xRow)
{
	//Make to I the Matrix[iRow][iCol]
	T v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
	Row_OpScMultiply(iRow, v);

	//Now for each row,.. make an internal operation...
	for (register UINT i = 0; i<m_nRows; i++)
	{
		if (i != iRow && i != xRow)
		{
			v = -Matrix[i][iCol];
			Row_OpInnerAdd(i, iRow, v);
		}
	}
}
template < typename T > void CMatrix_<T>::Row_Pivot(UINT iRow, UINT iCol, UINT xRow, UINT xCol)
{
	//Make to I the Matrix[iRow][iCol]
	T v = DOUBLE_ZERO_TEST(I / Matrix[iRow][iCol]);
	Row_OpScMultiply(iRow, xCol, v);

	//Now for each row,.. make an internal operation...
	for (register UINT i = 0; i<m_nRows; i++)
	{
		if (i != iRow && i != xRow)
		{
			v = -Matrix[i][iCol];
			Row_OpInnerAdd(i, iRow, xCol, v);
		}
	}
}
template < typename T > void CMatrix_<T>::Row_Partial_Pivot(UINT iRow, UINT iCol, bool down)
{
	T v;
	if ((Matrix[iRow][iCol] != I) && (Matrix[iRow][iCol] != Z))
	{
		v = I / Matrix[iRow][iCol];
		Row_OpScMultiply(iRow, v);
	}

	if (down)
	{
		for (UINT i = iRow + 1; i<m_nRows; i++)
		{
			v = -Matrix[i][iCol];
			Row_OpInnerAdd(i, iRow, v);
		}
	}
	else
	{
		for (UINT i = 0; i<iRow; i++)
		{
			v = -Matrix[i][iCol];
			Row_OpInnerAdd(i, iRow, v);
		}
	}
}
template < typename T > void CMatrix_<T>::Row_GetMinValue(UINT iRow, T *minValue)
{
	if (m_bFromFile)
	{

	}
	else
	{
		UINT index = 0;
		*minValue = Matrix[iRow][index];
		//*minValue = GetValue(iRow, index);

		for (register UINT i = 1; i<m_nCols; i++)
		{
			if (Matrix[iRow][i] < Matrix[iRow][index])
				//if(GetValue(iRow, i) < GetValue(iRow, index))
			{
				index = i;
			}
		}
		*minValue = Matrix[iRow][index];
	}
}
template < typename T > void CMatrix_<T>::Row_GetMinValue(UINT iRow, UINT *minIndex, T *minValue)
{
	*minIndex = iRow;
	*minValue = Matrix[iRow][*minIndex];

	for (register UINT i = 1; i<m_nCols; i++)
	{
		if (Matrix[iRow][i] < Matrix[iRow][*minIndex])
		{
			*minIndex = i;
		}
	}
	*minValue = Matrix[iRow][*minIndex];
}
template < typename T > void CMatrix_<T>::Row_GetMinValue(UINT iRow, UINT iColMin, UINT iColMax, UINT *index, T *minValue)
{
	if (iColMin >= 0)
	{
		if (iColMin <= m_nLastColumnIndex)
		{
			if (iColMax >= 0)
			{
				if (iColMax <= m_nLastColumnIndex)
				{
					if (iColMin == iColMax)
					{
						*minValue = Matrix[iRow][iColMin];
						*index = iColMin;
					}
					else
					{
						*minValue = Matrix[iRow][iColMin];
						*index = iColMin;

						for (register UINT i = iColMin + 1; i <= iColMax; i++)
						{
							if (Matrix[iRow][i] < Matrix[iRow][*index])
							{
								*index = i;
							}
						}
						*minValue = Matrix[iRow][*index];
					}
				}
			}
		}
	}
}
template < typename T > void CMatrix_<T>::Row_GetMinValue(UINT iRow, CARRAY_UINT* indexList, UINT *minIndex, T *minValue)
{
	if (indexList->GetSize() > 0)
	{
		*minIndex = (*indexList)[0];
		for (UINT i = 1; i<indexList->GetSize(); i++)
		{
			if (Matrix[iRow][(*indexList)[i]] < Matrix[iRow][(*indexList)[*minIndex]])
			{
				*minIndex = i;
			}
		}
		*minValue = Matrix[iRow][*minIndex];
	}
}
template < typename T > void CMatrix_<T>::Row_GetMaxValue(UINT iRow, T *maxValue)
{
	if (m_nCols == 1)
	{
		UINT index = 0;
		*maxValue = Matrix[iRow][index];
	}
	else if (m_nCols > 1)
	{
		UINT index = 0;
		*maxValue = Matrix[iRow][index];

		for (register UINT i = 0; i<m_nCols; i++)
		{
			if (Matrix[iRow][i]>Matrix[iRow][index])
			{
				index = i;
			}
		}
		*maxValue = Matrix[iRow][index];
	}
}
template < typename T > void CMatrix_<T>::Row_GetMaxValue(UINT iRow, UINT *maxIndex, T* maxValue)
{
	if (m_nCols == 1)
	{
		*maxIndex = 0;
		*maxValue = Matrix[iRow][*maxIndex];
	}
	else if (m_nCols > 1)
	{
		*maxIndex = 0;
		*maxValue = Matrix[iRow][*maxIndex];


		for (register UINT i = 1; i<m_nCols; i++)
		{
			if (Matrix[iRow][i]>Matrix[iRow][*maxIndex])
			{
				*maxIndex = i;
			}
		}
		*maxValue = Matrix[iRow][*maxIndex];
	}
}
template < typename T > void CMatrix_<T>::Row_GetMaxValue(UINT iRow, UINT iColMin, UINT iColMax, UINT *maxIndex, T* maxValue)
{
	if (iColMin >= 0)
	{
		if (iColMin <= m_nLastColumnIndex)
		{
			if (iColMax >= 0)
			{
				if (iColMax <= m_nLastColumnIndex)
				{
					if (iColMin == iColMax)
					{
						*maxValue = Matrix[iRow][iColMin];
						*maxIndex = iColMin;
					}
					else
					{
						*maxValue = Matrix[iRow][iColMin];
						*maxIndex = iColMin;

						for (register UINT i = iColMin + 1; i <= iColMax; i++)
						{
							if (Matrix[iRow][i] > Matrix[iRow][*maxIndex])
							{
								*maxIndex = i;
							}
						}
						*maxValue = Matrix[iRow][*maxIndex];
					}
				}
			}
		}
	}
}

template < typename T > void CMatrix_<T>::Row_GetMaxValue(UINT iRow, CARRAY_UINT* indexList, UINT *maxIndex, T *maxValue)
{
	if (indexList->GetSize() > 0)
	{
		*maxIndex = (*indexList)[0];
		for (UINT i = 1; i<indexList->GetSize(); i++)
		{
			if (Matrix[iRow][(*indexList)[i]] > Matrix[iRow][*maxIndex])
			{
				*maxIndex = (*indexList)[i];
			}
		}
		*maxValue = Matrix[iRow][*maxIndex];
	}
}
template < typename T > bool CMatrix_<T>::Row_IsPositive(UINT iRow)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (Matrix[iRow][i] <= 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsPositive(UINT iRow, UINT iColMin, UINT iColMax)
{
	for (register UINT i = iColMin; i<iColMax; i++)
	{
		if (Matrix[iRow][i] <= 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsPositive(UINT iRow, CARRAY_UINT indexList)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (indexList.Find(i))
		{
			if (Matrix[iRow][i] <= 0)
			{
				return false;
			}
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsPositiveZero(UINT iRow)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (Matrix[iRow][i] < 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsPositiveZero(UINT iRow, UINT iColMin, UINT iColMax)
{
	for (register UINT i = iColMin; i<iColMax; i++)
	{
		if (Matrix[iRow][i] < 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsPositiveZero(UINT iRow, CARRAY_UINT indexList)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (indexList.Find(i))
		{
			if (Matrix[iRow][i] < 0)
			{
				return false;
			}
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegative(UINT iRow)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (Matrix[iRow][i] >= 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegative(UINT iRow, UINT iMin, UINT iMax)
{
	for (register UINT i = iMin; i<iMax; i++)
	{
		if (Matrix[iRow][i] >= 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegative(UINT iRow, CARRAY_UINT indexlist)
{
	for (register UINT i = 0; i < m_nCols; i++)
	{
		if (indexlist.Find(i))
		{
			if (Matrix[iRow][i] >= 0)
			{
				return false;
			}
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegativeZero(UINT iRow)
{
	for (register UINT i = 0; i<m_nCols; i++)
	{
		if (Matrix[iRow][i] > 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegativeZero(UINT iRow, UINT iMin, UINT iMax)
{
	for (register UINT i = iMin; i<iMax; i++)
	{
		if (Matrix[iRow][i] > 0)
		{
			return false;
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_IsNegativeZero(UINT iRow, CARRAY_UINT indexlist)
{
	for (register UINT i = 0; i < m_nCols; i++)
	{
		if (indexlist.Find(i))
		{
			if (Matrix[iRow][i] > 0)
			{
				return false;
			}
		}
	}
	return true;
}
template < typename T > bool CMatrix_<T>::Row_LexicographicalRule_Min_Dual(UINT iRow, UINT jRow, UINT iColMin, UINT iColMax, UINT *minIndex, T *minValue)
{
	bool res = false;

	T value = T(0);
	*minIndex = 0;
	CARRAY_UINT IList;
	UINT i = 0;

	for (i = iColMin; i <= iColMax; i++)
	{
		//Matrix[iRow][k]/Matrix[jRow][k];
		if (Matrix[jRow][i] < Z)
		{
			if (!res)
			{
				*minIndex = i;
				*minValue = Matrix[iRow][*minIndex] / Matrix[jRow][*minIndex];
				res = true;

				//We have found a smallest element, we clean the list and add the new element...
				IList.RemoveAll();
				IList.Add(i);
			}
			else
			{
				value = Matrix[iRow][i] / Matrix[jRow][i];
				if (value < *minValue)
				{
					*minValue = value;
					*minIndex = i;

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add(i);
				}
				if (value == *minValue && IList.GetSize() > 0)
				{
					//We have found a similiar value we must addit to the list...
					IList.Add(i);
				}
			}
		}
	}

	return res;
}
template < typename T > bool CMatrix_<T>::Row_LexicographicalRule_Max_Dual(UINT iRow, UINT jRow, UINT iColMin, UINT iColMax, UINT *minIndex, T *minValue)
{
	bool res = false;

	T value = T(0);
	*minIndex = 0;
	CARRAY_UINT IList;
	UINT i = 0;

	for (i = iColMin; i <= iColMax; i++)
	{
		//Matrix[iRow][k]/Matrix[jRow][k];
		if (Matrix[jRow][i] < Z)
		{
			if (!res)
			{
				*minIndex = i;
				*minValue = Matrix[iRow][*minIndex] / Matrix[jRow][*minIndex];
				res = true;

				//We have found a smallest element, we clean the list and add the new element...
				IList.RemoveAll();
				IList.Add(i);
			}
			else
			{
				value = Matrix[iRow][i] / Matrix[jRow][i];
				if (value > *minValue)
				{
					*minValue = value;
					*minIndex = i;

					//We have found a smallest element, we clean the list and add the new element...
					IList.RemoveAll();
					IList.Add(i);
				}
				if (value == *minValue && IList.GetSize() > 0)
				{
					//We have found a similiar value we must addit to the list...
					IList.Add(i);
				}
			}
		}
	}

	return res;
}
template < typename T > void CMatrix_<T>::Row_DriebeekRule_Max_Penalization(UINT iColMin, UINT iColMax, T* value, UINT* row)
{
	//Driebeek Rule for all the rows in the matrix
	CArray_<T> m_values;
	for (UINT i = 1; i < m_nRows; i++)
	{
		Row_DriebeekRule_Max_Penalization(i, 0, m_nLastColumnIndex - 1, value);
		//m_values.Add(*value);
		m_values.Add();
		m_values[m_values.GetSize() - 1] = (*value);
	}
	m_values.GetMaxAbsoluteValue(value, row);
	//(*row) += 1;	
}
template < typename T > void CMatrix_<T>::Row_DriebeekRule_Max_Penalization(UINT iRow, UINT iColMin, UINT iColMax, T* value)
{
	//Driebeek Rule for only one row in the matrix
	//Searching costs for [Xbr]
	bool proc = false;
	CArray_<T> m_values;
	UINT f_ind = 0;
	T f_min = Z;
	for (UINT j = 0; j < m_nCols; j++)
	{
		//if the b value is not integer...
		if (Matrix[iRow][m_nLastColumnIndex] != T(int(Matrix[iRow][m_nLastColumnIndex])))
		{
			//if y[iRow][j] > 0...
			if (Matrix[iRow][j] > Z)
			{
				if (!proc)
				{
					f_min = Matrix[0][j] / Matrix[iRow][j];
					//cout << "Matrix[0]["<<j<<"] = " << (long double)Matrix[0][j] << endl;
					//cout << "Matrix["<<iRow<<"]["<<j<<"] = " << (long double)Matrix[iRow][j] << endl;
					//cout << "Matrix[0][j] / Matrix[iRow][j] = " << (long double)(Matrix[0][j] / Matrix[iRow][j]) << endl;

					proc = true;
				}
				else
				{
					(*value) = Matrix[0][j] / Matrix[iRow][j];
					if ((*value) < f_min)
					{
						f_min = (*value);
						//cout << "Matrix[0]["<<j<<"] = " << (long double)Matrix[0][j] << endl;
						//cout << "Matrix["<<iRow<<"]["<<j<<"] = " << (long double)Matrix[iRow][j] << endl;
						//cout << "Matrix[0][j] / Matrix[iRow][j] = " << (long double)(Matrix[0][j] / Matrix[iRow][j]) << endl;
					}
				}
			}
		}
	}
	//cout << "f_min def = " << (long double)f_min << endl << endl;
	f_min *= Matrix[iRow][m_nLastColumnIndex] - floor(Matrix[iRow][m_nLastColumnIndex]);
	//m_values.Add(f_min);
	m_values.Add();
	m_values[m_values.GetSize() - 1] = f_min;

	f_min = Z;
	proc = false;
	for (UINT j = 0; j < m_nCols; j++)
	{
		if (Matrix[iRow][m_nLastColumnIndex] != T(int(Matrix[iRow][m_nLastColumnIndex])))
		{
			if (Matrix[iRow][j] < Z)
			{
				if (!proc)
				{
					f_min = N*(Matrix[0][j] / Matrix[iRow][j]);
					//cout << "MMatrix[0]["<<j<<"] = " << (long double)Matrix[0][j] << endl;
					//cout << "MMatrix["<<iRow<<"]["<<j<<"] = " << (long double)Matrix[iRow][j] << endl;
					//cout << "MMatrix[0][j] / Matrix[iRow][j] = " << (long double)(Matrix[0][j] / Matrix[iRow][j]) << endl;
					proc = true;
				}
				else
				{
					(*value) = N*(Matrix[0][j] / Matrix[iRow][j]);
					if ((*value) < f_min)
					{
						f_min = (*value);
						//cout << "MMatrix[0]["<<j<<"] = " << (long double)Matrix[0][j] << endl;
						//cout << "MMatrix["<<iRow<<"]["<<j<<"] = " << (long double)Matrix[iRow][j] << endl;
						//cout << "MMatrix[0][j] / Matrix[iRow][j] = " << (long double)(Matrix[0][j] / Matrix[iRow][j]) << endl;
					}
				}
			}
		}
	}
	//cout << "f_max def = " << (long double)f_min << endl << endl;
	f_min *= I - Matrix[iRow][m_nLastColumnIndex] + floor(Matrix[iRow][m_nLastColumnIndex]);
	//m_values.Add(f_min);
	m_values.Add();
	m_values[m_values.GetSize() - 1] = f_min;

	m_values.GetMaxAbsoluteValue(value, &f_ind);
}
template < typename T > bool CMatrix_<T>::Row_DriebeekRule_Max_Penalization(UINT iColMin, UINT iColMax, CARRAY_UINT ex_list, CARRAY_UINT in_list, T* value, UINT* index)
{
	//Driebeek Rule for the elements of ex_list included in the in_list array
	CArray_<T> m_values;
	T a_val = T(0);
	CARRAY_UINT m_ind;
	UINT index_find = 0;
	for (UINT i = 0; i < ex_list.GetSize(); i++)
	{
		//mod desde a
		if (in_list.Find(ex_list[i], &index_find) /*a*/ && Matrix[index_find + 1][m_nLastColumnIndex] != T(int(Matrix[index_find + 1][m_nLastColumnIndex]))/*end a*/)
		{
			Row_DriebeekRule_Max_Penalization(index_find + 1, 0, m_nLastColumnIndex - 1, value);
			//			Row_DriebeekRule_Max_Penalization(index_find+1, 0, m_nLastColumnIndex - 1, &a_val);
			m_values.Add();
			m_values[m_values.GetSize() - 1] = (*value);
			//			m_values.Add(a_val);
			//m_ind.Add(index_find);
			m_ind.Add();
			m_ind[m_ind.GetSize() - 1] = index_find;

			//			a_val = 0;
			index_find = 0;
		}
	}
	if (m_values.GetSize()>0)
	{
		m_values.GetMaxAbsoluteValue(value, &index_find);
		//cout << " index_find " << index_find << endl;
		//cout << " m_values " << endl;
		//m_values.PrintToConsole();
		//cout << " m_ind " << endl;
		//m_ind.PrintToConsole();
		*index = m_ind[index_find];
		return true;
	}
	else
	{
		return false;
	}
}
template < typename T > void CMatrix_<T>::Row_Evaluate(UINT iRow, UINT iColMin, UINT iColMax, CArray_<T>* cList, T* value)
{
	*value = 0;
	for (register UINT i = iColMin; i <= iColMax; i++)
	{
		if (i < cList->GetSize())
		{
			*value += (*cList)[i] * Matrix[iRow][i];
		}
	}
}
template < typename T > void CMatrix_<T>::Row_Evaluate(UINT iRow, CARRAY_UINT* indexList, CArray_<T> *cList, T *value)
{
	*value = 0;
	for (register UINT i = 0; i<indexList->GetSize(); i++)
	{
		if ((*indexList)[i] < m_nCols)
		{
			*value += (*cList)[(*indexList)[i]] * Matrix[iRow][(*indexList)[i]];
		}
	}
}
template < typename T > bool CMatrix_<T>::GetSubMatrix(UINT iRowMin, UINT iRowMax, UINT iColMin, UINT iColMax, CMatrix_<T>* Sub)
{
	if (Sub->Resize(UINT(iRowMax - iRowMin), UINT(iColMax - iColMin)))
	{
		for (UINT i = 0; i < Sub->GetRowSize(); i++)
		{
#ifdef _CMP_VST_WIN
			CopyMemory(&(*Sub)[i][0], &Matrix[iRowMin + i][iColMin], Sub->m_nCols * sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
			memcpy(&(*Sub)[i][0], &Matrix[iRowMin + i][iColMin], Sub->m_nCols * sizeof(T));
#endif			
		}
		return true;
	}
	return false;
}
template < typename T > bool CMatrix_<T>::GetSubMatrix(CARRAY_UINT* colIndexList, CARRAY_UINT* rowIndexList, CMatrix_<T>* Sub)
{
	if (rowIndexList == NULL)
	{
		if (Sub->Resize(m_nRows, colIndexList->GetSize()))
		{
			for (UINT j = 0; j < Sub->GetRowSize(); j++)
			{
				for (UINT i = 0; i < Sub->GetColumnSize(); i++)
				{
#ifdef _CMP_VST_WIN
					CopyMemory(&(*Sub)[j][i], &Matrix[j][(*colIndexList)[i]], sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
					memcpy(&(*Sub)[j][i], &Matrix[j][(*colIndexList)[i]], sizeof(T));
#endif
				}
			}
			return true;
		}
	}
	else
	{
		if (colIndexList != NULL)
		{
			if (Sub->Resize(rowIndexList->GetSize(), colIndexList->GetSize()))
			{
				for (UINT j = 0; j < Sub->GetRowSize(); j++)
				{
					for (UINT i = 0; i < Sub->GetColumnSize(); i++)
					{
#ifdef _CMP_VST_WIN
						CopyMemory(&(*Sub)[j][i], &Matrix[(*rowIndexList)[j]][(*colIndexList)[i]], sizeof(T));
#endif
#ifdef _CMP_GCC_WIN
						memcpy(&(*Sub)[j][i], &Matrix[(*rowIndexList)[j]][(*colIndexList)[i]], sizeof(T));
#endif
					}
				}
				return true;
			}
		}
		else
		{
			if (Sub->Resize(rowIndexList->GetSize(), m_nCols))
			{
				for (UINT i = 0; i < Sub->GetRowSize(); i++)
				{
#ifdef _CMP_VST_WIN
					CopyMemory(&(*Sub)[i][0], &Matrix[(*rowIndexList)[i]][0], (Sub->GetColumnSize()) * sizeof(T));
#endif
				}
			}
			return true;
		}
	}
	return false;
}

template < typename T > void CMatrix_<T>::LDDeterminant(long double *det)
{
	CArray_<bool> col = CArray_<bool>(this->GetRowSize());
	for (register UINT i = 0; i < this->GetRowSize(); ++i)
	{
		col[i] = true;
	}
	*det = this->LDDeterminant(&col, this);
}
template < typename T > void CMatrix_<T>::LowerTriangularSystem(CMatrix_<T> *b, CMatrix_<T> *x, bool front)
{

	bool si = true;
	if (GetColumnSize() == GetRowSize() && front)
	{
		for (register UINT i = 0; i<GetColumnSize(); i++)
		{
			for (register UINT j = GetColumnSize() - 1; j>i; j--)
			{
				if (Matrix[i][j] != 0)
				{
					cout << "La Matrix no es triangular inferior" << endl;
					si = false;
				}
			}
			if (Matrix[i][i] == 0)
			{
				cout << "La Matrix no es triangular inferior" << endl;
				si = false;
			}
		}
		if (si)
		{
			x->Resize(1, 1);
			(*x)[0][0] = 0;
			x->Resize(GetRowSize(), 1);
			for (register UINT i = 0; i<GetColumnSize(); i++)
			{
				for (register UINT j = 0; j <= i; j++)
				{
					if (j == 0)
					{
						(*x)[i][0] = (*b)[i][0];
						//cout<<"("<<(*x)[i][0];
					}
					else
					{
						(*x)[i][0] -= (Matrix[i][j - 1] * (*x)[j - 1][0]);
						//cout<<"-"<<Matrix[i][j-1]<<"*"<<(*x)[j-1][0];
					}
				}
				//cout<<")/"<<Matrix[i][i]<<endl;
				(*x)[i][0] = (*x)[i][0] / Matrix[i][i];
			}
		}
	}
	if (GetColumnSize() == GetRowSize() && !front)
	{
		for (register UINT i = 0; i<GetColumnSize(); i++)
		{
			for (register UINT j = GetColumnSize() - 1; j>i; j--)
			{
				if (Matrix[i][j] != 0)
				{
					cout << "La Matrix no es triangular inferior" << endl;
					si = false;
				}
			}
			if (Matrix[i][i] == 0)
			{
				cout << "La Matrix no es triangular inferior" << endl;
				si = false;
			}
		}
		if (si)
		{
			x->Resize(1, 1);
			(*x)[0][0] = 0;
			x->Resize(1, GetRowSize());
			for (register int i = GetColumnSize() - 1; i >= 0; i--)
			{
				for (register int j = GetColumnSize() - 1; j >= i; j--)
				{
					if (j == GetColumnSize() - 1)
					{
						(*x)[0][i] = (*b)[i][0];
						//cout<<"("<<(*x)[0][i];
					}
					else
					{
						(*x)[0][i] -= (Matrix[j + 1][i] * (*x)[0][j + 1]);
						//cout<<"-"<<Matrix[j+1][i]<<"*"<<(*x)[0][j+1];
					}
				}
				//cout<<")/"<<Matrix[i][i]<<endl;
				//cout<<"TTTTTTTTTTTTTTTTTTTTTTTTTTT"<<endl;
				//x->PrintToConsole();
				//cout<<"TTTTTTTTTTTTTTTTTTTTTTTTTTT"<<endl;
				(*x)[0][i] = (*x)[0][i] / Matrix[i][i];
			}
		}
	}
}
#ifdef CUDA_PROJECT
//Copy matrix to a device array
//---------------------------------------------------------
//-Array: Dest array
//-getset: if true Getn info, else set info
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::CUDA_memcpy_to_Array(T **Array, bool getset)
{
	T* Aux;
	if (*Array == NULL)
	{
		cutilSafeCall(cudaMalloc((void**)Array, (m_nRows*m_nCols) * sizeof(T)));
	}
	Aux = *Array;
	if (getset)
	{
		for (register UINT i = 0; i < m_nRows; ++i)
		{
			cutilSafeCall(cudaMemcpy(Aux, Matrix[i], m_nCols * sizeof(T), cudaMemcpyHostToDevice));
			Aux += m_nCols;
		}
	}
	else
	{
		for (register UINT i = 0; i < m_nRows; ++i)
		{
			cutilSafeCall(cudaMemcpy(Matrix[i], Aux, m_nCols * sizeof(T), cudaMemcpyDeviceToHost));
			Aux += m_nCols;
		}
	}

}
#endif
//Copy matrix to a device array
//---------------------------------------------------------
//-Array: Dest array
//-getset: if true Getn info, else set info
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::memcpy_to_Array(T **Array, bool getset)
{
	T* Aux;
	if (*Array == NULL)
	{
		*Array = (T*)malloc((m_nRows*m_nCols) * sizeof(T));
	}
	Aux = *Array;
	if (getset)
	{
		for (register UINT i = 0; i < m_nRows; ++i)
		{
			memcpy(Aux, Matrix[i], m_nCols * sizeof(T));
			Aux += m_nCols;
		}
	}
	else
	{
		for (register UINT i = 0; i < m_nRows; ++i)
		{
			memcpy(Matrix[i], Aux, m_nCols * sizeof(T));
			Aux += m_nCols;
		}
	}

}

//Interchange 2 rows
//---------------------------------------------------------
//-row1: first row to interchange
//-row2: second row to interchange
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::InterchangeRows(UINT row1, UINT row2)
{
	T* aux;
	aux = Matrix[row1];
	Matrix[row1] = Matrix[row2];
	Matrix[row2] = aux;
}

//Interchange 2 rows
//---------------------------------------------------------
//-row1: first row to interchange
//-row2: second row to interchange
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::InterchangeRows(UINT* rows)
{
	T** Aux;
	Aux = (T**)malloc(m_nRows * sizeof(T*));
	for (register UINT i = 0; i < m_nRows; ++i)
	{
		Aux[i] = Matrix[rows[i]];
	}
	for (register UINT j = 0; j < m_nRows; ++j)
	{
		Matrix[j] = Aux[j];
		Aux[j] = NULL;
	}
	free(Aux);
}

//Interchange 2 rows
//---------------------------------------------------------
//-row1: first row to interchange
//-row2: second row to interchange
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::InterchangeRows(int* rows)
{
	T** Aux;
	Aux = (T**)malloc(m_nRows * sizeof(T*));
	for (register UINT i = 0; i < m_nRows; ++i)
	{
		Aux[i] = Matrix[rows[i]];
	}
	for (register UINT j = 0; j < m_nRows; ++j)
	{
		Matrix[j] = Aux[j];
		Aux[j] = NULL;
	}
	free(Aux);
}

//Interchange 2 rows
//---------------------------------------------------------
//-row1: first row to interchange
//-row2: second row to interchange
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::InterchangeRows(CArray_<int>* rows)
{
	T** Aux;
	Aux = (T**)malloc(m_nRows * sizeof(T*));
	for (register UINT i = 0; i < m_nRows; ++i)
	{
		Aux[i] = Matrix[rows[i]];
	}
	for (register UINT j = 0; j < m_nRows; ++j)
	{
		Matrix[j] = Aux[j];
		Aux[j] = NULL;
	}
	free(Aux);
}

//Interchange 2 rows
//---------------------------------------------------------
//-row1: first row to interchange
//-row2: second row to interchange
//---------------------------------------------------------
template < typename T > void CMatrix_<T>::InterchangeRows(CArray_<UINT>* rows)
{
	T** Aux;
	Aux = (T**)malloc(m_nRows * sizeof(T*));
	for (register UINT i = 0; i < m_nRows; ++i)
	{
		Aux[i] = Matrix[rows[i]];
	}
	for (register UINT j = 0; j < m_nRows; ++j)
	{
		Matrix[j] = Aux[j];
		Aux[j] = NULL;
	}
	free(Aux);
}

//Builds identity matrix of size n x m
//---------------------------------------------------------
//-pCMatrix: Identity Matrix
//-n: Number of columns, for the matrix
//-m: Number of rows, for the matrix
//---------------------------------------------------------
template < typename T > void CMatrix_Identity(CMatrix_<T>* pCMatrix, UINT n)
{
	if (pCMatrix->Resize(n, n))
	{
		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
			pCMatrix->Matrix[i][i] = pCMatrix->I;
			Aux[j] = NULL;
		}
	}
}
//Builds a random, matrix of size n x m, with values between minValue & maxValue
//---------------------------------------------------------
//-pCMatrix: Identity Matrix
//-n: Number of columns, for the matrix
//-m: Number of rows, for the matrix
//---------------------------------------------------------

template < typename T > void CMatrix_Random(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m)
{
	if (pCMatrix->Resize(n, m))
	{
		srand((UINT)time(NULL)*rand());
		for (UINT i = 0; i<pCMatrix->m_nRows; i++)
		{
			for (UINT j = 0; j<pCMatrix->m_nCols; j++)
			{
				T r = /*-*/((double)rand() / RAND_MAX);
				/*T d = maxValue - minValue;*/
				//T rews = r*d;
				//pCMatrix->Matrix[i][j] = ((T)minValue) + (((T)((double)rand() / RAND_MAX))*((T)(maxValue - minValue)));
				pCMatrix->Matrix[i][j] = ((T)(maxValue - minValue)*r) + minValue;
			}
		}
	}
}
template < typename T > void CMatrix_IntegerRandom(CMatrix_<T>* pCMatrix, T minValue, T maxValue, UINT n, UINT m)
{
	if (pCMatrix->Resize(n, m))
	{
		srand((UINT)time(NULL)*rand());
		for (UINT i = 0; i < pCMatrix->m_nRows; i++)
		{
			for (UINT j = 0; j < pCMatrix->m_nCols; j++)
			{
				T r = ((double)rand() / RAND_MAX);
				pCMatrix->Matrix[i][j] = T(int(((T)(maxValue - minValue)*r) + minValue));

			}
		}
	}
}

template < typename T > void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x)
{
	x->Resize(m->m_nRows, 1);
	for (register UINT i = 0; i<m->m_nCols; i++)
	{
		x->Matrix[i][0] = 0;
		for (register UINT j = 0; j<i; j++)
		{
			x->Matrix[i][0] += (m->Matrix[i][j])*(x->Matrix[j][0]);
		}
		x->Matrix[i][0] = ((b->Matrix[i][0]) - (x->Matrix[i][0])) / (m->Matrix[i][i]);
	}

}
template < typename T > void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CMatrix_<T>* x)
{
	x->Resize(m->m_nRows, 1);
	for (register UINT i = 1; i<m->m_nCols; i++)
	{
		x->Matrix[m->m_nCols - i][0] = 0;
		for (register UINT j = 1; j<i; j++)
		{
			x->Matrix[m->m_nCols - i][0] += (m->Matrix[m->m_nCols - i][m->m_nCols - j])*(x->Matrix[m->m_nCols - j][0]);
		}
		x->Matrix[m->m_nCols - i][0] = ((b->Matrix[m->m_nCols - i][0]) - (x->Matrix[m->m_nCols - i][0])) / (m->Matrix[m->m_nCols - i][m->m_nCols - i]);
	}
}

template < typename T > void CMatrix_Solve_SysTryINF(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x)
{
	x->Resize(m->m_nCols);
	for (register UINT i = 0; i<m->m_nCols; i++)
	{
		(*x)[i] = 0;
		for (register UINT j = 0; j<i; j++)
		{
			(*x)[i] += (m->Matrix[i][j])*((*x)[j]);
		}
		(*x)[i] = ((b->Matrix[i][0]) - ((*x)[i])) / m->Matrix[i][i];
	}
}

template < typename T > void CMatrix_Solve_SysTrySUP(const CMatrix_<T>* m, const CMatrix_<T>* b, CArray_<T>* x)
{
	x->Resize(m->m_nRows);
	for (register UINT i = 1; i <= m->m_nCols; i++)
	{
		cout << "hola  " << i << endl;
		(*x)[m->m_nCols - i] = 0;
		for (register UINT j = 1; j<i; j++)
		{
			(*x)[m->m_nCols - i] += (m->Matrix[m->m_nCols - i][m->m_nCols - j])*((*x)[m->m_nCols - j]);
		}
		(*x)[m->m_nCols - i] = (b->Matrix[m->m_nCols - i][0] - (*x)[m->m_nCols - i]) / m->Matrix[m->m_nCols - i][m->m_nCols - i];
	}
}

template < typename T > bool Get_TriangularMatrix(const CMatrix_<T>* Matrix, CARRAY_UINT* indexList, bool* bUpperMatrix)
{

	indexList->Resize(Matrix->m_nRows);
	for (register UINT j = 0; j<Matrix->m_nRows; j++)
	{
		(*indexList)[j] = Matrix->m_nCols;
	}
	for (register UINT i = 0; i<Matrix->m_nCols; i++)
	{
		for (register UINT j = 0; j<Matrix->m_nRows; j++)
		{
			if (Matrix->Matrix[j][i] != 0)
			{
				(*indexList)[j] = i;
				break;
			}
		}
	}
	for (register UINT i = 0; i<Matrix->m_nRows; i++)
	{
		if ((*indexList)[i] == Matrix->m_nCols)
		{
			for (register UINT j = 0; j<Matrix->m_nRows; j++)
			{
				(*indexList)[j] = Matrix->m_nCols;
			}
			for (register UINT i = 0; i<Matrix->m_nCols; i++)
			{
				for (register UINT j = 1; j <= Matrix->m_nRows; j++)
				{
					if (Matrix->Matrix[Matrix->m_nRows - j][i] != 0)
					{
						(*indexList)[Matrix->m_nRows - j] = i;
						break;
					}
				}
			}
			for (register UINT i = 0; i<Matrix->m_nRows; i++)
			{
				if ((*indexList)[i] == Matrix->m_nCols)
				{
					cout << "no hay matriz triangular" << endl;
					indexList->Resize(0);
					return false;
				}
			}
			*bUpperMatrix = true;
			return true;
		}
	}
	*bUpperMatrix = false;
	return true;
}

template < typename T > void LU_Factorization(CMatrix_<T>* Matrix, CMatrix_<T>* L_Matrix, CMatrix_<T>* U_Matrix)
{
	T aux;
	for (register UINT i = 0; i < (*Matrix).GetColumnSize(); ++i)
	{
		(*L_Matrix)[i][i] = Matrix->I;//diagonal de l
	}
	for (register UINT i = 0; i < (*Matrix).GetColumnSize(); ++i)
	{
		//i-esimo renglon de u
		for (register UINT j = i; j < (*Matrix).GetColumnSize(); ++j)
		{
			aux = Matrix->Z;
			for (register UINT k = 0; k < i; ++k)
			{
				aux += (*L_Matrix)[i][k] * (*U_Matrix)[k][j];
			}
			(*U_Matrix)[i][j] = (*Matrix)[i][j] - aux;
		}
		//i-esima columna de l
		for (register UINT j = i + 1; j < (*Matrix).GetColumnSize(); ++j)
		{
			aux = Matrix->Z;
			for (register UINT k = 0; k < i; ++k)
			{
				aux += (*L_Matrix)[j][k] * (*U_Matrix)[k][i];
			}
			(*L_Matrix)[j][i] = ((*Matrix)[j][i] - aux) / (*U_Matrix)[i][i];
		}
	}
}
template < typename T > void CMatrix_<T>::Plus(CMatrix_<T>* A, CMatrix_<T>* B)
{
	assert(A->GetRowSize() == B->GetRowSize());
	assert(A->GetColumnSize() == B->GetColumnSize());

	Resize(A->GetRowSize(), B->GetColumnSize());

	for (UINT i = 0; i < A->GetRowSize(); i++)
	{
		for (UINT j = 0; j < A->GetColumnSize(); j++)
		{
			Matrix[i][j] = (*A)[i][j] + (*B)[i][j];
		}
	}
}
template < typename T > void CMatrix_<T>::Minus(CMatrix_<T>* A, CMatrix_<T>* B)
{
	assert(A->GetRowSize() == B->GetRowSize());
	assert(A->GetColumnSize() == B->GetColumnSize());

	Resize(A->GetRowSize(), B->GetColumnSize());

	for (UINT i = 0; i < A->GetRowSize(); i++)
	{
		for (UINT j = 0; j < A->GetColumnSize(); j++)
		{
			Matrix[i][j] = (*A)[i][j] - (*B)[i][j];
		}
	}
}
template < typename T > void CMatrix_<T>::Multiply(CMatrix_<T>* A, CMatrix_<T>* B)
{
	assert(A->GetColumnSize() == B->GetRowSize());
	Resize(A->GetRowSize(), B->GetColumnSize());
	for (register UINT i = 0; i < m_nRows; i++)
	{
		for (register UINT j = 0; j < m_nCols; j++)
		{
			Matrix[i][j] = 0;
			for (register UINT k = 0; k < A->GetColumnSize(); k++)
			{
				Matrix[i][j] += ((*A)[i][k] * (*B)[k][j]);
			}
		}
	}
}
template < typename T > void PrintPMatrix(CMatrix_<T>* A, CArray_<UINT>* IndexList)
{
	char mat_string[255] = { 0 };
	cout << endl << "Matriz permutada:" << endl;
	for (UINT i = 0; i<A->GetRowSize(); i++)
	{
		for (UINT j = 0; j<A->GetColumnSize(); j++)
		{
			long double k = (*A)[(*IndexList)[i]][j];
			//printf(" %0.16lf\t", k);
			sprintf(mat_string, " %.16Lf", k);
			printf(" %s\t ", mat_string);
		}
		cout << endl;
	}
}
template < typename T > bool MakeDNZ(CMatrix_<T>* A, CArray_<UINT> *indexCols, CArray_<UINT> *per)
{
	UINT size;
	size = indexCols->GetSize();
	bool adelante = true;
	CArray_<UINT> ya;

	for (register UINT i = 0; i<A->GetColumnSize(); i++)
	{
		if (!indexCols->Find(i) && !ya.Find(i))
		{
			indexCols->Add(i);
			ya.Add(i);
			if ((*A)[i][size] != A->Z)
			{
				(*per)[size] = i;
				if (size == A->GetColumnSize() - 1)
				{
					return true;
				}
				else
				{
					if (MakeDNZ(A, indexCols, per))
					{
						return true;
					}
				}

			}
		}
		indexCols->Resize(size);

	}
	return false;
}
template < typename T > void GetPermutationIndex(CMatrix_<T>* A, CArray_<UINT>* IndexList)
{
	IndexList->Resize(A->GetColumnSize());
	for (register UINT i = 0; i < A->GetColumnSize(); ++i)
	{
		(*IndexList)[i] = i;
	}
	CArray_<UINT> cols;
	MakeDNZ(A, &cols, IndexList);
}
template < typename T > void LU_Factorization(CMatrix_<T>* Matrix, CMatrix_<T>* L_Matrix, CMatrix_<T>* U_Matrix, CArray_<UINT>* IndexList)
{
	T aux;
	for (register UINT i = 0; i < (*Matrix).GetColumnSize(); ++i)
	{
		(*L_Matrix)[i][i] = 1;//diagonal de l
	}
	for (register UINT i = 0; i < (*Matrix).GetColumnSize(); ++i)
	{
		//i-esimo renglon de u
		for (register UINT j = i; j < (*Matrix).GetColumnSize(); ++j)
		{
			aux = 0;
			for (register UINT k = 0; k < i; ++k)
			{
				aux += (*L_Matrix)[i][k] * (*U_Matrix)[k][j];
			}
			(*U_Matrix)[i][j] = (*Matrix)[(*IndexList)[i]][j] - aux;
		}
		//i-esima columna de l
		for (register UINT j = i + 1; j < (*Matrix).GetColumnSize(); ++j)
		{
			aux = 0;
			for (register UINT k = 0; k < i; ++k)
			{
				aux += (*L_Matrix)[j][k] * (*U_Matrix)[k][i];
			}
			(*L_Matrix)[j][i] = ((*Matrix)[(*IndexList)[j]][i] - aux) / (*U_Matrix)[i][i];
		}
	}
}

template < typename T > void Inverse(CMatrix_<T>* Matrix, CMatrix_<T>* Matrix1)
{
	T Aux1, Aux2;
	CMatrix_Identity(Matrix1, Matrix->GetColumnSize());


	for (register UINT i = 0; i < Matrix->GetColumnSize(); ++i)
	{
		Aux1 = Matrix->I / (*Matrix)[i][i];
		Matrix->Row_OpScMultiply(i, Aux1);
		Matrix1->Row_OpScMultiply(i, Aux1);
		for (register UINT j = 0; j < Matrix->GetColumnSize(); ++j)
		{
			if (i != j)
			{
				Aux2 = Matrix->N*(*Matrix)[j][i];
				Matrix->Row_OpInnerAdd(j, i, Aux2);
				Matrix1->Row_OpInnerAdd(j, i, Aux2);
			}
		}
	}
}
template < typename T > void InverseLUP1(CMatrix_<T>* Matrix, CMatrix_<T>* Matrix1, T* Det = NULL)
{
	CMatrix_<T> L, U;
	CMatrix_<T> L1, U1;
	T Aux;
	CArray_<UINT> IndexList;
	GetPermutationIndex<T>(Matrix, &IndexList);
	L.Resize(Matrix->GetRowSize(), Matrix->GetColumnSize());
	U.Resize(Matrix->GetRowSize(), Matrix->GetColumnSize());
	Matrix1->Resize(Matrix->GetRowSize(), Matrix->GetColumnSize());
	LU_Factorization<T>(Matrix, &L, &U, &IndexList);

	Aux = T(1);
	for (register UINT n = 0; n<U.GetRowSize(); ++n)
	{
		Aux = Aux*U[n][n];
	}
	if (Aux == Matrix->Z)
	{
		return;
	}
	Inverse(&L, &L1);
	Inverse(&U, &U1);

	IndexList.PrintToConsole();


	for (register UINT j = 0; j<L.GetRowSize(); ++j)
	{
		for (register UINT k = 0; k < U.GetColumnSize(); ++k)
		{
			Aux = Matrix1->Z;
			for (register UINT l = 0; l < U.GetRowSize(); ++l)
			{
				Aux += U1[j][l] * L1[l][k];

			}

			for (register UINT m = 0; m < U.GetColumnSize(); ++m)
			{
				if (IndexList[k] == m)
				{
					(*Matrix1)[j][m] = Aux;
				}
			}
		}
	}
}


///////////////////////////////////////////
////////////////CMATRIX_FLOAT4
//////////////////////////////////////////

CMATRIX_FLOAT4::CMATRIX_FLOAT4() : CMATRIX_FLOAT(4)
{}