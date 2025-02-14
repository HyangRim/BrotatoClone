#pragma once
#include "CObject.h"

class CTexture;
class CPlayer :
    public CObject
{

private:
    

public:
    virtual void update();
    virtual void render(HDC _dc);

private:
    void CreateMissile();

    //플레이어 오브젝트는 복사되면 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
