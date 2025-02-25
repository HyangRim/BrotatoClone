#include "pch.h"
#include "CFontCollectionLoader.h"
#include "CFontFileEnumerator.h"

HRESULT CFontCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* factory,
    const void* collectionKey,
    UINT32 collectionKeySize,
    IDWriteFontFileEnumerator** fontFileEnumerator
) {
    if (!factory || !fontFileEnumerator || !collectionKey || collectionKeySize != sizeof(UINT32)) {
        return E_INVALIDARG;
    }

    UINT32 index = *reinterpret_cast<const UINT32*>(collectionKey);
    if (index >= m_fontPaths.size()) {
        return E_FAIL;
    }

    *fontFileEnumerator = new CFontFileEnumerator(factory, m_fontPaths[index]);
    (*fontFileEnumerator)->AddRef();

    return S_OK;
}
