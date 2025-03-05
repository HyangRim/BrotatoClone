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
        // ���� �ð� ������Ʈ
        m_fAcc += fDT;

        // 1�ʸ��� ���� ����
        if (m_fAcc >= 1.f) {
            m_iNumber -= static_cast<int>(m_fAcc / 1.f); // �� ������ ����
            m_fAcc = fmod(m_fAcc, 1.f);               // ���� �ð� ����
        }
    }
}

void CTextUI::render(ID2D1HwndRenderTarget* _pRender)
{
    auto d2dManager = Direct2DMgr::GetInstance();

    Vec2 vPos = m_pOwner->GetFinalPos(); //���� GetPos()����

    if (m_bCamAffected) {
        vPos = CCamera::GetInstance()->GetRenderPos(vPos);
    }
    //vPos = CCamera::GetInstance()->GetRenderPos(vPos);
    //���ڸ� ���
    if (m_mode == TextUIMode::TEXT)
    {

        if (m_iHorizontal == 0) {
            d2dManager->RenderTextWithOutline(
                m_Text,                     // ����� �ؽ�Ʈ
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // ��� ���� (�»��, ���ϴ�)
                m_fontType,                          // ��Ʈ Ÿ��
                (float)m_iFontSize,                                  // ��Ʈ ũ��
                m_colorText,  // �ؽ�Ʈ ����
                m_colorOutline,    // �ܰ��� ����
                m_fOutlineThickness                                // �ܰ��� �β�
            );
        }
        else if (m_iHorizontal == 1) {
            d2dManager->RenderLeftTextWithOutline(
                m_Text,                     // ����� �ؽ�Ʈ
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // ��� ���� (�»��, ���ϴ�)
                m_fontType,                          // ��Ʈ Ÿ��
                (float)m_iFontSize,                                  // ��Ʈ ũ��
                m_colorText,  // �ؽ�Ʈ ����
                m_colorOutline,    // �ܰ��� ����
                m_fOutlineThickness                                // �ܰ��� �β�
            );
        }
        else if (m_iHorizontal == 2) {
            d2dManager->RenderRightTextWithOutline(
                m_Text,                     // ����� �ؽ�Ʈ
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // ��� ���� (�»��, ���ϴ�)
                m_fontType,                          // ��Ʈ Ÿ��
                (float)m_iFontSize,                                  // ��Ʈ ũ��
                m_colorText,  // �ؽ�Ʈ ����
                m_colorOutline,    // �ܰ��� ����
                m_fOutlineThickness                                // �ܰ��� �β�
            );
        }
    }
    //����ī��Ʈ �ٿ�
    else if(m_mode == TextUIMode::COUNT_DOWN)
    {
        if (m_iNumber > -1 && m_iNumber <= 5) {   //5�� �Ʒ��� ���� �ؽ�Ʈ�� ���� ��. 
            d2dManager->RenderTextWithOutline(
                std::to_wstring(m_iNumber),                     // ����� �ؽ�Ʈ
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // ��� ���� (�»��, ���ϴ�)
                m_fontType,                          // ��Ʈ Ÿ��
                (float)m_iFontSize,                                  // ��Ʈ ũ��
                m_RedColor,  // �ؽ�Ʈ ����
                m_colorOutline,    // �ܰ��� ����
                m_fOutlineThickness     // �ܰ��� �β�
            );
        }
        else if(m_iNumber > 5){//5�� �̻��� ���. 
            d2dManager->RenderTextWithOutline(
                std::to_wstring(m_iNumber),                     // ����� �ؽ�Ʈ
                D2D1::RectF(vPos.x + m_vOffsetLT.x, vPos.y + m_vOffsetLT.y, vPos.x + m_vOffsetRB.x, vPos.y + m_vOffsetRB.y), // ��� ���� (�»��, ���ϴ�)
                m_fontType,                          // ��Ʈ Ÿ��
                (float)m_iFontSize,                                  // ��Ʈ ũ��
                m_colorText,  // �ؽ�Ʈ ����
                m_colorOutline,    // �ܰ��� ����
                m_fOutlineThickness     // �ܰ��� �β�
            );
        }

        // ���� �ð� ������Ʈ
        m_fAcc += fDT;

        // 1�ʸ��� ���� ����
        if (m_fAcc >= 1.f) {
            m_iNumber -= static_cast<int>(m_fAcc / 1.f); // �� ������ ����
            m_fAcc = fmod(m_fAcc, 1.f);               // ���� �ð� ����
        }
    }
    //...
}

void CTextUI::update()
{
    UpdateNumber();
}

