#pragma once

class CScene;
class CObject;

class CSceneMgr
{
	SINGLE(CSceneMgr)

private:
	CObject* m_pPlayer;		//Player

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];	//¸ðµç ¾À¿¡ ´ëÇÑ Á¤º¸¸¦ ´Ù °¡Áü
	CScene* m_pCurScene;						//ÇöÀç ¾À 

public:
	void RegisterPlayer(CObject* _player) { m_pPlayer = _player; }
	CObject* GetPlayer() { return m_pPlayer; }

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
