#pragma once
#include "CObject.h"

class CMobSpawner;
class CBirth_Monster :
    public CObject
{

private:
    float       m_fDeleteDuration;
    float       m_fDeleteElapsed;
    MON_TYPE    m_eMonType;
    Vec2        m_vSavePos;
protected:
    virtual void update();


    CLONE(CBirth_Monster)

public:
    void SetMonType(MON_TYPE _eType) {
        m_eMonType = _eType;
    }
    void SetVecPos(Vec2 _vPos) {
        m_vSavePos = _vPos;
    }

public:
    CBirth_Monster();
    virtual ~CBirth_Monster();

    friend class CMobSpawner;
};

