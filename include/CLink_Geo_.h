#ifndef CLink_Geo_H
#define CLink_Geo_H

#define CLINK_GEO_DEBUG_CONSOLE
#define CLINK_GEO_VERSION 1.44

#include "Common_.h"

template <typename T > class CLink_Geo_
{
public:
	CLink_Geo_();
	CLink_Geo_(const UINT &nNodeStart, const UINT &nNodeEnd);
	CLink_Geo_(const CLink_Geo_ &nCLink_);
	virtual ~CLink_Geo_(void);

	UINT GetEnd();
	UINT GetStart();
	void PrintToConsole();
	UINT& GetNodeEnd();
	UINT& GetNodeStart();
	
	//Open a text format file, to fill one CLink with its content
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	void OpenFromTextFile(char* fileName);

	//Save the link object into a text format file
	//---------------------------------------------------------
	//-str: File name 
	//---------------------------------------------------------
	void SaveToTextFile(char* fileName);

	CLink_Geo_ operator = (const CLink_Geo_ &obj) 
	{
		m_nodeStart = obj.m_nodeStart;
		m_nodeEnd = obj.m_nodeEnd;
		return *this;
	}

	friend ostream& operator << (ostream &out, CLink_Geo_<T> &cLink)
	{
		out << cLink.m_nodeStart << " ";
		out << cLink.m_nodeEnd;
		return out;
	}


//Friend classes
//--------------------------------------------------------------------------------------------------------------------------------------
	//template < typename nodeType, typename linkType> friend class CGraph_;
	//template < typename nodeType, typename linkType> friend class CTree_;

private:
	UINT m_nodeStart;
	UINT m_nodeEnd;
};

template <typename T>CLink_Geo_<T>::CLink_Geo_()
{
}
template <typename T>CLink_Geo_<T>::CLink_Geo_(const UINT &nNodeStart, const UINT &nNodeEnd)
{
	m_nodeStart=nNodeStart;
	m_nodeEnd=nNodeEnd;
}
template <typename T>CLink_Geo_<T>::CLink_Geo_(const CLink_Geo_<T> &nCLink_)
{
	m_nodeStart=nCLink_.m_nodeStart;
	m_nodeEnd=nCLink_.m_nodeEnd;
}

template <typename T>CLink_Geo_<T>::~CLink_Geo_(void)
{

}

template <typename T> UINT CLink_Geo_<T>::GetEnd()
{
	return m_nodeEnd;
}

template <typename T> UINT CLink_Geo_<T>::GetStart()
{
	return m_nodeStart;
}

template <typename T> void CLink_Geo_<T>::OpenFromTextFile(char *fileName)
{
	ifstream fStream(fileName);

	if(!fStream)
	{
		//MessageBox(NULL, L"ADIOS", L"", 0);
		return;
	}

	char strCommand[256] = {0};

	fStream >> strCommand;    

	if( strcmp(strCommand, "#nodeStart") == 0)
	{
		fStream >> m_nodeStart;
	}

	fStream >> strCommand;    

	if( strcmp(strCommand, "#nodeEnd") == 0)
	{
		fStream >> m_nodeEnd;
	}

	fStream.close();
}

template <typename T> void CLink_Geo_<T>::PrintToConsole()
{
	cout<<m_nodeStart<<"---"<<m_nodeEnd<<endl;
}

template <typename T> void CLink_Geo_<T>::SaveToTextFile(char *fileName)
{
	ofstream fStream(fileName);
	if(fStream)
	{
		fStream << "#nodeStart " << m_nodeStart << endl;
		fStream << "#nodeEnd " << m_nodeEnd << endl;
		fStream << endl;
	}
	fStream.close();
}

template <typename T> UINT& CLink_Geo_<T>::GetNodeEnd()
{
	return m_nodeEnd;
}

template <typename T> UINT& CLink_Geo_<T>::GetNodeStart()
{
	return m_nodeStart;
}
#endif