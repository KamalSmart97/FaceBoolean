#include"Face.h"
#include"commonFunctions.h"

// this is done under the assumption that no edge loop having the common edge.
// No two loop shares the common vertex.
int main()
{
	Face redFace, blackFace;
	vertex* pVertex = NULL, * pPreviousVertex = NULL, * pLoopStartVertex = NULL;
	EdgeLoop* pEdgeLoop = NULL;
	edge* pEdge = NULL;
	int nNoOfVerticesOfloop = 0, nNoOfEdgeLoops = 0, nNoOfverticesOfTheFace = 0, nEnterTheEdgesOfFace = 0, nVerticesCount = 0,nTemp = 0;
	double dblackVertices[12][2] = { 3.0,0.5,2.0,9.0,7.0,9.0,4.75,5.0,9.0,3.0,4.0,8.0,3.0,6.5,5.0,6.5,3.5,4.0,5.5,4.0,5.5,2.0,3.5,2.0 };
	double dRedVertices[8][2] = { 1.5,8.5,7.5,6.5,1.0,1.0,4.5,6.25, 5.0,6.0,6.0,5.5,4.5,4.5,4.0,5.0 };
	double length = 0.0;

	//color back =1
	//color red = 2
	blackFace.setColor(1);
	redFace.setColor(2);

	cout << " enter the total no of vertices of the faces";
	cin >> nNoOfverticesOfTheFace;
	cout << " enter the total no of edges of the faces";
	cin >> nEnterTheEdgesOfFace;
	cout << " enter the no of edge loops";
	cin >> nNoOfEdgeLoops;

	createFace(blackFace, dblackVertices, nNoOfEdgeLoops);

	cout << " enter the total no of vertices of the red faces";
	cin >> nNoOfverticesOfTheFace;
	cout << " enter the total no of edges of the red faces";
	cin >> nEnterTheEdgesOfFace;
	cout << " enter the no of edge loops";
	cin >> nNoOfEdgeLoops;
	createFace(redFace, dRedVertices, nNoOfEdgeLoops);


	return 0;
		
	
}