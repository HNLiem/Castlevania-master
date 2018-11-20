#pragma once
#include "GameObject.h"
#include"Torch.h"
#define WEAPON_STATE_FIGHT 0
#define WEAPON_STATE_IDLE 1

#define WEAPON_ANI_FIGHT_LEVEL_A_RIGHT   14
#define WEAPON_ANI_FIGHT_LEVEL_A_LEFT	 15
#define WEAPON_ANI_FIGHT_LEVEL_B_RIGHT   16
#define WEAPON_ANI_FIGHT_LEVEL_B_LEFT	 17
#define WEAPON_ANI_FIGHT_LEVEL_C_RIGHT   18
#define WEAPON_ANI_FIGHT_LEVEL_C_LEFT	 19

#define WEAPON_BBOX_WIDTH				45
#define WEAPON_C_BBOX_WIDTH				85
#define WEAPON_BBOX_HEIGHT				16
#define WEAPON_UNTOUCHABLE_TIME 5000

 class Weapon:public GameObject
{
private:
	int level=1;
	int untouchable;
	DWORD untouchable_start;
	float xsimon, ysimon;
	int ani;
	bool box=false;
public:
	Weapon() :GameObject()
	{
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> coObjects);
	virtual void Render(float nx);
	void SetState(int state);
	void SetLevel(int l) { this->level = l; }
	int Getlevel() { return this->level; }
	void SetPosition(float x, float y);
	void GetBox(bool a) { this->box = a; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

