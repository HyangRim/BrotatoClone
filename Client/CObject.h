#pragma once
#include "global.h"

//������Ʈ��� ��, ���� �θ���� �༮. ������Ʈ���� ������ ���� 
//� �� UI, � �� ĳ����... 

//������Ʈ�� �ϰ������� ������ �༮�� �ʿ���. -> Scene

//�� ������Ʈ�� �θ� Ŭ����. -> ��ӹ��� ������Ʈ�� Scene�� ������Ʈ ������
//�θ��ʿ� �����Ǿ��ִ� update�Ӹ� �ƴ϶�, �� ������Ʈ(�ڽ�)�� ������ update�� �ϰ� ����


//�浹�� ������Ʈ, �浹���� ���� ������Ʈ(UI, �����...) <- �̷� ������ ��� Ʈ��ó�� ������ �� ���ܰ� �������� ����. 
// ���� ��ǰ����� ������ �ʿ���(������Ʈ ����)


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

	bool			m_bAlive;			//�ڱ� �ڽ��� Ȱ��ȭ or ��Ȱ��ȭ. (���� ����)
	bool			m_bEnable;			//�Ͻ����� Ȱ��ȭ or ��Ȱ��ȭ. 
	bool			m_bFlipX;


	//Render Wave�� Float
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
	void CreateTextUI(const wstring& _text, Vec2 _offsetLT, Vec2 _offsetRB		//text , �»�� offset, ���ϴ� offset
						, int _fontSize, D2D1::ColorF _colorText				//����ũ��, ���ڻ���(ColoF)
						, bool _bDrawOutline									//���ڿܰ��� ����(false��� �� 0���� �ۼ��Ұ�)
						, float _fOutlineThickness, D2D1::ColorF _colorOutline	//���ڿܰ��� �β�, ���ڿܰ��� ����
						, FONT_TYPE _eType										//��Ʈ Ÿ��(DEFAULT, KR)
						, TextUIMode _eMode, int _iStartNum);					//(TEXT,NUMBER)��� , NUMBER����� ��� ���ҽ����� ����(Ÿ�̸�)

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

	//���� �������� ����. -> �ܼ��� ���縸 �ϸ� m_pCollider�� �ּҰ� �״�� �����.
	//���� m_pCollider�� ���� �������� ��. 
	//m_bAlive�� ������ �ʿ�� ����. 

	friend class CEventMgr;
};