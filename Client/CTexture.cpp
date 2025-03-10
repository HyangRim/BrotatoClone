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

}


//����� ���ҽ��� Release������ ���� ����� �� ���� ��. 
//���ҽ��� bin���Ͽ� �״�� �־����. 

//����� ������ ������ ���� ���ҽ� ���µ�? 
//bin -> content�ȿ� �ִ� �� ���� �Ǵµ�?
void CTexture::Load(const wstring& _strFilePath)
{	
	m_pBitmap = new Gdiplus::Bitmap(_strFilePath.c_str());
	assert(nullptr != m_pBitmap);

	m_width = m_pBitmap->GetWidth();
	m_height = m_pBitmap->GetHeight();

	m_pGraphics = new Gdiplus::Graphics(m_pBitmap);
	assert(m_pGraphics != nullptr);
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{	
	m_pBitmap = new Gdiplus::Bitmap(_iWidth, _iHeight);
	assert(nullptr != m_pBitmap);

	m_width = m_pBitmap->GetWidth();
	m_height = m_pBitmap->GetHeight();

	m_pGraphics = new Gdiplus::Graphics(m_pBitmap);
	assert(m_pGraphics != nullptr);
}
