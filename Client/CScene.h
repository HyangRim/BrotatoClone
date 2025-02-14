#pragma once


#include "global.h"
//전방선언하는 이유는 컴파일 속도에 영향을 주지 않기 위해. 
class CObject;


class CScene
{

private:
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END];		//벡터 안에 모든 오브젝트 집어 넣겠다. 이런 특성(요소)를 가진만큼 나눠주기.
	//달리말하면 그룹 갯수만큼 나눠주기.

	wstring			m_strName; //Scene 이름

	UINT			m_iTileX;		//타일 가로 개수
	UINT			m_iTileY;		//타일 세로 개수. 

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC _dc);
	
	virtual void Enter() = 0;		//해당 Scene에 진입 시 호출.
	virtual void Exit() = 0;		//해당 Scene에 탈출 시 호출.



public:
	//클래스는 헤더에 구현하면 인라인 처리가 됨. 
	//따라서 함수 호출 비용이 사라짐. 
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) 
	{
		return m_arrObj[(UINT)_eType];
	}

	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	void DeleteGroup(GROUP_TYPE _eGroup);
	void DeleteAll();
	void CreateTile(UINT _IXCount, UINT _IYCount);
	void LoadTile(const wstring& _strRelativePath);

public:
	CScene();

	//소멸자의 가상함수 해줘야함. 씬 매니저가 모든 Scene을 부모 포인터로 관리함.
	// CSceneMgr에서 씬을 소멸시킬때, 소멸자는 부모인 CScene만 호출됨.  
	virtual ~CScene();
};

