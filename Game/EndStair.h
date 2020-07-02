#pragma once
#include "GameObject.h"

class CEndStair : public CGameObject
{
	int width;
	int height;

public:
	CEndStair(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};