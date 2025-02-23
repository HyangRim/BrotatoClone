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

	// Direct2DMgr 인스턴스 가져오기
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	//////////////////////////////////////////////////////
	vector<std::pair<D2D1_RECT_F, wstring>> bitmapsToRender = {
	{ D2D1::RectF(0, 0, 512, 512), L"BaseMap1" },
	{ D2D1::RectF(200, 200, 300, 300), L"BaseMap2" }
	};

	// 모든 비트맵 렌더링
	//pD2DMgr->RenderAllBitmaps(bitmapsToRender);
	//////////////////////////////////////////////////////

	for (int idx = 0; idx < bitmapsToRender.size(); idx++) {
		pD2DMgr->RenderBitmap(bitmapsToRender[idx].first, bitmapsToRender[idx].second);
	}
}

void CScene_Test::render(Gdiplus::Graphics* _pDGraphics)
{
	// 부모 클래스의 render 호출
	CScene::render(_pDGraphics);
}

// 흰색 픽셀인지 확인하는 함수
bool IsWhitePixel(Color color) {
	return color.GetR() == 255 && color.GetG() == 255 && color.GetB() == 255;
}

// 검정색이 아닌 픽셀인지 확인하는 함수
bool IsNonBlackPixel(Color color) {
	// 검정색에 가까운 픽셀을 제외 (임계값 설정)
	int brightnessThreshold = 50; // 밝기 임계값 (0~255)
	int brightness = (color.GetR() + color.GetG() + color.GetB()) / 3;
	return brightness > brightnessThreshold;
}

// CLSID 가져오기 함수
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	UINT num = 0;          // Image encoders 수량
	UINT size = 0;         // Image encoders 크기

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;         // 실패

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;         // 메모리 부족

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;      // 성공
		}
	}

	free(pImageCodecInfo);
	return -1;             // 실패
}

// GDI+ 초기화
void InitializeGDIPlus(ULONG_PTR& token) {
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&token, &gdiplusStartupInput, NULL);
}

// GDI+ 종료
void ShutdownGDIPlus(ULONG_PTR token) {
	GdiplusShutdown(token);
}

void CScene_Test::Enter()
{
	// Direct2DMgr 인스턴스 가져오기
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	pD2DMgr->LoadAndStoreBitmap(L"texture\\result1.png", L"BaseMap1");
	pD2DMgr->LoadAndStoreBitmap(L"texture\\result2.png", L"BaseMap2");

	/*
	// GDI+ 초기화
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
	// Graphics 객체 생성
	Graphics graphics(&tileOutline);

	// 타일 크기 정의
	const int tileWidth = 64;
	const int tileHeight = 64;

	// 배경 이미지 크기 가져오기
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();

	// 타일 이미지 크기 가져오기
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// 타일 이미지를 순회하며 흰색 부분에 픽셀 채우기
	for (int y = 0; y < outlineHeight; ++y) {
		for (int x = 0; x < outlineWidth; ++x) {
			Color pixelColor;
			tileOutline.GetPixel(x, y, &pixelColor);

			if (IsNonBlackPixel(pixelColor)) {
				// 타일 이미지에서 랜덤 또는 반복적으로 픽셀 가져오기
				int srcX = x % tilesWidth;
				int srcY = y % tilesHeight;

				Color tileColor;
				tiles.GetPixel(srcX, srcY, &tileColor);

				// 흰색 부분에 타일 픽셀 설정
				tileOutline.SetPixel(x, y, tileColor);
			}
		}
	}

	// 결과 저장
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	tileOutline.Save(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\result6.png", &pngClsid, NULL);

	// GDI+ 종료
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
		L"LifeBarBg", L"texture\\ui_lifebar_bg.png",       // 배경 이미지
		L"LifeBarFill", L"texture\\ui_lifebar_fill.png",   // 채우texture\\기 이미지
		L"LifeBarFrame", L"texture\\ui_lifebar_frame.png"  // 테두리 이미지
	);
	// 3. 위치와 크기 설정
	hpBar->SetPos(Vec2(400.f, 300.f));     // 화면 중앙에 위치
	hpBar->SetScale(Vec2(160.f, 24.f));   // 크기 설정

	// 4. HP 비율 설정 (50%로 설정)
	hpBar->SetHpRatio(0.9f);
	AddObject(hpBar, GROUP_TYPE::UI);*/
	
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
/*
	CTextUI* textUI = new CTextUI();
	textUI->SetPos(Vec2(0.f, 0.f));			  // 위치 설정(좌상단)
	//textUI->SetScale(Vec2(300.0f, 100.0f)); // 크기 설정 (텍스트 영역) 의미x 크기는 FontSize로
	textUI->SetMode(TextUIMode::NUMBER);
	textUI->SetText(L"");					  // 출력할 텍스트 설정
	textUI->SetFontSize(24);                  // 폰트 크기 설정
	textUI->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+용 빨간색 설정
	textUI->SetDrawOutline(true);             // 외곽선 활성화
	textUI->SetOutlineThickness(4.0f);        // 외곽선 두께 설정
	textUI->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // 외곽선 색상(GDI+)
	AddObject(textUI, GROUP_TYPE::UI);

	CTextUI* textUI2 = new CTextUI();
	textUI2->SetPos(Vec2(100.f, 200.0f));      // 위치 설정(좌상단)
	//textUI2->SetScale(Vec2(0, 0));		   // 크기 설정 (텍스트 영역) 의미x 크기는 FontSize로
	textUI2->SetMode(TextUIMode::TEXT);
	textUI2->SetText(L"Hello World!");         // 출력할 텍스트 설정
	textUI2->SetFontSize(24);                  // 폰트 크기 설정
	textUI2->SetGdiPlusTextColor(Gdiplus::Color(255, 255, 0, 255)); // GDI+용 빨간색 설정
	textUI2->SetDrawOutline(true);             // 외곽선 활성화
	textUI2->SetOutlineThickness(4.0f);        // 외곽선 두께 설정
	textUI2->SetGdiPlusOutlineColor(Gdiplus::Color(0, 0, 0)); // 외곽선 색상(GDI+)
	AddObject(textUI2, GROUP_TYPE::UI);
	*/
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}