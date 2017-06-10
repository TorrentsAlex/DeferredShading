#ifndef WATERENTITY
#define WATERENTITY

#include "Entity.h"

struct Wave {	
	// Amplitude
	float A;
	// frequency
	float o;
	// direction 
	glm::vec3 K;
	// wavelength
	float k;

	float wt;
	
	void update(float dt) {
		wt += dt;
		if (wt >= 2.0f * 3.1416f) {
			wt = 0.0f;
		}
	}
};

class Water : public Entity {
private:
	OBJ originalOBJ;
	std::vector<Wave> waves;

public:
	Water();
	~Water();

	void setup();
	void addWave(Wave wave);
	void update(float dt);
};
#endif