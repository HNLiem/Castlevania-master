#pragma once
#include "GameObject.h"
#include"Weapon.h"
#include"WeaponKnife.h"

#define SIMON_WALKING_SPEED		0.2f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_GRAVITY			0.002f
#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_FIGHT			400
#define SIMON_STATE_JUMP_FIGHT		500
#define SIMON_STATE_SIT				600
#define SIMON_STATE_SIT_FIGHT		700
#define SIMON_STATE_LEVEL_WEAPON	800
#define SIMON_STATE_KNIFE			900
#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_FIGHT_RIGHT		4
#define SIMON_ANI_FIGHT_LEFT		5
#define SIMON_ANI_JUMP_RIGHT		6
#define SIMON_ANI_JUMP_LEFT			7
#define SIMON_ANI_SIT_RIGHT			8
#define SIMON_ANI_SIT_LEFT			9
#define SIMON_ANI_SIT_FIGHT_RIGHT	10
#define SIMON_ANI_SIT_FIGHT_LEFT	11
#define SIMON_ANI_LEVEL_RIGHT		12
#define SIMON_ANI_LEVEL_LEFT		13
#define SIMON_ANI_KNIFE_RIGHT		14
#define SIMON_ANI_KNIFE_LEFT		15
#define SIMON_BBOX_WIDTH			30
#define SIMON_BBOX_HEIGHT			60
#define SIMON_UNTOUCHABLE_TIME		5000
using namespace std;
class Simon:public GameObject
{
	int untouchable;
	DWORD untouchable_start;
	int weaponLevel=1;
	int knife=0;
	int heart=0;
	
public:
	Weapon weapon;
	WeaponKnife weaponKnife;
	virtual void Update(DWORD dt,vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isStand = true;
	bool IsStand();
	bool IsHitting();
	bool isCompleteAnimation = false;
	int GetweaponLevel() { return this->weaponLevel; }
};

