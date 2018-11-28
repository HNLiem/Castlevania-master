
/*

Lỗi cần fix:
    1 nhận phím đánh - xong ->loi moi va cham voi cai roi o frame thu ba
	2 load lại vị trí của item
	3 Hết
*/
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"Game.h"
#include"GameObject.h"
#include"Textures.h"
#include"Simon.h"
#include"Animation.h"
#include"Animations.h"
#include"Sprite.h"
#include"Sprites.h"
#include"Weapon.h"
#include"Brick.h"
#include"Torch.h"
#include"debug.h"
#include"TileMap.h"
#include"ItemHeart.h"
#include"ItemWhip.h"
#include"ItemKnife.h"
#include"WeaponKnife.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"
#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define MAX_FRAME_RATE			90
#define ID_TEX_SIMON			0
#define ID_TEX_SIMON1			1
#define ID_TEX_SIMON2			2
#define ID_TEX_WEAPON			3
#define ID_TEX_WEAPON1			4
#define ID_TEX_TORCH			5
#define ID_TEX_MAP				6
#define ID_TEX_HEART			7
#define ID_TEX_ITEM_WEAPON		8
#define ID_TEX_ITEM_KNIFE		9
#define ID_TEX_ITEM_KNIFE1		10
#define ID_TEX_MISC				20


// xu ly va cham hai vat va cham khong van toc su dung AABB
Game *game;
Simon *simon;
TileMap * tilemap;
vector<LPGAMEOBJECT> objects;
class SampleKeyHander : public KeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

SampleKeyHander * keyHandler;

void SampleKeyHander::OnKeyDown(int KeyCode)
{
	if (simon->GetState() == SIMON_STATE_JUMP && !simon->IsStand())
	{
		return;
	}
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_F:
		simon->SetState(SIMON_STATE_FIGHT);
		//weapon->SetState(WEAPON_STATE_FIGHT,simon);
		break;
	case DIK_V:
		simon->SetState(SIMON_STATE_KNIFE);
	}
}

void SampleKeyHander::OnKeyUp(int KeyCode)
{
	
}

