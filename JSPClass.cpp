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
	//������ �޸� ����
	ClearList();

	_StartX = StartX;
	_StartY = StartY;
	_EndX = EndX;
	_EndY = EndY;

	//ó�� ������ ��� ���� �� list�� ����
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
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				_FinalNode = curNode;
				_TotalNode = OpenMap.size();
				_TotalNode += CloseMap.size();
				return true;
			}
			else {
				//���� ã��
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
					//UU �ڳ� Ž��
					FindCorner(curNode, UU, curX, curY - 1);
					//RU �ڳ� Ž��
					FindCorner(curNode, RU, curX + 1, curY - 1);
					//RR �ڳ� Ž��
					FindCorner(curNode, RR, curX + 1, curY);
					//RD �ڳ� Ž��
					FindCorner(curNode, RD, curX + 1, curY + 1);
					//DD �ڳ� Ž��
					FindCorner(curNode, DD, curX, curY + 1);
					//LD �ڳ� Ž��
					FindCorner(curNode, LD, curX - 1, curY + 1);
					//LL �ڳ� Ž��
					FindCorner(curNode, LL, curX - 1, curY);
					//LU �ڳ� Ž��
					FindCorner(curNode, LU, curX - 1, curY - 1);
				}
				break;
				case UU:
				{
					//���� �ڳ� Ž��
					FindCorner(curNode, UU, curX, curY - 1);
					//������ �ڳ��� ���
					if (curX > 0 && curY > 0) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//�������� �ڳ��� ���
					if (curX < _MaxX - 1 && curY>0) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
				}
				break;
				case RU:
				{
					//RU �밢�� �ڳ� Ž��
					FindCorner(curNode, RU, curX + 1, curY - 1);
					//���� �ڳ� Ž��
					FindCorner(curNode, UU, curX, curY - 1);
					//������ �ڳ� Ž��
					FindCorner(curNode, RR, curX + 1, curY);

					//LU������ �ڳ��� ���
					if (curX > 0 && curY > 0) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//RD������ �ڳ��� ���
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case RR:
				{
					//������ �ڳ� Ž��
					FindCorner(curNode, RR, curX + 1, curY);
					//������ �ڳ��� ���
					if (curX < _MaxX - 1 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
					//�Ʒ����� �ڳ��� ���
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case RD:
				{
					//RD �밢�� �ڳ� Ž��
					FindCorner(curNode, RD, curX + 1, curY + 1);
					//�Ʒ��� �ڳ� Ž��
					FindCorner(curNode, DD, curX, curY + 1);
					//������ �ڳ� Ž��
					FindCorner(curNode, RR, curX + 1, curY);

					//LD������ �ڳ��� ���
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//RU������ �ڳ��� ���
					if (curX < _MaxX - 1 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
							FindCorner(curNode, RU, curX + 1, curY - 1);
						}
					}
				}
				break;
				case DD:
				{
					//�Ʒ��� �ڳ� Ž��
					FindCorner(curNode, DD, curX, curY + 1);
					//������ �ڳ��� ���
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//�������� �ڳ��� ���
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case LD:
				{
					//LD �밢�� �ڳ� Ž��
					FindCorner(curNode, LD, curX - 1, curY + 1);
					//�Ʒ��� �ڳ� Ž��
					FindCorner(curNode, DD, curX, curY + 1);
					//���� �ڳ� Ž��
					FindCorner(curNode, LL, curX - 1, curY);

					//LU������ �ڳ��� ���
					if (curX > 0 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//RD������ �ڳ��� ���
					if (curX < _MaxX - 1 && curY < _MaxY - 1) {
						if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
							FindCorner(curNode, RD, curX + 1, curY + 1);
						}
					}
				}
				break;
				case LL:
				{
					//���� �ڳ� Ž��
					FindCorner(curNode, LL, curX - 1, curY);
					//������ �ڳ��� ���
					if (curX > 0 && curY > 0) {
						if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
							FindCorner(curNode, LU, curX - 1, curY - 1);
						}
					}
					//�Ʒ����� �ڳ��� ���
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
				}
				break;
				case LU:
				{
					//LU �밢�� �ڳ� Ž��
					FindCorner(curNode, LU, curX - 1, curY - 1);
					//���� �ڳ� Ž��
					FindCorner(curNode, UU, curX, curY - 1);
					//���� �ڳ� Ž��
					FindCorner(curNode, LL, curX - 1, curY);

					//LD������ �ڳ��� ���
					if (curX > 0 && curY < _MaxY - 1) {
						if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
							FindCorner(curNode, LD, curX - 1, curY + 1);
						}
					}
					//RU������ �ڳ��� ���
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
	//close�� ���Ե��� ���� ���
	if (CloseMap.find(make_pair(curX, curY)) == CloseMap.end()) {
		//Open���� ���ԵǾ� ���� ���� ���
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
		//Open�� ���ԵǾ� �ִ� ���
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
		//�� ���� 
		while (1) {
			//���� ���� ���
			if (curY < 0) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}

			//������ �ڳ��� ���
			if (curX > 0 && curY > 0) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//�������� �ڳ��� ���
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
			//���� ���� ���
			if (curY < 0 || curX >= _MaxX) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LU���� �ڳ� Ȯ��
			if (curX > 0 && curY > 0) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RD���� �ڳ� Ȯ��
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//���� Ž��
			int TempX = curX;
			int TempY = curY;
			while (1) {
				TempY--;
				//���� ���� ���
				if (TempY < 0) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�������� �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY>0) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}
			//������ Ž��
			TempX = curX;
			TempY = curY;
			//������ ����
			while (1) {
				TempX++;
				//���� ���� ���
				if (TempX >= _MaxX) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�Ʒ����� �ڳ��� ���
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
		//������ ����
		while (1) {

			//���� ���� ���
			if (curX >= _MaxX) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//������ �ڳ��� ���
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//�Ʒ����� �ڳ��� ���
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

			//���� ���� ���
			if (curY >= _MaxY || curX >= _MaxX) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LD���� �ڳ� Ȯ��
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RU���� �ڳ� Ȯ��
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//�Ʒ��� Ž��
			int TempX = curX;
			int TempY = curY;
			//�Ʒ� ����
			while (1) {
				TempY++;
				//���� ���� ���
				if (TempY >= _MaxY) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�������� �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}
			//������ Ž��
			TempX = curX;
			TempY = curY;
			//������ ����
			while (1) {
				TempX++;
				//���� ���� ���
				if (TempX >= _MaxX) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�Ʒ����� �ڳ��� ���
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
		//�Ʒ� ����
		while (1) {

			//���� ���� ���
			if (curY >= _MaxY) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//������ �ڳ��� ���
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX - 1][curY] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//�������� �ڳ��� ���
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

			//���� ���� ���
			if (curY >= _MaxY || curX < 0) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LU���� �ڳ� Ȯ��
			if (curX > 0 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RD���� �ڳ� Ȯ��
			if (curX < _MaxX - 1 && curY < _MaxY - 1) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//�Ʒ��� Ž��
			int TempX = curX;
			int TempY = curY;
			//�Ʒ� ����
			while (1) {
				TempY++;
				//���� ���� ���
				if (TempY >= _MaxY) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}

				//������ �ڳ��� ���
				if (TempX > 0 && TempY < _MaxY - 1) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�������� �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY < _MaxY - 1) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY + 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}



			}
			//���� Ž��
			TempX = curX;
			TempY = curY;
			//���� ����
			while (1) {
				TempX--;
				//���� ���� ���
				if (TempX < 0) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�Ʒ����� �ڳ��� ���
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
		//���� ����
		while (1) {

			//���� ���� ���
			if (curX < 0) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}

			//������ �ڳ��� ���
			if (curX > 0 && curY > 0) {
				if (_Map[curX][curY - 1] == CLOSE && _Map[curX - 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					break;
				}
			}
			//�Ʒ����� �ڳ��� ���
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

			//���� ���� ���
			if (curY < 0 || curX < 0) break;
			//���� ���
			if (_Map[curX][curY] == CLOSE)
				break;
			//�������� ���
			if (curX == _EndX && curY == _EndY) {
				InsertNode(curNode, curX, curY);
				return;
			}
			//LD���� �ڳ� Ȯ��
			if (curX > 0 && curY < _MaxY - 1) {
				if (_Map[curX][curY + 1] == CLOSE && _Map[curX - 1][curY + 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//RU���� �ڳ� Ȯ��
			if (curX < _MaxX - 1 && curY > 0) {
				if (_Map[curX + 1][curY] == CLOSE && _Map[curX + 1][curY - 1] == OPEN) {
					InsertNode(curNode, curX, curY);
					return;
				}
			}
			//���� Ž��
			int TempX = curX;
			int TempY = curY;
			while (1) {
				TempY--;
				//���� ���� ���
				if (TempY < 0) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX - 1][TempY] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�������� �ڳ��� ���
				if (TempX < _MaxX - 1 && TempY>0) {
					if (_Map[TempX + 1][TempY] == CLOSE && _Map[TempX + 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}


			}
			//���� Ž��
			TempX = curX;
			TempY = curY;
			//���� ����
			while (1) {
				TempX--;
				//���� ���� ���
				if (TempX < 0) break;
				//���� ���
				if (_Map[TempX][TempY] == CLOSE)
					break;
				//�������� ���
				if (TempX == _EndX && TempY == _EndY) {
					InsertNode(curNode, curX, curY);
					return;
				}
				//������ �ڳ��� ���
				if (TempX > 0 && TempY > 0) {
					if (_Map[TempX][TempY - 1] == CLOSE && _Map[TempX - 1][TempY - 1] == OPEN) {
						InsertNode(curNode, curX, curY);
						return;
					}
				}
				//�Ʒ����� �ڳ��� ���
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
	//X���� ����
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
	//Y���� ����
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
		//true�� �������� ���� �� �ִٴ� ��
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