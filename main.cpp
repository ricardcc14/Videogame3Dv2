#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "FirstPersonCameraScript.h"
#include "Script.h"
#include "AudioPlayer.h"
#include "VideoPlayer.h"
#include "TimerScript.h"
#include "LightScript.h"


#include "ECS.h"
#include "MazeSpawnerScript.h"
#include "SkyboxScript.h"
#include "ScriptRotate.h"
#include "BlockScript.h"

using std::cout; 
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 1920;
const unsigned int height = 1080;

float t = 0;
time_t current_time;

World* world;
vector<Entity*> gameEntities2d;

void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 1920x1080
	window = glfwCreateWindow(1920, 1080, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity2D(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, 
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default") {
	Entity* ent = world->create();
	ent->assign<Transform2D>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);
	return ent;
}


Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, glm::vec3 rotation, const char* meshFilepath, const char* texFilepath, const char* normalsFilepath = "Textures/flat_normals.png") {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale, rotation);
	ent->assign<MeshComponent>(texFilepath, meshFilepath, "red", normalsFilepath);
	return ent;
}

Entity* CreateEntity3DEmpty() {
	Entity* ent = world->create();
	
	return ent;
}

Entity* CreateCamera(glm::vec3 position) {
	Entity* ent = world->create();
	ent->assign<Camera>(position, glm::vec3(0., 0., -1.), glm::vec3(0., 1., 0.));

	return ent;
}

Entity* CreateSkybox(const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Skybox>(texFilepath, meshFilepath);

	return ent;
}

