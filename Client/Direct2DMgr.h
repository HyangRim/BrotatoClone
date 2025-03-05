#pragma once

#include "CFontMgr.h"

struct ScrollArea {
	D2D1_RECT_F viewRect;     // ȭ�鿡 ǥ�õǴ� ����
	D2D1_RECT_F contentRect;  // ��ü ������ ����
	D2D1_POINT_2F scrollPos;  // ���� ��ũ�� ��ġ
};

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	IWICImagingFactory* pWICFactory;

	IDWriteFactory* g_pDWriteFactory;
	
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // �� ���ۿ� ��� ���� �߰�

	// ���� ��Ʈ���� �����ϱ� ���� ��
	map<wstring, ID2D1Bitmap*> bitmapMap;
	// �ɰ��� ��Ʈ�� ����� ��
	map<wstring, vector<ID2D1Bitmap*>> splitBitmapMap;

	CFontMgr* pFontMgr;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }


public:
	HRESULT init(HWND hwnd);									//�ʱ�ȭ�Լ�

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

