#include "Animation.h"
#include"Game.h"
#include"debug.h"
#include"Sprites.h"
void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0)
	{
		t=this->defaultTime;
	}
	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new AnimationFrame(sprite, t);
	//frame->GetSprite()->SetPosition(x, y);
	frames.push_back(frame);
}

void Animation::Render(float x, float y)
{
	this->isOverAnimation = false;
	this->box = false;
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		// khung hien tai =0
		currentFrame = 0;
		//Thời gian khung cuối cùng
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size() - 1)
			{
				isOverAnimation = true;
				box = true;
				
			}
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
				

			}
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y);
	
}

Animation::~Animation()
{
}
