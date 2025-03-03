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
	void render(Gdiplus::Graphics* _pDGraphics);
	void render(ID2D1HwndRenderTarget* _pRender);

	bool IsWaveScene() {
		if (m_pCurScene == m_arrScene[(UINT)SCENE_TYPE::START]) return true;
		return false;
	}

public:
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetScene(SCENE_TYPE _eType) { return m_arrScene[(UINT)_eType]; }

private:

	void ChangeScene(SCENE_TYPE _eNext);
	friend class CEventMgr;
	friend class CScene_Select_Character;
};
