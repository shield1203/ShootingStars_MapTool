#pragma once
#include<Windows.h>
#include"tinyxml.h"
#include<vector>
#include<string>

using namespace std;

struct ENEMY_INFO {
	string strEnemyType; // �� ���� ����
	int nHP, nXPos, nYPos;
	string strMovePattern; // ������ ����
	string strAttackPattern; // ���� ����
	DWORD dwTime;
};

class CFileManager {
public:
	vector<ENEMY_INFO*> m_EnemyInfo;

	void LoadFile(const char* strFileName);
	void SaveFile(const char* strFileName);
	
	void VectorClear(vector<ENEMY_INFO*>& vec_ocj);
};