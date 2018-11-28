#include <d3dx9.h>
#include "Game.h"
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include"Textures.h"
#include<d3dx9.h>
#include<algorithm>
#include "debug.h"
using namespace std;
vector<LPANIMATION> GameObject::animations;
void GameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = Animations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}
void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);
}
LPCOLLISIONEVENT GameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		
	float ml, mt, mr, mb;		
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	
	float svx, svy;
	// toc do cua doi tuong other
	coO->GetSpeed(svx, svy);
	// quang duong x y cua doi tuong other
	float sdx = svx * dt;
	float sdy = svy * dt;
	// van toc se bang van toc cua object tru cho van toc cua other (xet va cham chi xet giua mot vat dong mot vat tinh neu hai vatchuyeen dong thi xem nhu mot van dong mot vat tinh tu van toc)

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	// lay box cho doi tuong object
	GetBoundingBox(ml, mt, mr, mb);
	//t thoi gian de va cham 
	// ml,mt,mr,mb :left top right bottom cua object
// sl,st,sr,sb: left top right bottom cua other
// dx,dy,nx, ny cua object m
	// kiem tra va cham giua object va other
	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);
	// OutputDebugStringW(L"aabb \n %f", dx);
	

	//ket thuc ham tra ve thoi gian  va cham t va huong va cham
	//nx=-1.0f phai nx=1.0f trai
	// ny=-1.0f tren ny=1.0f duoi
	CollisionEvent * e = new CollisionEvent(t, nx, ny, coO);
	return e;
	// tra ve mot doi tuong collisionevent
}

bool GameObject::AABBx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;
	float ml, mt, mr, mb;
	coO->GetBoundingBox(sl, st, sr, sb);
	GetBoundingBox(ml, mt, mr, mb);
	if (Game::AABB(ml, mt, mr, mb, sl, st, sr, sb) == true)
	{
		return true;
	}

}

void GameObject::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	// cho  mot vong lap bao gom cac doi tuong 
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// dua object vao ham kiem tra va cham 
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		// thoi gian va cham 
		if (e->t > 0 && e->t <= 1.0f)
		{
			// them doi tuong vao vector LPCOLLISIONEVENT
			coEvents.push_back(e);
		}
		else
		{
			// khong co xoa e
			delete e;
		}
	}
	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float & min_tx, float & min_ty, float & nx, float & ny)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) 
		{
			min_tx = c->t; 
			nx = c->nx; 
			min_ix = i;
		}

		if (c->t < min_ty  && c->ny != 0)
		{
			min_ty = c->t; 
			ny = c->ny; 
			min_iy = i;
		}
	}

	if (min_ix >= 0)
	{
		coEventsResult.push_back(coEvents[min_ix]);
	}
	if (min_iy >= 0)
	{
		coEventsResult.push_back(coEvents[min_iy]);
	}
}

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}



void GameObject::Render()
{
}

GameObject::~GameObject()
{
}
