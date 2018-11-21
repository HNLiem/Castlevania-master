#include "ItemWhip.h"

void ItemWhip::Render()
{
	if (fall)
	{
		int ani;
		if (state == ITEMWHIP_STATE_DIE)
		{
			ani = ITEMWHIP_ANI_DIE;
		}
		else
		{
			ani = ITEMWHIP_ANI;
		}
		animations[ani]->Render(x, y);
	}
}

void ItemWhip::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case ITEMWHIP_STATE_DIE:
		state = ITEMWHIP_ANI_DIE;
		break;
	case ITEMWHIP_STATE:
		state = ITEMWHIP_ANI;
		break;
	}
}

void ItemWhip::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (state == ITEMWHIP_STATE_DIE)
	{
		l = t = r = b = 0;

	}
	else
	{
		l = x;
		t = y;
		r = x + ITEMWHIP_BBOX_WIDTH;
		b = y + ITEMWHIP_BBOX_HEIGHT;
	}
}

void ItemWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	GameObject::Update(dt, colliable_objects);
	if (fall)
	{
		y += 0.1 * dt;
		if (y > 320 - ITEMWHIP_BBOX_HEIGHT)
		{
			vy = 0;
			y = 320 - ITEMWHIP_BBOX_HEIGHT;
		}
	}
}
