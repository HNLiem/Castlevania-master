#pragma once
#include"GameObject.h"
#define ITEMWHIP_STATE			100
#define ITEMWHIP_STATE_DIE		200
#define ITEMWHIP_ANI			84
#define ITEMWHIP_ANI_DIE		85 
#define ITEMWHIP_BBOX_HEIGHT	32
#define ITEMWHIP_BBOX_WIDTH		32
class ItemWhip:public GameObject
{
public:
	void Render();
	virtual void SetState(int state);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};

