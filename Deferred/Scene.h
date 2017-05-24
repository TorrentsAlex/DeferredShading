#ifndef TURRI_SCENE
#define TURRI_SCENE

#include "Entity.h"

#include "Light.h"
#include <vector>

using namespace std;
struct DecorObjects {
	Entity* e;
	vector<GameObject> g;
};

class Scene {
private:
	Entity sSkybox;
	Entity sTerrain;

	vector<Entity> sDecoration;

	vector<Light> sLights;

public:
	Scene();
	~Scene();

	vector<DecorObjects> listObjects;
	// Setters
	void setSkyBox(OBJ object, GLuint texture);
	void setTerrain(OBJ object, GLuint texture, Material material);
	void setDecoration(vector<Entity> decoration);
	void setLights(vector<Light> lights);

	// Getters
	Entity getTerrain();
	Entity getSkyBox();
	vector<Entity> getDecoration();
	vector<Light> getLights();

	void clean();
	void update();
};

#endif