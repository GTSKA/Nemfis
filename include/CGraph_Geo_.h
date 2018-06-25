#ifndef CGraph_Geo_H
#define CGraph_Geo_H

#define CGRAPH_GEO_DEBUG_CONSOLE
#define CGRAPH_GEO_VERSION 1.00

#define _COMMENT_

#include "Common_.h"
#include "CMatrix_.h"
#include "CArray_.h"
#include "CNode_Geo_.h"
#include "CLink_Geo_.h"

template < typename nodeType, typename linkType> class CGraph_Geo_
{
public:


	CGraph_Geo_();
	CGraph_Geo_(const CGraph_Geo_ &nCGraph);
	virtual ~CGraph_Geo_(void);

	// Add a new link between NodeStart and NodeEnd
	//-----------------------------------------------------------
	// - nodeStrat: The node where the new link will go out
	// - nodeEnd: The node where the new link will arrive
	void AddLink( UINT nodeStart, UINT nodeEnd);
	
	// Add a new node only if the graph is empty
	//-----------------------------------------------------------
	// - value: The value of the new node
	//-----------------------------------------------------------
	void AddNode(const nodeType &value);

	// Add a new node
	//-----------------------------------------------------------
	// - value: The value of the new node
	// - nodes: A array that contain the nodes which the new node will have a link
	// - weight: A array that contain the weight of the new links
	// - Direction: A array that contain the direction of the new links
	//-----------------------------------------------------------
	void AddNode(const nodeType &value, CArray_<UINT> *nodes);

	// Add a new node
	//-----------------------------------------------------------
	// - value: value of the new node
	// - nodeConnection: The node which the new  node will have a link
	// - weightLink: The value of the weight of the new link
	// - direction: The direction of the new link
	//-----------------------------------------------------------
	void AddNode(const nodeType &value, UINT nodeConnection);
	
	// Find all the cicle that contain the link that start in nodeStrat and finish in nodeEnd
	//-----------------------------------------------------------
	// - nodeStart: Is the node where the link goes out
	// - nodeEnd: Is the node where the link arrives
	// - cycle: Is an array that will contain all the cycles which contain the link
	//-----------------------------------------------------------
	bool AllCycles(UINT nodeStart, UINT nodeEnd, CArray_< CArray_<UINT> > *cycle);

	// Find all the cycle with direction that contain the link that start in nodeStrat and finish in nodeEnd 
	//-----------------------------------------------------------
	// - nodeStart: Is the node where the link goes out
	// - nodeEnd: Is the node where the link arrives
	// - cycle: Is an array that will contain all the cycles which contain the link
	//-----------------------------------------------------------
	bool AllCyclesDirection(UINT nodeStart, UINT nodeEnd, CArray_< CArray_<UINT> > *cycleNodes, CArray_< CArray_<UINT> > *cycleLinks);
	
	// Find if the link which start in "nodeStrat" and finish in "nodeEnd" is into a cycle
	//-----------------------------------------------------------
	// - nodeStrat: The node in which the link goes out
	// - nodeEnd: The node in which the link arrives
	// ---------------------------------------------------------- 
	bool Cycle(UINT nodeStart, UINT nodeEnd);

	// Save all the values of the nodes without specific order in obj
	//-----------------------------------------------------------
	// - obj: The array which will contain the values
	//-----------------------------------------------------------
	void GetElements(CArray_<nodeType> *obj);

	// Return the number of links that exist
	//-----------------------------------------------------------
	UINT GetLinkCount();

	// Return a link of type linkType
	//-----------------------------------------------------------
	// - index: it is the number of the link which we want
	//-----------------------------------------------------------
	CLink_Geo_<linkType> GetLink(UINT index);

	// Return the number of nodes in the graph
	//-----------------------------------------------------------
	UINT GetSize();	

	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<int> *matrix);
	
	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<nodeType> *matrix);

	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<int> *matrix, CMatrix_<linkType> *fObjetivo);

	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<nodeType> *matrix, CMatrix_<linkType> *fObjetivo);

	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<int> *matrix, CMatrix_<linkType> *fObjetivo, CMatrix_<int> *mij);

	//Return the incidence matrix of the graph
	//-----------------------------------------------------------
	// - matrix: It is a matrix that contain the incidence matrix
	//-----------------------------------------------------------
	void GetIncidenceMatrix(CMatrix_<nodeType> *matrix, CMatrix_<linkType> *fObjetivo, CMatrix_<int> *mij);

	// Insert one node between the two nodes which are the nodeStrat and nodeEnd of this link and erase the link
	// new links that connect the new node with the above are added in order with respect to the direction of the previous link
	//-----------------------------------------------------------
	// - link: It큦 the link in where we will insert other nuew node 
	// - value: It큦 is the value that the new node will have
	// - wLink1: It큦 the weight of the firt link
	// - wLink2: It큦 the weight of the second link
	//-----------------------------------------------------------
	bool InsertNode(nodeType value, UINT link);

	//Find the nodes which are adjacent to the node
	//-----------------------------------------------------------
	// - node: Is the node which we will find his adjacent nodes
	// - nAdj: A array in which we will return the adjacent nodes
	//-----------------------------------------------------------
	void NodesAdjacent(UINT node, CArray_<UINT>* Adyacentes);

	//Open a text format file, to fill a graph with his values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	bool OpenFromTextFile(char* fileName);

	// Print the graph but in the simple way
	//-----------------------------------------------------------
	void PrintToConsole();

	// Print the graph but node by node with it큦 links, weight and value
	//-----------------------------------------------------------
	void PrintToConsole(bool Todo);

	// Print the node on the console
	//-----------------------------------------------------------
	// - Nonode: is the number of the node which we want print
	//-----------------------------------------------------------
	void PrintToConsoleNode(UINT node);

	//Open a text format file, to fill a graph with his values
	//---------------------------------------------------------
	//-fileName: File name
	//---------------------------------------------------------
	bool ReadStream(ifstream *stream);

	// it is an auxiliar which save in an array the node which are in the path from a start node to end node
	// Return true if the path exist and false wherelse
	//-----------------------------------------------------------
	// - nodeStart: It큦 the start node
	// - nodeEnd: It큦 the end node
	// - Recorrido: The array which will contain the path
	//-----------------------------------------------------------
	bool Recorrido(UINT nodeStart, UINT nodeEnd, CArray_<UINT> *Recorrido);

	//it is an auxiliar which save in an array the node which are in the path from a start node to end node but 
	//the path can큧 contain a node which we will give. Return true if the path exist and false wherelse
	//-----------------------------------------------------------
	// - nodeStart: It큦 the start node
	// - nodeEnd: It큦 the end node
	// - EsteNo: It큦 the node which the path mustn큧 have
	// - Recorrido: The array which will contain the path
	//-----------------------------------------------------------
	bool Recorrido(UINT nodeStart, UINT nodeEnd, UINT EsteNo, CArray_<UINT> *Recorrido);

	//It큦 an auxiliar that help to do recursived to make the way that go out from nodeInicial and arrive at
	//nodeEnd but don큧 pass through by el nodo EsteNo
	//-----------------------------------------------------------
	// nodeInicial - It큦 the node that the way go out
	// nodeEnd - It큦 the node that the way arrive
	// EsteNo - It큦 the node by which the way can큧 pass
	//-----------------------------------------------------------
	bool RecorridoAdy(UINT nodeInical, UINT nodeEnd, UINT EsteNO);

	
	// Remove All the elements of the graph, the nodes and the links
	//-------------------------------------------------------------
	void RemoveAll();

	//Remove one node that has only twe link one of them is of "in" and the other is "out", also remove
	//the two links and add one new link that go out from the node that has the link that arrive at the
	//node removed and arrive at the link that has the link that go out from the link removed
	//-------------------------------------------------------------
	// - node: It큦 the number of the node that we want to remove
	// - wLink: It큦 the new link that will be build
	//-------------------------------------------------------------
	bool RemoveIntermediateNode(UINT node);

	// Remove a link but only if after we remove the link the graph remains connected
	// Return true if the link could be remove and false otherwise
	//-------------------------------------------------------------
	// - link: It큦 the link which we want to remove
	//-------------------------------------------------------------
	bool RemoveLink(UINT link);

	// Remove a link but only if after we remove the link the graph remains connected
	// Return true if the link could be remove and false otherwise
	//-------------------------------------------------------------
	// - nodeStart: It큦 the node where the link go out
	// - nodeEnd: It큦 the node where the link arrive
	// - weight: It큦 the weight of the link
	// - direction: It큦 the direction of the link
	//-------------------------------------------------------------
	bool RemoveLink(UINT nodeStart,UINT nodeEnd);

	// Remove a node but only if after removw thw nodw the graph remain connected
	// Return true if can remove the node and false otherwise
	//--------------------------------------------------------------
	// - node: It큦 the node which we want to remove
	//--------------------------------------------------------------
	bool RemoveNode(UINT node);

	//Save the graph object into a text format file
	//---------------------------------------------------------
	//-str: File name 
	//---------------------------------------------------------
	void SaveToTextFile(char *fileName);

	// Save in a arrey the shortest way from a start node to end node
	//--------------------------------------------------------------
	// - nodeStart: It큦 the start node
	// - nodeEnd: It큦 the end node
	// - way: It큦 the array which will contain the shortest way
	//--------------------------------------------------------------
	void ShortestWay(UINT nodeStart, UINT nodeEnd, CArray_<UINT> *way);

	// Save in a arrey the shortest way from a start node to end node
	//--------------------------------------------------------------
	// - nodeStart: It큦 the start node
	// - nodeEnd: It큦 the end node
	// - way: It큦 the array which will contain the shortest way
	//--------------------------------------------------------------
	void ShortestWayWithoutOneNode(UINT nodeStart, UINT nodeEnd, CArray_<UINT> *way, UINT node);

	// Return true if the matrix is Unimodular
	//--------------------------------------------------------------
	// - matrix: It큦 the matrix which we will see if is unimodular
	//--------------------------------------------------------------
	bool Unimodularidad(CMatrix_<int> *matrix);

	//Save the graph object into a stream
	//---------------------------------------------------------
	// - stream: fStream name 
	//---------------------------------------------------------
	void WriteStream(ofstream *stream);

	//Return in one array the numbers of the links that are related with a especific node
	//-----------------------------------------------------------
	// - node: Number of the node which we want to know its related links
	// - links: Array where we will save the nombers of the related links
	void GetRelatedLinks(UINT node, CArray_<UINT> *links);

	bool GetNumberLink(UINT nodeStart, UINT nodeEnd, UINT *link);

	bool R(UINT inicio, UINT fin, CArray_<UINT> *R);

	bool GetSubGraph(CArray_<UINT> *nodes, CGraph_Geo_<nodeType, linkType> *subGraph);

	bool IsThereLink(UINT nodeStart, UINT nodeEnd);

	CArray_< CNode_Geo_<nodeType> >& GetNodeList();

	CArray_< CLink_Geo_<linkType> >& GetLinkList();

	void RandomFill(nodeType aN, nodeType bN, linkType aL, linkType bL);

	void RandomFill(nodeType aN, nodeType bN, linkType aL, linkType bL, UINT seed);

	CNode_Geo_<nodeType>& operator [] (const UINT index)
	{
		assert( index >= 0 && index < nodeList.m_nSize);
		return nodeList[index]; 
	}

	CNode_Geo_<nodeType>& operator [] (const UINT index) const
	{
		assert( index >= 0 && index < nodeList.m_nSize);
		return nodeList[index]; 
	}

	CGraph_Geo_ operator = (CGraph_Geo_ &obj)  
	{
		
		nodeList.Resize(obj.GetSize());
		
		for(register UINT i=0; i<obj.GetSize(); i++)
		{
			nodeList[i]=obj.nodeList[i];
		}

		linkList.Resize(obj.GetLinkCount());

		for(register UINT i=0; i<obj.GetLinkCount(); i++)
		{
			linkList[i]=obj.linkList[i];
		}

		return *this;
	}

