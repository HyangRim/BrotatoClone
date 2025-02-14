#pragma once

class CUI;
class CUIMgr
{
	SINGLE(CUIMgr)

private:
	CUI*		m_pFocusedUI;

public:
	void update();

	//해당 ui는 활성화 되면서 자신을 보라고 쏴줌. 
	void SetFocusedUI(CUI* _pUI);

private:
	CUI* GetFocusedUI();						//현재 포커싱 된 ui
	CUI* GetTargetedUI(CUI* _pParentUI);		//부모 UI내에서 실제로 타겟팅 된 ui를 찾아서 반환한다. 



};

