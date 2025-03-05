#include "pch.h"
#include "CTextUI.h"
#include "CFontMgr.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CObject.h"

CTextUI::CTextUI()
    : m_pOwner(nullptr)
    , m_mode(TextUIMode::TEXT)
    , m_vOffsetLT(Vec2(0.f, 0.f))
    , m_vOffsetRB(Vec2(0.f, 0.f))
    , m_fontType(FONT_TYPE::DEFALUT)
    , m_Text(L"")
    , m_iFontSize(0)
    , m_colorText(D2D1::ColorF(0, 0, 0, 0))
    , m_bdrawOutline(false)
    , m_fOutlineThickness(0.f)
    , m_colorOutline(D2D1::ColorF(0, 0, 0, 0))
    , m_iNumber(0)
    , m_fAcc(0.f)
    , m_RedColor(D2D1::ColorF(D2D1::ColorF::Red))
    , m_bCamAffected(false)
    , m_iHorizontal(0)
{
   
}

CTextUI::~CTextUI()
{

}

void CTextUI::UpdateNumber()
{
    if (m_mode == TextUIMode::NUMBER) {
        // 누적 시간 업데이트
        m_fAcc += fDT;

        // 1초마다 숫자 증가
        if (m_fAcc >= 1.f) {
            m_iNumber -= static_cast<int>(m_fAcc / 1.f); // 초 단위로 증가
            m_fAcc = fmod(m_fAcc, 1.f);               // 남은 시간 유지
        }
    }
}

void CTextUI::render(ID2D1HwndRenderTarget* _pRender)
{
    auto d2dManager = Direct2DMgr::GetInstance();

    Vec2 vPos = m_pOwner->GetFinalPos(); //원래 GetPos()였음

    if (m_bCamAffected) {
        vPos = CCamera::GetInstance()->GetRenderPos(vPos);
    }
    //vPos = CCamera::GetInstance()->GetRenderPos(vPos);
    //글자만 출력
    if (m_mode == TextUIMode::TEXT)
    {

        if (m_iHorizontal == 0) {
            d2dManager->RenderTextWithOutline(
                m_Text,                     // 출력할 텍스트
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // 출력 영역 (좌상단, 우하단)
                m_fontType,                          // 폰트 타입
                (float)m_iFontSize,                                  // 폰트 크기
                m_colorText,  // 텍스트 색상
                m_colorOutline,    // 외곽선 색상
                m_fOutlineThickness                                // 외곽선 두께
            );
        }
        else if (m_iHorizontal == 1) {
            d2dManager->RenderLeftTextWithOutline(
                m_Text,                     // 출력할 텍스트
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // 출력 영역 (좌상단, 우하단)
                m_fontType,                          // 폰트 타입
                (float)m_iFontSize,                                  // 폰트 크기
                m_colorText,  // 텍스트 색상
                m_colorOutline,    // 외곽선 색상
                m_fOutlineThickness                                // 외곽선 두께
            );
        }
        else if (m_iHorizontal == 2) {
            d2dManager->RenderRightTextWithOutline(
                m_Text,                     // 출력할 텍스트
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // 출력 영역 (좌상단, 우하단)
                m_fontType,                          // 폰트 타입
                (float)m_iFontSize,                                  // 폰트 크기
                m_colorText,  // 텍스트 색상
                m_colorOutline,    // 외곽선 색상
                m_fOutlineThickness                                // 외곽선 두께
            );
        }
    }
    //숫자카운트 다운
    else if(m_mode == TextUIMode::COUNT_DOWN)
    {
        if (m_iNumber > -1 && m_iNumber <= 5) {   //5초 아래는 안쪽 텍스트가 붉은 색. 
            d2dManager->RenderTextWithOutline(
                std::to_wstring(m_iNumber),                     // 출력할 텍스트
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // 출력 영역 (좌상단, 우하단)
                m_fontType,                          // 폰트 타입
                (float)m_iFontSize,                                  // 폰트 크기
                m_RedColor,  // 텍스트 색상
                m_colorOutline,    // 외곽선 색상
                m_fOutlineThickness     // 외곽선 두께
            );
        }
        else if(m_iNumber > 5){//5초 이상은 흰색. 
            d2dManager->RenderTextWithOutline(
                std::to_wstring(m_iNumber),                     // 출력할 텍스트
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // 출력 영역 (좌상단, 우하단)
                m_fontType,                          // 폰트 타입
                (float)m_iFontSize,                                  // 폰트 크기
                m_colorText,  // 텍스트 색상
                m_colorOutline,    // 외곽선 색상
                m_fOutlineThickness     // 외곽선 두께
            );
        }

        // 누적 시간 업데이트
        m_fAcc += fDT;

        // 1초마다 숫자 증가
        if (m_fAcc >= 1.f) {
            m_iNumber -= static_cast<int>(m_fAcc / 1.f); // 초 단위로 증가
            m_fAcc = fmod(m_fAcc, 1.f);               // 남은 시간 유지
        }
    }
    //...
}

void CTextUI::update()
{
    UpdateNumber();
}

