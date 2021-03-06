#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

using namespace std;

#define ID_TEX_BBOX		-100	
#define ITEM_GRAVITY	0.0018f

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	float dx, dy;
	
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

enum class ItemType
{
	UNKNOWN = -1,
	BIG_HEART = 4,
	CHAIN = 5,
	DAGGER = 6,
	SMALL_HEART = 44,
	BOOMERANG = 61,
	MONEY_BAG = 10,
	HOLY_WATER = 82
};

class CGameObject
{
public:	
	float x;  
	float y;

	float dx;	// vx*dt
	float dy;	// vy*dt

	float vx;
	float vy;

	int width;
	int height;

	int nx;	 // right =1, lefft =-1
	int state;
	bool visible;
	int itemId;

	int score = 0;
	int healthPoint = 0;
	int damage = 0;

	DWORD start_untouchable = 0;
	DWORD dt; 

	CAnimationSet animations;
	LPANIMATION_SET animation_set;

public: 
	// Position
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	// Size
	void SetWidth(int w) { this->width = w; }
	int GetWidth() { return this->width; }
	void SetHeight(int h) { this->height = h; }
	int GetHeight() { return this->height; }
	// Speed
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }	
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	// Orientation
	void SetOrientation(int nx) { this->nx = nx; }
	int GetOrientation() { return nx; }	
	void ReDirection() { this->nx= -nx; this->vx = -vx; }
	// Visibility
	void SetVisible(bool visible) { this->visible = visible; }
	bool isVisible() { return this->visible; }	
	// Item
	int GetItemId() { return itemId; }
	void SetItemId(int id) { this->itemId = id; }
	// Check collision between 2 static object
	bool AABB(float left_a, float top_a, float right_a, float bottom_a,
		float left_b, float top_b, float right_b, float bottom_b);	
	
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
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void ResetAnimation();
	virtual void SetState(int state) { this->state = state; }
	virtual int GetState() { return this->state; }
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	CGameObject();
	~CGameObject();
	virtual bool IsOverlapping(LPGAMEOBJECT obj);
	void ReDirect(float positionX);
	void RenderBoundingBox();
	void Untouchable();
	void TakeDamage(int damage);
	void Die();
	bool IsInViewport();
};