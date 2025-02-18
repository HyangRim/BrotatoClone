#include "pch.h"
#include "CTexture.h"
#include "CCore.h"



CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
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
	//파일로부터 로딩한 데이터를 비트맵으로 생성. 
	m_hBit = (HBITMAP)LoadImageW(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	assert(m_hBit);
	//비트맵과 연결할 DC
	m_dc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	//비트맵과 DC연결

	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);

	//비트맵 정보 알아오기. 
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);

}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	HDC maindc = CCore::GetInstance()->GetMainDC();

	m_hBit = CreateCompatibleBitmap(maindc, _iWidth, _iHeight);
	m_dc = CreateCompatibleDC(maindc);

	HBITMAP holdBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(holdBit);

	//비트맵 정보 알아오기. 
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
