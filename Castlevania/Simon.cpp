#include "Simon.h"
#include <algorithm>
#include"Game.h"
#include"Weapon.h"
#include"ItemHeart.h"
#include"ItemKnife.h"
#include"ItemWhip.h"
using namespace std;

void Simon::Update(DWORD dt,vector<LPGAMEOBJECT> *coObjects)
{
	
	GameObject::Update(dt);
	vy += SIMON_GRAVITY*dt;
	this->weapon.SetPosition(this->Getx(),this->Gety());

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<ItemHeart *>(e->obj))
			{
				ItemHeart *itemheart = dynamic_cast<ItemHeart *>(e->obj);
				if (itemheart->GetState() != ITEMHEART_STATE_DIE)
				{
					itemheart->SetState(ITEMHEART_STATE_DIE);
					this->heart += 1;
				}			
				itemheart->SetDie(true);

			}
			else if (dynamic_cast<ItemWhip *>(e->obj))
			{
				ItemWhip *itemwhip = dynamic_cast<ItemWhip *>(e->obj);
				if (itemwhip->GetState() != ITEMWHIP_STATE_DIE)
				{
					itemwhip->SetState(ITEMWHIP_STATE_DIE);
					this->weaponLevel += 1;
					this->weapon.SetLevel(this->weaponLevel);
					this->state = SIMON_STATE_LEVEL_WEAPON;
				}
				itemwhip->SetDie(true);
			}
			else if (dynamic_cast<ItemKnife *>(e->obj))
			{
				ItemKnife *itemknife = dynamic_cast<ItemKnife *>(e->obj);
				if (itemknife->GetState() != ITEMKNIFE_STATE_DIE)
				{
					itemknife->SetState(ITEMKNIFE_STATE_DIE);
					this->knife += 1;
				}
				
				itemknife->SetDie(true);
			}
			else if (dynamic_cast<Torch *>(e->obj))
			{
				x += dx;
			}
			else
			{
				if (nx != 0)
				{
					vx = 0;
				}
				if (ny != 0)
				{
					vy = 0;
				}
			}
		}	
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
	
	
}

void Simon::Render()
{
	int ani;
	if (state == SIMON_STATE_FIGHT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_FIGHT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_FIGHT_LEFT;
		}
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if(nx > 0)
		{
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_JUMP_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SIT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_SIT_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT_FIGHT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SIT_FIGHT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_SIT_FIGHT_LEFT;
		}
	}
	else if(state==SIMON_STATE_LEVEL_WEAPON)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_LEVEL_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_LEVEL_LEFT;
		}
	}
	else if (vx == 0)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_IDLE_LEFT;
		}
	}
	else if (vx > 0)
	{
		ani = SIMON_ANI_WALKING_RIGHT;
	}
	
	
	else
	{
		ani = SIMON_ANI_WALKING_LEFT;
	}
	animations[ani]->Render(this->Getx(), this->Gety());
	isCompleteAnimation = animations[ani]->GetOver();
	this->weapon.GetBox(isCompleteAnimation);
	this->weapon.Render(this->Getnx());
	
	
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		isStand = true;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	case SIMON_STATE_WALKING_LEFT:
		isStand = true;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	case SIMON_STATE_JUMP:		
		isCompleteAnimation = false;
			vy = -SIMON_JUMP_SPEED_Y;
			this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	case SIMON_STATE_FIGHT:
		isCompleteAnimation = false;
		vx = 0;
		this->weapon.SetState(WEAPON_STATE_FIGHT);
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	case SIMON_STATE_SIT_FIGHT:
		vx = 0;
		//->weapon.SetState(WEAPON_STATE_IDLE);
		break;
		isCompleteAnimation = false;
	case SIMON_STATE_LEVEL_WEAPON:
		vx = vy = 0;
		this->weapon.SetState(WEAPON_STATE_IDLE);
		break;
	}
	//this->weapon.SetState(state);
}

void Simon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
    top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
	float l, t, r, b;
	this->weapon.GetBoundingBox(l,t,r,b);
}

bool Simon::IsStand()
{
	return vy == 0;
}

bool Simon::IsHitting()
{
	return this->isCompleteAnimation;
}


