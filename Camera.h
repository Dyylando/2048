#ifndef CAMERA
#define CAMERA

#include "Game.h"

class Camera
{
private:
	int angleX = -10;
	int angleY = 30;
	void CameraRotate(float xAngle, float zAngle);
public:
	float xAlfa = 0;
	float yAlfa = 0;
	float zAlfa = 90;

	float xRot = 0, yRot = 0;
	float posZ = -0.25;

	int Param;
	void MoveCamera();
	void WndResize(int x, int y);
	void CameraRotate();
	void Get_wParam(int wParam);
	void CameraApply();

	void Zoom();
};

#endif
