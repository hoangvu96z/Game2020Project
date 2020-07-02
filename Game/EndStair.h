#pragma once
#include "GameObject.h"

class CEndStair : public CGameObject
{
public:
	CEndStair(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};