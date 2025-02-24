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
	// GDI+ �ʱ�ȭ
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// �̹��� ���� ���
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

	// Ÿ�� ũ�� ����
	const int tileSize = 32;

	// tileOutline�� tiles�� ũ�� ��������
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// �� �̹����� ���� ���� ���
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// ��� �̹��� ���� ��� �⺻��
	std::wstring outputDir = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\map1\\";

	// CLSID ��������
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// ����ġ ���� (6��° ������ Ȯ���� ����)
	std::vector<int> weights(tilesCols * tilesRows, 1); // �⺻ ����ġ: 1
	weights[11] = 30; // ��: 6��° ������ ����ġ�� 10���� ����

	// ����ġ�� ������� �� ���� ����
	std::discrete_distribution<int> randomTile(weights.begin(), weights.end());

	// ���յ� �̹��� ���� �� ����
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			// ���ο� �� ��Ʈ�� ����
			Bitmap result(tileSize, tileSize);

			Graphics graphics(&result);

			bool hasNonBlackPixel = false;

			// ���� Ÿ�� ���� (����ġ ���)
			int randomTileIndex = randomTile(rng);
			int randomTx = randomTileIndex % tilesCols; // �� �ε��� ���
			int randomTy = randomTileIndex / tilesCols; // �� �ε��� ���

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
						result.SetPixel(x, y, pixelColor); // �������� �״�� ����
					}
				}
			}

			if (hasNonBlackPixel) {
				// ��� ���� ���� ��� ����
				std::wstring resultPath = outputDir + L"mapTile_" + std::to_wstring(resultIndex++) + L".png";

				// ��� �̹��� ����
				result.Save(resultPath.c_str(), &pngClsid, NULL);
			}
		}
	}
	// GDI+ ����
	GdiplusShutdown(gdiplusToken);
	*/

	/*
	 // GDI+ �ʱ�ȭ
	ULONG_PTR gdiplusToken;
	InitializeGDIPlus(gdiplusToken);

	// �̹��� ���� ���
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

	// Ÿ�� ũ�� ����
	const int tileSize = 32;
	std::vector<Gdiplus::Bitmap*> gdiPlusBitmaps;

	// tileOutline�� tiles�� ũ�� ��������
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// �� �̹����� ���� ���� ���
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// ��� �̹��� ���� ��� �⺻��
	std::wstring outputDir = L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\";

	// CLSID ��������
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// ���յ� �̹��� ���� �� ����
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			for (int ty = 0; ty < tilesRows; ++ty) {
				for (int tx = 0; tx < tilesCols; ++tx) {
					// ���ο� �� ��Ʈ�� ����
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
								result.SetPixel(x, y, pixelColor); // �������� �״�� ����
							}
						}
					}

					if (hasNonBlackPixel) {
						// ��� ���� ���� ��� ����
						std::wstring resultPath = outputDir + L"result_" + std::to_wstring(resultIndex++) + L".png";

						// ��� �̹��� ����
						result.Save(resultPath.c_str(), &pngClsid, NULL);
					}

				}
			}
		}
	}
	std::cout << "Images successfully generated and saved!" << std::endl;

	// GDI+ ����
	ShutdownGDIPlus(gdiplusToken);
	*/
	/*
	// GDI+ �ʱ�ȭ
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
	// Graphics ��ü ����
	Graphics graphics(&tileOutline);

	// Ÿ�� ũ�� ����
	const int tileWidth = TILE_SIZE / 2;
	const int tileHeight = TILE_SIZE/ 2;

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
	tileOutline.Save(L"C:\\Users\\c\\source\\repos\\HyangRim\\BrotatoClone\\Output\\bin\\content\\texture\\tiles\\map6.png", &pngClsid, NULL);

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