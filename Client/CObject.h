#pragma once
#include "global.h"

//오브젝트라는 건, 가장 부모격인 녀석. 오브젝트마다 성향이 있음 
//어떤 건 UI, 어떤 건 캐릭터... 

//오브젝트를 일괄적으로 관리할 녀석이 필요함. -> Scene

//이 오브젝트는 부모 클래스. -> 상속받은 오브젝트도 Scene의 업데이트 시점에
//부모쪽에 구현되어있는 update뿐만 아니라, 각 오브젝트(자식)들 마다의 update를 하고 싶음


//충돌할 오브젝트, 충돌하지 않을 오브젝트(UI, 배경등등...) <- 이런 식으로 계속 트리처럼 나누는 건 예외가 있을수도 있음. 
// 따라서 부품기반의 구조가 필요함(컴포넌트 구조)


#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidbody;
class CGravity;
class CImage;
class CTextUI;

class CObject
{
private:
	GROUP_TYPE		m_ObjType;

	wstring			m_ObjName;
	Vec2			m_vPos;
	Vec2			m_vPrevPos;
	Vec2			m_vScale;
	Vec2			m_vRenderScale;

	//Component
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;
	CRigidbody*		m_pRigidBody;
	CGravity*		m_pGravity;
	vector<CImage*>	m_pImages;
	CTextUI*		m_pTextUI;

	bool			m_bAlive;			//자기 자신이 활성화 or 비활성화. (삭제 전용)
	bool			m_bEnable;			//일시적인 활성화 or 비활성화. 
	bool			m_bFlipX;


	//Render Wave용 Float
	float			m_fWaveDuration;
	float			m_fWaveElapsed;

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { 
		m_vScale = _vScale; 
		m_vRenderScale = _vScale;
	}

	GROUP_TYPE GetObjType() { return m_ObjType; }
	Vec2 GetPos() { return m_vPos; }

	virtual Vec2 GetFinalPos() { return m_vPos; }

	Vec2 GetScale() { return m_vScale; }
	Vec2 GetRenderScale() { return m_vRenderScale; }

	void SetObjType(GROUP_TYPE _eType) { m_ObjType = _eType; }
	void SetName(const wstring& _strName) { m_ObjName = _strName; }
	void SetWaveDuration(float _fWaveTime) { m_fWaveDuration = _fWaveTime; }
	void SetRenderScale(Vec2 _vScale) { m_vRenderScale = _vScale; }

	void SetFlipX(bool _bFlipX) { m_bFlipX = _bFlipX; }
	bool GetFlipX() { return m_bFlipX; }

	const wstring& GetName() { return m_ObjName; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidbody* GetRigidbody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }

	vector<CImage*>& GetImages() { return m_pImages; }
	size_t GetImageCount() { return m_pImages.size(); }
	CImage* GetImage(int _Idx);
	void DeleteImage() { Safe_Delete_Vec(m_pImages); }
	void SetImage(CImage* _image, int _idx) { m_pImages[_idx] = _image; }
	//void AddImage(const wstring& tag);
	//void AddImage(const wstring& tag, ID2D1Bitmap* _bitmap);
	void AddImage(ID2D1Bitmap* _bitmap);

	CTextUI* GetTextUI() { return m_pTextUI; }

	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();
	void CreateGravity();
	void CreateImage();
	void CreateTextUI(const wstring& _text, Vec2 _offsetLT, Vec2 _offsetRB		//text , 좌상단 offset, 우하단 offset
						, int _fontSize, D2D1::ColorF _colorText				//글자크기, 글자색상(ColoF)
						, bool _bDrawOutline									//글자외곽선 여부(false라면 다 0으로 작성할것)
						, float _fOutlineThickness, D2D1::ColorF _colorOutline	//글자외곽선 두께, 글자외곽선 색상
						, FONT_TYPE _eType										//폰트 타입(DEFAULT, KR)
						, TextUIMode _eMode, int _iStartNum);					//(TEXT,NUMBER)모드 , NUMBER모드일 경우 감소시작할 숫자(타이머)

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	bool IsDead() {
		return !m_bAlive;
	}

	bool IsMove() {
		if (m_vPos.x != m_vPrevPos.x || m_vPos.y != m_vPrevPos.y) return true;
		return false;
	}

	void ShakeScale();

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void start() {};
	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC _dc);
	virtual void render(Gdiplus::Graphics* _pDGraphics);
	virtual void render(ID2D1HwndRenderTarget* _pRender);

	void component_render(HDC _dc);
	void component_render(Gdiplus::Graphics* _pDGraphics);
	void component_render(ID2D1HwndRenderTarget* _pRender);

	virtual CObject* Clone() = 0;
public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	//복사 생성자의 문제. -> 단순히 복사만 하면 m_pCollider의 주소가 그대로 복사됨.
	//따라서 m_pCollider을 따로 만들어줘야 함. 
	//m_bAlive도 가져올 필요는 없음. 

	friend class CEventMgr;
};