void SampleKeyHander::KeyState(BYTE *states)
{
	if (simon->GetState() == SIMON_STATE_JUMP && !simon->IsStand())
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_FIGHT && !simon->IsHitting())
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_KNIFE && !simon->IsHitting())
	{
		return;
	}
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_F))
		{
			simon->SetState(SIMON_STATE_FIGHT);
			//weapon->SetState(WEAPON_STATE_FIGHT, simon);
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
			
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_F))
		{
			simon->SetState(SIMON_STATE_FIGHT);
			//weapon->SetState(WEAPON_STATE_FIGHT, simon);
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_LEFT);
			//weapon->SetState(WEAPON_STATE_IDLE, simon);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(SIMON_STATE_SIT);
	//	weapon->SetState(WEAPON_STATE_IDLE, simon);
	}
	else if (game->IsKeyDown(DIK_F))
	{
		simon->SetState(SIMON_STATE_FIGHT);
		//weapon->SetState(WEAPON_STATE_FIGHT, simon);
	}
	else
	{
		simon->SetState(SIMON_STATE_IDLE);
		//weapon->SetState(WEAPON_STATE_IDLE, simon);
	}
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
void LoadResources()
{
	Textures * textures = Textures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"Resources\\simon1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON1, L"Resources\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WEAPON, L"Resources\\morningstar1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WEAPON1, L"Resources\\morningstar.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MISC, L"Resources\\ground\\2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TORCH, L"Resources\\ground\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MAP, L"Resources\\map1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HEART, L"Resources\\item\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM_WEAPON, L"Resources\\item\\3.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM_KNIFE, L"Resources\\item\\4.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM_KNIFE1, L"Resources\\weapon\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	Sprites * sprites = Sprites::GetInstance();
	Animations * animations = Animations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texSimon1 = textures->Get(ID_TEX_SIMON1);
	LPDIRECT3DTEXTURE9 texWeapon = textures->Get(ID_TEX_WEAPON);
	LPDIRECT3DTEXTURE9 texWeapon1 = textures->Get(ID_TEX_WEAPON1);
	LPDIRECT3DTEXTURE9 texbrick = textures->Get(ID_TEX_MISC);
	LPDIRECT3DTEXTURE9 textorch = textures->Get(ID_TEX_TORCH);
	LPDIRECT3DTEXTURE9 texmap = textures->Get(ID_TEX_MAP);
	LPDIRECT3DTEXTURE9 texheart = textures->Get(ID_TEX_HEART);
	LPDIRECT3DTEXTURE9 texItemWeapon = textures->Get(ID_TEX_ITEM_WEAPON);
	LPDIRECT3DTEXTURE9 texItemKnife = textures->Get(ID_TEX_ITEM_KNIFE);
	LPDIRECT3DTEXTURE9 texItemKnife1 = textures->Get(ID_TEX_ITEM_KNIFE1);
	LPDIRECT3DTEXTURE9 texbox = textures->Get(ID_TEX_BBOX);	
	// gach
	sprites->Add(20001, 0, 0, 32, 32, texbrick);
	// ngon duoc
	sprites->Add(20002, 0, 0, 32, 64, textorch);
	sprites->Add(20003, 32, 0, 62, 64, textorch);
	// khong duoc
	sprites->Add(20004, 0, 0, 0, 0, textorch);
	// item tim
	sprites->Add(20005, 0, 0, 16, 16, texheart);
	sprites->Add(20006, 0, 0, 0, 0, texheart);
	// item weapon
	sprites->Add(20007, 0, 0, 32, 32, texItemWeapon);
	sprites->Add(20008, 0, 0, 0, 0, texItemWeapon);
	// item knife
	sprites->Add(20009, 0, 0, 32, 18, texItemKnife);
	sprites->Add(20010, 0, 0, 0, 0, texItemKnife);
	sprites->Add(20011, 0,0, 32, 18, texItemKnife1);

	//di phai
	sprites->Add(10001, 314, 4, 344, 64, texSimon);
	sprites->Add(10002, 378, 2, 402, 64, texSimon);
	sprites->Add(10003, 436, 4, 467, 64, texSimon);
	// danh phai
	sprites->Add(10004, 120, 4, 168, 64, texSimon);
	sprites->Add(10005, 74, 4, 107, 64, texSimon);
	sprites->Add(10006, 15, 6, 58, 64, texSimon);
	// nhay phai
	sprites->Add(10008, 196, 1, 228, 47, texSimon);
	sprites->Add(10009, 196, 1, 228, 47, texSimon);
	// roi level a ben phai
	sprites->Add(10011, 488, 17, 504, 66, texWeapon, { -9,14 });
	sprites->Add(10012, 328, 12, 360, 50, texWeapon, { -30,8 });
	sprites->Add(10013, 243, 16, 288, 32, texWeapon, { 35,9 });
	// ngoi ben phai
	sprites->Add(10014, 196, 1, 228, 47, texSimon, {0,15});
	// ngoi danh ben phai
	sprites->Add(100015, 0, 67, 48, 113, texSimon);
	sprites->Add(100016, 437, 113, 469, 178, texSimon);
	sprites->Add(100017, 375, 135, 419, 178, texSimon);
	//simon giut giut
	sprites->Add(100018, 432, 198, 468, 264, texSimon);
	sprites->Add(100019, 373, 198, 408, 264, texSimon);
	sprites->Add(100020, 313, 198, 223, 264, texSimon);
	sprites->Add(100021, 252, 198, 288, 264, texSimon);
	// roi level b ben phai
	sprites->Add(10022, 488, 86, 504, 134, texWeapon, { -9,14 });
	sprites->Add(10023, 328, 80, 360, 118, texWeapon, { -30,8 });
	sprites->Add(10024, 243, 90, 288, 102, texWeapon, { 40,13 });
	// roi level c ben phai
	sprites->Add(10025, 488, 154, 504, 202, texWeapon, { -9,14 });
	sprites->Add(10026, 328, 148, 360, 186, texWeapon, { -30,8 });
	sprites->Add(10027, 244, 158, 320, 170, texWeapon, { 45,15 });
	//di trai
	sprites->Add(11001, 136, 3, 166, 64, texSimon1);
	sprites->Add(11002, 78, 2, 102, 64, texSimon1);
	sprites->Add(11003, 12, 4, 44, 64, texSimon1);
	// danh trai
	sprites->Add(11004, 421, 6, 464, 64, texSimon1);
	sprites->Add(11005, 373, 4, 405, 64, texSimon1);
	sprites->Add(11006, 311, 4, 360, 64, texSimon1);
	// nhay trai
	sprites->Add(11007, 136, 4, 166, 64, texSimon1);
	sprites->Add(11008, 252, 0, 284, 47, texSimon1);
	sprites->Add(11009, 252, 0, 284, 47, texSimon1);
	sprites->Add(11010, 136, 4, 166, 64, texSimon1);
	//danh roi trai
	sprites->Add(11011, 136, 18, 152, 66, texWeapon1, { 41,15 });
	sprites->Add(11012, 280, 11, 312, 50, texWeapon1, { 28,8 });
	sprites->Add(11013, 352, 15, 398, 32, texWeapon1, { -40,10 });
	// ngoi ben trai
	sprites->Add(11014, 252, 0, 284, 47, texSimon1, {0,15});
	// ngoi danh ben trai
	sprites->Add(110015, 432, 67, 0, 113, texSimon1);
	sprites->Add(110016, 10, 113, 43, 179, texSimon1);
	sprites->Add(110017, 61, 135, 105, 179, texSimon1);
	// simon giut giut trai
	sprites->Add(110018, 12, 198, 48, 264, texSimon1);
	sprites->Add(110019, 72, 198, 107, 264, texSimon1);
	sprites->Add(110020, 132, 198, 167, 264, texSimon1);
	sprites->Add(110021, 192, 198, 228, 264, texSimon1);
	// roi level b ben trai
	sprites->Add(11022, 136, 86, 152, 134, texWeapon1, { 41,15 });
	sprites->Add(11023, 280, 80, 312, 118, texWeapon1, { 28,8 });
	sprites->Add(11024, 352, 90, 398, 102, texWeapon1, { -40,12 });
	// roi level c ben trai
	sprites->Add(11025, 136, 154, 152, 202, texWeapon1, { 41,15 });
	sprites->Add(11026, 280, 148, 312, 186, texWeapon1, { 28,8 });
	sprites->Add(11027, 320, 158, 396, 170, texWeapon1, { -80,15 });
	LPANIMATION ani;
	//dung quay mat ve ben phai
	ani = new Animation(100);
	ani->Add(10001);
	animations->Add(400, ani);
	// dung quay mat ve ben trai
	ani = new Animation(100);
	ani->Add(11001);
	animations->Add(401, ani);
	// di ben phai
	ani = new Animation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);
	// di ben trai
	ani = new Animation(100);
	ani->Add(11001);
	ani->Add(11002);
	ani->Add(11003);
	animations->Add(501, ani);
	 // danh ben phai
	ani = new Animation(100);
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);
	animations->Add(600,ani);
	// danh ben trai
	ani = new Animation(100);
	ani->Add(11006);
	ani->Add(11005);
	ani->Add(11004);
	animations->Add(601,ani);
	// nhay ben phai
	ani = new Animation(100);
	ani->Add(10008);
	ani->Add(10009);
	animations->Add(700, ani);
	// nhay ben trai
	ani = new Animation(100);
	ani->Add(11008);
	ani->Add(11009);
	animations->Add(701, ani);
	// ngoi ben phai
	ani = new Animation(100);
	ani->Add(10014);
	animations->Add(800, ani);
	//ngoi ben trai
	ani = new Animation(100);
	ani->Add(11014);
	animations->Add(801, ani);
	// ngoi danh ben phai
	ani = new Animation(100);
	ani->Add(10015);
	ani->Add(10016);
	ani->Add(10017);
	animations->Add(900, ani);
	// ngoi danh ben trai
	ani = new Animation(100);
	ani->Add(11015);
	ani->Add(11016);
	ani->Add(11017);
	animations->Add(901, ani);
	// giut giut ben phai
	ani = new Animation(100);
	ani->Add(100018);
	ani->Add(100019);
	ani->Add(100020);
	ani->Add(100021);
	animations->Add(902, ani);
	// giut giut ben trai
	ani = new Animation(100);
	ani->Add(110018);
	ani->Add(110019);
	ani->Add(110020);
	ani->Add(110021);
	animations->Add(903, ani);
	// danh ben phai
	ani = new Animation(100);
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);
	animations->Add(904, ani);
	// danh ben trai
	ani = new Animation(100);
	ani->Add(11006);
	ani->Add(11005);
	ani->Add(11004);
	animations->Add(905, ani);
	

	simon = new Simon();
	simon->AddAnimation(400);	// dung ben phai	
	simon->AddAnimation(401);	//dung ben trai	
	simon->AddAnimation(500);	// di ben phai
	simon->AddAnimation(501);	// di ben trai
	simon->AddAnimation(600);	// danh ben phai
	simon->AddAnimation(601);	// danh ben trai
	simon->AddAnimation(700);	// nhay ben phai
	simon->AddAnimation(701);// nhay ben trai
	simon->AddAnimation(800);	// ngoi ben phai
	simon->AddAnimation(801);	// ngoi ben trai
	simon->AddAnimation(900);// ngoi danh ben phai
	simon->AddAnimation(901);// ngoi danh ben trai
	simon->AddAnimation(902);
	simon->AddAnimation(903);// simon giut giut ben trai
	simon->AddAnimation(904);
	simon->AddAnimation(905);

	objects.push_back(simon);
	// load cai roi 
	ani = new Animation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(1000, ani);

	ani = new Animation(100);
	ani->Add(11011);
	ani->Add(11012);
	ani->Add(11013);
	animations->Add(1001, ani);
	// roi level B
	ani = new Animation(100);
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	animations->Add(1100, ani);

	ani = new Animation(100);
	ani->Add(11022);
	ani->Add(11023);
	ani->Add(11024);
	animations->Add(1101, ani);
	// roi level C
	ani = new Animation(100);
	ani->Add(10025);
	ani->Add(10026);
	ani->Add(10027);
	animations->Add(1102, ani);

	ani = new Animation(100);
	ani->Add(11025);
	ani->Add(11026);
	ani->Add(11027);
	animations->Add(1103, ani);
	//weapon = new Weapon();
	Weapon::AddAnimation(1000);
	Weapon::AddAnimation(1001);
	Weapon::AddAnimation(1100);
	Weapon::AddAnimation(1101);
	Weapon::AddAnimation(1102);
	Weapon::AddAnimation(1103);
	
	// load nen gach
	ani = new Animation(100);
	ani->Add(20001);
	animations->Add(1002, ani);
	//objects.push_back(weapon);
	for (int i = 0; i < 50; i++)
	{
		Brick *brick = new Brick();
		brick->AddAnimation(1002);	
		brick->SetPosition(i * 32.0f, 320);	
		objects.push_back(brick);		
	}
	// load ngon duoc
	ani = new Animation(100);
	ani->Add(20002);
	ani->Add(20003);
	animations->Add(1003, ani);
	ani = new Animation(100);
	ani->Add(20004);
	animations->Add(1004, ani);
	for (int i = 1; i < 6; i++)
	{
		Torch * torch = new Torch;
		torch->AddAnimation(1003);
		torch->AddAnimation(1004);
		torch->SetPosition(i*250.0f, 320 - 64);
		objects.push_back(torch);
	}
	// load item
	ani = new Animation(100);
	ani->Add(20005);
	animations->Add(1005, ani);
	ani = new Animation(100);
	ani->Add(20006);
	animations->Add(1006, ani);
	for (int i = 1; i < 3; i++)
	{
		ItemHeart *itemheart = new ItemHeart();
		itemheart->AddAnimation(1005);
		itemheart->AddAnimation(1006);
		itemheart->SetPosition(i*250, 320-64);
		objects.push_back(itemheart);
	}
	// load item roi
	ani = new Animation(100);
	ani->Add(20007);
	animations->Add(1007, ani);
	ani = new Animation(100);
	ani->Add(20008);
	animations->Add(1008, ani);
	for (int i = 3; i < 5; i++)
	{
		ItemWhip *itemwhip = new ItemWhip();
		itemwhip->AddAnimation(1007);
		itemwhip->AddAnimation(1008);
		itemwhip->SetPosition(i*250, 320 - 64);
		objects.push_back(itemwhip);
	}
	// load item knife
	ani = new Animation(100);
	ani->Add(20009);
	animations->Add(1009, ani);
	ani = new Animation(100);
	ani->Add(20010);
	animations->Add(1010, ani);
	ItemKnife *itemknife = new ItemKnife();
	itemknife->AddAnimation(1009);
	itemknife->AddAnimation(1010);
	itemknife->SetPosition(250*5, 320 - 64);
	objects.push_back(itemknife);
	// load cai dao 
	ani = new Animation(100);
	ani->Add(20009);
	animations->Add(1011, ani);
	ani = new Animation(100);
	ani->Add(20011);
	animations->Add(1012, ani);
	//simon->weaponKnife.AddAnimation
	simon->weaponKnife.AddAnimation(1011);
	simon->weaponKnife.AddAnimation(1012);
	//simon->weaponKnife.SetPosition(20, 320-64);
	// load map
	tilemap = new TileMap(L"Resources\\map1.txt", 1536, 384, 32, 32);
	tilemap->SetTileMap(texmap);
	tilemap->LoadResources();
	//tilemap->Load_MapData();
}
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	simon->weapon.Update(dt, objects);
	simon->weaponKnife.Setnx(simon->Getnx());
	simon->weaponKnife.Update(dt,&coObjects);
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		if (objects[i]->GetDie()==true)
		{
			objects.erase(objects.begin() + i);
		}
	}
	float cx, cy;
	cx = simon->Getx();
	cy = simon->Gety();
	if (cx > SCREEN_WIDTH / 2 && cx + SCREEN_WIDTH / 2 < tilemap->GetMapWidth())
	{
		game->SetPosition(cx - SCREEN_WIDTH / 2, 0);
	}	
}
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		tilemap->Draw(game->GetPosition());
		
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}	
		spriteHandler->End();
		d3ddv->EndScene();
	}
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();
		DWORD dt = now - frameStart;
		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else 
		{
			Sleep(tickPerFrame - dt);
		}
	}
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);
	keyHandler = new SampleKeyHander();
	game->InitKeyboard(keyHandler);
	LoadResources();
	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT , SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	Run();
	return 0;
}