#include "pch.h"
#include "CPanelUI.h"
#include "CkeyMgr.h"



CPanelUI::CPanelUI()
	: CUI(false)
{

}

CPanelUI::~CPanelUI() {

}

void CPanelUI::update()
{
}

void CPanelUI::MouseOn()
{
	if (IsLbtnDown()) {
		Vec2 vDiff = MOUSE_POS - m_vDragStart;


		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	//패널 UI는 드래그해서 이동 가능하게 해야함. 
	//누른 순간에 기억을 해서 그걸 좌표를 기억하고, 움직인 좌표를 계산하여 똑같은 거리만큼 이동시키면 되는 것. 
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}