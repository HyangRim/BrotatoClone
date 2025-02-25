#pragma once

#include <dwrite.h>
#include <string>

class CFontFileEnumerator : public IDWriteFontFileEnumerator {
private:
    ULONG m_refCount;
    IDWriteFactory* m_factory;
    std::wstring m_fontPath;
    IDWriteFontFile* m_currentFile;
    bool m_hasEnumerated; // 폰트 파일이 이미 열거되었는지 여부

public:
    CFontFileEnumerator(IDWriteFactory* factory, const std::wstring& fontPath)
        : m_refCount(1), m_factory(factory), m_fontPath(fontPath), m_currentFile(nullptr), m_hasEnumerated(false) {}

    ~CFontFileEnumerator() {
        if (m_currentFile) {
            m_currentFile->Release();
        }
    }

    HRESULT STDMETHODCALLTYPE MoveNext(BOOL* hasCurrentFile) override;

    HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile** fontFile) override;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (riid == __uuidof(IDWriteFontFileEnumerator) || riid == __uuidof(IUnknown)) {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return ++m_refCount; }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG newCount = --m_refCount;
        if (newCount == 0) delete this;
        return newCount;
    }
};
