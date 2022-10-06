#include "Camera.h"
#include "Game.h"

POINTFLOAT pos = { START_CAMERA_X ,START_CAMERA_Y };

void Camera::MoveCamera() {
    float angle = -zAlfa / 180 * 3.14;
    float speed = 0;

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);

     if (GetKeyState(VK_RBUTTON) < 0) {
        POINT cur;
        static POINT base = { Win_W / 2, Win_H / 2 };
        GetCursorPos(&cur);
        CameraRotate((base.x - cur.x) / 7.0, (base.y - cur.y) / 7.0);
        SetCursorPos(base.x, base.y);
     }
    Zoom();
    CameraApply();
}

void Camera::CameraRotate(float xAngle, float yAngle) {
    yRot += yAngle;
    xRot += xAngle;
}

void Camera::CameraApply() {
    glRotatef(-yRot, 0, 1, 0);
    glRotatef(-xRot, 1, 0, 0);

}

void Camera::WndResize(int x, int y) {
    glViewport(0, 0, x, y);
    float k = x / (float)y;
    float sz = 0.1;
    glLoadIdentity();
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 100);
}

void Camera::Get_wParam(int wParam) {
    this->Param = wParam;
}

void Camera::Zoom() {
    if (Param > 0) {
        posZ += 0.2;
        glTranslatef(0, 0, 0.2);
    }
    else if (Param < 0) {
        posZ -= 0.2;
        glTranslatef(0, 0, -0.2);
    }
    glTranslatef(0, 0, posZ);
    Param = 0;
}
