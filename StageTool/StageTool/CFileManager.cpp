#include"CFileManager.h"

void CFileManager::LoadFile(const char* strFileName) {
	if (m_EnemyInfo.size() != 0) {
		VectorClear(m_EnemyInfo);
	}

	TiXmlDocument doc;
	doc.LoadFile(strFileName);

	//루트노드 접근
	TiXmlElement* pRoot = doc.FirstChildElement("Stage");
	if (!pRoot) return;

	TiXmlElement* pElem = pRoot->FirstChildElement("Enemy");
	TiXmlAttribute* pAttrib = nullptr;

	while (pElem) {
		pAttrib = pElem->FirstAttribute();

		ENEMY_INFO* addEnemy = new ENEMY_INFO();

		addEnemy->dwTime = atoi(pAttrib->Value()); // 시간
		pAttrib = pAttrib->Next();

		addEnemy->strAttackPattern = pAttrib->Value(); // 공격 패턴
		pAttrib = pAttrib->Next();

		addEnemy->strMovePattern = pAttrib->Value(); // 이동 패턴
		pAttrib = pAttrib->Next();

		addEnemy->nYPos = atoi(pAttrib->Value()); // YPos
		pAttrib = pAttrib->Next();

		addEnemy->nXPos = atoi(pAttrib->Value()); // XPos
		pAttrib = pAttrib->Next();

		addEnemy->nHP = atoi(pAttrib->Value()); // HP
		pAttrib = pAttrib->Next();

		addEnemy->strEnemyType = pAttrib->Value(); // 적 유닛 종류

		m_EnemyInfo.push_back(addEnemy);

		pElem = pElem->NextSiblingElement();
	}
}

void CFileManager::SaveFile(const char* strFileName) {
	TiXmlDocument SAVE;
	TiXmlDeclaration* DB = new TiXmlDeclaration("1.0", "", "");
	SAVE.LinkEndChild(DB);

	TiXmlElement* pSAVE = new TiXmlElement("Stage");
	SAVE.LinkEndChild(pSAVE);

	for (int i = 0; i < m_EnemyInfo.size(); i++) {
		TiXmlElement* pElem = new TiXmlElement("Enemy");
		pSAVE->LinkEndChild(pElem);

		pElem->SetAttribute("time", m_EnemyInfo[i]->dwTime); // 블록정보 저장
		pElem->SetAttribute("attackPattern", m_EnemyInfo[i]->strAttackPattern.c_str());
		pElem->SetAttribute("movePattern", m_EnemyInfo[i]->strMovePattern.c_str());
		pElem->SetAttribute("yPos", m_EnemyInfo[i]->nYPos);
		pElem->SetAttribute("xPos", m_EnemyInfo[i]->nXPos);
		pElem->SetAttribute("HP", m_EnemyInfo[i]->nHP);
		pElem->SetAttribute("enemyType", m_EnemyInfo[i]->strEnemyType.c_str());
	}

	SAVE.SaveFile(strFileName);
}

void CFileManager::VectorClear(vector<ENEMY_INFO*>& vec_ocj) {

	vec_ocj.clear();
}