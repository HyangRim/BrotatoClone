#include "CFontFileEnumerator.h"

HRESULT CFontFileEnumerator::MoveNext(BOOL* hasCurrentFile) {
    if (!hasCurrentFile) return E_INVALIDARG;

    // 이미 열거가 완료되었으면 FALSE 반환
    if (m_hasEnumerated) {
        *hasCurrentFile = FALSE;
        return S_OK;
    }

    // 폰트 파일 참조 생성
    HRESULT hr = m_factory->CreateFontFileReference(
        m_fontPath.c_str(),
        nullptr, // 마지막 수정 시간 (nullptr 사용)
        &m_currentFile
    );

    if (SUCCEEDED(hr)) {
        *hasCurrentFile = TRUE;
        m_hasEnumerated = true; // 열거 완료 상태로 설정
    }
    else {
        *hasCurrentFile = FALSE;
    }

    return hr;
}

HRESULT CFontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** fontFile) {
    if (!fontFile || !m_currentFile) return E_FAIL;

    *fontFile = m_currentFile;
    (*fontFile)->AddRef(); // 참조 카운트 증가

    return S_OK;
}
