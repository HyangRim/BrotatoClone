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
	
	//Scene ����
	
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new CScene_Main;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetName(L"Main Title Scene");

	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");

	m_arrScene[(UINT)SCENE_TYPE::TEST] = new CScene_Test;
	m_arrScene[(UINT)SCENE_TYPE::TEST]->SetName(L"Test Scene");

	m_arrScene[(UINT)SCENE_TYPE::SELECT_CHARACTER] = new CScene_Select_Character;
	m_arrScene[(UINT)SCENE_TYPE::SELECT_CHARACTER]->SetName(L"Select Character Scene");

	m_arrScene[(UINT)SCENE_TYPE::SELECT_WEAPON] = new CScene_Select_Weapon;
	m_arrScene[(UINT)SCENE_TYPE::SELECT_WEAPON]->SetName(L"Select Weapon Scene");

	m_arrScene[(UINT)SCENE_TYPE::SHOP] = new CScene_Shop;
	m_arrScene[(UINT)SCENE_TYPE::SHOP]->SetName(L"Shop Scene");

	m_arrScene[(UINT)SCENE_TYPE::RUN_END] = new CScene_Run_End;
	m_arrScene[(UINT)SCENE_TYPE::RUN_END]->SetName(L"Run End");

	//m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Stage01;
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Stage02;

	//���̺� ����
	CWaveMgr::GetInstance()->WaveInit();

	//���� �� ����
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::SHOP];

	m_pCurScene->Enter();

	//�θ� �����Ͷ� Start()�� ȣ���� ���� ����. 
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


//���� �ٲ� ��, ���� ���� �ִ� �����ͳ� �׷� ��. 
//�� �������� �۾��� �������ϰ�, Scene�� ������Ʈ �ؾ߸� ��. 
//���⼭ ���� �� �̺�Ʈ ���. ���� �����Ӻ��� ���� ������ ������ �̺�Ʈ�� �־��ش�. 
void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();
}

//DontDestroy ������Ʈ�� �����غ���. 


