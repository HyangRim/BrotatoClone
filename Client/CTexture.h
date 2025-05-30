#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{

private:
    //이미지 데이터는 하나만 메모리에 올리고, 그걸 모든 오브젝트가 같이 쓰면 됨.
    //비트맵을 하나 만들었다가, 그걸 옮겨다가 색칠해주면됨
    //GDI(Legacy)용도 변수. 
    HDC         m_dc;
    HBITMAP     m_hBit;


    //별도의 픽셀 데이터를 보유하고 있을 것. 그리고 그걸 연결지을 DC가 필요. 
    //비트맵 + dc가 텍스쳐. 
    BITMAP      m_bitInfo;
private:
    //GDI+ 용도 변수.

    Gdiplus::Bitmap*    m_pBitmap;
    Gdiplus::Graphics*  m_pGraphics;
    UINT                m_width;
    UINT                m_height;

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iWidth, UINT _iHeight);

    UINT Width() { return m_width; }
    UINT Height() { return m_height; }

    Gdiplus::Graphics* GetGraphics() { return m_pGraphics; }
    Gdiplus::Bitmap* GetBitmap() { return m_pBitmap; }
    HDC GetDC() { return m_dc; }

private:

    //HBITMAP LoadPngToHBITMAP(LPWSTR )

private:
    CTexture();
    virtual ~CTexture();


    friend class CResMgr;
};