protected:
	// It큦 the array which will contain all the nodes
	CArray_< CNode_Geo_<nodeType> > nodeList;
	// It큦 the array which will contain all the links
	CArray_< CLink_Geo_<linkType> > linkList;

private:

	// Help to AllCycles because this the recursive funtion
	//-----------------------------------------------------------
	void AllCyclesAux(UINT node, UINT nodeEnd, CArray_<UINT> *cycleAux, CArray_< CArray_<UINT> > *cycle);

	// Help to AllCycles because this the recursive funcion
	//-----------------------------------------------------------
	void AllCyclesAux02(UINT node, UINT nodeEnd, CArray_<UINT> *cycleAux, CArray_< CArray_<UINT> > *cycle);

	// Determine if a matrix corresponds to a connected graph 
	//-----------------------------------------------------------
	// - matrix: It큦 the matrix which we will check
	// - from: It is an integer that will help us to make the recurive process
	// - recuerdos: It is an array which will help us to not cycle the process
	//-----------------------------------------------------------
	void CheckTravel(CMatrix_<int> *matrix, UINT link, UINT node, CArray_< CArray_<UINT> > *recuerdos, CArray_<UINT> *check, CArray_<nodeType>* nodes, CArray_<linkType>* links);

	//It is an auxiliar. Find a way from a node to other node and save the way in recuerdos
	//-----------------------------------------------------------
	// - nodeStart: It is the node where the way start
	// - nodeEnd: It is the node where the way finish
	//-----------------------------------------------------------
	bool PorAqui(UINT nodeStart, UINT nodeEnd, CArray_<UINT>* Recuerdos);

	//It is an auxiliar. Find a way from a node to other node and save the way in recuerdos but without 
	//going through a node which we give before
	//-----------------------------------------------------------
	// - nodeStart: It is the node where the way start
	// - nodeEnd: It is the node where the way finish
	//-----------------------------------------------------------
	bool PorAquiSin(UINT nodeStart, UINT nodeEnd, CArray_<UINT>* Recuerdos);

	void TravelWithoutLink(UINT link, UINT node, CArray_<UINT> *aNodes);

	void TravelWithoutLink(CArray_<UINT> *link, UINT node, CArray_<UINT> *aNodes);
	
};

