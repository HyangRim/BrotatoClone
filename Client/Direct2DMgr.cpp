#include "pch.h"
#include "Direct2DMgr.h"
#include "CPathMgr.h"

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
        D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
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

    hr = pRenderTarget->CreateCompatibleRenderTarget(&pBitmapRenderTarget);
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT Direct2DMgr::LoadAndStoreBitmap(const std::wstring& filePath, const std::wstring& tag, bool split) {
    // 이미 로드된 비트맵이 있는지 확인
    if (bitmapMap.find(tag) != bitmapMap.end()) {
        return S_OK; // 이미 로드됨
    }

    ID2D1Bitmap* newBitmap = nullptr;

    const wstring strFinalPath = CPathMgr::GetInstance()->GetContentPath() + filePath;

    HRESULT hr = LoadBitmap(strFinalPath, &newBitmap);
    
    if (SUCCEEDED(hr)) {
        bitmapMap[tag] = newBitmap; // 맵에 저장

        // split이 true인 경우 비트맵을 쪼갬
        if (split) {
            hr = SplitBitmap(newBitmap, tag);
            if (FAILED(hr)) {
                MessageBox(nullptr, L"비트맵 분할 실패!", L"오류", MB_OK);
                return hr;
            }
        }
    }

    return hr;
}

ID2D1Bitmap* Direct2DMgr::GetStoredBitmap(const std::wstring& tag) {
    auto it = bitmapMap.find(tag);

    if (it != bitmapMap.end()) {
        return it->second; // 저장된 비트맵bitmapMap 반환
    }

    return nullptr; // 없음
}

vector<ID2D1Bitmap*> Direct2DMgr::GetSplitBitmaps(const wstring& tag) {
    auto it = splitBitmapMap.find(tag);

    if (it != splitBitmapMap.end()) {
        return it->second; // 저장된 비트맵 반환
    }

    return {}; // 없음
}

void Direct2DMgr::RenderBitmap(const D2D1_RECT_F& destRect, const std::wstring& baseTag) {
    if (!pRenderTarget) return;

    // 태그가 baseTag로 시작하는 비트맵만 렌더링
    auto searched_bitmap = GetStoredBitmap(baseTag);

    if (nullptr == searched_bitmap) {
        assert(nullptr);
    }
    pRenderTarget->DrawBitmap(searched_bitmap, destRect);

    // 백 버퍼 내용을 메인 렌더 타겟으로 복사
    /*
    pRenderTarget->BeginDraw();
    ID2D1Bitmap* backBufferBitmap = nullptr;
    pBitmapRenderTarget->GetBitmap(&backBufferBitmap);
    pRenderTarget->DrawBitmap(backBufferBitmap);
    backBufferBitmap->Release();
    

    hr = pRenderTarget->EndDraw();
    if (FAILED(hr)) {
        MessageBox(nullptr, L"화면 렌더링 실패!", L"오류", MB_OK);
    }
    */
}

void Direct2DMgr::RenderAllBitmaps(const std::vector<std::pair<D2D1_RECT_F, std::wstring>>& bitmapsToRender) {
    if (!pRenderTarget) return;

    // 전달받은 모든 비트맵을 순회하며 렌더링
    for (const auto& bitmapInfo : bitmapsToRender) {
        const D2D1_RECT_F& destRect = bitmapInfo.first;
        const std::wstring& tag = bitmapInfo.second;

        ID2D1Bitmap* bitmap = GetStoredBitmap(tag);
        if (bitmap) {
            pRenderTarget->DrawBitmap(bitmap, destRect);
        }
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

HRESULT Direct2DMgr::SplitBitmap(ID2D1Bitmap* bitmap, const wstring& tag) {
    if (!bitmap || !pRenderTarget) return E_FAIL;

    D2D1_SIZE_F bitmapSize = bitmap->GetSize();
    UINT rows = static_cast<UINT>(bitmapSize.height / 64);
    UINT cols = static_cast<UINT>(bitmapSize.width / 64);

    vector<ID2D1Bitmap*> splitBitmaps;

    for (UINT row = 0; row < rows; ++row) {
        for (UINT col = 0; col < cols; ++col) {
            // 각 64x64 영역 정의
            D2D1_RECT_F srcRect = D2D1::RectF(
                col * 64.0f,
                row * 64.0f,
                (col + 1) * 64.0f,
                (row + 1) * 64.0f
            );

            // 호환 렌더 타겟 생성
            ID2D1BitmapRenderTarget* compatibleRenderTarget = nullptr;
            HRESULT hr = pRenderTarget->CreateCompatibleRenderTarget(
                D2D1::SizeF(64.0f, 64.0f),
                &compatibleRenderTarget
            );
            if (FAILED(hr)) {
                MessageBox(nullptr, L"호환 렌더 타겟 생성 실패!", L"오류", MB_OK);
                return hr;
            }

            // 호환 렌더 타겟에 원본 비트맵 그리기
            compatibleRenderTarget->BeginDraw();
            compatibleRenderTarget->DrawBitmap(bitmap, D2D1::RectF(0, 0, 64, 64), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
            hr = compatibleRenderTarget->EndDraw();
            if (FAILED(hr)) {
                MessageBox(nullptr, L"비트맵 그리기 실패!", L"오류", MB_OK);
                compatibleRenderTarget->Release();
                return hr;
            }

            // 호환 렌더 타겟에서 비트맵 가져오기
            ID2D1Bitmap* subBitmap = nullptr;
            hr = compatibleRenderTarget->GetBitmap(&subBitmap);
            compatibleRenderTarget->Release(); // 렌더 타겟 해제
            if (FAILED(hr)) {
                MessageBox(nullptr, L"비트맵 가져오기 실패!", L"오류", MB_OK);
                return hr;
            }

            splitBitmaps.push_back(subBitmap);
        }
    }

    splitBitmapMap[tag] = splitBitmaps;
    return S_OK;
}

void Direct2DMgr::Cleanup() {
    for (auto& pair : bitmapMap) {
        if (pair.second) pair.second->Release();
    }

    bitmapMap.clear();

    if (pRenderTarget) {
        pRenderTarget->Release();
        pRenderTarget = nullptr;
    }

    if (pWICFactory) {
        pWICFactory->Release();
        pWICFactory = nullptr;
    }

    if (pD2DFactory) {
        pD2DFactory->Release();
        pD2DFactory = nullptr;
    }

    CoUninitialize();
}
