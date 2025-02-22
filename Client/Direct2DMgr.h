#pragma once

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	IWICImagingFactory* pWICFactory;

	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // 백 버퍼용 멤버 변수 추가

	// 여러 비트맵을 저장하기 위한 맵
	map<wstring, ID2D1Bitmap*> bitmapMap;
public:
	HRESULT init(HWND hwnd);									//초기화함수

	HRESULT LoadAndStoreBitmap(const wstring& filePath, const wstring& tag);
	ID2D1Bitmap* GetStoredBitmap(const wstring& filePath);

	void RenderAllBitmaps(const std::vector<std::pair<D2D1_RECT_F, std::wstring>>& bitmapsToRender);
	void RenderBitmap(const D2D1_RECT_F& destRect, const wstring& tag);

	void Cleanup();

private:
	HRESULT LoadBitmap(const wstring& filePath, ID2D1Bitmap** ppBitmap);
};

