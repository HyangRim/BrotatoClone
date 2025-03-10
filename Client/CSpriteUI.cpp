#include "pch.h"
#include "CSpriteUI.h"
#include "CUI.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CObject.h"
#include "CImage.h"

CSpriteUI::CSpriteUI()
	:CUI(false)
	, m_fHpRatio(1.0f)
    , m_colorRoundedNormal(D2D1::ColorF::Black)
    , m_colorRoundedMouseOn(D2D1::ColorF::Black)
    , m_colorFillMouseOn(D2D1::ColorF::Black)
    , m_colorFillNormal(D2D1::ColorF::Black)
    , m_bBackGround(false)

{
    
}

CSpriteUI::~CSpriteUI()
{

}

void CSpriteUI::finalupdate()
{
    CUI::finalupdate();

    if (GetParent())
    {
        Vec2 vParentPos = GetParent()->GetFinalPos();
        m_vFinalPos = vParentPos + m_vOffset;
    }
    else
    {
        m_vFinalPos = GetPos() + m_vOffset;
    }
}

void CSpriteUI::SetTexture(const wstring& _bgKey, const wstring& _bgPath 
    , const wstring& _fillKey, const wstring& _fillPath 
    , const wstring& _frameKey, const wstring& _framePath) {
    // CResMgr�� ���� �ؽ�ó�� �ε�
    m_pBgTexture = CResMgr::GetInstance()->LoadTexture(_bgKey, _bgPath);
    m_pFillTexture = CResMgr::GetInstance()->LoadTexture(_fillKey, _fillPath);
    m_pFrameTexture = CResMgr::GetInstance()->LoadTexture(_frameKey, _framePath);
}

void CSpriteUI::render(Graphics* _pDGraphics)
{
    if (!m_pBgTexture || !m_pFillTexture || !m_pFrameTexture) return;

    // UI�� ���� ��ġ�� ũ�� ���
    Vec2 vPos = GetFinalPos();

    Vec2 vScale = GetScale();

    // PNG �̹����� ȭ�鿡 ������
    Gdiplus::Rect destRect(
        static_cast<int>(vPos.x - vScale.x / 2.f),
        static_cast<int>(vPos.y - vScale.y / 2.f),
        static_cast<int>(vScale.x),
        static_cast<int>(vScale.y));

    _pDGraphics->DrawImage(m_pBgTexture->GetBitmap(), destRect);
    int fillWidth = static_cast<int>(destRect.Width * m_fHpRatio);
    // ä������ �κ� ������ (HP ������ ���� �ʺ� ����)
    Gdiplus::Rect fillRect(
        destRect.X ,
        destRect.Y,
        fillWidth, // �ʺ�� HP ������ ���� ����
        destRect.Height);
    _pDGraphics->DrawImage(m_pFillTexture->GetBitmap(), fillRect);

    // �׵θ� ������
    _pDGraphics->DrawImage(m_pFrameTexture->GetBitmap(), destRect);
}

void CSpriteUI::update()
{

}
void CSpriteUI::render(ID2D1HwndRenderTarget* _pRender)
{

    
    if (m_bBackGround)
    {
        Vec2 vOffset = GetOffset();
        Vec2 vPos = GetFinalPos();
        Vec2 vScale = GetScale();

        if (m_bCamAffected) {
            vPos = CCamera::GetInstance()->GetRenderPos(vPos);
        }

        // vPos�� vScale�� �̹� ���� ��ǥ�� ������ ���Դϴ�.
        D2D1_RECT_F rect = D2D1::RectF(
            vPos.x - vScale.x / 2.f,
            vPos.y - vScale.y / 2.f,
            vPos.x + vScale.x / 2.f,
            vPos.y + vScale.y / 2.f
        ); 
        D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
            D2D1::RectF(
                vPos.x - vScale.x / 2.f,
                vPos.y - vScale.y / 2.f,
                vPos.x + vScale.x / 2.f,
                vPos.y + vScale.y / 2.f), // �簢���� ��ǥ (left, top, right, bottom)
            m_fradiusX, // X�� ������ (radiusX)
            m_fradiusY  // Y�� ������ (radiusY)
        );

        // ���� ���� �귯�� ����
        ID2D1SolidColorBrush* pFillBrush = nullptr;
        HRESULT hrFill = _pRender->CreateSolidColorBrush(
            m_bLbtnDown ? m_colorFillMouseOn : m_colorFillNormal, // ���� ����
            &pFillBrush
        );

        // �ܰ��� ���� �귯�� ����
        ID2D1SolidColorBrush* pOutlineBrush = nullptr;
        HRESULT hrOutline = _pRender->CreateSolidColorBrush(
            m_bLbtnDown ? m_colorRoundedMouseOn : m_colorRoundedNormal, // �ܰ��� ���� (������ ����)
            &pOutlineBrush
        );

        if (SUCCEEDED(hrFill) && SUCCEEDED(hrOutline)) {
            // ���� ä���
            if (m_bIsRoundedRect) {
                _pRender->FillRoundedRectangle(roundedRect, pFillBrush);
            }
            else {
                _pRender->FillRectangle(rect, pFillBrush);
            }

            // �ܰ��� �׸���
            if (m_bIsRoundedRect) {
                _pRender->DrawRoundedRectangle(roundedRect, pOutlineBrush, 2.0f); // �β� 2.0f
            }
            else {
                _pRender->DrawRectangle(rect, pOutlineBrush, 2.0f); // �β� 2.0f
            }

            // �귯�� ����
            pFillBrush->Release();
            pOutlineBrush->Release();
        }
    }
    component_render(_pRender);
}

void CSpriteUI::SetHpRatio(float _ratio)
{
    m_fHpRatio = max(0.0f, min(1.0f, _ratio)); // Clamp between 0 and 1
}

void CSpriteUI::MouseLbtnClicked()
{
    if(GetParent())
        GetParent()->MouseLbtnClicked();
}
