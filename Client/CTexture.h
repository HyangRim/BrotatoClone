#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{

private:
    //�̹��� �����ʹ� �ϳ��� �޸𸮿� �ø���, �װ� ��� ������Ʈ�� ���� ���� ��.
    //��Ʈ���� �ϳ� ������ٰ�, �װ� �Űٰܴ� ��ĥ���ָ��
    //GDI(Legacy)�뵵 ����. 
    HDC         m_dc;
    HBITMAP     m_hBit;


    //������ �ȼ� �����͸� �����ϰ� ���� ��. �׸��� �װ� �������� DC�� �ʿ�. 
    //��Ʈ�� + dc�� �ؽ���. 
    BITMAP      m_bitInfo;


private:
    //GDI+ �뵵 ����.

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

