#pragma once
#include <iostream> 
#include <list> 

using namespace std;

static int edgeId = 1, faceId = 1, loopId = 1, vertexId = 1;
class edge
{
	int m_EdgeColor, m_id;
	double m_dEdgelength;
	int m_scratch;

	list<Face*> faceList;
	list<vertex*> vertexlist;
	vertex* pVertexArray[2];

public:
	edge()
	{
		m_id = edgeId;
		edgeId++;
		m_scratch = 0;
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
	int setLength(int dLength)
	{
		m_dEdgelength = dLength;
		return 1;
	}
	int getLength()
	{
		return m_dEdgelength;
	}

	int setVertexlist(vertex* pStartVertex, vertex* pEndVertex)
	{
		pVertexArray[0] = pStartVertex;
		pVertexArray[1] = pEndVertex;
		vertexlist.push_back(pStartVertex);
		vertexlist.push_back(pEndVertex);
		return 1;
	}
	vertex** getVertexArray()
	{
		return pVertexArray;
	}
	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = &vertexlist;
		return 1;
	}
	int setFacelist(Face* pFace)
	{
		faceList.push_back(pFace);
		return 1;
	}
	int getFaceList(list<Face*>** pFaceList)
	{
		*pFaceList = &faceList;
		return 1;
	}
};
class vertex
{
	double dXY[2];
	list<Face*> faceList;
	list<edge*> edgeList;
	int m_id, m_Scratch;

public:
	vertex(double* pXY)
	{
		for (int i = 0; i < 2; i++)
			dXY[i] = pXY[i];

		m_id = vertexId;
		vertexId++;
		m_Scratch = 0;
	}
	int setFacelist(Face* pFace)
	{
		faceList.push_back(pFace);
		return 1;
	}
	int getFaceList(list<Face*>** pFaceList)
	{
		*pFaceList = &faceList;
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = &edgeList;
		return 1;
	}
	int setEdgeList(edge* pEdge)
	{
		edgeList.push_back(pEdge);
		return 1;
	}
	double* getVertexPosition()
	{
		return dXY;
	}
};


class EdgeLoop
{
	Face* m_pParentFace;
	int m_nLoopType;
	int m_nScratch;

	double m_dArea, dBoundingBox[4];
	int m_id;

	list<edge*> edgeList;
	list<vertex*> vertexlist;
	vertex* pStartVertex;

public:
	EdgeLoop(vertex *pVertex)
	{
		m_id = loopId;
		loopId++;
		m_nLoopType = 0;
		m_nScratch = 0;
		pStartVertex = pVertex;
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
		boundingBox(&vertexlist, dBoundingBox, &dBoundingBox[2]);
		return 0;
	}
	double* getBoundingBox()
	{
		return dBoundingBox;
	}
	int setEdgeList(edge* pEdge)
	{
		edgeList.push_back(pEdge);
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = &edgeList;
		return 1;
	}
	int setVertexlist(vertex* pVertex)
	{
		vertexlist.push_back(pVertex);
		return 1;
	}
	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = &vertexlist;
		return 1;
	}

};
class Face
{
	int m_EdgeCount, m_vertexcount, m_loopcount, m_faceColor;
	int m_id;

	double m_dArea, dBoundingBox[4];

	list<edge*> edgeList;
	list<edge*> commonEdgeList;
	list<vertex*> vertexlist;
	list<EdgeLoop*>edgeLoopList;

public:

	Face()
	{
		m_id = faceId;
		faceId++;
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
		boundingBox(&vertexlist, dBoundingBox, &dBoundingBox[2]);
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
		edgeList.push_back(pEdge);
		return 1;
	}
	int getEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = &edgeList;
		return 1;
	}
	int getEdgeLoopList(list<EdgeLoop*>** pEdgeLoopList)
	{
		*pEdgeLoopList = &edgeLoopList;
		return 1;
	}
	int setEdgeLoopList(EdgeLoop* pEdgeLoop)
	{
		edgeLoopList.push_back(pEdgeLoop);
		return 1;
	}
	int setVertexlist(vertex* pVertex)
	{
		vertexlist.push_back(pVertex);
		return 1;
	}
	int getVertexList(list<vertex*>** pVertexList)
	{
		*pVertexList = &vertexlist;
		return 1;
	}
	int getCommonEdgeList(list<edge*>** pEdgeList)
	{
		*pEdgeList = &commonEdgeList;
		return 1;
	}
	int setCommonEdgeList(edge* pEdge)
	{
		commonEdgeList.push_back(pEdge);
		return 1;
	}
};

