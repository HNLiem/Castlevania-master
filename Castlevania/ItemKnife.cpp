#include "ItemKnife.h"



void ItemKnife::Render()
{
	if (fall)
	{
		int ani;
		if (state == ITEMKNIFE_STATE_DIE)
		{
			ani = ITEMKNIFE_ANI_DIE;
		}
		else
		{
			ani = ITEMKNIFE_ANI;
		}
		animations[ani]->Render(x, y);
	}
}

void ItemKnife::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	caseITEMKNIFE_STATE_DIE:
		state = ITEMKNIFE_ANI_DIE;
		break;
	case ITEMKNIFE_STATE:
		state = ITEMKNIFE_ANI;
		break;
	}
}

void ItemKnife::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (state == ITEMKNIFE_STATE_DIE)
	{
		l = t = r = b = 0;

	}
	else
	{
		l = x;
		t = y;
		r = x + ITEMKNIFE_BBOX_WIDTH;
		b = y + ITEMKNIFE_BBOX_HEIGHT;
	}
}

void ItemKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	GameObject::Update(dt, colliable_objects);
	if (fall)
	{
		y += 0.1 * dt;
		if (y > 320 - ITEMKNIFE_BBOX_HEIGHT)
		{
			vy = 0;
			y = 320 - ITEMKNIFE_BBOX_HEIGHT;
		}
	}
}
