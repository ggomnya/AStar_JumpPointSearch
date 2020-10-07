#include "JSPClass.h"

JSP::JSP(int MaxX, int MaxY) {
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

JSP::~JSP() {
	ClearList();
	for (int i = 0; i < _MaxX; i++) {
		free(_Map[i]);
	}
	free(_Map);
}



void JSP::ClearList() {
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

void JSP::SetMapInfo(int curX, int curY, int Info) {
	_Map[curX][curY] = Info;
}

Node* JSP::GetFinalNode() {
	return _FinalNode;
}

bool JSP::FindPath(int StartX, int StartY, int EndX, int EndY) {
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
				//방향 찾기
				int Dir;
				if (curNode->pParent == NULL)
					Dir = START;
				else {
					if (curNode->pParent->X == curNode->X && curNode->pParent->Y > curNode->Y) {
						Dir = UU;
					}
					else if (curNode->pParent->X < curNode->X && curNode->pParent->Y > curNode->Y) {
						Dir = RU;
					}
					else if (curNode->pParent->X < curNode->X && curNode->pParent->Y == curNode->Y) {
						Dir = RR;
					}
					else if (curNode->pParent->X < curNode->X && curNode->pParent->Y < curNode->Y) {
						Dir = RD;
					}
					else if (curNode->pParent->X == curNode->X && curNode->pParent->Y < curNode->Y) {
						Dir = DD;
					}
					else if (curNode->pParent->X > curNode->X&& curNode->pParent->Y < curNode->Y) {
						Dir = LD;
					}
					else if (curNode->pParent->X > curNode->X&& curNode->pParent->Y == curNode->Y) {
						Dir = LL;
					}
					else if (curNode->pParent->X > curNode->X&& curNode->pParent->Y > curNode->Y) {
						Dir = LU;
					}
				}
				switch (Dir) {
				case START:
				{
					//UU 코너 탐색
					FindCorner(curNode, UU, curX, curY - 1);
					//RU 코너 탐색
					FindCorner(curNode, RU, curX + 1, curY - 1);
					//RR 코너 탐색
					FindCorner(curNode, RR, curX + 1, curY);
					//RD 코너 탐색
					FindCorner(curNode, RD, curX + 1, curY + 1);
					//DD 코너 탐색
					FindCorner(curNode, DD, curX, curY + 1);
					//LD 코너 탐색
					FindCorner(curNode, LD, curX - 1, curY + 1);
					//LL 코너 탐색
					FindCorner(curNode, LL, curX - 1, curY);
					//LU 코너 탐색
					FindCorner(curNode, LU, curX - 1, curY - 1);
				}
				break;
				case UU:
				{
					//위쪽 코너 탐색
					FindCorner(curNode, UU, curX, curY - 1);
					//왼쪽이 코너인 경우
					if (curX > 0 && curY > 0) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//오른쪽이 코너인 경우
					if (curX < _MaxX - 1 && curY>0) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
				}
				break;
				case RU:
				{
					//RU 대각선 코너 탐색
					FindCorner(curNode, RU, curX + 1, curY - 1);
					//위쪽 코너 탐색
					FindCorner(curNode, UU, curX, curY - 1);
					//오른쪽 코너 탐색
					FindCorner(curNode, RR, curX + 1, curY);

					//LU방향이 코너인 경우
					if (curX > 0 && curY > 0) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//RD방향이 코너인 경우
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case RR:
				{
					//오른쪽 코너 탐색
					FindCorner(curNode, RR, curX + 1, curY);
					//위쪽이 코너인 경우
					if (curX < _MaxX - 1 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
					//아래쪽이 코너인 경우
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case RD:
				{
					//RD 대각선 코너 탐색
					FindCorner(curNode, RD, curX + 1, curY + 1);
					//아래쪽 코너 탐색
					FindCorner(curNode, DD, curX, curY + 1);
					//오른쪽 코너 탐색
					FindCorner(curNode, RR, curX + 1, curY);

					//LD방향이 코너인 경우
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//RU방향이 코너인 경우
					if (curX < _MaxX - 1 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
				}
				break;
				case DD:
				{
					//아래쪽 코너 탐색
					FindCorner(curNode, DD, curX, curY + 1);
					//왼쪽이 코너인 경우
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//오른쪽이 코너인 경우
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case LD:
				{
					//LD 대각선 코너 탐색
					FindCorner(curNode, LD, curX - 1, curY + 1);
					//아래쪽 코너 탐색
					FindCorner(curNode, DD, curX, curY + 1);
					//왼쪽 코너 탐색
					FindCorner(curNode, LL, curX - 1, curY);

					//LU방향이 코너인 경우
					if (curX > 0 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//RD방향이 코너인 경우
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case LL:
				{
					//왼쪽 코너 탐색
					FindCorner(curNode, LL, curX - 1, curY);
					//위쪽이 코너인 경우
					if (curX > 0 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//아래쪽이 코너인 경우
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
				}
				break;
				case LU:
				{
					//LU 대각선 코너 탐색
					FindCorner(curNode, LU, curX - 1, curY - 1);
					//위쪽 코너 탐색
					FindCorner(curNode, UU, curX, curY - 1);
					//왼쪽 코너 탐색
					FindCorner(curNode, LL, curX - 1, curY);

					//LD방향이 코너인 경우
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//RU방향이 코너인 경우
					if (curX < _MaxX - 1 && curY > 0) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
				}
				break;
				}
			}
			OpenList.sort(compare);
		}
		else {
			return false;
		}

	}
}
void JSP::InsertNode(Node* curNode, int curX, int curY) {
	map<pair<int, int>, Node*>::iterator it;
	//close에 포함되지 않은 경우
	if (CloseMap.find(make_pair(curX, curY)) == CloseMap.end()) {
		//Open에도 포함되어 있지 않은 경우
		it = OpenMap.find(make_pair(curX, curY));
		if (it == OpenMap.end()) {
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->pParent = curNode;
			newNode->X = curX;
			newNode->Y = curY;
			newNode->G = curNode->G + abs(curX - curNode->X) + abs(curY - curNode->Y);
			newNode->H = abs(curX - _EndX) + abs(curY - _EndY);
			newNode->F = newNode->G + newNode->H;
			OpenList.push_back(newNode);
			OpenMap.insert(make_pair(make_pair(curX, curY), newNode));
		}
		//Open에 포함되어 있는 경우
		else {
			if (it->second->G > (curNode->G + abs(curX - curNode->X) + abs(curY - curNode->Y))) {
				it->second->G = curNode->G + abs(curX - curNode->X) + abs(curY - curNode->Y);
				it->second->F = it->second->G + it->second->H;
			}
		}
	}
}
void JSP::FindCorner(Node* curNode, int Dir, int StartX, int StartY) {
	int curX = StartX;
	int curY = StartY;
	switch (Dir) {
	case UU:
	{
		//윗 방향 
		while (1) {
			//범위 밖인 경우
			if (curY < 0) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}

			//왼쪽이 코너인 경우
			if (curX > 0 && curY > 0) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//오른쪽이 코너인 경우
			if (curX < _MaxX - 1 && curY>0) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}

			curY--;
		}
	}
	break;
	case RU:
	{
		while (1) {
			//범위 밖인 경우
			if (curY < 0 || curX >= _MaxX) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LU방향 코너 확인
			if (curX > 0 && curY > 0) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RD방향 코너 확인
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//위쪽 탐색
			int TempX = curX;
			int TempY = curY;
			while (1) {
				TempY--;
				//범위 밖인 경우
				if (TempY < 0) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//왼쪽이 코너인 경우
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//오른쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY>0) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}
			//오른쪽 탐색
			TempX = curX;
			TempY = curY;
			//오른쪽 방향
			while (1) {
				TempX++;
				//범위 밖인 경우
				if (TempX >= _MaxX) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//위쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//아래쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX][TempY + 1] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}


			curY--;
			curX++;
		}
	}
	break;
	case RR:
	{
		//오른쪽 방향
		while (1) {

			//범위 밖인 경우
			if (curX >= _MaxX) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//위쪽이 코너인 경우
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//아래쪽이 코너인 경우
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}


			curX++;
		}
	}
	break;
	case RD:
	{
		while (1) {

			//범위 밖인 경우
			if (curY >= _MaxY || curX >= _MaxX) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LD방향 코너 확인
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RU방향 코너 확인
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//아래쪽 탐색
			int TempX = curX;
			int TempY = curY;
			//아래 방향
			while (1) {
				TempY++;
				//범위 밖인 경우
				if (TempY >= _MaxY) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//왼쪽이 코너인 경우
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//오른쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}
			//오른쪽 탐색
			TempX = curX;
			TempY = curY;
			//오른쪽 방향
			while (1) {
				TempX++;
				//범위 밖인 경우
				if (TempX >= _MaxX) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//위쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//아래쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX][TempY + 1] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}


			curY++;
			curX++;
		}
	}
	break;
	case DD:
	{
		//아래 방향
		while (1) {

			//범위 밖인 경우
			if (curY >= _MaxY) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//왼쪽이 코너인 경우
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//오른쪽이 코너인 경우
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}

			curY++;
		}
	}
	break;
	case LD:
	{
		while (1) {

			//범위 밖인 경우
			if (curY >= _MaxY || curX < 0) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LU방향 코너 확인
			if (curX > 0 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RD방향 코너 확인
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//아래쪽 탐색
			int TempX = curX;
			int TempY = curY;
			//아래 방향
			while (1) {
				TempY++;
				//범위 밖인 경우
				if (TempY >= _MaxY) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}

				//왼쪽이 코너인 경우
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//오른쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}
			//왼쪽 탐색
			TempX = curX;
			TempY = curY;
			//왼쪽 방향
			while (1) {
				TempX--;
				//범위 밖인 경우
				if (TempX < 0) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//위쪽이 코너인 경우
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//아래쪽이 코너인 경우
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX][TempY + 1] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}

			curY++;
			curX--;
		}
	}
	break;
	case LL:
	{
		//왼쪽 방향
		while (1) {

			//범위 밖인 경우
			if (curX < 0) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}

			//위쪽이 코너인 경우
			if (curX > 0 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//아래쪽이 코너인 경우
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}

			curX--;
		}
	}
	break;
	case LU:
	{
		while (1) {

			//범위 밖인 경우
			if (curY < 0 || curX < 0) break;
			//막힌 경우
			if (_Map[curX][curY] == CLOSE)
				break;
			//목적지인 경우
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LD방향 코너 확인
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RU방향 코너 확인
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//위쪽 탐색
			int TempX = curX;
			int TempY = curY;
			while (1) {
				TempY--;
				//범위 밖인 경우
				if (TempY < 0) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//왼쪽이 코너인 경우
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//오른쪽이 코너인 경우
				if (TempX < _MaxX - 1 && TempY>0) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}
			//왼쪽 탐색
			TempX = curX;
			TempY = curY;
			//왼쪽 방향
			while (1) {
				TempX--;
				//범위 밖인 경우
				if (TempX < 0) break;
				//막힌 경우
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//목적지인 경우
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//위쪽이 코너인 경우
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//아래쪽이 코너인 경우
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX][TempY + 1] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}

			curY--;
			curX--;
		}
	}
	break;
	}
}

bool JSP::DrawLine(int StartX, int StartY, int EndX, int EndY) {
	bool bAxle;
	char quadrant = 0;
	int iError = 0;
	int iXaxle = abs(StartX - EndX);
	int iYaxle = abs(StartY - EndY);
	if (iXaxle > iYaxle) {
		bAxle = true;
	}
	else
	{
		bAxle = false;
	}
	if (EndX >= StartX && EndY >= StartY)
		quadrant = 1;
	else if (EndX<StartX && EndY > StartY) {
		quadrant = 2;
	}
	else if (EndX <= StartX && EndY <= StartY) {
		quadrant = 3;
	}
	else if (EndX > StartX&& EndY < StartY) {
		quadrant = 4;
	}
	//X축이 기준
	if (bAxle) {
		switch (quadrant) {
		case 1:
		{
			int curY = StartY;
			for (int curX = StartX; curX <= EndX; curX++) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iYaxle;
				if (iError > iXaxle / 2) {
					iError -= iXaxle;
					curY++;
				}
			}
		}
		break;
		case 2:
		{
			int curY = StartY;
			for (int curX = StartX; curX >= EndX; curX--) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iYaxle;
				if (iError > iXaxle / 2) {
					iError -= iXaxle;
					curY++;
				}

			}
		}
		break;
		case 3:
		{
			int curY = StartY;
			for (int curX = StartX; curX >= EndX; curX--) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iYaxle;
				if (iError > iXaxle / 2) {
					iError -= iXaxle;
					curY--;
				}

			}
		}
		break;
		case 4:
		{
			int curY = StartY;
			for (int curX = StartX; curX <= EndX; curX++) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iYaxle;
				if (iError > iXaxle / 2) {
					iError -= iXaxle;
					curY--;
				}

			}
		}
		break;

		default:
			break;
		}
	}
	//Y축이 기준
	else {
		switch (quadrant) {
		case 1:
		{
			int curX = StartX;
			for (int curY = StartY; curY <= EndY; curY++) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iXaxle;
				if (iError > iYaxle / 2) {
					iError -= iYaxle;
					curX++;
				}

			}
		}
		break;
		case 2:
		{
			int curX = StartX;
			for (int curY = StartY; curY <= EndY; curY++) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iXaxle;
				if (iError > iYaxle / 2) {
					iError -= iYaxle;
					curX--;
				}

			}
		}
		break;
		case 3:
		{
			int curX = StartX;
			for (int curY = StartY; curY >= EndY; curY--) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iXaxle;
				if (iError > iYaxle / 2) {
					iError -= iYaxle;
					curX--;
				}

			}
		}
		break;
		case 4:
		{
			int curX = StartX;
			for (int curY = StartY; curY >= EndY; curY--) {
				if (_Map[curX][curY] == CLOSE)
					return false;
				iError += iXaxle;
				if (iError > iYaxle / 2) {
					iError -= iYaxle;
					curX++;
				}

			}
		}
		break;
		default:
			break;
		}
	}
	return true;
}

void JSP::FindShortestPath() {
	if (_FinalNode == NULL)
		return;
	Node* curNode = _FinalNode;

	while (1) {
		if (curNode->pParent == NULL)
			break;
		if (curNode->pParent->pParent == NULL)
			break;
		Node* temp = curNode->pParent->pParent;
		//true면 직선으로 이을 수 있다는 뜻
		if (DrawLine(curNode->X, curNode->Y, temp->X, temp->Y)) {
			curNode->pParent = temp;
			continue;
		}
		else {
			curNode = curNode->pParent;
		}
	}

}

int JSP::GetTotalNode() {
	return _TotalNode;
}