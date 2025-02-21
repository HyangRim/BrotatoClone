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

    // COM 라이브러리 초기화
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) return hr;

    // Direct2D 팩토리 생성
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    if (FAILED(hr)) return hr;

    // 렌더 타겟 생성
    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &pRenderTarget
    );
    if (FAILED(hr)) return hr;

    // WIC 팩토리 생성
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,   // CLSID for WIC Imaging Factory
        nullptr,                   // No aggregation
        CLSCTX_INPROC_SERVER,      // Run in the same process
        IID_PPV_ARGS(&pWICFactory) // Output pointer to IWICImagingFactory
    );
    if (FAILED(hr)) {
        MessageBox(nullptr, L"WIC 팩토리 생성 실패!", L"오류", MB_OK);
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

    return LoadBitmap(filePath, &m_pStoredBitmap); // 기존 LoadBitmap 메서드 재사용
}

void Direct2DMgr::RenderStoredBitmap(const D2D1_RECT_F& destRect) {
    if (!pRenderTarget || !m_pStoredBitmap) return;

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    pRenderTarget->DrawBitmap(m_pStoredBitmap, destRect);

    HRESULT hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"렌더링 실패!", L"오류", MB_OK);
    }
}

HRESULT Direct2DMgr::LoadBitmap(const std::wstring& filePath, ID2D1Bitmap** ppBitmap) {
    // 파일 존재 여부 확인
    if (!pWICFactory) {
        MessageBox(nullptr, L"WIC 팩토리가 초기화되지 않았습니다!", L"오류", MB_OK);
        return E_FAIL;
    }
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pSource = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    HRESULT hr = pWICFactory->CreateDecoderFromFilename(
        filePath.c_str(),            // 파일 경로
        nullptr,                     // 특정 공급업체 GUID (NULL 사용)
        GENERIC_READ,                // 읽기 전용 액세스
        WICDecodeMetadataCacheOnLoad,// 메타데이터 캐시 옵션
        &pDecoder                    // 디코더 출력
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"이미지 디코더 생성 실패!", L"오류", MB_OK);
        return hr;
    }

    // 첫 번째 프레임 가져오기
    hr = pDecoder->GetFrame(0, &pSource);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"이미지 프레임 가져오기 실패!", L"오류", MB_OK);
        pDecoder->Release();
        return hr;
    }

    // 포맷 변환기 생성
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) {
        MessageBox(nullptr, L"포맷 변환기 생성 실패!", L"오류", MB_OK);
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // 포맷 변환 초기화
    hr = pConverter->Initialize(
        pSource,
        GUID_WICPixelFormat32bppPBGRA, // Direct2D에서 사용하는 포맷
        WICBitmapDitherTypeNone,
        nullptr,
        0.f,
        WICBitmapPaletteTypeMedianCut
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, L"이미지 포맷 변환 실패!", L"오류", MB_OK);
        pConverter->Release();
        pSource->Release();
        pDecoder->Release();
        return hr;
    }

    // Direct2D 비트맵 생성
    hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);

    if (FAILED(hr)) {
        MessageBox(nullptr, L"Direct2D 비트맵 생성 실패!", L"오류", MB_OK);
    }

    // 메모리 해제
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

    // 비트맵 크기 가져오기
    D2D1_SIZE_F size = pBitmap->GetSize();

    // 비트맵 그리기
    pRenderTarget->DrawBitmap(
        pBitmap,
        D2D1::RectF(0.0f, 0.0f, size.width, size.height)
    );

    HRESULT hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"렌더링 실패!", L"오류", MB_OK);
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
