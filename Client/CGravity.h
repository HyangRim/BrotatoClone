#pragma once

class CObject;
class CGravity
{

private:
	CObject*	m_pOwner;		//collider를 소유하고 있는 오브젝트. 

	bool		m_bGround;

public:
	void SetGround(bool _b);


public:
	void finalupdate();
public:
	CGravity();
	~CGravity();

public:
	friend class CObject;
};

