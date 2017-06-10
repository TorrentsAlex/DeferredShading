#include "Water.h"



Water::Water() {
}


Water::~Water() {
}


void Water::setup() {
	originalOBJ = eMesh;

	eGameObject.angle = 0;
	eGameObject.translate = glm::vec3(0.0f, 0.0f, 0.0f);
	eGameObject.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	eGameObject.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Water::addWave(Wave wave) {
	waves.push_back(wave);
}

void Water::update(float dt) {
	for (int i = 0; i < waves.size(); i++) {
		waves.at(i).update(dt);
	}

	for (int i = 0; i < eMesh.numVertices; ++i ) {
		for (Wave w : waves) {
			// Calculate the waves 
			float K1dotPos = glm::dot(w.K, originalOBJ.mesh[i].position.toVec3());
			// Wave 
			glm::vec3 xzWaves = glm::vec3(-(w.K / w.k)* w.A * glm::sin(K1dotPos - w.o * w.wt));
			float yWaves = w.A * glm::cos(K1dotPos - w.o*w.wt);

			eMesh.mesh[i].position.x = originalOBJ.mesh[i].position.toVec3().x - xzWaves.x;
			eMesh.mesh[i].position.z = originalOBJ.mesh[i].position.toVec3().z - xzWaves.z;
			eMesh.mesh[i].position.y = originalOBJ.mesh[i].position.toVec3().y - yWaves;
		}
	}
	
}