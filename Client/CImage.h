#pragma once

class CObject;
struct ID2D1Bitmap;

//�� ������Ʈ�� Direct2D�θ� �۵��մϴ�. 
class CImage
{

private:
	CObject*			m_pOwner;		//image�� �����ϰ� �ִ� ������Ʈ. 
	ID2D1Bitmap*		m_pBitmap;

public:
	CObject* GetObj() { return m_pOwner; }
	ID2D1Bitmap* GetBitmap() { return m_pBitmap; }
	void		 SetBitmap(ID2D1Bitmap* _bitmap) { m_pBitmap = _bitmap; }

public:
	void render(HDC _dc);
	void render(ID2D1HwndRenderTarget* _renderTarget);
public:
	CImage();
	~CImage();

public:
	friend class CObject;
};

