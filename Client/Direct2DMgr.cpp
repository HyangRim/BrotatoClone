#include "pch.h"
#include "Direct2DMgr.h"

Direct2DMgr::Direct2DMgr()
{

}


Direct2DMgr::~Direct2DMgr()
{
    Cleanup();
}

HRESULT Direct2DMgr::init(HWND hwnd)
{
    HRESULT hr;

    // COM ���̺귯�� �ʱ�ȭ
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) return hr;

    // Direct2D ���丮 ����
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    if (FAILED(hr)) return hr;

    // ���� Ÿ�� ����
    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &pRenderTarget
    );
    if (FAILED(hr)) return hr;

    // WIC ���丮 ����
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,   // CLSID for WIC Imaging Factory
        nullptr,                   // No aggregation
        CLSCTX_INPROC_SERVER,      // Run in the same process
        IID_PPV_ARGS(&pWICFactory) // Output pointer to IWICImagingFactory
    );
    if (FAILED(hr)) {
        MessageBox(nullptr, L"WIC ���丮 ���� ����!", L"����", MB_OK);
        return hr;
    }

    return S_OK;
}

// Direct2DMgr.cpp
HRESULT Direct2DMgr::LoadAndStoreBitmap(const std::wstring& filePath) {
    if (m_pStoredBitmap) {
        m_pStoredBitmap->Release();
        m_pStoredBitmap = nullptr;
    }

    return LoadBitmap(filePath, &m_pStoredBitmap); // ���� LoadBitmap �޼��� ����
}

void Direct2DMgr::RenderStoredBitmap(const D2D1_RECT_F& destRect) {
    if (!pRenderTarget || !m_pStoredBitmap) return;

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    pRenderTarget->DrawBitmap(m_pStoredBitmap, destRect);

    HRESULT hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"������ ����!", L"����", MB_OK);
    }
}

HRESULT Direct2DMgr::LoadBitmap(const std::wstring& filePath, ID2D1Bitmap** ppBitmap) {
    // ���� ���� ���� Ȯ��
    if (!pWICFactory) {
        MessageBox(nullptr, L"WIC ���丮�� �ʱ�ȭ���� �ʾҽ��ϴ�!", L"����", MB_OK);
        return E_FAIL;
    }
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pSource = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    HRESULT hr = pWICFactory->CreateDecoderFromFilename(
        filePath.c_str(),            // ���� ���
        nullptr,                     // Ư�� ���޾�ü GUID (NULL ���)
        GENERIC_READ,                // �б� ���� �׼���
        WICDecodeMetadataCacheOnLoad,// ��Ÿ������ ĳ�� �ɼ�
        &pDecoder                    // ���ڴ� ���
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ���ڴ� ���� ����!", L"����", MB_OK);
        return hr;
    }

    // ù ��° ������ ��������
    hr = pDecoder->GetFrame(0, &pSource);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ������ �������� ����!", L"����", MB_OK);
        pDecoder->Release();
        return hr;
    }

    // ���� ��ȯ�� ����
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"���� ��ȯ�� ���� ����!", L"����", MB_OK);
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // ���� ��ȯ �ʱ�ȭ
    hr = pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA, // Direct2D���� ����ϴ� ����
        WICBitmapDitherTypeNone,
        nullptr,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"�̹��� ���� ��ȯ ����!", L"����", MB_OK);
        pConverter->Release();
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // Direct2D ��Ʈ�� ����
    hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);

    if (FAILED(hr)) {
        MessageBox(nullptr, L"Direct2D ��Ʈ�� ���� ����!", L"����", MB_OK);
    }

    // �޸� ����
    if (pConverter) pConverter->Release();
    if (pSource) pSource->Release();
    if (pDecoder) pDecoder->Release();

    return hr;
}

void Direct2DMgr::Render()
{
    if (!pRenderTarget || !pBitmap) return;

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // ��Ʈ�� ũ�� ��������
    D2D1_SIZE_F size = pBitmap->GetSize();

    // ��Ʈ�� �׸���
    pRenderTarget->DrawBitmap(
        pBitmap,
        D2D1::RectF(0.0f, 0.0f, size.width, size.height)
    );

    HRESULT hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"������ ����!", L"����", MB_OK);
    }
}

void Direct2DMgr::Cleanup()
{
    if (pBitmap) { pBitmap->Release(); pBitmap = nullptr; }
    if (pRenderTarget) { pRenderTarget->Release(); pRenderTarget = nullptr; }
    if (pWICFactory) { pWICFactory->Release(); pWICFactory = nullptr; }
    if (pD2DFactory) { pD2DFactory->Release(); pD2DFactory = nullptr; }

    CoUninitialize();
}
