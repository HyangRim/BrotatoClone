#include "pch.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CScene_Main.h"
#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CScene_Test.h"
#include "CScene_Select_Character.h"
#include "CScene_Select_Weapon.h"
#include "CScene_Run_End.h"
#include "CScene_Shop.h"


#include "CEventMgr.h"
#include "CWaveMgr.h"

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
	, m_arrScene{}
{

}

CSceneMgr::~CSceneMgr() {
	for (UINT sceneIDX = 0; sceneIDX < (UINT)SCENE_TYPE::END; sceneIDX++) {

		if (m_arrScene[sceneIDX] != nullptr) {
			delete m_arrScene[sceneIDX];
		}
	}
}

void CSceneMgr::init()
{

	//Pause Init
	
	//Scene 생성
	
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new CScene_Main;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetName(L"Main Title Scene");
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetSceneType(SCENE_TYPE::MAIN);

	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	m_arrScene[(UINT)SCENE_TYPE::START]->SetSceneType(SCENE_TYPE::START);

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetSceneType(SCENE_TYPE::TOOL);

	m_arrScene[(UINT)SCENE_TYPE::TEST] = new CScene_Test;
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetName(L"Test Scene");
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetSceneType(SCENE_TYPE::TEST);

	m_arrScene[(UINT)SCENE_TYPE::SELECT_CHARACTER] = new CScene_Select_Character;
	m_arrScene[(UINT)SCENE_TYPE::SELECT_CHARACTER]->SetName(L"Select Character Scene");
	m_arrScene[(UINT)SCENE_TYPE::SELECT_CHARACTER]->SetSceneType(SCENE_TYPE::SELECT_CHARACTER);

	m_arrScene[(UINT)SCENE_TYPE::SELECT_WEAPON] = new CScene_Select_Weapon;
	m_arrScene[(UINT)SCENE_TYPE::SELECT_WEAPON]->SetName(L"Select Weapon Scene");
	m_arrScene[(UINT)SCENE_TYPE::SELECT_WEAPON]->SetSceneType(SCENE_TYPE::SELECT_WEAPON);

	m_arrScene[(UINT)SCENE_TYPE::SHOP] = new CScene_Shop;
	m_arrScene[(UINT)SCENE_TYPE::SHOP]->SetName(L"Shop Scene");
	m_arrScene[(UINT)SCENE_TYPE::SHOP]->SetSceneType(SCENE_TYPE::SHOP);

	m_arrScene[(UINT)SCENE_TYPE::RUN_END] = new CScene_Run_End;
	m_arrScene[(UINT)SCENE_TYPE::RUN_END]->SetName(L"Run End");
	m_arrScene[(UINT)SCENE_TYPE::RUN_END]->SetSceneType(SCENE_TYPE::RUN_END);

	//웨이브 세팅
	CWaveMgr::GetInstance()->WaveInit();

	//현재 씬 설정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::MAIN];

	m_pCurScene->Enter();

	//부모 포인터라 Start()를 호출할 수가 없음. 
}

void CSceneMgr::update()
{
	m_pCurScene->update();

	m_pCurScene->finalupdate();

	CWaveMgr::GetInstance()->update();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::render(Gdiplus::Graphics* _pDGraphics)
{
	m_pCurScene->render(_pDGraphics);
}

void CSceneMgr::render(ID2D1HwndRenderTarget* _pRender)
{
	m_pCurScene->render(_pRender);
}


//씬을 바꿀 때, 기존 씬에 있던 포인터나 그런 거. 
//한 프레임은 작업을 마무리하고, Scene을 업데이트 해야만 함. 
//여기서 해줄 건 이벤트 등록. 다음 프레임부터 다음 씬으로 가도록 이벤트를 넣어준다. 
void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();
}

//DontDestroy 오브젝트를 구현해보기. 


