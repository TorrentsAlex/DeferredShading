#include "Timeline.h"


template<class T>
T lerp(T a, T b, float alpha) {
	return (1.0f - alpha)*a + alpha * b;
}

Timeline::Timeline() {
}

Timeline::Timeline(int algorithm) {
	tAlgorithm = algorithm;
}

Timeline::~Timeline() {
}	

void Timeline::setValues(float start, float end) {
	startValue = start;
	endValue = end;
}


void Timeline::setDesiredTime(float newTime) {
	desiredTime = newTime;
}

void Timeline::playFromStart() {
	running = true;
	finished = false;
	elapsedTime = 0.0f;
}

void Timeline::update(float dt) {

	if (!running) {
		return;
	}

	elapsedTime += dt;

	if (elapsedTime >= desiredTime) {
		running = false;
		finished = true;
		elapsedTime = desiredTime;
	}
}

float Timeline::getValue() {
	if (running) {
		float value;
		switch (tAlgorithm) {
		case 0:
			value = lerp(startValue, endValue, elapsedTime/desiredTime);
			break;
		case 1:
			value = -(1.0 + glm::cos((1.0 / desiredTime) * 3.14*elapsedTime)) * 0.5f + 1.0f;
				break;
		case 2:
			value = (1.0 + glm::cos((1.0 / desiredTime) * 3.14*elapsedTime)) * 0.5f;
				break;
		case 3:
			value = (desiredTime * desiredTime);
			break;
		case 4:
			value = -(desiredTime * desiredTime) + 1.0;
			break;
		}
		return value;
	} else {
		return -1.0f;
	}
}

float Timeline::getKey() {
	return elapsedTime;
}

bool Timeline::isFinished() {
	return finished;
}

bool Timeline::isRunning() {
	return running;
}