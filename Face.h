#pragma once
#include <iostream> 
#include <list> 
#include"commonFunctions.h"

using namespace std;

class Face;
class edge;
class vertex;
class EdgeLoop;

int boundingBox(list<vertex*>* pVertexList, double* pMin, double* pMax);
int createFace(Face& face, double pPoints[][2], int nNoOfEdgeLoops, int color);
int faceBoolean(Face& face1, Face& face2, Face& BooleanFace);
int splitEdgeAtGivenPoint(edge* pInputEdge, Face& Inputface, vertex* pStartVertex, vertex* pEndVertex, double* pIntersectionPoint);
static int edgeId = 1, faceId = 1, loopId = 1, vertexId = 1;
class edge
{
	int m_EdgeColor, m_id;
	double m_dEdgelength;
	int m_scratch;
	EdgeLoop* m_pEdgeLoop;

	vertex* m_pStartVertex, * m_pEndVertex;

	//list<Face*> faceList;
	list<vertex*>* vertexlist;

	list<Face*>* faceList;

	list<Face*>::iterator it;

public:
	edge()
	{
		m_id = edgeId;
		edgeId++;
		m_scratch = 0;
		faceList = new list<Face*>;
		vertexlist = new list<vertex*>;
	}

	int setColor(int color)
	{
		m_EdgeColor = color;
		return 1;
	}
	int getColor()
	{
		return m_EdgeColor;
	}
	int setLength(double dLength)
	{
		m_dEdgelength = dLength;
		return 1;
	}
	double getLength()
	{
		return m_dEdgelength;
	}

	int setVertexlist(vertex* pStartVertex, vertex* pEndVertex)
	{
		if (vertexlist->size() >= 2)
			vertexlist->clear();
		m_pStartVertex = pStartVertex;
		m_pEndVertex = pEndVertex;
		vertexlist->push_back(pStartVertex);
		vertexlist->push_back(pEndVertex);
		return 1;
	}

	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = vertexlist;
		return 1;
	}
	int getEdgeVertices(vertex** PStartVertex, vertex** pEndVertex)
	{
		*PStartVertex = m_pStartVertex;
		*pEndVertex = m_pEndVertex;

		return 1;
	}
	int setFacelist(Face* pFace)
	{
		faceList->push_back(pFace);
		return 1;
	}
	int getFaceList(list<Face*>** pFaceList)
	{
		*pFaceList = faceList;
		return 1;
	}
	int setEdgeLoop(EdgeLoop* pEdgeLoop)
	{
		m_pEdgeLoop = pEdgeLoop;
		return 1;
	}
	EdgeLoop* getEdgeLoop()
	{
		return m_pEdgeLoop;
	}
	int setScratch(int nScratch)
	{
		m_scratch = nScratch;
		return 1;
	}
	int getSratch()
	{
		return m_scratch;
	}
};
class vertex
{
	double dXY[2];
	list<Face*>* faceList;
	list<edge*>* edgeList;
	int m_id, m_Scratch;

public:
	vertex(double* pXY)
	{
		for (int i = 0; i < 2; i++)
			dXY[i] = pXY[i];

		m_id = vertexId;
		vertexId++;
		m_Scratch = 0;
		faceList = new list<Face*>;
		edgeList = new list<edge*>;
	}
	int setFacelist(Face* pFace)
	{
		faceList->push_back(pFace);
		return 1;
	}
	int getFaceList(list<Face*>** pFaceList)
	{
		*pFaceList = faceList;
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = edgeList;
		return 1;
	}
	int setEdgeList(edge* pEdge)
	{
		edgeList->push_back(pEdge);
		return 1;
	}
	double* getVertexPosition()
	{
		return dXY;
	}
	int setScratch(int nScratch)
	{
		m_Scratch = nScratch;
		return 1;
	}
	int getSratch()
	{
		return m_Scratch;
	}
};


class EdgeLoop
{
	Face* m_pParentFace;
	int m_nLoopType;
	int m_nScratch;

	double m_dArea, dBoundingBox[4];
	int m_id;

