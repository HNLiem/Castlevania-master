#include "WeaponKnife.h"
#include"Game.h"
#include"GameObject.h"
#include"Torch.h"
#include"debug.h"
#include<algorithm>
#include"ItemHeart.h"
#include"ItemKnife.h"
#include"ItemWhip.h"

void WeaponKnife::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->vx = WEAPON_KNIFE_SPEED;
	GameObject::Update(dt);		
	if (nx > 0)
	{
		x += vx * dt;
	}
	else
	{
		x -= vx * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (GetTickCount() - untouchable_start > WEAPON_KNIFE_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (coEvents.size() == 0)
	{
		x += dx;	
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Torch *>(e->obj))
			{
				Torch *torch = dynamic_cast<Torch *>(e->obj);
				if (torch->GetState() != TORCH_STATE_DIE)
				{
					torch->SetState(TORCH_STATE_DIE);
					torch->SetDie(true);
					DebugOut(L"\n va cham dao voi den");
				}			
			}
		}
	}
}

void WeaponKnife::Render(float nx)
{
	if (state == WEAPON_KNIFE_STATE_FIGHT)
	{
		if (nx > 0)
		{
			ani = WEAPON_KNIFE_ANI_FIGHT_RIGHT;
		}
		else
		{
			ani = WEAPON_KNIFE_ANI_FIGHT_LEFT;
		}
		animations[ani]->Render(x, y);
		RenderBoundingBox();
	}
}

void WeaponKnife::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WEAPON_KNIFE_STATE_FIGHT:
		break;
	}
}

void WeaponKnife::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void WeaponKnife::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (state == WEAPON_KNIFE_STATE_FIGHT)
	{	
			left = x ;
			top = y ;
			right = left + WEAPON_KNIFE_BBOX_WIDTH;
			bottom = top + WEAPON_KNIFE_BBOX_HEIGHT;
	}
}
