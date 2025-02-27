#include "pch.h"
#include "CScene_Tool.h"
#include "CkeyMgr.h"
#include "CSceneMgr.h"
#include "CTile.h"
#include "CCore.h"
#include "CResMgr.h"

#include "resource.h"
#include "CSceneMgr.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include "CUIMgr.h"
#include "CPathMgr.h"


void ChangeScene(DWORD_PTR, DWORD_PTR);

CScene_Tool::CScene_Tool()
{

}

CScene_Tool::~CScene_Tool()
{

}

void CScene_Tool::Enter()
{
	//툴 Scene에서 사용할 메뉴를 붙인다. 
	CCore::GetInstance()->DockMenu();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();


	//UI가 무언가 역할을 수행하려면 클릭할 수 있어야만 함. 
	//전반적인 UI의 이벤트 설계를 해준다면 <- 특정 상황때 호출하는 함수가 명확

	
	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(500.f, 300.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));
	
	CUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 40.f));
	pBtnUI->SetPos(Vec2(0.f, 0.f));

	//버튼 UI에 등록해둔 함수를 출력할 수 있게 해놨다. 
	//함수의 형태는 반드시 반환이 void이고, 인자가 2개인 함수만 받을 수 있음.
	//그런데 내가 넣고 싶은 함수는 전역함수가 아닌 멤버함수임...

	//멤버 함수를 쓰려면 반드시 객체가 필요함. 
	//호출 시킬 객체가 반드시 필요함. 
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);

	//((CBtnUI*)pBtnUI)->SetClickedCallBack(ChangeScene, 0, 0);
	pPanelUI->AddChild(pBtnUI);
	AddObject(pPanelUI, GROUP_TYPE::UI);
	
	/*
	CUI* pClonePanel = pPanelUI->Clone();
	pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-500.f, 0.f));

	//이렇게 해주면 기능이야 같겠지만, 다른 포인터.
	((CBtnUI*)pClonePanel->GetChildsUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);
	AddObject(pClonePanel, GROUP_TYPE::UI);

	m_pUI = pClonePanel;*/
	//Camera Look 지점.
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}


//씬 안에 존재하는 모든 것은 오브젝트. 
//1. 무언가를 입력 받을 수 있는 오브젝트를 만들면 되는 거 아닐까.
//값을 세팅하거나, 그런 게 있는데 윈도우에서 어느정도 기능을 제공하는 것이 있음. 
void CScene_Tool::Exit()
{
	CCore::GetInstance()->DivideMenu();
	//이전 씬에서 포커싱되었던 것을 UIMgr이 기억하고 있는데...
	//그래서 씬 바꿀때는 그런 거 다 해제 해야함. 

	//여기서 타일 정보 저장할 수 있어야함. 
	DeleteAll();
}


void CScene_Tool::update()
{
	//부모쪽 기능 호출. 
	CScene::update();


	SetTileIdx();

	if (KEY_TAP(KEY::CTRL)) {
		LoadTileData();
		//LoadTile(L"tile\\Test.tile");
	}
}

void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN)) {
		Vec2 vMousePos = MOUSE_POS;

		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		if (vMousePos.x < 0.f || iTileX <= iCol
			|| vMousePos.y < 0.f || iTileY <= iRow) return;


		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

		((CTile*)vecTile[iIdx])->AddImgIdx();
	}
}


void CScene_Tool::SaveTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	
	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	//Modal 방식. 
	if (GetSaveFileName(&ofn)) {
		//저장으로 누르면 true, 취소는 false.
		CPathMgr::GetInstance()->GetRelativePath(szName);
		SaveTile(szName);
	}
}


void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	//커널 오브젝트
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	assert(pFile);

	//데이터 저장. 

	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	//모든 타일들을 개별적으로 저장할 데이터를 저장하게 함. 
	for (size_t tileIdx = 0; tileIdx < vecTile.size(); tileIdx++) {
		((CTile*)vecTile[tileIdx])->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::LoadTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInstance()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInstance()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//Modal 방식. 
	if (GetOpenFileName(&ofn)) {
		//저장으로 누르면 true, 취소는 false.
		wstring strRelativePath = CPathMgr::GetInstance()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
}



//=============================
// Tile Count Window Proc
//=============================

// 정보 대화 상자의 메시지 처리기입니다.

//__stdcall은 함수 호출 규약. 
INT_PTR __stdcall TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)// ok나 X창 누르면
		{
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();

			//이게 성공하면 정상적으로 캐스팅이 완료 
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));    //Dialog 끝내기. 
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));    
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



//저장해놔야 작업하던 거 다시 로딩 가능. 


//BMP파일은 alpha값이 없는데...
// 우리가 할 일을 alpha값을 넣어주기. 여러 색이 겹치면?

