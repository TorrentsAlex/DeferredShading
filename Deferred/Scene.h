#ifndef TURRI_SCENE
#define TURRI_SCENE

#include "Entity.h"
#include "Water.h"

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

	/* Variable deprecated, using DecorObjects for render */
	vector<Entity> sDecoration;

	vector<Light> sLights;
	GLuint sCubemap;
	Water water;

public:
	Scene();
	~Scene();

	vector<DecorObjects> listObjects;
	// Setters
	void setSkyBox(OBJ object, GLuint texture);
	void setTerrain(OBJ object, GLuint texture, Material material);
	void setDecoration(vector<Entity> decoration);
	void setLights(vector<Light> lights);
	void setCubemap(GLuint cubemap);

	// Getters
	Entity getTerrain();
	Entity getSkyBox();
	vector<Entity> getDecoration();
	vector<Light> getLights();
	GLuint getCubemap();

	void setWater(OBJ object, Material material, std::vector<Wave> waves);
	Entity getWater();

	void clean();
	void update(float dt);
};

#endif