template < typename nodeType, typename linkType> class GRAPH_TEMP_GEO
{
public:
	CGraph_Geo_<nodeType, linkType> graph;

	//GRAPH_& operator = (GRAPH_& obj) 
	//{
	//	graph=obj.graph;
	//	return *this;
	//}

	GRAPH_TEMP_GEO()
	{

	}
	GRAPH_TEMP_GEO(const CGraph_Geo_<nodeType, linkType> graph_)
	{
		this->graph = graph_;		
	}

	CGraph_Geo_<nodeType, linkType>& operator = (const GRAPH_TEMP_GEO<nodeType, linkType>& obj) 
	{
		(graph) = (CGraph_Geo_<nodeType, linkType>(obj.graph));
		return graph;
	}

	//CGRAPH_TEMP<nodeType, linkType>& operator = (const GRAPH_TEMP<nodeType, linkType>& obj) 
	//{
	//	//(graph) = (CGraph_<nodeType, linkType>(obj.graph));
	//	this = obj;		
	//	return this;
	//}
};


template < typename nodeType, typename linkType> CGraph_Geo_<nodeType,linkType>::CGraph_Geo_()
{

}

template < typename nodeType, typename linkType>CGraph_Geo_<nodeType,linkType>::CGraph_Geo_(const CGraph_Geo_ &nCGraph)
{
	nodeList=nCGraph.nodeList;
	linkList=nCGraph.linkList;
}

template < typename nodeType, typename linkType>CGraph_Geo_<nodeType,linkType>::~CGraph_Geo_(void)
{

}


