#pragma once
#include "CObject.h"

class CDamageUI :
    public CObject
{
private:

    //흔들리는 것. 
    Vec2  m_vPivotPos;
    float m_fWaveLength;
    float m_fWaveDuration;
    float m_fWaveElapsed;

    //자동 삭제 관련.
    float m_fDestroyDuration;
    float m_fDestoryElapsed;

private:
    virtual void start();
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void SetDuration(float _fDuration) { m_fDestroyDuration = _fDuration; }
    void SetPivotPos() {
        m_vPivotPos = GetPos();
    }
public:
    CLONE(CDamageUI)
public:
    CDamageUI();
    virtual ~CDamageUI();
};

