#pragma once
#include<Windows.h>
#include<d3dx9.h>
#include <unordered_map>
#include"AnimationFrame.h"
using namespace std;
class Animation
{
public:
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool box=false;
	bool isOverAnimation=false;
	Animation(int defaultTime) { this->defaultTime = defaultTime; this->lastFrameTime = -1; this->currentFrame = -1; };
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y);
	bool GetBoolBox() { return box;}
	bool GetOver() { return isOverAnimation;}
	~Animation();

};
typedef Animation *LPANIMATION;

