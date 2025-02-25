#pragma once
#include "CObject.h"

class CMobSpawner;
class CBirth_Monster :
    public CObject
{

private:
    float m_fDeleteDuration;
    float m_fDeleteElapsed;
protected:
    virtual void update();


    CLONE(CBirth_Monster)


public:
    CBirth_Monster();
    virtual ~CBirth_Monster();

    friend class CMobSpawner;
};

