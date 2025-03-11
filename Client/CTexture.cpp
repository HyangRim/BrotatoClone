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


//사용할 리소스는 Release버전이 에서 사용할 수 있을 것. 
//리소스도 bin파일에 그대로 있어야함. 

//디버그 버전에 실행할 때는 리소스 없는데? 
//bin -> content안에 있는 거 쓰면 되는데?
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
