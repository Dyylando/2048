#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include "constants.h"
#include "Colors.h"
#include <string>

Camera cam;

float board[] = { 0, 0, 0, 
		0, 1, 0, 
		1, 1, 0, 
		1, 0, 0, 
		0, 0, 0.5, 
		0, 1, 0.5, 
		1, 1, 0.5, 
		1, 0, 0.5 
};

float tiles[] = { 0, 0, 0, 
		0, 1, 0, 
		1, 1, 0, 
		1, 0, 0, 
		0, 0, 0.4, 
		0, 1, 0.4, 
		1, 1, 0.4, 
		1, 0, 0.4 
};

GLuint tileIndex[] = { 0, 1, 2,  2,3,0,  4,5,6,  6,7,4,  3,2,6, 6,7,3,  0,1,5,  5,4,0,
	1,2,6, 6,5,1, 0,3,7, 7,4,0 };


Colors TileColors[12] = {
	Colors(0.831, 0.764, 0.686),
	Colors(0.929, 0.878, 0.784),
	Colors(0.949, 0.694, 0.474),
	Colors(0.960, 0.584, 0.388),
	Colors(0.964, 0.486, 0.376),
	Colors(0.964, 0.368, 0.231),
	Colors(0.929, 0.811, 0.450),
	Colors(0.929, 0.8, 0.384),
	Colors(0.929, 0.784, 0.313),
	Colors(0.929, 0.772, 0.247),
	Colors(1, 0.835, 0.258),
	Colors(1, 0.803, 0.121)
};

void Game::GameStart() {
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glFrustum(-1, 1, -1, 1, 0.8, 80);
	GameInit();
}

void Game::ShowGame() {
	glTranslatef(TRANSLATE_FIELD_X, TRANSLATE_FIELD_Y, 0);
	glPushMatrix();
	glTranslatef(0.5, 0.5, 0);
	ShowTiles();
	glPopMatrix();
	ShowField();
}

void Game::ShowText() {
	glPushMatrix();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	char score_text[128];
	char text[128];

	sprintf(score_text, "Score: %d", score);
	sprintf(text, "Time: %d sec.", sec);

	glColor3f(1, 1, 1);
	glTranslatef(0, 0, -1);

	drawString(GLUT_BITMAP_TIMES_ROMAN_24, score_text, -0.95, 0.9);
	drawString(GLUT_BITMAP_TIMES_ROMAN_24, "H - Help", 0.65, 0.9);
	drawString(GLUT_BITMAP_TIMES_ROMAN_24, text , -0.95, 0.8);

	if (GetKeyState('H') < 0) {
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, "R - restart    Mousewheel - Zoom    Arrows - control", -0.9, -0.9);
	}
	glPopMatrix();
	

	if (win && !isPlaying) {
		glPushMatrix();
		glColor3f(0.466, 0.431, 0.396);
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -1);
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, "You win!", -0.15, 0);
		glPopMatrix();
	}

	if (!win && !isPlaying) {
		glPushMatrix();
		glColor3f(0.466, 0.431, 0.396);
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -1);
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Game over!", -0.2, 0);
		drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Press <R> to restart.", -0.3, -0.1);
		glPopMatrix();
	}
}

void Game::GameInit() {
	Camera cam;
	cam.xRot = 0;
	cam.yRot = 0;
	cam.CameraApply();

	score = 0;
	isPlaying = true;
	win = false;
	elemCount = START_COUNT;
	sec = 0;

	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			field[i][j] = 0;
		}
	}

	static int row, col;
	srand(time(NULL));

	for (int i = 0; i < START_COUNT; ++i) {
		do
		{
			row = rand() % FIELD_SIZE;
			col = rand() % FIELD_SIZE;
		} while (field[row][col] != 0);

		field[row][col] = InitValue();
	}
}

int Game::InitValue() {
	int temp = rand() % 10;

	if (temp == 1) {
		return 4;
	}
	else {
		return 2;
	}
}

void Game::Line(float x1, float y1, float x2, float y2) {
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
}

