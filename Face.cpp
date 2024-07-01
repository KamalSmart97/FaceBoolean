#include"Face.h"
#define _CRT_SECURE_NO_WARNINGS

int createFace(Face& face, double pPoints[][2], int nNoOfEdgeLoops, int color)
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

		cout << " enter the no vertices in the loop " << i + 1 << "\n";
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
					pEdge->setLength(length);
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

//one case need to be handled empty loop within empty loop

int faceBoolean(Face& face1, Face& face2, Face& BooleanFace)
{

	list<edge*>* pEdgeListFace1 = NULL, * pEdgeListFace2 = NULL, * pLoopEdgeListFace1 = NULL, * pLoopEdgeListFace2 = NULL, * pEdgeList = NULL;
	list<EdgeLoop*>* pFace1LoopList = NULL, * pFace2LoopList = NULL;
	list<vertex*>* pVertexList = NULL, * pLoopVertexListFace1 = NULL, * pLoopVertexListFace2 = NULL;

	vertex* pEdge1Vertex1 = NULL, * pEdge1Vertex2 = NULL, * pEdge2Vertex1 = NULL, * pEdge2Vertex2 = NULL;

	face1.getEdgeList(&pEdgeListFace1);
	face2.getEdgeList(&pEdgeListFace2);

	double dIntersectionPoint[2] = { 0.0 }, dLength[2] = { 0.0 }, dTemp[2] = { 0.0 }, dDistance = 0.0, dTempDistance = 0.0, dCopy1[2] = { 0.0 }, dCopy2[2] = { 0.0 };
	bool bIntersect = false, bSkipEdge = false, bSplit = false;

	double* p[4] = { NULL };
	int nLoopTypeFace1 = 0, nLoopTypeFace2 = 0, nInersection = 0, nReduceIntersection = 0;
	// create the intesection vertices, edges formed after the intersection
	//update the edge loop

	for (list<edge*>::iterator pEdge1 = pEdgeListFace1->begin(); pEdge1 != pEdgeListFace1->end(); pEdge1++)
	{
		for (list<edge*>::iterator pEdge2 = pEdgeListFace2->begin(); pEdge2 != pEdgeListFace2->end(); pEdge2++)
		{
			(*pEdge1)->getEdgeVertices(&pEdge1Vertex1, &pEdge1Vertex2);
			(*pEdge2)->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);

			dLength[0] = (*pEdge1)->getLength();
			dLength[1] = (*pEdge2)->getLength();

			p[0] = pEdge1Vertex1->getVertexPosition();
			p[1] = pEdge1Vertex2->getVertexPosition();
			p[2] = pEdge2Vertex1->getVertexPosition();
			p[3] = pEdge2Vertex2->getVertexPosition();

			bSkipEdge = false;
			for (int i = 0; i < 2; i++)
			{
				dTemp[0] = distanceBetweenPoints(p[i], p[2]);
				dTemp[1] = distanceBetweenPoints(p[i], p[3]);

				if ((dTemp[0] < 0.000001) || (dTemp[1] < 0.000001))
				{
					bSkipEdge = true;
					break;
				}
			}
			if (!bSkipEdge)
			{
				bIntersect = LineLineIntersect(p[0], p[1], p[2], p[3], dIntersectionPoint);

				if (bIntersect)
				{
					bSplit = true;
					for (int i = 0; i < 2; i++)
					{
						dTemp[0] = distanceBetweenPoints(p[2 * i], dIntersectionPoint);
						dTemp[1] = distanceBetweenPoints(p[2 * i + 1], dIntersectionPoint);

						if ((dTemp[0] > dLength[i]) || (dTemp[1] > dLength[i]))
						{
							bSplit = false;
							break;
						}
					}
					if (bSplit)
					{
						splitEdgeAtGivenPoint((*pEdge1), face1, pEdge1Vertex1, pEdge1Vertex2, dIntersectionPoint);
						splitEdgeAtGivenPoint((*pEdge2), face2, pEdge2Vertex1, pEdge2Vertex2, dIntersectionPoint);
					}
				}
			}
		}
	}

	//find the vertices of empty edge loop of one color inside the filled edge loop of other color
	//finding the vertices of empty edge loop of one color inside the empty loop of other color is not done yet.
	face1.getEdgeLoopList(&pFace1LoopList);
	face2.getEdgeLoopList(&pFace2LoopList);
	for (auto pEdgeLoop1 : *pFace1LoopList)
	{
		nLoopTypeFace1 = pEdgeLoop1->getLoopType();
		pEdgeLoop1->getEdgeList(&pLoopEdgeListFace1);
		pEdgeLoop1->getVertexList(&pLoopVertexListFace1);
		for (auto pEdgeLoop2 : *pFace2LoopList)
		{
			nLoopTypeFace2 = pEdgeLoop2->getLoopType();
			if (nLoopTypeFace1 == nLoopTypeFace2)
				continue;

			pEdgeLoop2->getVertexList(&pLoopVertexListFace2);
			pEdgeLoop2->getEdgeList(&pLoopEdgeListFace2);
			if (nLoopTypeFace1 == 1)
			{
				pVertexList = pLoopVertexListFace2;
				pEdgeList = pLoopEdgeListFace1;
			}
			else
			{
				pVertexList = pLoopVertexListFace1;
				pEdgeList = pLoopEdgeListFace2;
			}
			for (auto pVertex : *pVertexList)
			{
				if (pVertex->getSratch() == 1)
					continue;
				nInersection = 0;
				nReduceIntersection = 0;
				for (auto pEdge : *pEdgeList)
				{
					pEdge->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);
					p[0] = pVertex->getVertexPosition();
					dCopy1[0] = 0.0;
					dCopy1[1] = p[0][1];
					p[1] = dCopy1;
					dLength[0] = distanceBetweenPoints(p[0], p[1]);
					p[2] = pEdge2Vertex1->getVertexPosition();
					p[3] = pEdge2Vertex2->getVertexPosition();
					dLength[1] = pEdge->getLength();

					bIntersect = LineLineIntersect(p[0], p[1], p[2], p[3], dIntersectionPoint);

					if (bIntersect)
					{
						bSplit = true;
						for (int i = 0; i < 2; i++)
						{
							dTemp[0] = distanceBetweenPoints(p[2 * i], dIntersectionPoint);
							dTemp[1] = distanceBetweenPoints(p[2 * i + 1], dIntersectionPoint);

							if ((dTemp[0] > (dLength[i]+0.000001)) || (dTemp[1] > (dLength[i]+0.000001)))
							{
								bSplit = false;
								break;
							}
						}
					}
					if (bSplit)
					{
						dTemp[0] = distanceBetweenPoints(dIntersectionPoint, p[2]);
						dTemp[1] = distanceBetweenPoints(dIntersectionPoint, p[3]);

						if ((dTemp[0] < 0.000001) || (dTemp[1] < 0.000001))
							++nReduceIntersection;

						nInersection++;
					}
				}
				if (nReduceIntersection > 0)
				{
					nReduceIntersection = nReduceIntersection / 2;
					nInersection -= nReduceIntersection;
				}
				if (nInersection % 2 != 0)
					pVertex->setScratch(1);
			}
		}
	}
	// mark the edges which should not be there after boolean
	//conditions: if it is a empty edge loop  both the vertices are scratched to one then it should not be there after boolean.
	// because the edge is inside the filled loop of other color.
	// if the edge loop is filled and both the vertices are scratched to one the find wether the edge is inside the filled loop of other color
	// remove it to.
	for (auto pEdgeLoop1 : *pFace1LoopList)
	{
		nLoopTypeFace1 = pEdgeLoop1->getLoopType();
		pEdgeLoop1->getEdgeList(&pLoopEdgeListFace1);

		for (auto pEdge : *pLoopEdgeListFace1)
		{
			pEdge->getEdgeVertices(&pEdge1Vertex1, &pEdge1Vertex2);

			if ((pEdge1Vertex1->getSratch() == 1) && (pEdge1Vertex2->getSratch() == 1))
			{
				if (nLoopTypeFace1 != 1)
				{
					pEdge->setScratch(1);
				}
				else
				{
					/*for (auto pEdgeLoop2 : *pFace2LoopList)
					{
						nLoopTypeFace2 = pEdgeLoop2->getLoopType();
						pEdgeLoop2->getEdgeList(&pLoopEdgeListFace2);

						if (nLoopTypeFace1 == nLoopTypeFace2)
						{
							nInersection = 0;
							nReduceIntersection = 0;
							for (auto pTraversedge : *pLoopEdgeListFace2)
							{
								pTraversedge->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);
								dLength[0] = distanceBetweenPoints(p[0], p[1]);
								p[2] = pEdge2Vertex1->getVertexPosition();
								p[3] = pEdge2Vertex2->getVertexPosition();

								p[0] = pEdge1Vertex1->getVertexPosition();
								p[1] = pEdge1Vertex2->getVertexPosition();
								dCopy1[0] = (p[0][0] +  p[1][0]) / 2;
								dCopy1[1] = (p[0][1] + p[1][1]) / 2;
								dCopy2[0] = 0.0;
								dCopy2[1] = dCopy1[1];
								p[0] = dCopy1;
								p[1] = dCopy2;
								dLength[1] = pEdge->getLength();

								bIntersect = LineLineIntersect(p[0], p[1], p[2], p[3], dIntersectionPoint);

								dTemp[0] = distanceBetweenPoints(dIntersectionPoint, p[2]);
								dTemp[1] = distanceBetweenPoints(dIntersectionPoint, p[3]);

								if ((dTemp[0] < 0.000001) || (dTemp[1] < 0.000001))
									++nReduceIntersection;

								if (bIntersect)
								{
									bSplit = true;
									for (int i = 0; i < 2; i++)
									{
										dTemp[0] = distanceBetweenPoints(p[2 * i], dIntersectionPoint);
										dTemp[1] = distanceBetweenPoints(p[2 * i + 1], dIntersectionPoint);

										if ((dTemp[0] > dLength[i]) || (dTemp[1] > dLength[i]))
										{
											bSplit = false;
											break;
										}
									}
								}
								if (bSplit)
									nInersection++;
							}
							if (nReduceIntersection > 0)
							{
								nReduceIntersection = nReduceIntersection / 2;
								nInersection -= nReduceIntersection;
							}
							if (nInersection % 2 != 0)
								pEdge->setScratch(1);
						}
					}*/
				}
			}
		}
	}
	for (auto pEdgeLoop2 : *pFace2LoopList)
	{
		nLoopTypeFace2 = pEdgeLoop2->getLoopType();
		pEdgeLoop2->getEdgeList(&pLoopEdgeListFace2);

		for (auto pEdge : *pLoopEdgeListFace2)
		{
			pEdge->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);

			if ((pEdge2Vertex1->getSratch() == 1) && (pEdge2Vertex2->getSratch() == 1))
			{
				if (nLoopTypeFace2 != 1)
				{
					pEdge->setScratch(1);
				}
				else
				{
					/*for (auto pEdgeLoop1 : *pFace1LoopList)
					{
						nLoopTypeFace1 = pEdgeLoop1->getLoopType();
						pEdgeLoop1->getEdgeList(&pLoopEdgeListFace1);

						if (nLoopTypeFace1 == nLoopTypeFace2)
						{
							nInersection = 0;
							nReduceIntersection = 0;
							for (auto pTraversedge : *pLoopEdgeListFace1)
							{
								pTraversedge->getEdgeVertices(&pEdge1Vertex1, &pEdge1Vertex2);
								dLength[0] = distanceBetweenPoints(p[0], p[1]);
								p[2] = pEdge1Vertex1->getVertexPosition();
								p[3] = pEdge1Vertex2->getVertexPosition();

								p[0] = pEdge2Vertex1->getVertexPosition();
								p[1] = pEdge2Vertex2->getVertexPosition();
								dCopy1[0] = (p[0][0] + p[1][0]) / 2;
								dCopy1[1] = (p[0][1] + p[1][1]) / 2;
								dCopy2[0] = 0.0;
								dCopy2[1] = dCopy1[1];
								p[0] = dCopy1;
								p[1] = dCopy2;
								dLength[1] = pEdge->getLength();

								bIntersect = LineLineIntersect(p[0], p[1], p[2], p[3], dIntersectionPoint);

								dTemp[0] = distanceBetweenPoints(dIntersectionPoint, p[2]);
								dTemp[1] = distanceBetweenPoints(dIntersectionPoint, p[3]);

								if ((dTemp[0] < 0.000001) || (dTemp[1] < 0.000001))
									++nReduceIntersection;

								if (bIntersect)
								{
									bSplit = true;
									for (int i = 0; i < 2; i++)
									{
										dTemp[0] = distanceBetweenPoints(p[2 * i], dIntersectionPoint);
										dTemp[1] = distanceBetweenPoints(p[2 * i + 1], dIntersectionPoint);

										if ((dTemp[0] > dLength[i]) || (dTemp[1] > dLength[i]))
										{
											bSplit = false;
											break;
										}
									}
								}
								if (bSplit)
									nInersection++;
							}
							if (nReduceIntersection > 0)
							{
								nReduceIntersection = nReduceIntersection / 2;
								nInersection -= nReduceIntersection;
							}
							if (nInersection % 2 != 0)
								pEdge->setScratch(1);
						}
					}*/
				}
			}
		}
	}

	FILE* pDebugFile = NULL;
	FILE* pDebugFile1 = NULL;

	pDebugFile1 = fopen("d:\\edgeDumpnew.txt", "w");
	//pDebugFile = fopen("d:\\edgeDumpwithScratch.txt", "w");
	//face1.getVertexList(&pVertexList);
	face1.getEdgeLoopList(&pFace1LoopList);

	for (auto pEdgeLoop1 : *pFace1LoopList)
	{
		pEdgeLoop1->getEdgeList(&pEdgeList);
		for (auto pEdge1 : *pEdgeList)
		{
			pEdge1->getEdgeVertices(&pEdge1Vertex1, &pEdge1Vertex2);
			if (pEdge1->getSratch() == 1)
				fprintf(pDebugFile1, "%lf, %lf, %lf, %lf,remove\n", pEdge1Vertex1->getVertexPosition()[0], pEdge1Vertex1->getVertexPosition()[1], pEdge1Vertex2->getVertexPosition()[0], pEdge1Vertex2->getVertexPosition()[1]);
			else
				fprintf(pDebugFile1, "%lf, %lf, %lf, %lf, \n", pEdge1Vertex1->getVertexPosition()[0], pEdge1Vertex1->getVertexPosition()[1], pEdge1Vertex2->getVertexPosition()[0], pEdge1Vertex2->getVertexPosition()[1]);
		}
	}
	//face2.getVertexList(&pVertexList);
	face2.getEdgeLoopList(&pFace2LoopList);
	for (auto pEdgeLoop1 : *pFace2LoopList)
	{
		pEdgeLoop1->getEdgeList(&pEdgeList);
		for (auto pEdge2 : *pEdgeList)
		{
			pEdge2->getEdgeVertices(&pEdge2Vertex1, &pEdge2Vertex2);
			if (pEdge2->getSratch() == 1)
				fprintf(pDebugFile1, "%lf, %lf, %lf, %lf,remove\n", pEdge2Vertex1->getVertexPosition()[0], pEdge2Vertex1->getVertexPosition()[1], pEdge2Vertex2->getVertexPosition()[0], pEdge2Vertex2->getVertexPosition()[1]);
			else
				fprintf(pDebugFile1, "%lf, %lf, %lf, %lf, \n", pEdge2Vertex1->getVertexPosition()[0], pEdge2Vertex1->getVertexPosition()[1], pEdge2Vertex2->getVertexPosition()[0], pEdge2Vertex2->getVertexPosition()[1]);
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
	pVertex->setScratch(1);
	
	pEdgeLoop = pInputEdge->getEdgeLoop();
	pEdgeLoop->getEdgeList(&pLoopEdgeList);

	//create a new edge 
	pEdge = new edge();
	Inputface.setEdgeList(pEdge);

	pEdge->setVertexlist(pVertex, pEndVertex);
	pVertex->setEdgeList(pEdge);
	pVertex->setEdgeList(pInputEdge);

	pEdge->setFacelist(&Inputface);
	pEdge->setColor(2);

	dLength = distanceBetweenPoints(pVertex->getVertexPosition(), pEndVertex->getVertexPosition());
	pEdge->setLength(dLength);
	dLength = distanceBetweenPoints(pStartVertex->getVertexPosition(), pVertex->getVertexPosition());
	pInputEdge->setLength(dLength);

	pInputEdge->setVertexlist(pStartVertex, pVertex);

	for (list<edge*>::iterator i = pLoopEdgeList->begin(); i != pLoopEdgeList->end(); i++)
	{
		if (*i == pInputEdge)
		{
			pLoopEdgeList->insert((++i), pEdge);
			break;
		}
	}

	pEdgeLoop->getVertexList(&pLoopVertexList);
	for (list<vertex*>::iterator i = pLoopVertexList->begin(); i != pLoopVertexList->end(); i++)
	{
		if (*i == pStartVertex)
		{
			pLoopVertexList->insert((++i), pVertex);
			break;
		}
	}
	pEdge->setEdgeLoop(pEdgeLoop);
	return 1;	
}