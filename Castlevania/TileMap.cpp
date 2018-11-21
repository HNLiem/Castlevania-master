#include "TileMap.h"
#define SCREEN_WIDTH 640


TileMap::TileMap( LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height)
{
	
	this->filePath_data = filePath_data;

	sprites = Sprites::GetInstance();

	this->map_Width = map_width;
	this->map_Height = map_height;

	this->tile_Width = tile_width;
	this->tile_Height = tile_height;

	nums_row = map_Height / tile_Height;
	nums_col = map_Width / tile_Width;
}

void TileMap::LoadResources(LPDIRECT3DTEXTURE9 texTileMap)
{
	/*Textures * texture = Textures::GetInstance();

	texture->Add(ID, filePath_tex, D3DCOLOR_XRGB(255, 255, 255));


	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);*/

	// lấy thông tin về kích thước của texture lưu các block tiles (filePath_tex)

	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	// tính toán số hàng, số cột cần thiết để load tile 
	int nums_rowToRead = surfaceDesc.Height / tile_Height;
	int nums_colToRead = surfaceDesc.Width / tile_Width;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1;

	for (int i = 0; i < nums_rowToRead; i++)
	{
		for (int j = 0; j < nums_colToRead; j++)
		{
			sprites->Add(id_sprite, tile_Width * j, tile_Height * i, tile_Width * (j + 1), tile_Height * (i + 1), texTileMap);
			id_sprite += 1;
		}
	}
	fstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);

		// tách số từ chuỗi đọc được

		vector<int> numInLine;
		stringstream ss(line);
		int n;

		while (ss >> n) {
			numInLine.push_back(n);
		}

		// thêm vào ma trận map_Data

		map_Data.push_back(numInLine);
	}


	fs.close();
}

//void TileMap::Load_MapData()
//{
//	fstream fs;
//	fs.open(filePath_data, ios::in);
//
//	if (fs.fail())
//	{
//		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", ID);
//		fs.close();
//		return;
//	}
//
//	string line;
//
//	while (!fs.eof())
//	{
//		getline(fs, line);
//
//		// tách số từ chuỗi đọc được
//
//		vector<int> numInLine;
//		stringstream ss(line);
//		int n;
//
//		while (ss >> n) {
//			numInLine.push_back(n);
//		}
//
//		// thêm vào ma trận map_Data
//
//		map_Data.push_back(numInLine);
//	}
//
//
//	fs.close();
//}

void TileMap::Draw(D3DXVECTOR2 camPosition)
{
	int start_col_to_draw = (int)camPosition.x / 32;
	int end_col_to_draw = (int)(camPosition.x + SCREEN_WIDTH) / 32;

	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm draw có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt (thực ra giá trị này bằng vx*dt, chính là quãng đường dịch chuyển của simon)
			float x = tile_Width * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % 32;
			float y = tile_Height * i;

			sprites->Get(map_Data[i][j])->Draw(x, y);
		}
	}
}


TileMap::~TileMap()
{
}
