#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];	//모든 씬에 대한 정보를 다 가짐
	CScene* m_pCurScene;						//현재 씬 
public:
	void init();

	void update();
	void render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }

private:

	void ChangeScene(SCENE_TYPE _eNext);
	friend class CEventMgr;
};
