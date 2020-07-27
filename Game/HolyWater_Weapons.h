#pragma once
#include "GameObject.h"

class HolyWater_Weapons : public CGameObject
{
	static HolyWater_Weapons* __instance;
	vector<vector<float>> hitEffects; //a vector for saving the hit effects coordinates when the whip hits the target
	int startShow = 0;
	CAnimation* hitEffect = CAnimations::GetInstance()->Get(HIT_EFFECT_ANIMATION);
public:
	HolyWater_Weapons();
	static HolyWater_Weapons* GetInstance();
	void ShowHitEffect();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
}; 