void Game::DrawNumbers(int a) {
	glPushMatrix();
	glTranslatef(1.0, 0, 0.5);
	glRotatef(90, 0, 0, 1);
		glLineWidth(3);
		glBegin(GL_LINES);
		if ((a != 1) && (a != 4)) Line(0.3, 0.85, 0.7, 0.85);
		if ((a != 0) && (a != 1) && (a != 7)) Line(0.3, 0.5, 0.7, 0.5);
		if ((a != 1) && (a != 4) && (a != 7)) Line(0.3, 0.15, 0.7, 0.15);

		if ((a != 5) && (a != 6)) Line(0.7, 0.5, 0.7, 0.85);
		if ((a != 2)) Line(0.7, 0.5, 0.7, 0.15);

		if ((a != 1) && (a != 2) && (a != 3) && (a != 7)) Line(0.3, 0.5, 0.3, 0.85);
		if ((a == 0) || (a == 2) || (a == 6) || (a == 8)) Line(0.3, 0.5, 0.3, 0.15);
		glEnd();
	glPopMatrix();
}

void Game::ShowTiles() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, tiles);
	int c;
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			glPushMatrix();
			glTranslatef(i * TILE_PADDING, j * TILE_PADDING, 0);
			if (field[i][j] == 0) {
				c = 0;
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 2) {
				c = 1;
				glPushMatrix();
				glTranslatef(0.25, 0.25, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(0.466, 0.431, 0.396);
				DrawNumbers(field[i][j]);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 4) {
				c = 2;
				glPushMatrix();
				glTranslatef(0.25, 0.25, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(field[i][j]);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 8) {
				c = 3;
				glPushMatrix();
				glTranslatef(0.25, 0.25, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(field[i][j]);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 16) {
				c = 4;
				glPushMatrix();
				glTranslatef(0.25, 0, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(1);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(6);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 32) {
				c = 5;
				glPushMatrix();
				glTranslatef(0.25, 0.1, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(3);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(2);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 64) {
				c = 6;
				glPushMatrix();
				glTranslatef(0.25, 0.1, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(6);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(4);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 128) {
				c = 7;
				glPushMatrix();
				glTranslatef(0.25, -0.15, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(1);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(2);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(8);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 256) {
				c = 8;
				glPushMatrix();
				glTranslatef(0.25, -0.05, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(2);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(5);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(6);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 512) {
				c = 9;
				glPushMatrix();
				glTranslatef(0.25, 0.05, 0.4);
				glScalef(0.5, 0.5, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(5);
				glTranslatef(0, 0.2, 0);
				DrawNumbers(1);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(2);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else if (field[i][j] == 1024) {
				c = 10;
				glPushMatrix();
				glTranslatef(0.25, -0.2, 0.4);
				glScalef(0.44, 0.44, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(1);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(0);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(2);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(4);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			else {
				c = 11;
				glPushMatrix();
				glTranslatef(0.25, -0.05, 0.4);
				glScalef(0.4, 0.4, 0);
				glColor3f(1, 1, 1);
				DrawNumbers(2);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(0);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(4);
				glTranslatef(0, 0.6, 0);
				DrawNumbers(8);
				glPopMatrix();
				glColor3f(TileColors[c].r, TileColors[c].g, TileColors[c].b);
			}
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, tileIndex);
			glPopMatrix();
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Game::ShowField() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, board);

	glPushMatrix();
	glColor3f(0.698, 0.627, 0.564);
	glScalef(5.5, 5.5, 0.5);
	glTranslatef(0, 0, -0.2);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, tileIndex);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Game::KeyboardSpecial() {
	if (GetKeyState('R') < 0) {
		GameStart();
	}
}

void Game::KeyControl() {
	if (isPlaying) {
		if (GetKeyState(VK_UP) < 0) {
			MoveUp();

			if (!Check())
				AddBlock();

			Sleep(50);
		}
		if (GetKeyState(VK_DOWN) < 0) {
			MoveDown();

			if (!Check())
				AddBlock();

			Sleep(50);
		}
		if (GetKeyState(VK_LEFT) < 0) {
			MoveLeft();

			if (!Check())
				AddBlock();

			Sleep(50);
		}
		if (GetKeyState(VK_RIGHT) < 0) {
			MoveRight();

			if (!Check())
				AddBlock();

			Sleep(50);
		}
	}
	if (isPlaying) {
		CheckOver();
	}
}

void Game::MoveUp()
{
	int row, col;
	for (int j = 0; j < FIELD_SIZE; j++)
	{
		row = 0, col = j;
		for (int i = 1; i < FIELD_SIZE; i++)
		{
			if (field[i][j] != 0)
			{
				if (field[i - 1][j] == 0 || field[i - 1][j] == field[i][j])
				{
					if (field[row][col] == field[i][j])
					{
						field[row][col] *= 2;
						score += field[row][col];
						field[i][j] = 0;
						elemCount--;
					}
					else
					{
						if (field[row][col] == 0)
						{
							field[row][col] = field[i][j];
							field[i][j] = 0;
						}
						else
						{
							field[++row][col] = field[i][j];
							field[i][j] = 0;
						}
					}
				}
				else row++;
			}
		}
	}
}

void Game::MoveDown()
{
	int row, col;
	for (int j = 0; j < FIELD_SIZE; j++)
	{
		row = FIELD_SIZE - 1, col = j;
		for (int i = 2; i >= 0; i--)
		{
			if (field[i][j] != 0)
			{
				if (field[i + 1][j] == 0 || field[i + 1][j] == field[i][j])
				{
					if (field[row][col] == field[i][j])
					{
						field[row][col] *= 2;
						score += field[row][col];
						field[i][j] = 0;
						elemCount--;
					}
					else
					{
						if (field[row][col] == 0)
						{
							field[row][col] = field[i][j];
							field[i][j] = 0;
						}
						else
						{
							field[--row][col] = field[i][j];
							field[i][j] = 0;
						}
					}
				}
				else row--;
			}
		}
	}
}

void Game::MoveLeft()
{
	int row, col;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		row = i, col = 0;
		for (int j = 1; j < FIELD_SIZE; j++)
		{
			if (field[i][j] != 0)
			{
				if (field[i][j - 1] == 0 || field[i][j - 1] == field[i][j])
				{
					if (field[row][col] == field[i][j])
					{
						field[row][col] *= 2;
						score += field[row][col];
						field[i][j] = 0;
						elemCount--;
					}
					else
					{
						if (field[row][col] == 0)
						{
							field[row][col] = field[i][j];
							field[i][j] = 0;
						}
						else
						{
							field[row][++col] = field[i][j];
							field[i][j] = 0;
						}
					}
				}
				else col++;
			}
		}
	}
}

void Game::MoveRight()
{
	int row, col;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		row = i, col = FIELD_SIZE - 1;
		for (int j = FIELD_SIZE / 2; j >= 0; j--)
		{
			if (field[i][j] != 0)
			{
				if (field[i][j + 1] == 0 || field[i][j + 1] == field[i][j])
				{
					if (field[row][col] == field[i][j])
					{
						field[row][col] *= 2;
						score += field[row][col];
						field[i][j] = 0;
						elemCount--;
					}
					else
					{
						if (field[row][col] == 0)
						{
							field[row][col] = field[i][j];
							field[i][j] = 0;
						}
						else
						{
							field[row][--col] = field[i][j];
							field[i][j] = 0;
						}
					}
				}
				else col--;
			}
		}
	}
}

void Game::AddBlock()
{
	static int row, col;
	srand(time(NULL));
	do
	{
		row = rand() % FIELD_SIZE;
		col = rand() % FIELD_SIZE;
	} while (field[row][col] != 0);

	field[row][col] = InitValue();
	++elemCount;
}

int Game::Check()
{
	int fl = 1;
	for (int i = 0; i < FIELD_SIZE; i++)
		for (int j = 0; j < FIELD_SIZE; j++)
			if (temp[i][j] != field[i][j])
			{
				fl = 0;
				break;
			}
	return fl;
}

void Game::CheckOver()
{
	for (int i = 0; i < FIELD_SIZE; ++i) {
		for (int j = 0; j < FIELD_SIZE; ++j) {
			if (field[i][j] == WIN_VALUE) {
				win = true;
				isPlaying = false;
			}
		}
	}
	if (elemCount == FIELD_SIZE * FIELD_SIZE) {

		bool isMoveAvailable = false;
		for (int i = 0; i < FIELD_SIZE; ++i) {
			for (int j = 0; j < FIELD_SIZE; ++j) {
				if (((i - 1) >= 0 && field[i][j] == field[i - 1][j]) ||
					(i + 1 < FIELD_SIZE && field[i][j] == field[i + 1][j]) ||
					(j - 1 >= 0 && field[i][j] == field[i][j - 1]) ||
					(j + 1 < FIELD_SIZE && field[i][j] == field[i][j + 1])) {
					isMoveAvailable = true;
					break;
				}
			}
			if (isMoveAvailable) break;
		}
		if (!isMoveAvailable) {
			GameOver();
		}
	}
}

void Game::GameOver() {
	isPlaying = false;
	win = false;
}

void Game::drawString(void* font, const char* text, float x, float y)
{
	if (!text)
	{
		return;
	}

	glRasterPos2f(x, y);
	while (*text)
	{
		glutBitmapCharacter(font, *text);
		text++;
	}
}