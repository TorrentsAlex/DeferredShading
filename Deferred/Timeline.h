#pragma once
#include <glm\glm.hpp>


class Timeline {
protected:
	bool running, finished;
	float elapsedTime;

	float desiredTime;
	float startValue, endValue;
	int tAlgorithm;
public:
	Timeline();
	Timeline(int algorithm);
	~Timeline();

	void setValues(float start, float end);
	
	/*
		For the moment desiredTime only works with curves algorithm
	*/
	void setDesiredTime(float time);
	
	void playFromStart();
	
	void update(float dt);

	float getValue();
	float getKey();
	bool isFinished();
	bool isRunning();
};