int createFace(Face& face, double pPoints[][2], int nNoOfEdgeLoops)
{
	int nTemp = 0;
	double length = 0.0;
	vertex* pVertex = NULL, * pPreviousVertex = NULL, * pLoopStartVertex = NULL;
	EdgeLoop* pEdgeLoop = NULL;
	edge* pEdge = NULL;
	int nNoOfVerticesOfloop = 0, nVerticesCount = 0;
	//creating black face
	for (int i = 0; i < nNoOfEdgeLoops; i++)
	{
		nVerticesCount = 0;
		pLoopStartVertex = new vertex(pPoints[nTemp]);
		face.setVertexlist(pLoopStartVertex);
		pPreviousVertex = pLoopStartVertex;
		pLoopStartVertex->setFacelist(&face);

		nVerticesCount++;
		pEdgeLoop = new EdgeLoop(pLoopStartVertex);
		pEdgeLoop->setVertexlist(pLoopStartVertex);
		nTemp++;

		cout << " enter the no vertices in the loop %d" << i + 1 << "\n";
		cin >> nNoOfVerticesOfloop;
		cout << "\n enter the vertices xy in the order of the loop";

		for (int j = 1; j < nNoOfVerticesOfloop; j++)
		{
			//cin>>
			pVertex = new vertex(pPoints[nTemp]);
			face.setVertexlist(pVertex);
			pVertex->setFacelist(&face);
			nVerticesCount++;
			if (nVerticesCount == 2)
			{
				nVerticesCount = 1;
				pEdge = new edge();
				face.setEdgeList(pEdge);
				pVertex->setFacelist(&face);

				pEdge->setVertexlist(pPreviousVertex, pVertex);
				pPreviousVertex->setEdgeList(pEdge);
				pVertex->setEdgeList(pEdge);

				pEdge->setFacelist(&face);
				pEdge->setColor(1);

				length = distanceBetweenPoints(pPreviousVertex->getVertexPosition(), pVertex->getVertexPosition());
				pEdge->setLength(length);

				pEdgeLoop->setEdgeList(pEdge);
				if (j == (nNoOfVerticesOfloop - 1))
				{
					pEdge = new edge();
					pEdge->setVertexlist(pVertex, pLoopStartVertex);
					pLoopStartVertex->setEdgeList(pEdge);
					pVertex->setEdgeList(pEdge);
					pLoopStartVertex->setFacelist(&face);
					face.setEdgeList(pEdge);

					pEdge->setFacelist(&face);
					pEdge->setColor(1);

					length = distanceBetweenPoints(pVertex->getVertexPosition(), pLoopStartVertex->getVertexPosition());
					pEdgeLoop->setEdgeList(pEdge);
				}
			}
			pPreviousVertex = pVertex;
		}
		nTemp = nTemp + 1;
		pEdgeLoop->updateBoundingBox();
	}
}

int boundingBox(list<vertex*>* pVertexList, double* pMin, double* pMax)
{
	double dMax[2] = { DBL_MIN, DBL_MIN }, dMin[2] = { DBL_MAX, DBL_MAX };
	double* pXY = NULL;

	for (auto vertex : *pVertexList)
	{
		pXY = vertex->getVertexPosition();

		for (int i = 0; i < 2; i++)
		{
			if (pXY[i] > dMax[i])
				dMax[i] = pXY[i];

			if (pXY[i] < dMin[i])
				dMin[i] = pXY[i];
		}
	}
	for (int i = 0; i < 2; i++)
	{
		pMin[i] = dMin[i];
		pMax[i] = dMax[i];
	}
	return 0;
}
