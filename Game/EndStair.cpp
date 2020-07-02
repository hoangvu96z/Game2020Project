#include "EndStair.h"

CEndStair::CEndStair(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}
void CEndStair::Render()
{
	RenderBoundingBox();
}

void CEndStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}