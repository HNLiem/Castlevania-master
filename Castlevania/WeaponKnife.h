#pragma once
#include"GameObject.h"
#define WEAPON_KNIFE_STATE_FIGHT				0	
#define WEAPON_KNIFE_STATE_DIE					1
#define	WEAPON_KNIFE_ANI_FIGHT_RIGHT			92
#define	WEAPON_KNIFE_ANI_FIGHT_LEFT				93
#define WEAPON_KNIFE_ANI_DIE					94
#define WEAPON_KNIFE_BBOX_HEIGHT				18
#define WEAPON_KNIFE_BBOX_WIDTH					32
#define WEAPON_KNIFE_UNTOUCHABLE_TIME			5000
#define WEAPON_KNIFE_SPEED						0.5f
class WeaponKnife:public GameObject
{
private:
	int untouchable;
	DWORD untouchable_start;
	float xsimon, ysimon;
	int ani;
public:
	WeaponKnife() :GameObject() { untouchable = 0; }
	~WeaponKnife();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render(float nx);
	void SetState(int state);
	void SetPosition(float x, float y);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

