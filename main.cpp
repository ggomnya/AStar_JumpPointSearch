/**
	JSP와 AStar 성능 비교

	여러가지 맵에 대해서 성능 비교
	지표 1 : 수행 시간
	지표 2 : 메모리 사용량(Node 생성 갯수)
	지표 3 : 최단 거리 
	지표 4 : 개선 여부 

**/

#include "AStar.h"
#include "JSPClass.h"
#include "hw6_my_profile.h"
#include <stdio.h>


#define MAXX	100
#define MAXY	50
enum { START, UU, RU, RR, RD, DD, LD, LL, LU };
enum { OPEN, CLOSE, PATH };


int g_Map[MAXX][MAXY];
int g_StartX = 20;
int g_StartY = 20;
int g_EndX = 28;
int g_EndY = 24;

AStar g_AStar(MAXX, MAXY);
JSP g_JSP(MAXX, MAXY);
int g_tNodeAStar = 0;
int g_tNodeJSP = 0;

FILE* fp;

int wmain() {
	//map 로드하기
	
	_wfopen_s(&fp, L"MAP3.map", L"rb");
	fread(&g_StartX, sizeof(int), 1, fp);
	fread(&g_StartY, sizeof(int), 1, fp);
	fread(&g_EndX, sizeof(int), 1, fp);
	fread(&g_EndY, sizeof(int), 1, fp);
	fread(g_Map, MAXX * MAXY * sizeof(int), 1, fp);
	fclose(fp);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 50; j++) {
			g_AStar.SetMapInfo(i, j, g_Map[i][j]);
			g_JSP.SetMapInfo(i, j, g_Map[i][j]);
		}
	}
	ProfileInit();
	for (int i = 0; i < 100; i++) {
		PRO_BEGIN(L"AStar");
		g_AStar.FindPath(g_StartX, g_StartY, g_EndX, g_EndY);
		PRO_END(L"AStar");
		g_tNodeAStar += g_AStar.GetTotalNode();
	}

	for (int i = 0; i < 100; i++) {
		PRO_BEGIN(L"JSP");
		g_JSP.FindPath(g_StartX, g_StartY, g_EndX, g_EndY);
		PRO_END(L"JSP");
		g_tNodeJSP += g_JSP.GetTotalNode();
	}
	ProfileDataOutText();
	wprintf(L"AvgNode      AStar : %d   JSP : %d\n", g_tNodeAStar / 100, g_tNodeJSP / 100);
	return 0;
}