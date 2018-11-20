#include "ItemHeart.h"
#include"GameObject.h"
using namespace std;
void ItemHeart::Render()
{
	int ani;
	if (state == ITEMHEART_STATE_DIE)
	{
		ani = ITEMHGEART_ANI_DIE;
	}
	else
	{
		ani = ITEMHEART_ANI;
	}
	animations[ani]->Render(x, y);
}

void ItemHeart::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case ITEMHEART_STATE_DIE:
		state = ITEMHGEART_ANI_DIE;
		break;
	case ITEMHEART_STATE:
		state = ITEMHEART_ANI;
		break;
	}
}

void ItemHeart::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (state == ITEMHEART_STATE_DIE)
	{
		l = t = r = b = 0;
		
	}
	else
	{	
		l = x;
		t = y;
		r = x + ITEMHGEART_BBOX_WIDTH;
		b = y + ITEMHEART_BBOX_HEIHT;
	}
}

void ItemHeart::Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects )
{
	GameObject::Update(dt, colliable_objects);
}
