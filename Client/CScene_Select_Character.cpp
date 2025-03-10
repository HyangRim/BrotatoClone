#include "pch.h"

#include "CCore.h"
#include "CSpriteUI.h"
#include "CCamera.h"
#include "CBtnUI.h"
#include "CPanelUI.h"
#include "CImage.h"
#include "CTextUI.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "Direct2DMgr.h"
#include "CharacterInfoMgr.h"
#include "ItemMgr.h"

#include "CScene_Select_Character.h"
#include "CScene_Select_Weapon.h"

CScene_Select_Character::CScene_Select_Character()
{

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
	if (ItemMgr::GetInstance()->m_basicCharacter != nullptr)
	{
		ItemMgr::GetInstance()->SetBasicCharacter(nullptr);
		ItemMgr::GetInstance()->m_vPassiveItems.pop_back();
		ItemMgr::GetInstance()->m_bBaseCharacterIsAdded = false;
	}


	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	//Object �߰�.
	//���� ������ ��ü�� �÷��̾�, �ּҸ� ���� �� �θ� Ŭ����. 
	CreatePlayerObject();

	/////////////////�� ��� ������Ʈ/////////////////
	CreateBackGround(pD2DMgr, vResolution);
	/////////////////�� ��� ������Ʈ/////////////////

	/////////////////�ڷ� ���� ��ư///////////////////
	CreateBackBtn();
	/////////////////�ڷ� ���� ��ư///////////////////

	/////////////////ĳ���� ���� ���� UI(���)///////////////////
	CreateItemSelectText(vResolution);
	/////////////////ĳ���� ���� ���� UI(���)///////////////////

	//////////////////////////ĳ���͵� UI////////////////////////
	CreateCharacterItemBtn();
	//////////////////////////ĳ���͵� UI////////////////////////
}



void CScene_Select_Character::Exit()
{
	DeleteAll();
}

void CScene_Select_Character::CreatePlayerObject()
{
	//Object �߰�.
	//���� ������ ��ü�� �÷��̾�, �ּҸ� ���� �� �θ� Ŭ����. 
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(65.f, 65.f));
	pObj->SetName(L"Player");
	//AddObject(pObj, GROUP_TYPE::PLAYER);
	RegisterPlayer(pObj);
	CObject* tmp = GetPlayer();
	CSceneMgr::GetInstance()->RegisterPlayer(tmp);
}

void CScene_Select_Character::CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution)
{
	/////////////////�� ��� ������Ʈ/////////////////
	CObject* backGround = new CSpriteUI;
	backGround->SetObjType(GROUP_TYPE::DEFAULT);
	backGround->AddImage(_pD2DMgr->GetStoredBitmap(L"shop_background"));
	backGround->SetPos(_vResolution / 2.f);
	backGround->SetScale(_vResolution);
	AddObject(backGround, GROUP_TYPE::DEFAULT);
	/////////////////�� ��� ������Ʈ/////////////////

}

