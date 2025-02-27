#pragma once

class CObject;
struct ID2D1Bitmap;

//이 오브젝트는 Direct2D로만 작동합니다. 
class CImage
{

private:
	CObject*			m_pOwner;		//image를 소유하고 있는 오브젝트. 
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

