#include "CFontFileEnumerator.h"

HRESULT CFontFileEnumerator::MoveNext(BOOL* hasCurrentFile) {
    if (!hasCurrentFile) return E_INVALIDARG;

    // �̹� ���Ű� �Ϸ�Ǿ����� FALSE ��ȯ
    if (m_hasEnumerated) {
        *hasCurrentFile = FALSE;
        return S_OK;
    }

    // ��Ʈ ���� ���� ����
    HRESULT hr = m_factory->CreateFontFileReference(
        m_fontPath.c_str(),
        nullptr, // ������ ���� �ð� (nullptr ���)
        &m_currentFile
    );

    if (SUCCEEDED(hr)) {
        *hasCurrentFile = TRUE;
        m_hasEnumerated = true; // ���� �Ϸ� ���·� ����
    }
    else {
        *hasCurrentFile = FALSE;
    }

    return hr;
}

HRESULT CFontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** fontFile) {
    if (!fontFile || !m_currentFile) return E_FAIL;

    *fontFile = m_currentFile;
    (*fontFile)->AddRef(); // ���� ī��Ʈ ����

    return S_OK;
}
