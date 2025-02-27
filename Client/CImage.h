#pragma once

class CObject;
struct ID2D1Bitmap;

//�� ������Ʈ�� Direct2D�θ� �۵��մϴ�. 
class CImage
{

private:
	CObject*			m_pOwner;		//image�� �����ϰ� �ִ� ������Ʈ. 
	ID2D1Bitmap*		m_pBitmap;

	Vec2				m_vOffSet;
	Vec2				m_vFinalPos;

	float				m_fRatio;

public:
	CObject* GetObj() { return m_pOwner; }
	ID2D1Bitmap* GetBitmap() { return m_pBitmap; }
	void		 SetBitmap(ID2D1Bitmap* _bitmap) { m_pBitmap = _bitmap; }

	void SetRatio(float _ratio) { m_fRatio = _ratio; }
	void SetOffset(Vec2 _vOffset) { m_vOffSet = _vOffset; }

public:
	void finalupdate();
	void render(HDC _dc);
	void render(ID2D1HwndRenderTarget* _renderTarget);
public:
	CImage();
	~CImage();

public:
	friend class CObject;
};

