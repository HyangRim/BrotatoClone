#pragma once

#include "CFontMgr.h"

struct ScrollArea {
	D2D1_RECT_F viewRect;     // 화면에 표시되는 영역
	D2D1_RECT_F contentRect;  // 전체 콘텐츠 영역
	D2D1_POINT_2F scrollPos;  // 현재 스크롤 위치
};

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	IWICImagingFactory* pWICFactory;

	IDWriteFactory* g_pDWriteFactory;
	
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // 백 버퍼용 멤버 변수 추가

	// 여러 비트맵을 저장하기 위한 맵
	map<wstring, ID2D1Bitmap*> bitmapMap;
	// 쪼개진 비트맵 저장용 맵
	map<wstring, vector<ID2D1Bitmap*>> splitBitmapMap;

	CFontMgr* pFontMgr;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }


public:
	HRESULT init(HWND hwnd);									//초기화함수

	HRESULT LoadAndStoreBitmap(const wstring& filePath, const wstring& tag, bool split);
	ID2D1Bitmap* GetStoredBitmap(const wstring& tag);
	vector<ID2D1Bitmap*> GetSplitBitmaps(const wstring& tag);

	void RenderAllBitmaps(const std::vector<std::pair<D2D1_RECT_F, std::wstring>>& bitmapsToRender);
	void RenderBitmap(const D2D1_RECT_F& destRect, const wstring& tag);

	HRESULT StoreBitmapsFromFolder(const std::wstring& folderPath, const std::wstring& tag);

	void Cleanup();

	void RenderText(const std::wstring& text, const D2D1_RECT_F& layoutRect, FONT_TYPE fontType, float fontSize, const D2D1_COLOR_F& color);
	void RenderTextWithOutline(const std::wstring& text, const D2D1_RECT_F& layoutRect, FONT_TYPE fontType, float fontSize, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& outlineColor, float outlineThickness);
	void RenderLeftTextWithOutline(const std::wstring& text, const D2D1_RECT_F& layoutRect, FONT_TYPE fontType, float fontSize, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& outlineColor, float outlineThickness);
	void RenderRightTextWithOutline(const std::wstring& text, const D2D1_RECT_F& layoutRect, FONT_TYPE fontType, float fontSize, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& outlineColor, float outlineThickness);
private:
	HRESULT LoadBitmap(const wstring& filePath, ID2D1Bitmap** ppBitmap);
	HRESULT SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag);

};

