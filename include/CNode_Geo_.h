#ifndef CNode_Geo_H
#define CNode_Geo_H

#define CNODE_GEO_DEBUG_CONSOLE
#define CNODE_GEO_VERSION 1.43

#include "Common_.h"
#include "CArray_.h"

class CNodeLinkDef_Geo_
{
public:
	CNodeLinkDef_Geo_();
	CNodeLinkDef_Geo_(const CArray_<UINT> nIndexList);
	CNodeLinkDef_Geo_(const CNodeLinkDef_Geo_ &nCNodeLinkDef);
	virtual ~CNodeLinkDef_Geo_(void);

	//Open a text format file, to fill one CNodeLinkDef values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	void OpenFromTextFile(char* fileName);

	//Save the node object into a text format file
	//---------------------------------------------------------
	//-str: File name 
	//---------------------------------------------------------
	void SaveToTextFile(char* fileName);

	UINT GetLinkIndexSize();

	CArray_<UINT>& GetLinkIndex();

	CNodeLinkDef_Geo_& operator = (const CNodeLinkDef_Geo_& obj)
	{
		m_linkIndex=obj.m_linkIndex;
		return *this;
	}

	//Friend classes
	//-----------------------------------------------------------------------
	//template < typename nodeType, typename linkType> friend class CGraph_;
	//template < typename nodeType, typename linkType> friend class CTree_;
	//template < typename T> friend class CNode_;

	friend ostream& operator << (ostream &out, CNodeLinkDef_Geo_ &cNodeLinkDef)
	{
		out << cNodeLinkDef.m_linkIndex.GetSize()<< " ";
		
		for(UINT i=0; i<cNodeLinkDef.m_linkIndex.GetSize(); i++)
		{
			out << cNodeLinkDef.m_linkIndex[i] << " ";
		}
		return out;
	}

protected:
	//List of the links that in or out of this node
	CArray_<UINT> m_linkIndex;
};

//*************************************************************************************
//*************************************************************************************

template <typename T > class CNode_Geo_
{
public:
	
	CNode_Geo_();
	CNode_Geo_(const T &nValue, CNodeLinkDef_Geo_ nNodeLinkDef);
	CNode_Geo_(const CNode_Geo_ &nCNode);

	virtual ~CNode_Geo_(void);

	//Open a text format file, to fill one CNode with its values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	void OpenFromTextFile(char* fileName);
	
	//Save the Node object into a text format file
	//---------------------------------------------------------
	//-str: File name 
	//---------------------------------------------------------
	void SaveToTextFile(char* fileName);

	CNodeLinkDef_Geo_& GetCNodeLinkDef();

	T& GetValue();

	CNode_Geo_ operator = (const CNode_Geo_ &obj) 
	{
		m_Value = obj.m_Value;
		m_nodeLinkDef = obj.m_nodeLinkDef;

		return *this;
	}

	CNode_Geo_ operator () (int a)
	{
		return *this;
	}

	friend ostream& operator << (ostream &out, CNode_Geo_<T> &cNode)
	{
		
		out << cNode.m_Value;
		out << " ";
		out << cNode.m_nodeLinkDef;
		
		return out;
	}


	//Friend classes
	//--------------------------------------------------------------------------------------------------------------------------------------
	//template <typename nodeType, typename linkType> friend class CGraph_;
	//template <typename nodeType, typename linkType> friend class CTree_;
	
protected:
	T m_Value;
	CNodeLinkDef_Geo_ m_nodeLinkDef;
};

template <typename T>CNode_Geo_<T>::CNode_Geo_()
{

}
template <typename T>CNode_Geo_<T>::CNode_Geo_(const T &nValue, CNodeLinkDef_Geo_ nNodeLinkDef)
{
	m_Value=nValue;
	m_nodeLinkDef=nNodeLinkDef;
}

template <typename T>CNode_Geo_<T>::CNode_Geo_(const CNode_Geo_<T> &nCNode)
{
	m_Value=nCNode.m_Value;
	m_nodeLinkDef=nCNode.m_nodeLinkDef;
}

template <typename T>CNode_Geo_<T>::~CNode_Geo_()
{

}

template <typename T> CNodeLinkDef_Geo_& CNode_Geo_<T>::GetCNodeLinkDef()
{
	return m_nodeLinkDef;
}

template <typename T> T& CNode_Geo_<T>::GetValue()
{
	return m_Value;
}

template <typename T> void CNode_Geo_<T>::OpenFromTextFile(char *fileName)
{
	ifstream fStream(fileName);

	if(!fStream)
	{
		//MessageBox(NULL, L"ADIOS", L"", 0);
		return;
	}

	UINT a = 0;

	// File input
    char strCommand[256] = {0};
    
	fStream >> strCommand;    

	if( strcmp(strCommand, "#value") == 0)
	{
		fStream >> m_Value;
	}

	fStream >> strCommand;

	if( strcmp(strCommand, "#nodeLinkDefinition") == 0)
	{
		fStream >> a;
		m_nodeLinkDef.m_linkIndex.Resize(a);
		for(register UINT i=0; i<a; i++)
		{
			fStream >> m_nodeLinkDef.m_linkIndex[i];
		}
	}
	
	fStream >> strCommand;

	fStream.close();
}

template <typename T> void CNode_Geo_<T>::SaveToTextFile(char *fileName)
{
	ofstream fStream(fileName);
	if(fStream)
	{
		fStream << "#value " << m_Value << endl;
		fStream << "#nodeLinkDefinition " << m_nodeLinkDef << endl;		
	}
	fStream.close();
}

#endif