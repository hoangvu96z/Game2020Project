#include "Skeleton_Enemies.h"

void Skeleton_Enemies::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void Skeleton_Enemies::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

}

void Skeleton_Enemies::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}

Skeleton_Enemies::Skeleton_Enemies(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	this->healthPoint = 2;
	SetState(SKELETON_STATE_DANCING);
}

void Skeleton_Enemies::SetState(int state)
{
	CGameObject::SetState(state);
}