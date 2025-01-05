#include "LightScript.h"
#include <iostream>


void LightScript::startScript() {

}
void LightScript::tickScript(float deltaTime) {
	
	if (!running && (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
		running = true;
	}
	if (reset) {
		accTime = 0.0f;
		brightness = 1.f;
		
		reset = false;
	}
	if (running) {

		accTime += deltaTime * 0.00003;
		brightness = std::max(minBrightness, (float)(1 - accTime));
		//cout << "Brightness: " << brightness << endl;
		world->each<MeshComponent>([&](Entity* ent, ComponentHandle<MeshComponent> meshComponent) {
			//meshComponent->color *= brightness;
			meshComponent->bright = brightness;
			});

	}
	cout << "openlights2 Brigt:" << brightness << endl;
}
void LightScript::openLight() {
	reset = true;
}