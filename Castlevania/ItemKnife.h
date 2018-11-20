#pragma once
#include"GameObject.h"
#define ITEMKNIFE_STATE				100
#define ITEMKNIFE_STATE_DIE			200
#define ITEMKNIFE_ANI				88
#define ITEMKNIFE_ANI_DIE			89
#define ITEMKNIFE_BBOX_HEIGHT        18
#define ITEMKNIFE_BBOX_WIDTH		32
class ItemKnife:public GameObject
{
public:
	void Render();
	virtual void SetState(int state);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};