void CScene_Select_Character::CreateBackBtn()
{
	/////////////////�ڷ� ���� ��ƴ///////////////////
	CBtnUI* backBtn = new CBtnUI;
	backBtn->SetName(L"BackBtn");
	backBtn->SetObjType(GROUP_TYPE::UI);
	backBtn->SetScale(Vec2(94.f, 28.f));
	backBtn->SetPos(Vec2(71.f, 38.f));
	backBtn->SetIsRound(true, 10.f, 10.f);
	backBtn->SetColor(ColorNormalize(237, 237, 237), ColorNormalize(28, 28, 28));
	backBtn->SetClickedCallBack(ChangeScene, (DWORD_PTR)SCENE_TYPE::MAIN, 0);
	backBtn->CreateTextUI(L"�ڷ�", Vec2(-47.f, -14.f), Vec2(47.f, 14.f)
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(backBtn, GROUP_TYPE::UI);
	/////////////////�ڷ� ���� ��ƴ///////////////////
}

void CScene_Select_Character::CreateItemSelectText(Vec2 _vResolution)
{
	/////////////////ĳ���� ���� ���� UI(���)///////////////////
	CObject* panelTextSelectCharacter = new CSpriteUI;
	panelTextSelectCharacter->SetObjType(GROUP_TYPE::UI);
	panelTextSelectCharacter->SetPos(Vec2(_vResolution.x / 2.f, 50.f));
	panelTextSelectCharacter->SetScale(Vec2(300.f, 36.f));
	panelTextSelectCharacter->CreateTextUI(L"ĳ���� ����", Vec2(-150.f, -18.f), Vec2(150.f, 18.f)
		, 30, D2D1::ColorF::White, true, 2.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	AddObject(panelTextSelectCharacter, GROUP_TYPE::UI);
	/////////////////ĳ���� ���� ���� UI(���)///////////////////
}

void CScene_Select_Character::CreateCharacterItemBtn()
{
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
				, reinterpret_cast<DWORD_PTR>(pTag)
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

void ShowCharacterInfo(DWORD_PTR lParam, DWORD_PTR wParam)
{
	//lParam -> �±�
	//wParam -> ��ư���� ��ü������ on�� �����Ǹ� panel�� image�� delete ��Ű�� ���� �����ص� ���� ������
	printf("tesT\n");
	wstring tag = *reinterpret_cast<wstring*>(lParam); //well_rounded , icon ����
	vector<CObject*>* tempObjects = reinterpret_cast<vector<CObject*>*>(wParam);
	Item* characterInfo = ItemMgr::GetInstance()->GetItem(tag);
	wchar_t buffer[20];

	if (characterInfo == nullptr || tempObjects == nullptr) return;

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	////////////////////////////�� �г�/////////////////////////////
	CPanelUI* panel = new CPanelUI;	
	panel->SetScale(Vec2(186.f, 250.f));
	panel->SetPos(Vec2(vResolution.x / 2.f, 210.f));
	panel->SetName(L"Parent");
	panel->SetRadius(10.f, 10.f);
	panel->SetColor(ColorNormalize(0, 0, 0), ColorNormalize(0, 0, 0));
	////////////////////////////�� �г�/////////////////////////////
	
	/////////////////////////ĳ���� �̹���//////////////////////////
	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	CSpriteUI* characterImage = panel->AddChild<CSpriteUI>(Vec2(vPos));
	wstring iconTag = tag + L"_icon";
	characterImage->AddImage(pD2DMgr->GetStoredBitmap(iconTag));
	characterImage->SetBackGround(true);
	characterImage->SetIsRound(true, 10.f, 10.f);
	characterImage->SetBackGroundColor(ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0)
		, ColorNormalize(30, 30, 30), ColorNormalize(0, 0, 0));
	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetName(L"Child");
	characterImage->SetScale(Vec2(48.f, 48.f));
	/////////////////////////ĳ���� �̹���//////////////////////////
	 
	/////////////////////////ĳ���� �̸�//////////////////////////
	CSpriteUI* characterName = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f));
	characterName->SetScale(Vec2(116.f,20.f));
	swprintf_s(buffer, L"%s", characterInfo->m_sName.c_str());
	characterName->CreateTextUI(buffer, -(characterName->GetScale() / 2.f), characterName->GetScale() / 2.f
		, 12, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	characterName->GetTextUI()->SetHorizontal(1);
	/////////////////////////ĳ���� �̸�//////////////////////////
	
	/////////////////////////ĳ���� Ÿ��(ĳ����or����)//////////////////////////
	//y�� offset���� 11->panel���� ���� , 10->characterName ����ũ��, 5->���� ��ü�� ����, 10->�� ��ü ũ�� ����
	CSpriteUI* itemType = panel->AddChild<CSpriteUI>(Vec2(32.f, -(panel->GetScale().y / 2.f) + 11.f + 10.f + 5.f + 10.f));
	itemType->SetScale(Vec2(116.f, 20.f));
	swprintf_s(buffer, L"ĳ����");
	itemType->CreateTextUI(buffer, -(itemType->GetScale() / 2.f), itemType->GetScale() / 2.f
		, 12, ColorNormalize(202,195,152), true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR
		, TextUIMode::TEXT
		, 0);
	itemType->GetTextUI()->SetHorizontal(1);
	/////////////////////////ĳ���� Ÿ��(ĳ����or����)//////////////////////////

	/////////////////////////ĳ���� ����(����)/////////////////////////////////
	// 
	 /*
	struct {
		float   m_fDefaultSpeed;    // �⺻ ���ǵ�
		int     m_iAddMaxHP;         // + �ִ� ü��
		float   m_fDamageCoef;      // ���� ������ %
		float   m_fMeleeCoef;       // �ٰŸ� ���� ������ %
		float   m_fRangeCoef;       // ���Ÿ� ���� ������ %
		float   m_fAttackSpeedCoef; // ���� �ӵ� %
		int     m_iCriticalAcc;     // ũ��Ƽ�� Ȯ�� %
		float   m_fSpeed;           // �ӵ� ���
	} PassiveStats;                 // �нú� ������ ���� ���� (PASSIVE Ÿ���� �� ���)
	*/
	//�� ���� ����
	int tmp = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 && characterInfo->PassiveStats.m_fDefaultSpeed == 0.f) continue;
		else if (i == 1 && characterInfo->PassiveStats.m_iAddMaxHP == 0) continue;
		else if (i == 2 && characterInfo->PassiveStats.m_fDamageCoef == 0.f) continue;
		else if (i == 3 && characterInfo->PassiveStats.m_fMeleeCoef == 0.f) continue;
		else if (i == 4 && characterInfo->PassiveStats.m_fRangeCoef == 0.f) continue;
		else if (i == 5 && characterInfo->PassiveStats.m_fAttackSpeedCoef == 0.f) continue;
		else if (i == 6 && characterInfo->PassiveStats.m_iCriticalAcc == 0) continue;
		else if (i == 7 && characterInfo->PassiveStats.m_fSpeed == 0.f) continue;

		CPanelUI* statpanel = panel->AddChild<CPanelUI>(Vec2(0.f, -50.f + tmp * 21.f));
		statpanel->SetScale(Vec2(164.f, 16.f));
		statpanel->SetRadius(0.f, 0.f);
		//statpanel->SetColor(ColorNormalize(255, 255, 255), ColorNormalize(255, 255, 255));
		statpanel->SetMouseOnAlpha(0.f);
		statpanel->SetNormalAlpha(0.f);


		CSpriteUI* statNumber = statpanel->AddChild<CSpriteUI>(Vec2(-statpanel->GetScale().x / 2.f + 8.f, 0.f));
		statNumber->SetScale(Vec2(26.f, 16.f));
		if (i == 0)	  	 { swprintf_s(buffer, L"+%d", (int)characterInfo->PassiveStats.m_fDefaultSpeed); }
		else if (i == 1) { swprintf_s(buffer, L"+%d", characterInfo->PassiveStats.m_iAddMaxHP); }
		else if (i == 2) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fDamageCoef); }
		else if (i == 3) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fMeleeCoef); }
		else if (i == 4) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fRangeCoef); }
		else if (i == 5) { swprintf_s(buffer, L"+%d%%", (int)characterInfo->PassiveStats.m_fAttackSpeedCoef); }
		else if (i == 6) { swprintf_s(buffer, L"+%d", characterInfo->PassiveStats.m_iCriticalAcc); }
		else if (i == 7) { swprintf_s(buffer, L"+%d", (int)characterInfo->PassiveStats.m_fSpeed); }
		statNumber->CreateTextUI(buffer, -(statNumber->GetScale() / 2.f), statNumber->GetScale() / 2.f
			, 10, D2D1::ColorF::Green, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		statNumber->GetTextUI()->SetHorizontal(1);

		CSpriteUI* statText = statpanel->AddChild<CSpriteUI>(Vec2(0.f, 0.f));
		statText->SetScale(Vec2(120.f, 16.f));
		if (i == 0)		 { swprintf_s(buffer, L"�⺻�ӵ�"); }
		else if (i == 1) { swprintf_s(buffer, L"�ִ�ü��"); }
		else if (i == 2) { swprintf_s(buffer, L"����������"); }
		else if (i == 3) { swprintf_s(buffer, L"�ٰŸ� ����������"); }
		else if (i == 4) { swprintf_s(buffer, L"���Ÿ� ����������"); }
		else if (i == 5) { swprintf_s(buffer, L"���ݼӵ�"); }
		else if (i == 6) { swprintf_s(buffer, L"ġ��Ÿ Ȯ��"); }
		else if (i == 7) { swprintf_s(buffer, L"�ӵ����"); }
		statText->CreateTextUI(buffer, -(statText->GetScale() / 2.f), statText->GetScale() / 2.f
			, 10, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
			, FONT_TYPE::KR
			, TextUIMode::TEXT
			, 0);
		statText->GetTextUI()->SetHorizontal(1);


		tmp++;
	}
	/////////////////////////ĳ���� ����(����)/////////////////////////////////

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

	Item* basicCharater = ItemMgr::GetInstance()->GetItem(tag);

	CPlayer* player = static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer());
	playerParameter& tmp = player->GetCharacterParam();

	/*
	float			m_fDefaultSpeed;	// �⺻ ���ǵ�.
	int				m_AddMaxHP;			// + �ִ� ü��
	float			m_fDamageCoef;		//������ %
	float			m_fMeleeCoef;		//�ٰŸ� ������ %
	float			m_fRangeCoef;		//���Ÿ� ������ %
	float			m_fAttackSpeedCoef; // ���ݼӵ� %
	float			m_fCriticalAcc;		// ũȮ %
	float			m_fSpeed;			// �ӵ� ���.
	*/

	printf("�÷��̾�� ��\n");
	printf("%d\n", (int)tmp.m_fDefaultSpeed);
	printf("%d\n", tmp.m_AddMaxHP);
	printf("%d\n", (int)tmp.m_fDamageCoef);
	printf("%d\n", (int)tmp.m_fMeleeCoef);
	printf("%d\n", (int)tmp.m_fRangeCoef);
	printf("%d\n", (int)tmp.m_fAttackSpeedCoef);
	printf("%d\n", tmp.m_iCriticalAcc);
	printf("%d\n", (int)tmp.m_fSpeed);

	printf("===========================\n");
	printf("������ ���� \n");
	printf("%d\n", (int)basicCharater->PassiveStats.m_fDefaultSpeed);
	printf("%d\n", basicCharater->PassiveStats.m_iAddMaxHP);
	printf("%d\n", (int)basicCharater->PassiveStats.m_fDamageCoef);
	printf("%d\n", (int)basicCharater->PassiveStats.m_fMeleeCoef);
	printf("%d\n", (int)basicCharater->PassiveStats.m_fRangeCoef);
	printf("%d\n", (int)basicCharater->PassiveStats.m_fAttackSpeedCoef);
	printf("%d\n", basicCharater->PassiveStats.m_iCriticalAcc);
	printf("%d\n", (int)basicCharater->PassiveStats.m_fSpeed);

	tmp.m_fDefaultSpeed += basicCharater->PassiveStats.m_fDefaultSpeed;
	tmp.m_AddMaxHP += basicCharater->PassiveStats.m_iAddMaxHP;
	tmp.m_fDamageCoef += basicCharater->PassiveStats.m_fDamageCoef;
	tmp.m_fMeleeCoef += basicCharater->PassiveStats.m_fMeleeCoef;
	tmp.m_fRangeCoef += basicCharater->PassiveStats.m_fRangeCoef;
	tmp.m_fAttackSpeedCoef += basicCharater->PassiveStats.m_fAttackSpeedCoef;
	tmp.m_iCriticalAcc += basicCharater->PassiveStats.m_iCriticalAcc;
	tmp.m_fSpeed += basicCharater->PassiveStats.m_fSpeed;

	printf("�÷��̾�� ��\n");
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fDefaultSpeed);
	printf("%d\n", static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_AddMaxHP);
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fDamageCoef);
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fMeleeCoef);
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fRangeCoef);
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fAttackSpeedCoef);
	printf("%d\n", static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_iCriticalAcc);
	printf("%d\n", (int)static_cast<CPlayer*>(CSceneMgr::GetInstance()->GetPlayer())->GetCharacterParam().m_fSpeed);

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
