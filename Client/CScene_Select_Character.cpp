#include "pch.h"

#include "CCore.h"
#include "CSpriteUI.h"
#include "CCamera.h"
#include "CBtnUI.h"
#include "CPanelUI.h"
#include "CImage.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "Direct2DMgr.h"
#include "CharacterInfoMgr.h"
#include "ItemMgr.h"

#include "CScene_Select_Character.h"
#include "CScene_Select_Weapon.h"

CScene_Select_Character::CScene_Select_Character()
{
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\ui\\menus\\shop\\shop_background.png", L"BackGround", false);
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\global\\locked_icon.png", L"LockedIcon", false);
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\global\\random_icon.png", L"RandomIcon", false);
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"items\\characters\\well_rounded\\well_rounded_icon.png", L"WellRounded", false);

}

CScene_Select_Character::~CScene_Select_Character()
{

}


void CScene_Select_Character::update()
{
	CScene::update();
}

void CScene_Select_Character::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}


void CScene_Select_Character::Enter()
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	//Object �߰�.
	//���� ������ ��ü�� �÷��̾�, �ּҸ� ���� �� �θ� Ŭ����. 
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(65.f, 65.f));
	pObj->SetName(L"Player");
	//AddObject(pObj, GROUP_TYPE::PLAYER);
	RegisterPlayer(pObj);

	/////////////////�� ��� ������Ʈ/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(vResolution/2.f);
	backGround->SetScale(vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////�� ��� ������Ʈ/////////////////

	/////////////////�ڷ� ���� ��ƴ///////////////////
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetObjType(GROUP_TYPE::UI);
	backBtn->SetScale(Vec2(94.f, 28.f)); 
	backBtn->SetPos(Vec2(71.f, 38.f));
	backBtn->SetIsRound(true, 10.f, 10.f);
	backBtn->SetColor(ColorNormalize(237,237,237), ColorNormalize(28,28,28));
	backBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	backBtn->CreateTextUI(L"�ڷ�", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	/////////////////�ڷ� ���� ��ƴ///////////////////

	/////////////////ĳ���� ���� ���� UI(���)///////////////////
	CObject* panelTextSelectCharacter = new CSpriteUI;
	panelTextSelectCharacter->SetObjType(GROUP_TYPE::UI);
	panelTextSelectCharacter->SetPos(Vec2(vResolution.x / 2.f, 50.f));
	panelTextSelectCharacter->SetScale(Vec2(300.f, 36.f));
	panelTextSelectCharacter->CreateTextUI(L"ĳ���� ����", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 30, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextSelectCharacter, GROUP_TYPE::UI);
	/////////////////ĳ���� ���� ���� UI(���)///////////////////



	//////////////////////////ĳ���͵� UI////////////////////////
	float leftMargin = 32.f;
	float LR_interval = 5.f;
	float UD_interval = 4.f;

	Vec2 startPos(56.f, 387.f);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 17; j++)
		{		
			CBtnUI* selectCharacterUI = new CBtnUI;
			selectCharacterUI->SetObjType(GROUP_TYPE::UI);
			selectCharacterUI->SetScale(Vec2(48.f, 48.f));
			selectCharacterUI->SetName(L"TEST");
			selectCharacterUI->SetUIType(UI_TYPE::BTN);

			wstring tag;
			
			CharacterMapping(selectCharacterUI, i, j, tag);
			wstring* pTag = new wstring(tag);
			
			CImage* image = selectCharacterUI->GetImage(0);
			selectCharacterUI->SetOnCallBack(ShowCharacterInfo
				, reinterpret_cast<DWORD_PTR>(image)
				, reinterpret_cast<DWORD_PTR>(&selectCharacterUI->GetTrashCan()));
			if (tag.compare(L"") != 0)
			{
				selectCharacterUI->SetClickedCallBack(SelectCharacter
					, reinterpret_cast<DWORD_PTR>(pTag)
					, reinterpret_cast<DWORD_PTR>(nullptr));
			}

			selectCharacterUI->SetPos(startPos + Vec2((selectCharacterUI->GetScale().x + LR_interval) * j
				, (selectCharacterUI->GetScale().y + UD_interval) * i));
			selectCharacterUI->SetIsRound(true, 10.f, 10.f);
			selectCharacterUI->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(31, 31, 31));

			AddObject(selectCharacterUI, GROUP_TYPE::UI);
		}
	}
	//////////////////////////ĳ���͵� UI////////////////////////
}



void CScene_Select_Character::Exit()
{
	//������ CSceneMgr�ʿ� Player��� ��û
	CObject* tmp = GetPlayer();
	CSceneMgr::GetInstance()->RegisterPlayer(tmp);

	DeleteAll();
}



void ShowCharacterInfo(DWORD_PTR lParam, DWORD_PTR wParam)
{
	//lParam -> �̹���
	//wParam -> ��ư���� ��ü������ on�� �����Ǹ� panel�� image�� delete ��Ű�� ���� �����ص� ���� ������
	printf("tesT\n");
	CImage* image = reinterpret_cast<CImage*>(lParam);
	vector<CObject*>* tempObjects = reinterpret_cast<vector<CObject*>*>(wParam);

	if (image == nullptr || tempObjects == nullptr) return;

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	CPanelUI* panel = new CPanelUI;	
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetPos(Vec2(vResolution.x / 2.f, 210.f));
	panel->SetName(L"Parent");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));

	CObject* characterImage = new CSpriteUI;
	characterImage->AddImage(image->GetBitmap());
	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	characterImage->GetImage(0)->SetOffset(vPos);
	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetName(L"Child");
	characterImage->SetScale(Vec2(48.f, 48.f));
	characterImage->SetPos(panel->GetPos());

	panel->AddChild((CUI*)characterImage);

	CSceneMgr::GetInstance()->GetCurScene()->AddObject(panel, GROUP_TYPE::UI);

	tempObjects->push_back(panel);
}

void SelectCharacter(DWORD_PTR lParam, DWORD_PTR wParam)
{
	// lParam -> �̹��� �±�
	// wParam -> x
	wstring tag = *reinterpret_cast<wstring*>(lParam);

	if (tag.compare(L"random") == 0)
	{
		int random_character_index = rand() % character_tag_list.size();
		tag = character_tag_list[random_character_index];
	}

	Item* basicCharater = new Item(ITEM_TYPE::PASSIVE);
	basicCharater->tag = tag;
	basicCharater->m_eItemType = ITEM_TYPE::PASSIVE;

	ItemMgr::GetInstance()->SetBasicCharacter(basicCharater);

	ChangeScene(SCENE_TYPE::SELECT_WEAPON);
}























void CScene_Select_Character::CharacterMapping(CBtnUI* _btn, int i, int j, wstring& tag)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	if (i == 0 && j == 0)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"random_icon"));
		tag = L"random";
	}
	else if (i == 0 && j == 1)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"well_rounded_icon"));
		tag = L"well_rounded";
	}
	else if (i == 0 && j == 2)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"brawler_icon"));
		tag = L"brawler";
	}
	else if (i == 0 && j == 3)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"crazy_icon"));
		tag = L"crazy";
	}
	else if (i == 0 && j == 4)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"ranger_icon"));
		tag = L"ranger";
	}
	else if (i == 0 && j == 5)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"mage_icon"));
		tag = L"mage";
	}
	else if (i == 0 && j == 6)
	{
		_btn->AddImage(pD2DMgr->GetStoredBitmap(L"chunky_icon"));
		tag = L"chunky";
	}
	else _btn->AddImage(pD2DMgr->GetStoredBitmap(L"locked_icon"));
}
