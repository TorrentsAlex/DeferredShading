#include "Camera.h"

Camera::Camera():
	cAspectRatio(1),
	cFOV(45.0f),
	cFar(8000.0f),
	cNear(1.5f),
	cProjectionWidth(15.0f),
	cProjectionHeight(15.0f),
	cCameraPos(0.0f, 100.0f, 140.0f),
	cCameraFront(0.0f, -1.0f, -2.0f),
	cCameraUp(0.0f, 0.0f, 1.0f),
	cCameraRight(1.0f, 0.0f, 0.0f),
	prevMouse(0.0f) {
}

Camera::Camera(Camera& c) {
	cAspectRatio = c.cAspectRatio;
	cFOV = c.cFOV;
	cFar = c.cFar;
	cNear = c.cNear;
	cProjectionWidth = c.cProjectionWidth;
	cProjectionHeight = c.cProjectionHeight;
	cCameraPos = c.cCameraPos;
	cCameraFront = c.cCameraFront;
	cCameraUp = c.cCameraUp;
}

Camera::~Camera() {}

void Camera::initializeZBuffer(glm::vec2 windowResolution) {
	//Initialize the Zbuffer
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowResolution.x, windowResolution.y);
	cAspectRatio = windowResolution.x / windowResolution.y;
	cScreenSize = windowResolution;
}

// Setters
void Camera::setPerspectiveCamera() {
	cProjectionMatrix = glm::perspective(cFOV, cAspectRatio, cNear, cFar);
}

void Camera::setViewMatrix() {
	glm::vec3 cameraDirection = glm::normalize(-cCameraFront);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cCameraUp = glm::cross(cameraDirection, cCameraRight);
	cViewMatrix = glm::lookAt(cCameraPos, cCameraPos + cCameraFront, cCameraUp);
}

void Camera::setCameraPosition(glm::vec3 pos) {
	cCameraPos = pos;
	setViewMatrix();
}

void Camera::setCameraFront(glm::vec3 front) {
	cCameraFront = glm::vec3(front.x, front.y, front.z + 1.0);
}

// Getters
glm::mat4 Camera::getProjectionCamera() {
	return cProjectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return cViewMatrix;
}

glm::vec3 Camera::getPosition() {
	return cCameraPos;
}

void Camera::setAngle(float angle) {
	cAngle = angle;
}

float Camera::getAngle() {
	return cAngle;
}

void Camera::rotate(glm::vec2 mousePos) {
	mousePos = (mousePos / cScreenSize) * 2.0f -1.0f;
	glm::vec2 diff = (mousePos - prevMouse) * 2.5f;

	cCameraFront += glm::vec3(diff.x, -diff.y, 0.0f);
	prevMouse = mousePos;
	setViewMatrix();
}

void Camera::moveTo(CameraMove cm) {
	switch (cm) {
	case  CameraMove::forward:
		setCameraPosition(cCameraPos += glm::normalize(cCameraFront)* 0.05f);
	break;
	case  CameraMove::backwards:
		setCameraPosition(cCameraPos -= glm::normalize(cCameraFront)* 0.05f);
	break;
	case  CameraMove::left:
		cCameraPos -= glm::normalize(glm::cross(cCameraFront, cCameraUp)) * 0.05f;
		setCameraPosition(cCameraPos);
	break;
	case  CameraMove::right:
		cCameraPos += glm::normalize(glm::cross(cCameraFront, cCameraUp))* 0.05f;
		setCameraPosition(cCameraPos);
	break;
	case  CameraMove::up:
		cCameraPos += cCameraUp* 0.05f;
		setCameraPosition(cCameraPos);
	break;
	case  CameraMove::down:
		cCameraPos -= cCameraUp* 0.05f;
		setCameraPosition(cCameraPos);
	break;
	case CameraMove::none:
	default:
	break;
	}
}
