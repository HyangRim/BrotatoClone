#pragma once

#include <dwrite.h>
#include <vector>
#include <string>

class CFontCollectionLoader : public IDWriteFontCollectionLoader {
private:
    std::vector<std::wstring> m_fontPaths;

public:
    CFontCollectionLoader(const std::vector<std::wstring>& fontPaths)
        : m_fontPaths(fontPaths) {}

    HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
        IDWriteFactory* factory,
        const void* collectionKey,
        UINT32 collectionKeySize,
        IDWriteFontFileEnumerator** fontFileEnumerator
    ) override;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (riid == __uuidof(IDWriteFontCollectionLoader) || riid == __uuidof(IUnknown)) {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
};
