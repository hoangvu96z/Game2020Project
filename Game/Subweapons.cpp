#include "SubWeapons.h"
#include "Simon.h"

CSubWeapons* CSubWeapons::__instance = NULL;

void CSubWeapons::Add(int weapon, LPGAMEOBJECT obj)
{
	subweapons[weapon].push_back(obj);
}


void CSubWeapons::Select(int weapon)
{
	switch (weapon)
	{
	case (int)SubWeapon::DAGGER:
		UseDagger();
		break;

	case (int)SubWeapon::BOOMERANG:
		UseBoomerang();
		break;

	case (int)SubWeapon::HOLYWATER:
		UseHolyWater();
		break;

	default:
		break;
	}
}

void CSubWeapons::UseDagger()
{
}

void CSubWeapons::UseBoomerang()
{
}

void CSubWeapons::UseHolyWater()
{
	LPGAMEOBJECT holywater = GetWeapon((int)SubWeapon::HOLYWATER);
	if (holywater == NULL) return;
	holywater->SetVisible(true);
	holywater->nx = CSimon::GetInstance()->GetOrientation();
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	float xW = xS;
	float yW= yS + 5;
	holywater->SetPosition(xW, yW);
}

LPGAMEOBJECT CSubWeapons::GetWeapon(int weapon)
{
	if (subweapons[weapon].empty()) {

	}
	else
	{
		for (auto i = subweapons[weapon].begin(); i != subweapons[weapon].end(); ++i)
		{
			if ((*i)->isVisible() == false)
			{
				return (*i);
				break;
			}
		}
	}
}

CSubWeapons* CSubWeapons::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSubWeapons();

	return __instance;
} 