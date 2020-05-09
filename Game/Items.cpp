#include "Items.h"

CItems* CItems::__instance = NULL;

void CItems::AddItem(int itemType, LPGAMEOBJECT item)
{
	items[itemType].push_back(item);
}

void CItems::CheckAndDrop(LPGAMEOBJECT obj)
{
	int itemTypeValue = obj->GetItemId();
	float x, y;
	obj->GetPosition(x, y);
	Drop(itemTypeValue, x,y);
}

void CItems::Drop(int iType, float x, float y)
{
	for (auto it = items[iType].begin(); it != items[iType].end(); ++it)
	{
		if ((*it)->isVisible() == false)
		{
			(*it)->SetPosition(x, y);
			(*it)->SetVisible(true);
			break;
		}
	}
}

CItems* CItems::GetInstance()
{
	if (__instance == NULL)
		__instance = new CItems();

	return __instance;
}