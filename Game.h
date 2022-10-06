#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "resource.h" //for icon
#include "Camera.h"
#include <glut.h>
#include "Colors.h"
#include <windows.h>
#include <cmath>
#include <ctime>

class Game
{
private:
	int elemCount;
	int temp[FIELD_SIZE][FIELD_SIZE]{};
	//

	int InitValue();
	void GameInit();

	void ShowTiles();
	void ShowField();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	int Check();
	void AddBlock();
	void CheckOver();
	void GameOver();

	void DrawNumbers(int a);
	void Line(float x1, float y1, float x2, float y2);

	void drawString(void* font, const char* text, float x, float y);

public:
	int field[FIELD_SIZE][FIELD_SIZE];
	int score;
	time_t sec;
	bool isPlaying, win;

	void GameStart();

	void ShowGame();
	void ShowText();

	void KeyControl();
	void KeyboardSpecial();
};

#endif

