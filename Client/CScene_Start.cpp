#include "pch.h"
#include "CScene_start.h"
#include "CObject.h"
#include "CCore.h"
#include "CTexture.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCollisionMgr.h"
#include "CPathMgr.h"

#include "CkeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"

CScene_Start::CScene_Start()
{

}

CScene_Start::~CScene_Start()
{

}

void CScene_Start::update()
{
	//부모쪽 기능 호출. 
	CScene::update();

	if (KEY_TAP(KEY::ENTER)) {
		ChangeScene(SCENE_TYPE::TOOL);
	}

	if (KEY_TAP(KEY::LBTN)) {
		Vec2 vLookAt = CCamera::GetInstance()->GetRealPos(MOUSE_POS);


		CCamera::GetInstance()->SetLookAt(vLookAt);

		int a = 0;
	}
}

void CScene_Start::Enter()
{


	//Object 추가.
	//실제 생성된 객체는 플레이어, 주소를 받은 건 부모 클래스. 
	CObject* pObj = new CPlayer;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));

	AddObject(pObj, GROUP_TYPE::PLAYER);

	//CCamera::GetInstance()->SetTarget(pObj);

	//몬스터 배치. 
	int iMonCount = 8;
	float fMoveDist = 25.f;
	float fObjScale = 50.f;
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	CMonster* pMonsterObj = nullptr;

	float fTerm = (vResolution.x - ((fMoveDist + fObjScale / 2.f) * 2)) / (float)(iMonCount - 1);
	for (int idx = 0; idx < iMonCount; idx++) {
		//Monster Object 추가.
		pMonsterObj = new CMonster;
		pMonsterObj->SetName(L"Monster");
		pMonsterObj->SetPos(Vec2((fMoveDist + fObjScale / 2.f) + (float)idx * fTerm, 50.f));
		pMonsterObj->SetCenterPos(pMonsterObj->GetPos());
		pMonsterObj->SetMaxMoveDistance(fMoveDist);
		pMonsterObj->SetScale(Vec2(fObjScale, fObjScale));
		AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	}



	//타일 로딩
	//LoadTile(L"Tile\\Start.tile");

	//충돌 지점. 
	//Player 그룹과 Monster그룹간의 충돌 체크 
	//update에서 로직, finalupdate에서 최종 움직임 완료

	//이제 새로운 충돌이 발생할수도 있음. 
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::MONSTER);

	//Camera Look 지점.
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Start::Exit()
{
	//나갈때 전부 삭제해줘야함.

	DeleteAll();
	//충돌도 전부 초기화 해주기. 
	CCollisionMgr::GetInstance()->Reset();
}