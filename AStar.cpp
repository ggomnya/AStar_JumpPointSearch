#include "AStar.h"

AStar::AStar(int MaxX, int MaxY) {
	_MaxX = MaxX;
	_MaxY = MaxY;
	_TotalNode = 0;
	_Map = (int**)malloc(sizeof(int*) * _MaxX);
	for (int i = 0; i < _MaxX; i++) {
		_Map[i] = (int*)malloc(sizeof(int) * _MaxY);
		for (int j = 0; j < _MaxY; j++)
			_Map[i][j] = g_Map[i][j];
	}
}
AStar::~AStar() {
	ClearList();
	for (int i = 0; i < _MaxX; i++) {
		free(_Map[i]);
	}
	free(_Map);
}
void AStar::ClearList() {
	OpenMap.clear();
	CloseMap.clear();
	while (!OpenList.empty()) {
		Node* Temp = OpenList.front();
		free(Temp);
		OpenList.pop_front();
	}
	while (!CloseList.empty()) {
		Node* Temp = CloseList.front();
		free(Temp);
		CloseList.pop_front();
	}
}
bool AStar::FindPath(int StartX, int StartY, int EndX, int EndY) {

	//시작전 메모리 정리
	ClearList();

	_StartX = StartX;
	_StartY = StartY;
	_EndX = EndX;
	_EndY = EndY;

	//처음 시작점 노드 생성 후 list에 삽입
	Node* startNode = (Node*)malloc(sizeof(Node));
	startNode->pParent = NULL;
	startNode->X = _StartX;
	startNode->Y = _StartY;
	startNode->G = 0;
	startNode->H = abs(_StartX - _EndX) + abs(_StartY - _EndY);
	startNode->F = startNode->G + startNode->H;
	OpenList.push_front(startNode);
	OpenMap.insert(make_pair(make_pair(startNode->X, startNode->Y), startNode));
	_FinalNode = NULL;



	while (1) {
		if (!OpenList.empty()) {
			Node* curNode = OpenList.front();
			int curX = curNode->X;
			int curY = curNode->Y;
			OpenList.pop_front();
			OpenMap.erase(OpenMap.find(make_pair(curX, curY)));
			CloseList.push_back(curNode);
			CloseMap.insert(make_pair(make_pair(curX, curY), curNode));
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				_FinalNode = curNode;
				_TotalNode = OpenMap.size();
				_TotalNode += CloseMap.size();
				return true;
			}
			else {
				//주변 방향 노드들 삽입
				//UU
				if (curY > 0 && _Map[curX][curY - 1] != CLOSE) {
					InsertNode(curNode, curX, curY - 1, true);
				}
				//RU
				if (curY > 0 && curX < MAXX - 1 && _Map[curX + 1][curY - 1] != CLOSE) {
					InsertNode(curNode, curX + 1, curY - 1, false);
				}
				//RR
				if (curX < MAXX - 1 && _Map[curX + 1][curY] != CLOSE) {
					InsertNode(curNode, curX + 1, curY, true);
				}
				//RD
				if (curY < MAXY - 1 && curX < MAXX - 1 && _Map[curX + 1][curY + 1] != CLOSE) {
					InsertNode(curNode, curX + 1, curY + 1, false);
				}
				//DD
				if (curY < MAXY - 1 && _Map[curX][curY + 1] != CLOSE) {
					InsertNode(curNode, curX, curY + 1, true);
				}
				//LD
				if (curX > 0 && curY < MAXY - 1 && _Map[curX - 1][curY + 1] != CLOSE) {
					InsertNode(curNode, curX - 1, curY + 1, false);
				}
				//LL
				if (curX > 0 && _Map[curX - 1][curY] != CLOSE) {
					InsertNode(curNode, curX - 1, curY, true);
				}
				//LU
				if (curX > 0 && curY > 0 && _Map[curX - 1][curY - 1] != CLOSE) {
					InsertNode(curNode, curX - 1, curY - 1, false);
				}
			}
			OpenList.sort(compare);
		}
		else {
			return false;
		}
	}
}
void AStar::InsertNode(Node* curNode, int curX, int curY, bool bLine) {
	map<pair<int, int>, Node*>::iterator it;
	//close에 포함되지 않은 경우
	if (CloseMap.find(make_pair(curX, curY)) == CloseMap.end()) {
		it = OpenMap.find(make_pair(curX, curY));
		//Open에 포함되어 있지만 경로가 더 짧은 경우 갱신
		if (it != OpenMap.end()) {
			if (bLine) {
				if (it->second->G > curNode->G + 1) {
					it->second->G = curNode->G + 1;
					it->second->F = it->second->G + it->second->H;
					it->second->pParent = curNode;
				}
			}
			else {
				if (it->second->G > curNode->G + 1.5) {
					it->second->G = curNode->G + 1.5;
					it->second->F = it->second->G + it->second->H;
					it->second->pParent = curNode;
				}
			}
		}
		//Open에도 포함되어 있지 않은 경우
		else {
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->pParent = curNode;
			newNode->X = curX;
			newNode->Y = curY;
			if (bLine) {
				newNode->G = curNode->G + 1;
			}
			else {
				newNode->G = curNode->G + 1.5;
			}
			newNode->H = abs(curX - _EndX) + abs(curY - _EndY);
			newNode->F = newNode->G + newNode->H;
			OpenList.push_back(newNode);
			OpenMap.insert(make_pair(make_pair(curX, curY), newNode));
		}
	}
}
void AStar::SetMapInfo(int curX, int curY, int Info) {
	_Map[curX][curY] = Info;
}
Node* AStar::GetFinalNode() {
	return _FinalNode;
}

int AStar::GetTotalNode() {
	return _TotalNode;
}