
#pragma once
#include "GameObject.h"

class CStartStair : public CGameObject
{
public:
	CStartStair(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};