template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AddLink(UINT nodeStart, UINT nodeEnd)
{
	if(nodeStart<nodeList.GetSize() && nodeEnd<nodeList.GetSize())
	{
		linkList.Add();
		linkList[linkList.GetSize()-1].GetNodeStart()=nodeStart;
		linkList[linkList.GetSize()-1].GetNodeEnd()=nodeEnd;
		
		nodeList[nodeStart].GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
		nodeList[nodeEnd].GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
	}
}
template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AddNode(const nodeType &Value)
{
	if(linkList.GetSize()==0)
	{
		nodeList.Add();
		nodeList[0].GetValue() = Value;
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AddNode(const nodeType &value, CArray_<UINT> *nodes)
{																																 
	UINT i=0;
	
	if(nodeList.GetSize()==0)
	{
		nodeList.Add();
		nodeList.GetLastValue().GetValue()=value;
	}
	else
	{
		nodeList.Add();
		nodeList.GetLastValue().GetValue()=value;
		while(i<nodes->GetSize())
		{
			if((*nodes)[i]<nodeList.GetSize())
			{
				linkList.Add();
				linkList.GetLastValue().GetNodeEnd()=nodeList.GetSize()-1;
				linkList.GetLastValue().GetNodeStart()=(*nodes)[i];
			}
			nodeList.GetLastValue().GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
			nodeList[(*nodes)[i]].GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
			i++;
		}	
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AddNode(const nodeType &value, UINT nodeRelacionado)
{
	
	if(nodeList.GetSize()==0)
	{
		nodeList.Add();
		nodeList[0].GetValue()= value;
	}
	else
	{
			if(nodeRelacionado<nodeList.GetSize())
			{
				linkList.Add();
				linkList[linkList.GetSize()-1].GetNodeEnd()=nodeList.GetSize();
				linkList[linkList.GetSize()-1].GetNodeStart()=nodeRelacionado;
			}
			nodeList.Add();
			nodeList.GetLastValue().GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
			nodeList[nodeRelacionado].GetCNodeLinkDef().GetLinkIndex().Add(linkList.GetSize()-1);
			nodeList.GetLastValue().GetValue()=value;
	}
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::AllCycles(UINT nodeStart, UINT nodeEnd, CArray_< CArray_<UINT> > *cycle)
{
	CArray_<UINT> cycleAux;
	cycle->RemoveAll();
	//cout<<"entra bien"<<endl;
	UINT link;
	if(GetNumberLink(nodeEnd,nodeStart,&link))
	{
		cycle->Resize(1);
		(*cycle)[0].Add(nodeEnd);
		(*cycle)[0].Add(nodeStart);
	}
	AllCyclesAux(nodeEnd, nodeStart, &cycleAux, cycle);
	//cout<<"entra bien"<<endl;

	if(cycle->GetSize()!=0)
	{
		return true;
	}
	else
	{
		return false;
	}			
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AllCyclesAux(UINT node, UINT nodeEnd,CArray_<UINT> *cycleAux, CArray_< CArray_<UINT> > *cycle)
{
	CArray_<UINT> Adj;
	bool yaEsta=false;
	
	UINT size;
	NodesAdjacent(node, &(Adj));	
	cycleAux->Add(node);
	
	if(node==nodeEnd)
	{
		if(cycleAux->GetSize()>2)
		{
			cycle->Add(*cycleAux);
		}
	}
	else
	{
		
		for(register UINT i=0; i<Adj.GetSize(); i++)
		{
			for(register UINT j=0; j< cycleAux->GetSize(); j++)
			{
				if(Adj[i]==(*cycleAux)[j])
				{
					yaEsta=true;
				}
			}
			if(yaEsta==false)
			{
				size=cycleAux->GetSize();
				AllCyclesAux(Adj[i], nodeEnd, cycleAux, &(*cycle));
				cycleAux->Resize(size);	
			}
			yaEsta=false;
		}
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::AllCyclesAux02(UINT node, UINT nodeEnd,CArray_<UINT> *cycleAux, CArray_< CArray_<UINT> > *cycle)
{
	CArray_<UINT> Adj;
	bool yaEsta=false;
	
	UINT size;
	NodesAdjacent(node, &(Adj));	
	cycleAux->Add(node);
	
	if(node==nodeEnd)
	{
		if(cycleAux->GetSize()>1)
		{
			cycle->Add(*cycleAux);
		}
	}
	else
	{
		
		for(register UINT i=0; i<Adj.GetSize(); i++)
		{
			for(register UINT j=0; j< cycleAux->GetSize(); j++)
			{
				if(Adj[i]==(*cycleAux)[j])
				{
					yaEsta=true;
				}
			}
			if(yaEsta==false)
			{
				size=cycleAux->GetSize();
				AllCyclesAux02(Adj[i], nodeEnd, cycleAux, &(*cycle));
				cycleAux->Resize(size);	
			}
			yaEsta=false;
		}
	}
}
template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::CheckTravel(CMatrix_<int> *matrix, UINT link, UINT node, CArray_< CArray_<UINT> > *recuerdos, CArray_<UINT> *check, CArray_<nodeType>* nodes, CArray_<linkType>* links)
{
	bool hola=false, hola2=false;
	UINT a,b;
	check->Add(node);
	(*recuerdos)[node].Add(link);
	for(register UINT k=0; k< matrix->GetRowSize(); k++)
	{
		if((*matrix)[k][link]==-((*matrix)[node][link]))
		{
			/*if((*matrix)[k][link]>0)
			{
				AddNode((*nodes)[k], node, (*links)[link], CG_NODE_LINK_OUT); 
			}
			else
			{
				AddNode((*nodes)[k], node, (*links)[link], CG_NODE_LINK_IN); 
			}*/
			(*recuerdos)[k].Add(link);
			check->Add(k);
			for(register UINT i=0; i<matrix->GetColumnSize(); i++)
			{
				if((*matrix)[k][i]!=0)
				{
					for(register UINT j=0; j<(*recuerdos)[k].GetSize(); j++)
					{
						if(i==((*recuerdos)[k])[j])
						{
							hola =true;
						}
					}
					if(hola=false)
					{
						CheckTravel(matrix, i, k,recuerdos, check, nodes, links);
						hola2=true;
						//break;
					}
				}
				hola=false;
			}
			if(hola2==false)
			{
				hola=false;
				for(register UINT i=0; i<matrix->GetColumnSize(); i++)
				{
					if((*matrix)[node][i]!=0)
					{
						for(register UINT j=0; j<(*recuerdos)[node].GetSize(); j++)
						{
							if(i==((*recuerdos)[node])[j])
							{
								hola=true;
							}
						}
						if(hola==false)
						{
							CheckTravel(matrix, i, node, recuerdos, check, nodes, links);
							//break;
						}
					}
					hola=false;
				}			
			}

			//break;
		}
	}
	/*for(register UINT i=0; i<matrix->GetRowSize(); i++)
	{
		if((*matrix)[node][i]!=0)
		{
			for(register UINT j=0; j<(*recuerdos)[node].GetSize(); j++)
			{
				if(((*recuerdos)[node])[j]==i)
				{
					hola = true;
					break;
				}
			}
			if(hola==false)
			{				
				(*recuerdos)[node].Add(i);
				for(register UINT j=0; j<<matrix->GetColumnSize(); j++)
				{
					if(matrix[j][i]==-(*matrix)[from][i])
					{
						(*recuerdos)[j].Add(i);
						check->Add(j);
						CheckTravel(matrix, node, j, recuerdos, check);
						break;
					}
				}
			}
		}
		hola=false;
	}
	if(check->GetSize()==matrix->GetColumnSize())
	{
		check->PrintToConsole();
		return true:
	}
	else
	{
		return false;
	}*/
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::Cycle(UINT nodeStart, UINT nodeEnd)
{
	CArray_<UINT> R;
	if(Recorrido(nodeEnd, nodeStart, nodeStart, &R))
	{
		return true;
	}
	else
	{
		return false;
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::GetElements(CArray_<nodeType> *obj)
{
	obj->RemoveAll();
	for(register UINT i=0; i<nodeList.GetSize(); i++)
	{
		obj->Add(nodeList[i].m_Value);

	}
	
}

template < typename nodeType, typename linkType > UINT CGraph_Geo_<nodeType,linkType>::GetLinkCount()
{
	return linkList.GetSize();
}

template < typename nodeType, typename linkType > CLink_Geo_<linkType> CGraph_Geo_<nodeType,linkType>::GetLink(UINT index)
{
	return linkList[index];
}

template < typename nodeType, typename linkType > UINT CGraph_Geo_<nodeType,linkType>::GetSize()
{
	return nodeList.GetSize();
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::GetIncidenceMatrix(CMatrix_<int> *matrix)
{
	matrix->Resize(nodeList.GetSize(),linkList.GetSize());
	
	for(register UINT i=0; i<linkList.GetSize(); i++)
	{
		(*matrix)[linkList[i].GetNodeStart()][i] = 1;
		(*matrix)[linkList[i].GetNodeEnd()][i] = 1;
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::GetIncidenceMatrix(CMatrix_<nodeType> *matrix)
{
	matrix->Resize(nodeList.GetSize(),linkList.GetSize());
	
	for(register UINT i=0; i<linkList.GetSize(); i++)
	{
		(*matrix)[linkList[i].GetNodeStart()][i] = (nodeType)1;
		(*matrix)[linkList[i].GetNodeEnd()][i] = (nodeType)1;
	}
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::InsertNode(nodeType value, UINT link)
{
	if(link<linkList.GetSize())
	{
		AddNode(value, linkList[link].GetNodeStart());
		AddLink(nodeList.GetSize()-1, linkList[link].GetNodeEnd());		
		RemoveLink(link);
		return true;
	}
	else
	{
		return false;
	}
}
template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::NodesAdjacent(UINT node, CArray_<UINT> *Adyacentes)
{
	Adyacentes->Resize(nodeList[node].GetCNodeLinkDef().GetLinkIndexSize());
	for(register UINT i=0; i<nodeList[node].GetCNodeLinkDef().GetLinkIndexSize(); i++)
	{
		if(node==linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart())
		{
			(*Adyacentes)[i]=linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd();
		}
		else
		{
			(*Adyacentes)[i]=linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart();
		}
	}
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::OpenFromTextFile(char *fileName)
{
	ifstream fStream(fileName);

	if(!fStream)
	{
		//MessageBox(NULL, L"ADIOS", L"", 0);
		return false;
	}
	
	RemoveAll();

	UINT nNodes = 0;
	UINT nLinks = 0;
	UINT aux = 0;

	// File input
    char strCommand[256] = {0};
	//for(UINT t=0; ;)

    fStream >> strCommand;
	//cout << strCommand << endl;
    if(fStream)
	{
		if( strcmp(strCommand, "#nNodes") == 0)
		{
			fStream >> nNodes;
			//cout << "nRows = " << nRows << endl; 
		}

		fStream >> strCommand;

		if( strcmp(strCommand, "#nLinks") == 0)
		{
			fStream >> nLinks;
			//cout << "nCols = " << nCols << endl; 
		}

		fStream.ignore();
		nodeList.Resize(nNodes);
		for(register UINT i=0; i<nNodes; i++)
		{
			fStream >> nodeList[i].m_Value;
			fStream >> aux;
			nodeList[i].GetCNodeLinkDef().GetLinkIndex().Resize(aux);
			for(register UINT j=0; j<aux; j++)
			{
				fStream >> nodeList[i].GetCNodeLinkDef().GetLinkIndex()[j];
			}
		}

		linkList.Resize(nLinks);
		for(register UINT i=0; i<nLinks; i++)
		{
			fStream >> linkList[i].GetNodeStart();
			fStream >> linkList[i].GetNodeEnd();
		}
		fStream.close();
		return true;
	}
	return false;
}


template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::PorAqui(UINT nodeStart, UINT nodeEnd, CArray_<UINT>* Recuerdos)
{
	
	bool yaEsta=false;
	CArray_<UINT>  Adyacentes;

	for(register UINT i=0; i<Recuerdos->GetSize();i++)
	{
		if((*Recuerdos)[i] == nodeStart)
		{
			yaEsta=true;
			return false;
		}
	}
	
	if(yaEsta==false)
	{
		Recuerdos->Add(nodeStart);
	}
	
	NodesAdjacent(nodeStart,&Adyacentes);

	for(register UINT i=0; i<Adyacentes.GetSize(); i++)
	{
		if(nodeEnd==Adyacentes[i])
		{
			Recuerdos->Add(nodeEnd);
			return true;
		}		
	}
	
	
	for(register UINT i=0; i<Adyacentes.GetSize(); i++)
	{
		yaEsta=false;
		for(register UINT j=0; j<Recuerdos->GetSize(); j++)
		{
			if((*Recuerdos)[j]==Adyacentes[i])
			{				
				yaEsta=true;
			}
		}
		if(yaEsta==false)
		{
			if(PorAqui(Adyacentes[i],nodeEnd,&(*Recuerdos)))
			{
				return true;
			}
		}
	}
	return false;
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::PorAquiSin(UINT nodeStart, UINT nodeEnd, CArray_<UINT>* Recuerdos)
{
	
	bool yaEsta=false;
	CArray_<UINT>  Adyacentes;
	
	for(register UINT i=0; i<Recuerdos->GetSize(); i++)
	{
		if((*Recuerdos)[i] == nodeStart)
		{
			yaEsta=true;
		}
	}
	
	if(yaEsta==false)
	{
		Recuerdos->Add(nodeStart);
	}
	

	NodesAdjacent(nodeStart,&Adyacentes);

	for(register UINT i=0; i<Adyacentes.GetSize(); i++)
	{
		if(Recuerdos->GetSize()==2)
		{
		}
		else
		{
			if(nodeEnd==Adyacentes[i])
			{
				Recuerdos->Add(nodeEnd);
				return true;
			}
		}
	}
	
	
	for(register UINT i=0; i<Adyacentes.GetSize(); i++)
	{
		yaEsta=false;
		for(register UINT j=0; j<Recuerdos->GetSize(); j++)
		{
			if(Adyacentes[i]==(*Recuerdos)[j])
			{				
				yaEsta=true;
			}
		}
		if(yaEsta==false)
		{
			if(PorAqui(Adyacentes[i],nodeEnd,&(*Recuerdos)))
			{
				return true;
			}
		}
	}
	return false;
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::PrintToConsole()
{
	for(register UINT i=0; i<linkList.GetSize(); i++)
	{
		cout<<"node("<<linkList[i].GetNodeStart()<<") V("<<nodeList[linkList[i].GetNodeStart()].GetValue()<<")>>->>->node("<<linkList[i].GetNodeEnd()<<") V("<<nodeList[linkList[i].GetNodeEnd()].GetValue()<<")"<<endl;
		cout<<endl;
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::PrintToConsole(bool Todo)
{
	if(Todo==true)
	{
	
		for(register UINT j=0; j<nodeList.GetSize(); j++)
		{
			cout<<"node No.	*"<<j<<"*"<<endl;
			cout<<"Valor	>*"<<nodeList[j].GetValue()<<"*<";
			cout<<endl;
			for(register UINT i=0; i<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize(); i++)
			{
				cout<<"		No de Link:"<<nodeList[j].GetCNodeLinkDef().GetLinkIndex()[i];
				if(linkList[nodeList[j].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart() != j)
				{
					cout<<"   ----"<<linkList[nodeList[j].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart()<<endl;
				}
				else
				{
					cout<<"   ----"<<linkList[nodeList[j].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd()<<endl;
				}
			}				
		}
	}
	else
	{
		for(register UINT i=0; i<linkList.GetSize(); i++)
		{
			cout<<"node("<<linkList[i].GetNodeStart()<<") V("<<nodeList[linkList[i].GetNodeStart()].GetValue()<<")----node("<<linkList[i].GetNodeEnd()<<") V("<<nodeList[linkList[i].GetNodeEnd()].GetValue()<<")"<<endl;
			cout<<endl;

		}
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::PrintToConsoleNode(UINT node)
{
	cout<<"node No.	*"<<node<<"*"<<endl;
	cout<<"Valor	>*"<<nodeList[node].m_Value<<"*<"<<endl;
	for(register UINT i=0; i<nodeList[node].GetCNodeLinkDef().GetLinkIndexSize(); i++)
	{
		cout<<"		No de Link:"<<nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i];
		if(nodeList[node].GetCNodeLinkDef().GetLinkDirectionIndex()[i]==CG_NODE_LINK_IN)
		{
			cout<<"   <<-<<-<<--"<<linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart();

		}
		else
		{
			if(nodeList[node].GetCNodeLinkDef().GetLinkDirectionIndex()[i]==CG_NODE_LINK_OUT)
			{
				cout<<"   -->>->>->>"<<linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd();
			}
			else
			{
				cout<<"   <<-<<->>->>"<<linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd();
			}
		}
		cout<<"   weight del Link ="<<linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetWeight()<<endl;
	}
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::ReadStream(ifstream *fStream)
{
	if(!(*fStream))
	{
		//MessageBox(NULL, L"ADIOS", L"", 0);
		return false;
	}
	
	RemoveAll();

	UINT nNodes = 0;
	UINT nLinks = 0;
	UINT aux = 0;

	// File input
    char strCommand[256] = {0};
	//for(UINT t=0; ;)

    (*fStream) >> strCommand;
	//cout << strCommand << endl;
    if(fStream)
	{
		if( strcmp(strCommand, "#nNodes") == 0)
		{
			(*fStream) >> nNodes;
			//cout << "nRows = " << nRows << endl; 
		}

		(*fStream) >> strCommand;

		if( strcmp(strCommand, "#nLinks") == 0)
		{
			(*fStream) >> nLinks;
			//cout << "nCols = " << nCols << endl; 
		}

		fStream->ignore();
		nodeList.Resize(nNodes);
		for(register UINT i=0; i<nNodes; i++)
		{
			(*fStream) >> nodeList[i].GetValue();
			(*fStream) >> aux;
			nodeList[i].GetCNodeLinkDef().GetLinkIndex().Resize(aux);
			for(register UINT j=0; j<aux; j++)
			{
				(*fStream) >> nodeList[i].GetCNodeLinkDef().GetLinkIndex()[j];
			}
		}

		linkList.Resize(nLinks);
		for(register UINT i=0; i<nLinks; i++)
		{
			(*fStream) >> linkList[i].GetNodeStart();
			(*fStream) >> linkList[i].GetNodeEnd();
		}
		//fStream.close();
		return true;
	}
	return false;
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::Recorrido(UINT nodeStart, UINT nodeEnd, CArray_<UINT>* Recorrido)
{
	if(nodeStart==nodeEnd)
	{
		Recorrido->Add(nodeStart);
		return true;
	}
	if(PorAqui(nodeStart, nodeEnd, Recorrido))
	{
		return true;
	}
	else
	{
		Recorrido->RemoveAll();
		return false;
	}	
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::Recorrido(UINT nodeStart, UINT nodeEnd, UINT EsteNo, CArray_<UINT> *Recorrido)
{
	
	Recorrido->Add(EsteNo);
	if(nodeStart==nodeEnd)
	{
		Recorrido[0]=nodeStart;
		return true;
	}
	
	if(PorAquiSin(nodeStart, nodeEnd, Recorrido))
	{
		Recorrido->Remove((UINT)0);
		return true;
	}
	else
	{
		Recorrido->RemoveAll();
		return false;
	}	
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::RecorridoAdy(UINT nodeStart, UINT nodeEnd, UINT EsteNo)
{
	CArray_<UINT> R;
	R.Add(EsteNo);
	if(nodeStart==nodeEnd)
	{
		return true;
	}

	if(PorAqui(nodeStart, nodeEnd, &R))
	{
		return true;
	}
	else
	{
		return false;
	}	
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::RemoveAll()
{
	nodeList.RemoveAll();
	linkList.RemoveAll();
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::RemoveLink(UINT NoLink)
{
	bool yaNo=false;
	CArray_<UINT> R;
	if(nodeList[linkList[NoLink].GetNodeEnd()].GetCNodeLinkDef().GetLinkIndexSize()==1 || nodeList[linkList[NoLink].GetNodeEnd()].GetCNodeLinkDef().GetLinkIndexSize()==1)
	{
		return false;
	}
	else
	{
		if(Recorrido(linkList[NoLink].GetNodeStart(),linkList[NoLink].GetNodeEnd(),linkList[NoLink].GetNodeEnd(),&R)==true)
		{
			//cout<<"pasa la prueba del recorrido"<<endl;
			for(register UINT j=0; j<nodeList.GetSize(); j++)
			{
				for(register UINT k=0; k<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize(); k++)
				{
					if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]==NoLink)
					{
						nodeList[j].GetCNodeLinkDef().GetLinkIndex().Remove(k);
					}
					if(k<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize())
					{
						if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]>NoLink)
						{
							//cout<<"si aqui"<<endl;
							nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]--;
						}
						//cout<<"no aqui no"<<endl;
					}
					
				}
			}
			linkList.Remove(NoLink);
			return true;			
		}
		else
		{
			return false;
		}
	}
}

/*template < typename nodeType, typename linkType > bool CGraph_<nodeType,linkType>::RemoveLink(UINT noLink)
{
	CArray_<UINT> a;
	cout<<"aqui vamos"<<endl;
	TravelWithoutLink( noLink, linkList[noLink].GetNodeStart(), &a);
	
	for(register UINT i=0; i<a.GetSize(); i++)
	{
		if(a[i]==linkList[noLink].GetNodeEnd())
		{
			cout<<"si llega"<<endl;
			for(register UINT j=0; j<nodeList.GetSize(); j++)
			{
				for(register UINT k=0; k<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize(); k++)
				{
					if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]==noLink)
					{
						nodeList[j].GetCNodeLinkDef().GetLinkIndex().Remove(k);
						nodeList[j].GetCNodeLinkDef().GetLinkDirectionIndex().Remove(k);
					}
					if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]>noLink)
					{
						nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]--;
					}
				}
			}
			linkList.Remove(noLink);
			return true;
		}
	}
	return false;
		
		
}*/

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::RemoveIntermediateNode(UINT node)
{
	CArray_<UINT> a;
	NodesAdjacent(node, &a);
	if(a.GetSize()==2)
	{
		AddLink(a[0],a[1]);
		RemoveNode(node);
		return true;
	}
	else
	{
		return false;
	}
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::RemoveLink(UINT nodeStart, UINT nodeEnd)
{
 	for(register UINT i=0; i< linkList.GetSize(); i++)
	{
		if(linkList[i].GetNodeStart()==nodeStart && linkList[i].GetNodeEnd()==nodeEnd)
		{
			return RemoveLink(i);
		}
		if(linkList[i].GetNodeStart()==nodeEnd && linkList[i].GetNodeEnd()==nodeStart)
		{
			return RemoveLink(i);
		}
	}
	return false;
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::RemoveNode(UINT node)
{
	bool sePuede=true;
	UINT a,b;
	CArray_<UINT> adj,c;
	c.Add(node);

	if(node>=nodeList.GetSize())
	{
		return false;
	}
	else
	{
		NodesAdjacent(node,&adj);
		for(register UINT i=0; i<adj.GetSize(); i++)
		{
			for(register UINT j=0; j<adj.GetSize(); j++)
			{
				sePuede=R(adj[j],adj[i],&c);
				if(!sePuede)
				{
					return false;
				}
				c.Resize(1);
			}	 			
		}
		
		a=nodeList[node].GetCNodeLinkDef().GetLinkIndexSize();
		for(register UINT i=0; i<a; i++)
		{
			b=nodeList[node].GetCNodeLinkDef().GetLinkIndex()[0];
			for(register UINT j=0; j<nodeList.GetSize(); j++)
			{
				for(register UINT k=0; k<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize(); k++)
				{
					if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]==b)
					{
						nodeList[j].GetCNodeLinkDef().GetLinkIndex().Remove(k);
					}
				}
				for(register UINT k=0; k<nodeList[j].GetCNodeLinkDef().GetLinkIndexSize(); k++)
				{
					if(nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]>b)
					{
						nodeList[j].GetCNodeLinkDef().GetLinkIndex()[k]--;
					}
				}
			}
			linkList.Remove(b);
		}

		for(register UINT i=0; i<linkList.GetSize(); i++)
		{
			if(linkList[i].GetNodeStart()>node)
			{
				linkList[i].GetNodeStart()--;
			}
			if(linkList[i].GetNodeEnd()>node)
			{
				linkList[i].GetNodeEnd()--;
			}
		}
		nodeList.Remove(node);
		return true;		
	}
}



template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::SaveToTextFile(char *fileName)
{
	ofstream fStream(fileName);
	if(fStream)
	{
		fStream << "#nNodes " << nodeList.GetSize() << endl;
		fStream << "#nLinks " << linkList.GetSize() << endl;
		fStream << endl;

		for(UINT i=0; i<nodeList.GetSize(); i++)
		{
			fStream << nodeList[i] <<endl;
		}
		for(UINT i=0; i<linkList.GetSize(); i++)
		{
			fStream << linkList[i] << endl;
		}
	}
	fStream.close();

}
template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::ShortestWay(UINT nodeStart, UINT nodeEnd, CArray_<UINT> *way)
{
	CArray_< CArray_<UINT> > ways;

	CArray_<UINT> cycleAux;

	AllCyclesAux02(nodeStart, nodeEnd, &cycleAux, &ways);

	for(register UINT i=0; i<ways.GetSize(); i++)
	{
		if(i==0)
		{
			*way=ways[0];
		}
		else
		{
			if(way->GetSize()>ways[i].GetSize())
			{
				*way=ways[i];
			}
		}
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::ShortestWayWithoutOneNode(UINT nodeStart, UINT nodeEnd, CArray_<UINT> *way, UINT node)
{
	CArray_< CArray_<UINT> > ways;

	CArray_<UINT> cycleAux;

	cycleAux.Add(node);

	AllCyclesAux(nodeStart, nodeEnd, &cycleAux, &ways);

	for(register UINT i=0; i<ways.GetSize(); i++)
	{
		ways[i].Remove((UINT)0);
	}
	
	for(register UINT i=0; i<ways.GetSize(); i++)
	{
		if(i==0)
		{
			*way=ways[0];
		}
		else
		{
			if(way->GetSize()>ways[i].GetSize())
			{
				*way=ways[i];
			}
		}
	}
}



template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::Unimodularidad(CMatrix_<int> *matrix)
{
	int a=0;
	for(register UINT i=0; i< matrix->GetColumnSize(); i++)
	{
		for(register UINT j=0; j< matrix->GetRowSize(); j++)
		{
			a=a+(*matrix)[i][j];
		}
	}
	if(a==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::WriteStream(ofstream *stream)
{
	if(*stream)
	{
		(*stream) << "#nNodes " << nodeList.GetSize() << endl;
		(*stream) << "#nLinks " << linkList.GetSize() << endl;
		(*stream) << endl;

		for(UINT i=0; i<nodeList.GetSize(); i++)
		{
			(*stream) << nodeList[i] <<endl;
		}
		for(UINT i=0; i<linkList.GetSize(); i++)
		{
			(*stream) << linkList[i] << endl;
		}
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::GetRelatedLinks(UINT node, CArray_<UINT> *links)
{
	(*links)=nodeList[node].GetCNodeLinkDef().GetLinkIndex();
}
template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::R(UINT inicio, UINT fin, CArray_<UINT> *Rec)
{
	UINT a;
	bool adelante=true;
	CArray_<UINT> adj;
	if(inicio==fin)
	{
		return true;
	}
	else
	{
		for(register UINT i=0; i<Rec->GetSize(); i++)
		{
			if((*Rec)[i]==inicio)
			{
				return false;
			}
		}
		Rec->Add(inicio);
		NodesAdjacent(inicio,&adj);
		for(register UINT i=0; i<adj.GetSize(); i++)
		{
			for(register UINT j=0; j<Rec->GetSize(); j++)
			{
				if(adj[i]==(*Rec)[j])
				{
					adelante=false;
					break;
				}
			}
			if(adelante)
			{
				if(adj[i]==fin)
				{
					return true;
				}				
			}
			adelante=true;
		}
		for(register UINT i=0; i<adj.GetSize(); i++)
		{
			a=Rec->GetSize();
			if(R(adj[i],fin,Rec))
			{
				return true;
			}
			Rec->Resize(a);
		}
		return false;
	}
}



template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::GetNumberLink(UINT nodeStart, UINT nodeEnd, UINT *link)
{
	for(register UINT i=0; i<linkList.GetSize(); i++)
	{
		if(linkList[i].GetNodeStart()==nodeStart && linkList[i].GetNodeEnd()==nodeEnd)
		{
			(*link)=i;
			return true;
		}
	}
	return false;
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::TravelWithoutLink(UINT link, UINT node, CArray_<UINT> *aNodes)
{
	
	bool yaEsta=false;
	aNodes->Add(node);
	/*aNodes->PrintToConsole();
	getchar();*/
	for(register UINT i=0; i<nodeList[node].GetCNodeLinkDef().GetLinkIndex().GetSize(); i++)
	{
		if(nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]!=link)
		{
			if(linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart()==node)
			{
				for(register UINT j=0; j<aNodes->GetSize(); j++)
				{
					if((*aNodes)[j]==linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd())
					{
						yaEsta=true;
						break;
					}
				}
				if(!yaEsta)
				{
					TravelWithoutLink( link, linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd(), aNodes);
				}
			}
			if(linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd()==node)
			{
				for(register UINT j=0; j<aNodes->GetSize(); j++)
				{
					if((*aNodes)[j]==linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart())
					{
						yaEsta=true;
						break;
					}
				}
				if(!yaEsta)
				{
					TravelWithoutLink( link, linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart(), aNodes);
				}
			}
			yaEsta=false;
		}
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::TravelWithoutLink(CArray_<UINT> *link, UINT node, CArray_<UINT> *aNodes)
{
	bool yaEsta=false;
	aNodes->Add(node);
	bool pertenece=false;
	/*aNodes->PrintToConsole();
	getchar();*/
	for(register UINT i=0; i<nodeList[node].GetCNodeLinkDef().GetLinkIndex().GetSize(); i++)
	{
		for(register UINT j=0; j<link->GetSize(); j++)
		{
			if(nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]==(*link)[j])
			{
				pertenece=true;
				break;
			}
		}		
		if(!pertenece)
		{
			if(linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart()==node)
			{
				for(register UINT j=0; j<aNodes->GetSize(); j++)
				{
					if((*aNodes)[j]==linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd())
					{
						yaEsta=true;
						break;
					}
				}
				if(!yaEsta)
				{
					TravelWithoutLink( link, linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd(), aNodes);
				}
			}
			if(linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeEnd()==node)
			{
				for(register UINT j=0; j<aNodes->GetSize(); j++)
				{
					if((*aNodes)[j]==linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart())
					{
						yaEsta=true;
						break;
					}
				}
				if(!yaEsta)
				{
					TravelWithoutLink( link, linkList[nodeList[node].GetCNodeLinkDef().GetLinkIndex()[i]].GetNodeStart(), aNodes);
				}
			}
			yaEsta=false;
		}
		pertenece=false;
	}
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::GetSubGraph(CArray_<UINT> *nodes, CGraph_Geo_<nodeType, linkType> *subGraph)
{
	CArray_<UINT> adj;
	CArray_<UINT> nNode;
	CArray_<bool> yaEsta;
	UINT link;
	bool ya=false;

	nNode.Resize(nodes->GetSize());
	yaEsta.Resize(nodes->GetSize());
	for(register UINT i=0; i<nodes->GetSize(); i++)
	{
		yaEsta[i]=false;
	}
	subGraph->RemoveAll();

	for(register UINT i=0; i<nodes->GetSize(); i++)
	{
		if(i==0)
		{
			subGraph->AddNode(nodeList[(*nodes)[i]].GetValue());
			nNode[0]=0;
		}
		for(register UINT j=0; j<linkList.GetSize(); j++)
		{
			if(linkList[j].GetNodeStart()==(*nodes)[i])
			{
				for(register UINT k=i; k<nodes->GetSize(); k++)
				{
					if(linkList[j].GetNodeEnd()==(*nodes)[k])
					{
						if(yaEsta[k])
						{
							if(linkList[j].GetBi())
							{
								subGraph->AddLink(nNode[i],nNode[k],linkList[j].GetWeight(),CG_NODE_LINK_BI);
							}
							else
							{
								subGraph->AddLink(nNode[i],nNode[k],linkList[j].GetWeight(),CG_NODE_LINK_OUT);
							}
						}
						else
						{
							if(linkList[j].GetBi())
							{
								subGraph->AddNode(nodeList[(*nodes)[k]].GetValue(),nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_BI);
								nNode[k]=subGraph->GetSize()-1;
								yaEsta[k]=true;
							}
							else
							{
								subGraph->AddNode(nodeList[(*nodes)[k]].GetValue(),nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_IN);
								nNode[k]=subGraph->GetSize()-1;
								yaEsta[k]=true;
							}
						}
					}
				}
			}
			
			if(linkList[j].GetNodeEnd()==(*nodes)[i])
			{
				for(register UINT k=i; k<nodes->GetSize(); k++)
				{
					if(linkList[j].GetNodeStart()==(*nodes)[k])
					{
						if(yaEsta[k])
						{
							if(linkList[j].GetBi())
							{
								subGraph->AddLink(nNode[k], nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_BI);
							}
							else
							{
								subGraph->AddLink(nNode[k],nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_OUT);
							}
						}
						else
						{
							if(linkList[j].GetBi())
							{
								subGraph->AddNode(nodeList[(*nodes)[k]].GetValue(),nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_BI);
								nNode[k]=subGraph->GetSize()-1;
								yaEsta[k]=true;
							}
							else
							{
								subGraph->AddNode(nodeList[(*nodes)[k]].GetValue(),nNode[i],linkList[j].GetWeight(),CG_NODE_LINK_IN);
								nNode[k]=subGraph->GetSize()-1;
								yaEsta[k]=true;
							}
						}
					}
				}
			}
		}
	}
	if(nodes->GetSize()==subGraph->GetSize())
	{
		return true;
	}
	else
	{
		subGraph->RemoveAll();
		return false;
	}
	
}

template < typename nodeType, typename linkType > bool CGraph_Geo_<nodeType,linkType>::IsThereLink(UINT nodeStart, UINT nodeEnd)
{
	for(register UINT i=0; i<linkList.GetSize(); i++)
	{
		if(linkList[i].GetNodeStart()==nodeStart && linkList[i].GetNodeEnd()==nodeEnd)
		{
			return true;
		}
	}
	return false;
}

template < typename nodeType, typename linkType > CArray_< CNode_Geo_<nodeType> >& CGraph_Geo_<nodeType,linkType>::GetNodeList()
{
	return nodeList;
}
template < typename nodeType, typename linkType > CArray_< CLink_Geo_<linkType> >& CGraph_Geo_<nodeType,linkType>::GetLinkList()
{
	return linkList;
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::RandomFill(nodeType aN, nodeType bN, linkType aL, linkType bL)
{
	nodeType tN;
	linkType tL;
	srand(time(NULL));
	for(register UINT i=0; i<GetSize(); i++)
	{
		tN=(nodeType)(rand())/(nodeType)(RAND_MAX);
		nodeList[i].GetValue()=aN+tN*(bN-aN);
	}
	for(register UINT i=0; i<GetSize(); i++)
	{
		tL=(linkType)(rand())/(linkType)(RAND_MAX);
		linkList[i].GetWeight()=aL+tL*(bL-aL);
	}
}

template < typename nodeType, typename linkType > void CGraph_Geo_<nodeType,linkType>::RandomFill(nodeType aN, nodeType bN, linkType aL, linkType bL, UINT seed)
{
	nodeType tN;
	linkType tL;
	srand(seed);
	for(register UINT i=0; i<GetSize(); i++)
	{
		tN=(nodeType)(rand())/(nodeType)(RAND_MAX);
		nodeList[i].GetValue()=aN+tN*(bN-aN);
	}
	for(register UINT i=0; i<GetSize(); i++)
	{
		tL=(linkType)(rand())/(linkType)(RAND_MAX);
		linkList[i].GetWeight()=aL+tL*(bL-aL);
	}
}

#endif