void SetupWorld() {

	RenderSystem* rs = new RenderSystem(width, height);

	world = World::createWorld();
	world->registerSystem(rs);
	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();


	Entity* ent = CreateCamera(glm::vec3(10.0f, 6.f, 10.0f));
	Entity* lightEnt = CreateEntity3DWithMesh(glm::vec3(20, 5, 20), 1, glm::vec3(0, 0, 0), "Meshes/cube.obj", "Textures/gameboy.png");
	lightEnt->assign<CubeCollider>(2, 2, 2);
	lightEnt->assign<Tag>(Tag{ EntityType::light });
	LightScript* lightScript = new LightScript(window, world, lightEnt);
	lightEnt->assign<ScriptComponent>(scriptManager->AddScript(lightScript));

	FirstPersonCameraScript* fps = new FirstPersonCameraScript(window, world, ent, lightScript);

	ent->assign<ScriptComponent>(scriptManager->AddScript(fps));

	rs->setCamera(ent);
	//Entity* sprite = CreateEntity2D(glm::vec2(0., 0.), 0.f, 1.f, "Textures/science_dog.png", glm::vec3(1., 1., 1.), false, glm::vec2(5., 5.));
	Entity* paddle_ent = CreateEntity2D(glm::vec2(100, 50), 0.f, 1.f, "Textures/button_yellow.png", glm::vec3(1., 1., 1.), true);
	paddle_ent->get<Sprite>()->visible = false;
	gameEntities2d.push_back(paddle_ent);
	/*Entity* spawner = CreateEntity3DEmpty();
	SpawnerScript* spawner_script = new SpawnerScript(window, world, spawner);
	spawner->assign<ScriptComponent>(scriptManager->AddScript(spawner_script));*/

	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/skybox.png");
	Entity* crosshair = CreateEntity2D(glm::vec2(960., 480.), 0, 1, "Textures/crosshair.png", glm::vec3(1., 1., 1.), false, glm::vec2(30, 30));
	gameEntities2d.push_back(crosshair);
	crosshair->get<Sprite>()->visible = false;
	Entity* floor = CreateEntity3DWithMesh(glm::vec3(60, 0, 60), 60, glm::vec3(0, 0, 0), "Meshes/plane.obj", "Textures/wall.png");
	floor->assign<Tag>(Tag{ EntityType::fixed });


	//Blocks
	Entity* cube1 = CreateEntity3DWithMesh(glm::vec3(20, 0, 20), 1, glm::vec3(0, 0, 0), "Meshes/cube.obj", "Textures/gameboy.png");
	cube1->assign<CubeCollider>(2, 2, 2);
	cube1->assign<Tag>(Tag{ EntityType::mobile });
	BlockScript* cube1_script = new BlockScript(window, world, cube1, 20.f);
	cube1->assign<ScriptComponent>(scriptManager->AddScript(cube1_script));

	glm::vec3 direction = glm::vec3(0.0, 1.0, 10.0);


	glm::vec3 eulerAngles = glm::vec3(45, 0, 0);

	cout << eulerAngles.x << endl;
	cout << eulerAngles.y << endl;
	cout << eulerAngles.z << endl;
	
	//Pool de walls
	int nWalls = 400;
	list<Entity*> wallsPool;
	for (int n = 0; n < nWalls; n++) {
		Entity* wall = CreateEntity3DWithMesh(glm::vec3(0.f), 2, glm::vec3(0, 0, 0), "Meshes/my_wall.obj", "Textures/bricks_albedo.png", "Textures/bricks_normal.png");
		wall->assign<CubeCollider>(3.5, 22, 3.5);
		wall->assign<Tag>(Tag{ EntityType::fixed });
		wallsPool.push_back(wall);
	}
	MazeSpawnerScript* mazeSpawnerScript = new MazeSpawnerScript (window, world, wallsPool);
	Entity* mazeManager = CreateEntity3DEmpty();
	mazeManager->assign<ScriptComponent>(scriptManager->AddScript(mazeSpawnerScript));
	
	Entity* video_intro = CreateEntity2D(glm::vec2(960.f, 540.f), 0.f, 1.f, "Textures/videogame_intro/frame_0001.png", glm::vec3(0.f), false, glm::vec2(1920, 1080));
	VideoPlayer* videoPlayer = new VideoPlayer(window, world, video_intro);
	video_intro->assign<ScriptComponent>(scriptManager->AddScript(videoPlayer));
	
	Entity* timerEnt1 = CreateEntity2D(glm::vec2(900.f, 840.f), 0.f, 1.f, "Textures/7seg_0.png", glm::vec3(0.f), false, glm::vec2(45.f, 61.f));
	Entity* timerEnt2 = CreateEntity2D(glm::vec2(940.f, 840.f), 0.f, 1.f, "Textures/7seg_0.png", glm::vec3(0.f), false, glm::vec2(45.f, 61.f));
	Entity* timerEnt3 = CreateEntity2D(glm::vec2(980.f, 840.f), 0.f, 1.f, "Textures/7seg_0.png", glm::vec3(0.f), false, glm::vec2(45.f, 61.f));
	Entity* timerEnt4 = CreateEntity2D(glm::vec2(1020.f, 840.f), 0.f, 1.f, "Textures/7seg_0.png", glm::vec3(0.f), false, glm::vec2(45.f, 61.f));
	Entity* timerEnt = CreateEntity2D(glm::vec2(960.f, 840.f), 0.f, 1.f, "Textures/button_yellow.png", glm::vec3(0.f), false, glm::vec2(90.f, 122.f));
	Entity* timerEntities[4] = { timerEnt1, timerEnt2, timerEnt3, timerEnt4 };
	TimerScript* timerScript = new TimerScript(window, world, timerEnt, timerEntities);
	timerEnt->assign<ScriptComponent>(scriptManager->AddScript(timerScript));

	



}

void showGameEntities() {
	for (Entity* entity : gameEntities2d) {
		entity->get<Sprite>()->visible = true;
	}

}

int main() {
	
	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupWorld();

	float dt = 0;
	float time = clock();

	AudioPlayer::Init();

	//AudioPlayer::PlayAudio("./Audio/industrious_ferret.wav", false);

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			world->tick(dt);
		}
		//GAME
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			showGameEntities();
		}
		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}