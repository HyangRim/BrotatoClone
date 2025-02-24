#pragma once

class Direct2DMgr
{
	SINGLE(Direct2DMgr)

private:
	ID2D1Factory* pD2DFactory;
	IWICImagingFactory* pWICFactory;

	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1BitmapRenderTarget* pBitmapRenderTarget; // �� ���ۿ� ��� ���� �߰�

	// ���� ��Ʈ���� �����ϱ� ���� ��
	map<wstring, ID2D1Bitmap*> bitmapMap;
	// �ɰ��� ��Ʈ�� ����� ��
	map<wstring, vector<ID2D1Bitmap*>> splitBitmapMap;

public:
	ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }
public:
	HRESULT init(HWND hwnd);									//�ʱ�ȭ�Լ�

	HRESULT LoadAndStoreBitmap(const wstring& filePath, const wstring& tag, bool split);
	ID2D1Bitmap* GetStoredBitmap(const wstring& filePath);
	vector<ID2D1Bitmap*> GetSplitBitmaps(const wstring& tag);

	void RenderAllBitmaps(const std::vector<std::pair<D2D1_RECT_F, std::wstring>>& bitmapsToRender);
	void RenderBitmap(const D2D1_RECT_F& destRect, const wstring& tag);

	void Cleanup();

private:
	HRESULT LoadBitmap(const wstring& filePath, ID2D1Bitmap** ppBitmap);

	HRESULT SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag);
};