	list<edge*>* edgeList;
	list<vertex*>* vertexlist;
	vertex* pStartVertex;

public:
	EdgeLoop(vertex *pVertex)
	{
		m_id = loopId;
		loopId++;
		m_nLoopType = 0;
		m_nScratch = 0;
		pStartVertex = pVertex;
		m_pParentFace = NULL;
		m_dArea = 0.0;
		for (int i = 0; i < 4; i++)
			dBoundingBox[i] = 0.0;

		edgeList = new list<edge*>;
		vertexlist = new list<vertex*>;

	}

	int setArea(double area)
	{
		m_dArea = area;

		return 1;
	}
	double getArea()
	{
		return m_dArea;
	}

	int setBoundingBox(double* dMINBBox, double* dMaxBBox)
	{
		for (int i = 0; i < 2; i++)
		{
			dBoundingBox[i] = dMINBBox[i];
			dBoundingBox[i + 2] = dMaxBBox[i];
		}
		return 1;
	}
	int updateBoundingBox()
	{
		boundingBox(vertexlist, dBoundingBox, &dBoundingBox[2]);
		return 0;
	}
	double* getBoundingBox()
	{
		return dBoundingBox;
	}
	int setEdgeList(edge* pEdge)
	{
		edgeList->push_back(pEdge);
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = edgeList;
		return 1;
	}
	int setVertexlist(vertex* pVertex)
	{
		vertexlist->push_back(pVertex);
		return 1;
	}
	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = vertexlist;
		return 1;
	}
	//nType =1 outer loop
	//nType = 0 inner loop
	int setLoopType(int ntype)
	{
		m_nLoopType = ntype;
		return 1;
	}
	int getLoopType()
	{
		return m_nLoopType;
	}

};
class Face
{
	int m_EdgeCount, m_vertexcount, m_loopcount, m_faceColor;
	int m_id;

	double m_dArea, dBoundingBox[4];

	list<edge*>* edgeList;
	list<edge*>* commonEdgeList;
	list<vertex*>* vertexlist;
	list<EdgeLoop*>* edgeLoopList;

public:

	Face()
	{
		m_id = faceId;
		faceId++;

		edgeList = new list<edge*> ;
		commonEdgeList = new list<edge*>;
		vertexlist = new list<vertex*>;
		edgeLoopList = new list<EdgeLoop*>;
		m_dArea = 0.0;

	}
	int setArea(double area)
	{
		m_dArea = area;

		return 1;
	}
	double getArea()
	{
		return m_dArea;
	}

	int setBoundingBox(double *dMINBBox, double *dMaxBBox)
	{
		for (int i = 0; i < 2; i++)
		{
			dBoundingBox[i] = dMINBBox[i];
			dBoundingBox[i + 2] = dMaxBBox[i];
		}
		return 1;
	}
	double* getBoundingBox()
	{
		return dBoundingBox;
	}
	int updateBoundingBox()
	{
		boundingBox(vertexlist, dBoundingBox, &dBoundingBox[2]);
		return 0;
	}
	int setColor(int color)
	{
		m_faceColor = color;
		return 1;
	}
	int getColor()
	{
		return m_faceColor;
	}
	int setEdgeList(edge* pEdge)
	{
		edgeList->push_back(pEdge);
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = edgeList;
		return 1;
	}
	int getEdgeLoopList(list<EdgeLoop*>** pEdgeLoopList)
	{
		*pEdgeLoopList = edgeLoopList;
		return 1;
	}
	int setEdgeLoopList(EdgeLoop* pEdgeLoop)
	{
		edgeLoopList->push_back(pEdgeLoop);
		return 1;
	}
	int setVertexlist(vertex* pVertex)
	{
		vertexlist->push_back(pVertex);
		return 1;
	}
	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = vertexlist;
		return 1;
	}
	int getCommonEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = commonEdgeList;
		return 1;
	}
	int setCommonEdgeList(edge* pEdge)
	{
		commonEdgeList->push_back(pEdge);
		return 1;
	}
};


