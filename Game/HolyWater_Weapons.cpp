#include "HolyWater_Weapons.h"

HolyWater_Weapons* HolyWater_Weapons::__instance = NULL;

HolyWater_Weapons::HolyWater_Weapons()
{
	SetVisible(false);
	this->damage = 1;
}

HolyWater_Weapons* HolyWater_Weapons::GetInstance()
{
	if (__instance == NULL)__instance = new HolyWater_Weapons();
	return __instance;
}

void HolyWater_Weapons::ShowHitEffect()
{
	if (hitEffects.size() > 0)
	{
		if (startShow == 0)
		{
			startShow = GetTickCount();
		}
		else if (GetTickCount() - startShow > 100)
		{
			startShow = 0;
			hitEffects.clear();
		}
		// rendering hit effect based on the coordinate vector
		for (auto coord : hitEffects)
			hitEffect->Render(coord[0], coord[1], -1);
	}
}

void HolyWater_Weapons::Render()
{
	ShowHitEffect();
	animation_set->at(state)->Render(x, y, nx);
}

void HolyWater_Weapons::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void HolyWater_Weapons::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}