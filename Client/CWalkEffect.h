#pragma once
#include "CObject.h"
class CWalkEffect :
    public CObject
{

private:
    int             m_iParticleImageIndex;
    float           m_fParticleChangeCool;


    float           m_fEffectLifeDuration;
    float           m_fEffectLifeElapsed;

    const Vec2      m_vMaxScale;
    const Vec2      m_vMinScale;
public:
    virtual void update();


public:
    CWalkEffect();
    ~CWalkEffect();

    CLONE(CWalkEffect)
};

