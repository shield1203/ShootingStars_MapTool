#pragma once
#include<Windows.h>
#include"tinyxml.h"
#include<vector>
#include<string>

using namespace std;

struct ENEMY_INFO {
	string strEnemyType; // 적 유닛 종류
	int nHP, nXPos, nYPos;
	string strMovePattern; // 움직임 패턴
	string strAttackPattern; // 공격 패턴
	DWORD dwTime;
};

class CFileManager {
public:
	vector<ENEMY_INFO*> m_EnemyInfo;

	void LoadFile(const char* strFileName);
	void SaveFile(const char* strFileName);
	
	void VectorClear(vector<ENEMY_INFO*>& vec_ocj);
};