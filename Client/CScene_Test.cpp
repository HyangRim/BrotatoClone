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
#include "CGround.h"
#include "CImage.h"

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

void CScene_Test::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Test::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	MakeMapTile(L"texture\\tiles\\tiles_outline.png", L"texture\\tiles\\tiles_1.png", L"texture\\tiles\\map\\", 30, 1);
	pD2DMgr->StoreBitmapsFromFolder(L"texture\\tiles\\map\\", L"Map");
	MakeTile(L"Map");

	/*
	CObject* pObj = new CGround;
	pObj->SetPos(Vec2(100.f,100.f));
	pObj->SetScale(Vec2((float)TILE_SIZE / 2.f, (float)TILE_SIZE / 2.f));
	pObj->SetName(L"TILE");
	pObj->CreateImage();
	pObj->GetImage()->SetBitmap(splitBitmap[0]);
	AddObject(pObj, GROUP_TYPE::TILE);
	*/
	/*
	// GDI+ 초기화
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// 이미지 파일 경로
	const WCHAR* tileOutlinePath = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_outline.png";
	const WCHAR* tilesPath = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_1.png";

	Bitmap tileOutline(tileOutlinePath);
	if (tileOutline.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tile_outline.png" << std::endl;
		GdiplusShutdown(gdiplusToken);
		return;
	}

	Bitmap tiles(tilesPath);
	if (tiles.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tiles_1.png" << std::endl;
		GdiplusShutdown(gdiplusToken);
		return;
	}

	// 타일 크기 정의
	const int tileSize = 32;

	// tileOutline과 tiles의 크기 가져오기
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// 각 이미지의 조각 개수 계산
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// 결과 이미지 저장 경로 기본값
	std::wstring outputDir = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\map1\\";

	// CLSID 가져오기
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// 가중치 설정 (6번째 조각의 확률을 높임)
	std::vector<int> weights(tilesCols * tilesRows, 1); // 기본 가중치: 1
	weights[11] = 30; // 예: 6번째 조각의 가중치를 10으로 설정

	// 가중치를 기반으로 한 분포 생성
	std::discrete_distribution<int> randomTile(weights.begin(), weights.end());

	// 조합된 이미지 생성 및 저장
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			// 새로운 빈 비트맵 생성
			Bitmap result(tileSize, tileSize);

			Graphics graphics(&result);

			bool hasNonBlackPixel = false;

			// 랜덤 타일 선택 (가중치 기반)
			int randomTileIndex = randomTile(rng);
			int randomTx = randomTileIndex % tilesCols; // 열 인덱스 계산
			int randomTy = randomTileIndex / tilesCols; // 행 인덱스 계산

			for (int y = 0; y < tileSize; ++y) {
				for (int x = 0; x < tileSize; ++x) {
					Color pixelColor;
					tileOutline.GetPixel(ox * tileSize + x, oy * tileSize + y, &pixelColor);

					if (IsNonBlackPixel(pixelColor)) {
						hasNonBlackPixel = true;

						Color tilePixelColor;
						tiles.GetPixel(randomTx * tileSize + x % tileSize, randomTy * tileSize + y % tileSize, &tilePixelColor);

						result.SetPixel(x, y, tilePixelColor);
					}
					else {
						result.SetPixel(x, y, pixelColor); // 검은색은 그대로 유지
					}
				}
			}

			if (hasNonBlackPixel) {
				// 결과 파일 저장 경로 설정
				std::wstring resultPath = outputDir + L"mapTile_" + std::to_wstring(resultIndex++) + L".png";

				// 결과 이미지 저장
				result.Save(resultPath.c_str(), &pngClsid, NULL);
			}
		}
	}
	// GDI+ 종료
	GdiplusShutdown(gdiplusToken);
	*/

	/*
	 // GDI+ 초기화
	ULONG_PTR gdiplusToken;
	InitializeGDIPlus(gdiplusToken);

	// 이미지 파일 경로
	const WCHAR* tileOutlinePath = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_outline1.png";
	const WCHAR* tilesPath = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_1_1.png";

	Bitmap tileOutline(tileOutlinePath);
	if (tileOutline.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tile_outline.png" << std::endl;
		ShutdownGDIPlus(gdiplusToken);
		return;
	}

	Bitmap tiles(tilesPath);
	if (tiles.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tiles_1.png" << std::endl;
		ShutdownGDIPlus(gdiplusToken);
		return;
	}

	// 타일 크기 정의
	const int tileSize = 32;
	std::vector<Gdiplus::Bitmap*> gdiPlusBitmaps;

	// tileOutline과 tiles의 크기 가져오기
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// 각 이미지의 조각 개수 계산
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// 결과 이미지 저장 경로 기본값
	std::wstring outputDir = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\";

	// CLSID 가져오기
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// 조합된 이미지 생성 및 저장
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			for (int ty = 0; ty < tilesRows; ++ty) {
				for (int tx = 0; tx < tilesCols; ++tx) {
					// 새로운 빈 비트맵 생성
					Bitmap result(tileSize, tileSize);

					Graphics graphics(&result);

					bool hasNonBlackPixel = false;

					for (int y = 0; y < tileSize; ++y) {
						for (int x = 0; x < tileSize; ++x) {
							Color pixelColor;
							tileOutline.GetPixel(ox * tileSize + x, oy * tileSize + y, &pixelColor);

							if (IsNonBlackPixel(pixelColor)) {
								hasNonBlackPixel = true;

								Color tilePixelColor;
								tiles.GetPixel(tx * tileSize + x % tileSize, ty * tileSize + y % tileSize, &tilePixelColor);

								result.SetPixel(x, y, tilePixelColor);
							}
							else {
								result.SetPixel(x, y, pixelColor); // 검은색은 그대로 유지
							}
						}
					}

					if (hasNonBlackPixel) {
						// 결과 파일 저장 경로 설정
						std::wstring resultPath = outputDir + L"result_" + std::to_wstring(resultIndex++) + L".png";

						// 결과 이미지 저장
						result.Save(resultPath.c_str(), &pngClsid, NULL);
					}

				}
			}
		}
	}
	std::cout << "Images successfully generated and saved!" << std::endl;

	// GDI+ 종료
	ShutdownGDIPlus(gdiplusToken);
	*/
	/*
	// GDI+ 초기화
	ULONG_PTR gdiplusToken;
	InitializeGDIPlus(gdiplusToken);

	Bitmap tileOutline(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_outline.png");
	if (tileOutline.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tile_outline.png" << std::endl;
		return ;
	}
	Bitmap tiles(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\tiles_6.png");
	if (tiles.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tiles_1.png" << std::endl;
		return ;
	}
	// Graphics 객체 생성
	Graphics graphics(&tileOutline);

	// 타일 크기 정의
	const int tileWidth = TILE_SIZE / 2;
	const int tileHeight = TILE_SIZE/ 2;

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
	tileOutline.Save(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\map6.png", &pngClsid, NULL);

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