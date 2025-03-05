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
    // CResMgr를 통해 텍스처를 로드
    m_pBgTexture = CResMgr::GetInstance()->LoadTexture(_bgKey, _bgPath);
    m_pFillTexture = CResMgr::GetInstance()->LoadTexture(_fillKey, _fillPath);
    m_pFrameTexture = CResMgr::GetInstance()->LoadTexture(_frameKey, _framePath);
}

void CSpriteUI::render(Graphics* _pDGraphics)
{
    if (!m_pBgTexture || !m_pFillTexture || !m_pFrameTexture) return;

    // UI의 최종 위치와 크기 계산
    Vec2 vPos = GetFinalPos();

    Vec2 vScale = GetScale();

    // PNG 이미지를 화면에 렌더링
    Gdiplus::Rect destRect(
        static_cast<int>(vPos.x - vScale.x / 2.f),
        static_cast<int>(vPos.y - vScale.y / 2.f),
        static_cast<int>(vScale.x),
        static_cast<int>(vScale.y));

    _pDGraphics->DrawImage(m_pBgTexture->GetBitmap(), destRect);
    int fillWidth = static_cast<int>(destRect.Width * m_fHpRatio);
    // 채워지는 부분 렌더링 (HP 비율에 따라 너비 조정)
    Gdiplus::Rect fillRect(
        destRect.X ,
        destRect.Y,
        fillWidth, // 너비는 HP 비율에 따라 조정
        destRect.Height);
    _pDGraphics->DrawImage(m_pFillTexture->GetBitmap(), fillRect);

    // 테두리 렌더링
    _pDGraphics->DrawImage(m_pFrameTexture->GetBitmap(), destRect);
}

void CSpriteUI::update()
{

}
void CSpriteUI::render(ID2D1HwndRenderTarget* _pRender)
{

    /*
    Vec2 vOffset = GetOffset();
    Vec2 vPos = GetFinalPos();
    Vec2 vScale = GetScale();

    if (m_bCamAffected) {
        vPos = CCamera::GetInstance()->GetRenderPos(vPos);
    }

    // vPos와 vScale은 이미 계산된 좌표와 스케일 값입니다.
    D2D1_RECT_F rect = D2D1::RectF(
        vPos.x - vScale.x / 2.f,
        vPos.y - vScale.y / 2.f,
        vPos.x + vScale.x / 2.f,
        vPos.y + vScale.y / 2.f
    );

    ID2D1SolidColorBrush* pBrush = nullptr;
    HRESULT hr = _pRender->CreateSolidColorBrush(
        m_bLbtnDown ? D2D1::ColorF(D2D1::ColorF::White)   // 마우스 왼쪽 버튼이 눌렸다면 녹색
        : D2D1::ColorF(D2D1::ColorF::Black),  // 아니면 흰색
        &pBrush
    );

    if (SUCCEEDED(hr))
    {
        _pRender->FillRectangle(rect, pBrush);
        pBrush->Release();
    }*/
    component_render(_pRender);
}

void CSpriteUI::SetHpRatio(float _ratio)
{
    m_fHpRatio = max(0.0f, min(1.0f, _ratio)); // Clamp between 0 and 1
}

void CSpriteUI::MouseLbtnClicked()
{
    GetParent()->MouseLbtnClicked();
}
