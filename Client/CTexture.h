#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{

private:
    //이미지 데이터는 하나만 메모리에 올리고, 그걸 모든 오브젝트가 같이 쓰면 됨.
    //비트맵을 하나 만들었다가, 그걸 옮겨다가 색칠해주면됨
    HDC         m_dc;
    HBITMAP     m_hBit;


    //별도의 픽셀 데이터를 보유하고 있을 것. 그리고 그걸 연결지을 DC가 필요. 
    //비트맵 + dc가 텍스쳐. 

    BITMAP      m_bitInfo;

public:
    void Load(const wstring& _strFilePath);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

private:
    CTexture();
    virtual ~CTexture();


    friend class CResMgr;
};

