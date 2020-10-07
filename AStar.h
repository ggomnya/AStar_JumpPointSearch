#pragma once
#include <stdlib.h>
#include <map>
#include <list>
#include "Node.h"
using namespace std;

extern int g_Map[100][50];

class AStar {
private:

	#define MAXX	100
	#define MAXY	50

	enum { OPEN, CLOSE, PATH };

	int _StartX;
	int _StartY;
	int _EndX;
	int _EndY;
	int _MaxX;
	int _MaxY;
	int** _Map;
	Node* _FinalNode;
	list<Node*> OpenList;
	list<Node*> CloseList;
	map<pair<int, int>, Node*>OpenMap;
	map<pair<int, int>, Node*>CloseMap;
	int _TotalNode;

public:
	AStar(int MaxX = MAXX, int MaxY = MAXY);
	~AStar();
	void ClearList();
	bool FindPath(int StartX, int StartY, int EndX, int EndY);
	void InsertNode(Node* curNode, int curX, int curY, bool bLine);
	void SetMapInfo(int curX, int curY, int Info);
	Node* GetFinalNode();
	int GetTotalNode();
};
