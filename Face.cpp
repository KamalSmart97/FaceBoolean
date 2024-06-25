#include"Face.h"

int createFace(Face& face, double pPoints[][2], int nNoOfEdgeLoops)
{
	int nTemp = 0, nNoOfVerticesOfloop = 0, nVerticesCount = 0;
	double length = 0.0, dBBlength = 0, *pBoundingBox = NULL, dTempLen = DBL_MIN;
	vertex* pVertex = NULL, * pPreviousVertex = NULL, * pLoopStartVertex = NULL;
	EdgeLoop* pEdgeLoop = NULL, *pTempLoop = NULL;
	edge* pEdge = NULL;

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
			pEdgeLoop->setVertexlist(pVertex);
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
				pEdge->setEdgeLoop(pEdgeLoop);

				if (j == (nNoOfVerticesOfloop - 1))
				{
					pEdge = new edge();
					pEdge->setVertexlist(pVertex, pLoopStartVertex);
					pLoopStartVertex->setEdgeList(pEdge);
					pVertex->setEdgeList(pEdge);
					pEdge->setEdgeLoop(pEdgeLoop);
					pLoopStartVertex->setFacelist(&face);
					face.setEdgeList(pEdge);

					pEdge->setFacelist(&face);
					pEdge->setColor(1);

					length = distanceBetweenPoints(pVertex->getVertexPosition(), pLoopStartVertex->getVertexPosition());
					pEdgeLoop->setEdgeList(pEdge);
				}
			}
			pPreviousVertex = pVertex;
			nTemp = nTemp + 1;
		}
		//only one edge loop can be of big size
		//others should be inner loop and should be a void
		pEdgeLoop->updateBoundingBox();

		face.setEdgeLoopList(pEdgeLoop);

		pBoundingBox = pEdgeLoop->getBoundingBox();

		dBBlength = distanceBetweenPoints(pBoundingBox, &pBoundingBox[2]);

		if (dBBlength > dTempLen)
		{
			if (pTempLoop)
				pTempLoop->setLoopType(0);
			dTempLen = dBBlength;
			pEdgeLoop->setLoopType(1);
			pTempLoop = pEdgeLoop;
		}
	}
	return 1;
}

int boundingBox(list<vertex*>* pVertexList, double* pMin, double* pMax)
{
	double dMax[2] = { DBL_MIN, DBL_MIN }, dMin[2] = { DBL_MAX, DBL_MAX };
	double* pXY = NULL;

	for (auto pVertex : *pVertexList)
	{
		pXY = pVertex->getVertexPosition();

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

int faceBoolean(Face& face1, Face& face2, Face& BooleanFace)
{

	list<edge*>* pEdgeListFace1 = NULL, * pEdgeListFace2 = NULL;

	vertex* pEdge1Vertex1 = NULL, * pEdge1Vertex2 = NULL, * pEdge2Vertex1 = NULL, * pEdge2Vertex2 = NULL;

	face1.getEdgeList(&pEdgeListFace1);
	face2.getEdgeList(&pEdgeListFace2);

	double dIntersectionPoint[2] = { 0.0 }, dLength = 0.0;
	bool bIntersect = false;
	// create the intesection vertices, edges formed after the intersection
	//update the edge loop
	for (auto pEdge1 : *pEdgeListFace1)
	{
		pEdge1->getEdgeVertices(&pEdge1Vertex1, &pEdge1Vertex2);
		for (auto pEdge2 : *pEdgeListFace2)
		{
			pEdge2->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);
			bIntersect = LineLineIntersect(pEdge1Vertex1->getVertexPosition(), pEdge1Vertex2->getVertexPosition(), pEdge2Vertex1->getVertexPosition(), pEdge2Vertex2->getVertexPosition(), dIntersectionPoint);
			if (bIntersect)
			{
				splitEdgeAtGivenPoint(pEdge1, face1, pEdge1Vertex1, pEdge1Vertex2, dIntersectionPoint);
				splitEdgeAtGivenPoint(pEdge2, face2, pEdge2Vertex1, pEdge2Vertex2, dIntersectionPoint);
			}
		}
	}
	return 1;
}

int splitEdgeAtGivenPoint(edge* pInputEdge, Face& Inputface, vertex* pStartVertex, vertex* pEndVertex, double* pIntersectionPoint)
{
	list<edge*>* pLoopEdgeList = NULL;
	list<vertex*>* pLoopVertexList = NULL;

	vertex* pVertex = NULL;
	edge* pEdge = NULL;
	EdgeLoop* pEdgeLoop = NULL;

	double dIntersectionPoint[2] = { 0.0 }, dLength = 0.0;

	pVertex = new vertex(pIntersectionPoint);
	Inputface.setVertexlist(pVertex);
	pVertex->setFacelist(&Inputface);

	pEdgeLoop = pInputEdge->getEdgeLoop();
	pEdgeLoop->getEdgeList(&pLoopEdgeList);

	//create a new edge 
	pEdge = new edge();
	Inputface.setEdgeList(pEdge);

	pEdge->setVertexlist(pVertex, pEndVertex);
	pVertex->setEdgeList(pEdge);
	pVertex->setEdgeList(pInputEdge);

	pEdge->setFacelist(&Inputface);
	pEdge->setColor(1);

	dLength = distanceBetweenPoints(pVertex->getVertexPosition(), pEndVertex->getVertexPosition());
	pEdge->setLength(dLength);
	dLength = distanceBetweenPoints(pStartVertex->getVertexPosition(), pVertex->getVertexPosition());
	pInputEdge->setLength(dLength);

	pInputEdge->setVertexlist(pStartVertex, pVertex);

	for (list<edge*>::iterator i = pLoopEdgeList->begin(); i != pLoopEdgeList->end(); i++)
	{
		if (*i == pInputEdge)
		{
			pLoopEdgeList->insert((i++), pEdge);
			break;
		}
	}

	pEdgeLoop->getVertexList(&pLoopVertexList);
	for (list<vertex*>::iterator i = pLoopVertexList->begin(); i != pLoopVertexList->end(); i++)
	{
		if (*i == pVertex)
		{
			pLoopVertexList->insert((i++), pVertex);
			break;
		}
	}
	pEdge->setEdgeLoop(pEdgeLoop);
	return 1;	
}