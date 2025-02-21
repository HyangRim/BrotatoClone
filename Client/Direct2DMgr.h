#pragma once

class Direct2DMgr
{
	SINGLE(Direct2DMgr);

private:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1Bitmap* pBitmap;
	IWICImagingFactory* pWICFactory;
	ID2D1Bitmap* m_pStoredBitmap = nullptr; // 저장된 비트맵


public:
	HRESULT init(HWND hwnd);									//초기화함수
	HRESULT LoadBitmap(const std::wstring& filePath, ID2D1Bitmap** ppBitmap);
	ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }
	ID2D1Bitmap* GetBitMap() { return pBitmap; }

	HRESULT LoadAndStoreBitmap(const std::wstring& filePath);
	void RenderStoredBitmap(const D2D1_RECT_F& destRect);     // 저장된

	void Render();
	void Cleanup();

	friend class CScene_Test;
};

