#include "Weapon.h"
#include"Game.h"
#include"GameObject.h"
#include"Torch.h"
#include"debug.h"
#include<algorithm>
#include"ItemHeart.h"
#include"ItemKnife.h"
#include"ItemWhip.h"
using namespace std;

void Weapon::Update(DWORD dt,vector<LPGAMEOBJECT> &coObjects)
{
	GameObject::Update(dt);
	if (box==true) 
	{
		DebugOut(L"\n true");
		for (UINT i = 0; i < coObjects.size(); i++)
		{
			LPGAMEOBJECT e = coObjects[i];
			if (dynamic_cast<Torch *>(e))
			{
				if (this->AABBx(e) == true)
				{
					if (e->GetState() != TORCH_STATE_DIE)
					{
						e->SetState(TORCH_STATE_DIE);
						e->SetFall(true);
						for (UINT j = i+1; j < coObjects.size(); j++)
						{						
							LPGAMEOBJECT a = coObjects[j];
							if (dynamic_cast<ItemHeart *>(a) || dynamic_cast<ItemWhip *>(a) || dynamic_cast<ItemKnife *>(a))
							{
								a->SetFall(e->GetFall());
								break;
							}
						}					
					}
					coObjects[i]->SetDie(true);
				}
			}
		}
	}
}

void Weapon::Render(float nx)
{
	if (state == WEAPON_STATE_FIGHT)
	{		
		if (level == 1)
		{
			if (nx > 0)
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_A_RIGHT;
			}
			else
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_A_LEFT;
			}
		}
		else if(level==2)
		{
			if (nx > 0)
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_B_RIGHT;
			}
			else
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_B_LEFT;
			}
		}
		else
		{
			if (nx > 0)
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_C_RIGHT;
			}
			else
			{
				ani = WEAPON_ANI_FIGHT_LEVEL_C_LEFT;
			}
		}
		animations[ani]->Render(x, y);
		
		RenderBoundingBox();	
	}	
}

void Weapon::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WEAPON_STATE_FIGHT:
		break;
	}
}

void Weapon::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Weapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{	
	if (state == WEAPON_STATE_FIGHT)
	{		
			if (ani == WEAPON_ANI_FIGHT_LEVEL_A_RIGHT )
			{		
				left = x + 35;
				top = y + 9;
				right = left + WEAPON_BBOX_WIDTH ;
				bottom = top+ WEAPON_BBOX_HEIGHT;
			}
			else if(ani == WEAPON_ANI_FIGHT_LEVEL_B_RIGHT)
			{
				left = x + 40;
				top = y + 13;
				right = left+ WEAPON_BBOX_WIDTH;
				bottom = top+ WEAPON_BBOX_HEIGHT;
			}
			else if (ani == WEAPON_ANI_FIGHT_LEVEL_C_RIGHT)
			{
				left = x + 45;
				top = y + 15;
				right = left + WEAPON_C_BBOX_WIDTH;
				bottom = top + WEAPON_BBOX_HEIGHT;
			}
			else if(ani == WEAPON_ANI_FIGHT_LEVEL_A_LEFT)
			{

				left = x - 40;
				top = y + 10;
				right = left+ WEAPON_BBOX_WIDTH;
				bottom = top + WEAPON_BBOX_HEIGHT;

			}
			else if (ani == WEAPON_ANI_FIGHT_LEVEL_B_LEFT)
			{
				left = x - 40;
				top = y + 12;
				right = left + WEAPON_BBOX_WIDTH;
				bottom = top + WEAPON_BBOX_HEIGHT;
			}
			else 
			{
				left = x - 80;
				top = y + 15;
				right = left + WEAPON_C_BBOX_WIDTH;
				bottom = top + WEAPON_BBOX_HEIGHT;
			}	
	}
}



