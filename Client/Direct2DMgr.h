#pragma once

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	IWICImagingFactory* pWICFactory;

	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // �� ���ۿ� ��� ���� �߰�

	// ���� ��Ʈ���� �����ϱ� ���� ��
	map<wstring, ID2D1Bitmap*> bitmapMap;
public:
	HRESULT init(HWND hwnd);									//�ʱ�ȭ�Լ�

	HRESULT LoadAndStoreBitmap(const wstring& filePath, const wstring& tag);
	ID2D1Bitmap* GetStoredBitmap(const wstring& filePath);

	void RenderAllBitmaps(const std::vector<std::pair<D2D1_RECT_F, std::wstring>>& bitmapsToRender);
	void RenderBitmap(const D2D1_RECT_F& destRect, const wstring& tag);

	void Cleanup();

private:
	HRESULT LoadBitmap(const wstring& filePath, ID2D1Bitmap** ppBitmap);
};

