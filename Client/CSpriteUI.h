#pragma once
#include "CUI.h"
class CSpriteUI 
    : public CUI
{
private:
    //CTexture* m_pTexture; // GDI+ Bitmap ��ü (PNG �̹���)
    CTexture* m_pBgTexture;     // ��� �ؽ�ó
    CTexture* m_pFillTexture;   // ä������ �ؽ�ó
    CTexture* m_pFrameTexture;  // �׵θ� �ؽ�ó
    float m_fHpRatio;            // HP ���� (0.0 ~ 1.0)

public:
    CLONE(CSpriteUI);

public:
    CSpriteUI();
    virtual ~CSpriteUI();
    void SetTexture(const wstring& _bgKey, const wstring& _bgPath
        , const wstring& _fillKey, const wstring& _fillPath
        , const wstring& _frameKey, const wstring& _framePath);
    void SetHpRatio(float _ratio);
    virtual void render(Graphics* _pDGraphics);  // GDI+ ��� ������ �Լ�
};