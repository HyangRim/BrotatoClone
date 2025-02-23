#include "pch.h"
#include "CScene_Test.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"
#include "CSpriteUI.h"
#include "Direct2DMgr.h"

CScene_Test::CScene_Test()
	: m_fAcc(0.f)
	, m_iTest(0)
{

}

CScene_Test::~CScene_Test()
{

}

void CScene_Test::update()
{
	CScene::update();
}

void CScene_Test::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Test::render(HDC _dc)
{
	CScene::render(_dc);

	// Direct2DMgr �ν��Ͻ� ��������
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	//////////////////////////////////////////////////////
	vector<std::pair<D2D1_RECT_F, wstring>> bitmapsToRender = {
	{ D2D1::RectF(0, 0, 512, 512), L"BaseMap1" },
	{ D2D1::RectF(200, 200, 300, 300), L"BaseMap2" }
	};

	// ��� ��Ʈ�� ������
	//pD2DMgr->RenderAllBitmaps(bitmapsToRender);
	//////////////////////////////////////////////////////

	for (int idx = 0; idx < bitmapsToRender.size(); idx++) {
		pD2DMgr->RenderBitmap(bitmapsToRender[idx].first, bitmapsToRender[idx].second);
	}
}

void CScene_Test::render(Gdiplus::Graphics* _pDGraphics)
{
	// �θ� Ŭ������ render ȣ��
	CScene::render(_pDGraphics);
}

// ��� �ȼ����� Ȯ���ϴ� �Լ�
bool IsWhitePixel(Color color) {
	return color.GetR() == 255 && color.GetG() == 255 && color.GetB() == 255;
}

// �������� �ƴ� �ȼ����� Ȯ���ϴ� �Լ�
bool IsNonBlackPixel(Color color) {
	// �������� ����� �ȼ��� ���� (�Ӱ谪 ����)
	int brightnessThreshold = 50; // ��� �Ӱ谪 (0~255)
	int brightness = (color.GetR() + color.GetG() + color.GetB()) / 3;
	return brightness > brightnessThreshold;
}

// CLSID �������� �Լ�
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	UINT num = 0;          // Image encoders ����
	UINT size = 0;         // Image encoders ũ��

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;         // ����

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;         // �޸� ����

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;      // ����
		}
	}

	free(pImageCodecInfo);
	return -1;             // ����
}

// GDI+ �ʱ�ȭ
void InitializeGDIPlus(ULONG_PTR& token) {
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&token, &gdiplusStartupInput, NULL);
}

// GDI+ ����
void ShutdownGDIPlus(ULONG_PTR token) {
	GdiplusShutdown(token);
}

