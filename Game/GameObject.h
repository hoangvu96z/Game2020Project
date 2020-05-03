#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:	
	float x;  
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 // riight =1, lefft =-1

	int state;

	bool visible;
	int itemId;
	DWORD dt; 

	CAnimationSet animations;
	LPANIMATION_SET animation_set;

public: 
	// Position
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	D3DXVECTOR2 GetPosition() { return D3DXVECTOR2(x, y); }
	// Check collision between 2 static object
	bool AABB(float left_a, float top_a, float right_a, float bottom_a,
		float left_b, float top_b, float right_b, float bottom_b);	
		// Speed
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }	
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	// Orientation
	void SetOrientation(int nx) { this->nx = nx; }
	int GetOrientation() { return nx; }	
	// Item
	void SetItemId(int id) { this->itemId = id; }
	void SetVisible(bool visible) { this->visible = visible; }
	bool isVisible() { return this->visible; }	
	// Collision
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void ResetAnimation();
	virtual void SetState(int state) { this->state = state; }
	virtual int GetState() { return this->state; }
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	CGameObject();
	~CGameObject();
	void RenderBoundingBox();
};