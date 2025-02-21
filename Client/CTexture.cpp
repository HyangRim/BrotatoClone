#include "pch.h"
#include "CTexture.h"
#include "CCore.h"


CTexture::CTexture()
	: m_hBit(0)
	, m_dc(nullptr)
	, m_bitInfo{}
	, m_pBitmap(nullptr)
	, m_pGraphics(nullptr)
	, m_width(0)
	, m_height(0)
{

}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);

	//if (nullptr != m_pBitmap) delete m_pBitmap;
	//if (nullptr != m_pGraphics) delete m_pGraphics;

}


//����� ���ҽ��� Release������ ���� ����� �� ���� ��. 
//���ҽ��� bin���Ͽ� �״�� �־����. 

//����� ������ ������ ���� ���ҽ� ���µ�? 
//bin -> content�ȿ� �ִ� �� ���� �Ǵµ�?
void CTexture::Load(const wstring& _strFilePath)
{
	//Image* pImage = Image::FromFile(_strFilePath.c_str());
	//assert(pImage && pImage->GetLastStatus() == Ok);

	
	
	//���Ϸκ��� �ε��� �����͸� ��Ʈ������ ����. 
	/*
	m_hBit = (HBITMAP)LoadImageW(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	assert(m_hBit);
	//��Ʈ�ʰ� ������ DC
	m_dc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	//��Ʈ�ʰ� DC����

	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);

	//��Ʈ�� ���� �˾ƿ���. 
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
	*/
	
	
	m_pBitmap = new Gdiplus::Bitmap(_strFilePath.c_str());
	assert(nullptr != m_pBitmap);

	m_width = m_pBitmap->GetWidth();
	m_height = m_pBitmap->GetHeight();

	m_pGraphics = new Gdiplus::Graphics(m_pBitmap);
	assert(m_pGraphics != nullptr);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	/*
	HDC maindc = CCore::GetInstance()->GetMainDC();
	m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight);
	m_dc = CreateCompatibleDC(maindc);

	HBITMAP holdBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(holdBit);

	//��Ʈ�� ���� �˾ƿ���. 
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);*/
	
	
	m_pBitmap = new Gdiplus::Bitmap(_iWidth, _iHeight);
	assert(nullptr != m_pBitmap);

	m_width = m_pBitmap->GetWidth();
	m_height = m_pBitmap->GetHeight();

	m_pGraphics = new Gdiplus::Graphics(m_pBitmap);
	assert(m_pGraphics != nullptr);
}