void CScene_Test::Enter()
{
	// Direct2DMgr �ν��Ͻ� ��������
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	pD2DMgr->LoadAndStoreBitmap(L"texture\\result1.png", L"BaseMap1");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\result2.png", L"BaseMap2");

	/*
	// GDI+ �ʱ�ȭ
	ULONG_PTR gdiplusToken;
	InitializeGDIPlus(gdiplusToken);

	Bitmap tileOutline(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles_outline.png");
	if (tileOutline.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tile_outline.png" << std::endl;
		return ;
	}
	Bitmap tiles(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles_6.png");
	if (tiles.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tiles_1.png" << std::endl;
		return ;
	}
	// Graphics ��ü ����
	Graphics graphics(&tileOutline);

	// Ÿ�� ũ�� ����
	const int tileWidth = 64;
	const int tileHeight = 64;

	// ��� �̹��� ũ�� ��������
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();

	// Ÿ�� �̹��� ũ�� ��������
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// Ÿ�� �̹����� ��ȸ�ϸ� ��� �κп� �ȼ� ä���
	for (int y = 0; y < outlineHeight; ++y) {
		for (int x = 0; x < outlineWidth; ++x) {
			Color pixelColor;
			tileOutline.GetPixel(x, y, &pixelColor);

			if (IsNonBlackPixel(pixelColor)) {
				// Ÿ�� �̹������� ���� �Ǵ� �ݺ������� �ȼ� ��������
				int srcX = x % tilesWidth;
				int srcY = y % tilesHeight;

				Color tileColor;
				tiles.GetPixel(srcX, srcY, &tileColor);

				// ��� �κп� Ÿ�� �ȼ� ����
				tileOutline.SetPixel(x, y, tileColor);
			}
		}
	}

	// ��� ����
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	tileOutline.Save(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\result6.png", &pngClsid, NULL);

	// GDI+ ����
	ShutdownGDIPlus(gdiplusToken);
	
	*/
	/*
	CSpriteUI* pSpriteUI = new CSpriteUI();
	pSpriteUI->SetTexture(L"MySprite", L"texture\\ui_lifebar_bg.png");
	pSpriteUI->SetPos(Vec2(300.f, 100.f));
	pSpriteUI->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI, GROUP_TYPE::UI);
	
	CSpriteUI* pSpriteUI2 = new CSpriteUI();
	pSpriteUI2->SetTexture(L"MySprite2", L"texture\\ui_lifebar_fill.png");
	pSpriteUI2->SetPos(Vec2(0, 0));
	pSpriteUI2->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI2, GROUP_TYPE::UI);

	CSpriteUI* pSpriteUI3 = new CSpriteUI();
	pSpriteUI3->SetTexture(L"MySprite3", L"texture\\ui_lifebar_frame.png");
	pSpriteUI3->SetPos(Vec2(0, 0));
	pSpriteUI3->SetScale(Vec2(320.f, 48.f) * 0.5f);
	AddObject(pSpriteUI3, GROUP_TYPE::UI);*/

	/*
	CSpriteUI* hpBar = new CSpriteUI();
	
	hpBar->SetTexture(
		L"LifeBarBg", L"texture\\ui_lifebar_bg.png",       // ��� �̹���
		L"LifeBarFill", L"texture\\ui_lifebar_fill.png",   // ä��texture\\�� �̹���
		L"LifeBarFrame", L"texture\\ui_lifebar_frame.png"  // �׵θ� �̹���
	);
	// 3. ��ġ�� ũ�� ����
	hpBar->SetPos(Vec2(400.f, 300.f));     // ȭ�� �߾ӿ� ��ġ
	hpBar->SetScale(Vec2(160.f, 24.f));   // ũ�� ����

	// 4. HP ���� ���� (50%�� ����)
	hpBar->SetHpRatio(0.9f);
	AddObject(hpBar, GROUP_TYPE::UI);*/
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
/*
	CTextUI* textUI = new CTextUI();
	textUI->SetPos(Vec2(0.f, 0.f));			  // ��ġ ����(�»��)
	//textUI->SetScale(Vec2(300.0f, 100.0f)); // ũ�� ���� (�ؽ�Ʈ ����) �ǹ�x ũ��� FontSize��
	textUI->SetMode(TextUIMode::NUMBER);
	textUI->SetText(L"");					  // ����� �ؽ�Ʈ ����
	textUI->SetFontSize(24);                  // ��Ʈ ũ�� ����
	textUI->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+�� ������ ����
	textUI->SetDrawOutline(true);             // �ܰ��� Ȱ��ȭ
	textUI->SetOutlineThickness(4.0f);        // �ܰ��� �β� ����
	textUI->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // �ܰ��� ����(GDI+)
	AddObject(textUI, GROUP_TYPE::UI);

	CTextUI* textUI2 = new CTextUI();
	textUI2->SetPos(Vec2(100.f, 200.0f));      // ��ġ ����(�»��)
	//textUI2->SetScale(Vec2(0, 0));		   // ũ�� ���� (�ؽ�Ʈ ����) �ǹ�x ũ��� FontSize��
	textUI2->SetMode(TextUIMode::TEXT);
	textUI2->SetText(L"Hello World!");         // ����� �ؽ�Ʈ ����
	textUI2->SetFontSize(24);                  // ��Ʈ ũ�� ����
	textUI2->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+�� ������ ����
	textUI2->SetDrawOutline(true);             // �ܰ��� Ȱ��ȭ
	textUI2->SetOutlineThickness(4.0f);        // �ܰ��� �β� ����
	textUI2->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // �ܰ��� ����(GDI+)
	AddObject(textUI2, GROUP_TYPE::UI);
	*/
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}