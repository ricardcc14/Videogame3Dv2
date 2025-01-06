#include "FirstPersonCameraScript.h"
#include "BlockScript.h"
#include "LightScript.h";
#include <chrono>



void FirstPersonCameraScript::startScript() {
	
}

void FirstPersonCameraScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	float width = 1920;
	float height = 1080;

	ComponentHandle<Camera> cam = entity->get<Camera>();

	glm::vec3 currentPosition = cam->position;
	glm::vec3 desiredPosition = cam->position;

	//Show position debug

	static float timePos = 0.f;

	timePos += deltaTime;

	if (timePos >= 1000.f) {
		cout << "Position player-> X: " << currentPosition.x << " , Z: " << currentPosition.z << endl;
		timePos -= 1000.f;
	} 


	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * cam->orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -cam->orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up));
	}

	// Perquè no es mogui la camera verticalment només l'orientació de la camera
	desiredPosition.y = currentPosition.y;


	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * cam->up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -cam->up;
	}

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

		glm::vec3 pos = ent->get<Transform3D>()->position;

		//Sliding
		if (desiredPosition.x < pos.x + cubeColl->width && desiredPosition.x > pos.x - cubeColl->width &&
			desiredPosition.y < pos.y + cubeColl->height && desiredPosition.y > pos.y - cubeColl->height &&
			desiredPosition.z < pos.z + cubeColl->length && desiredPosition.z > pos.z - cubeColl->length) {

			// Adjust the desired position based on current position for sliding
			if (currentPosition.x <= pos.x - cubeColl->width) {
				desiredPosition.x = pos.x - cubeColl->width;
				// Allow sliding along Z-axis
				if (desiredPosition.z > pos.z + cubeColl->length) desiredPosition.z = pos.z + cubeColl->length;
				if (desiredPosition.z < pos.z - cubeColl->length) desiredPosition.z = pos.z - cubeColl->length;
			}
			if (currentPosition.x >= pos.x + cubeColl->width) {
				desiredPosition.x = pos.x + cubeColl->width;
				// Allow sliding along Z-axis
				if (desiredPosition.z > pos.z + cubeColl->length) desiredPosition.z = pos.z + cubeColl->length;
				if (desiredPosition.z < pos.z - cubeColl->length) desiredPosition.z = pos.z - cubeColl->length;
			}
			if (currentPosition.z <= pos.z - cubeColl->length) {
				desiredPosition.z = pos.z - cubeColl->length;
				// Allow sliding along X-axis
				if (desiredPosition.x > pos.x + cubeColl->width) desiredPosition.x = pos.x + cubeColl->width;
				if (desiredPosition.x < pos.x - cubeColl->width) desiredPosition.x = pos.x - cubeColl->width;
			}
			if (currentPosition.z >= pos.z + cubeColl->length) {
				desiredPosition.z = pos.z + cubeColl->length;
				// Allow sliding along X-axis
				if (desiredPosition.x > pos.x + cubeColl->width) desiredPosition.x = pos.x + cubeColl->width;
				if (desiredPosition.x < pos.x - cubeColl->width) desiredPosition.x = pos.x - cubeColl->width;
			}
			if (currentPosition.y <= pos.y - cubeColl->height) desiredPosition.y = pos.y - cubeColl->height;
			if (currentPosition.y >= pos.y + cubeColl->height) desiredPosition.y = pos.y + cubeColl->height;
		}

		//Finish sliding




		/*
		//Desired position inside cube
		if (desiredPosition.x < pos.x + cubeColl->width && desiredPosition.x > pos.x - cubeColl->width &&
			desiredPosition.y < pos.y + cubeColl->height && desiredPosition.y > pos.y - cubeColl->height &&
			desiredPosition.z < pos.z + cubeColl->length && desiredPosition.z > pos.z - cubeColl->length) {

			time_t result = time(NULL);

			char str[26];
			ctime_s(str, sizeof str, &result);

			if (currentPosition.x <= pos.x - cubeColl->width) desiredPosition.x = pos.x - cubeColl->width;
			if (currentPosition.x >= pos.x + cubeColl->width) desiredPosition.x = pos.x + cubeColl->width;
			if (currentPosition.z <= pos.z - cubeColl->length) desiredPosition.z = pos.z - cubeColl->length;
			if (currentPosition.z >= pos.z + cubeColl->length) desiredPosition.z = pos.z + cubeColl->length;
			if (currentPosition.y <= pos.y - cubeColl->height) desiredPosition.y = pos.y - cubeColl->height;
			if (currentPosition.y >= pos.y + cubeColl->height) desiredPosition.y = pos.y + cubeColl->height;
		}
		*/

		});
	if (desiredPosition.y < 1.5f) desiredPosition.y = 1.5f;
	cam->position = desiredPosition;

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		rotX = glm::clamp(rotX, -85.0f, 85.0f);
		// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(cam->orientation, cam->up)));
		cam->orientation = m * glm::vec4(cam->orientation, 1.);
		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), cam->up);

		cam->orientation = m2 * glm::vec4(cam->orientation, 1.);


		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	std::vector<glm::vec3> rayPoints;
	//RayCast agafar objectes
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !holdingObj) {
		
			float maxDistRay = 20.f;
			float stepSize = 0.1f;
			glm::vec3 rayPoint = cam->position;
			glm::vec3 rayDir = glm::normalize(cam->orientation);

			float rayCurrentDist = 0.0f;
			while (rayCurrentDist < maxDistRay) {

				//checkCollisions()
				world->each<CubeCollider>([&](Entity* cube_ent, ComponentHandle<CubeCollider> cube_collider) {
					ComponentHandle<Tag> cube_tag = cube_ent->get<Tag>();

					if (cube_tag && cube_tag->type == EntityType::fixed) {
						return;
					}
					if (cube_tag->type == EntityType::mobile || cube_tag->type == EntityType::light || cube_tag->type == EntityType::dynamite) {
						ComponentHandle<Transform3D> cube_transform = cube_ent->get<Transform3D>();

						glm::vec3 cubeMinPos = cube_transform->position - glm::vec3(cube_collider->width, cube_collider->length, cube_collider->height) * 0.5f;
						glm::vec3 cubeMaxPos = cube_transform->position + glm::vec3(cube_collider->width, cube_collider->length, cube_collider->height) * 0.5f;
						//code to check collions

						if (rayPoint.x >= cubeMinPos.x && rayPoint.x <= cubeMaxPos.x &&
							rayPoint.y >= cubeMinPos.y && rayPoint.y <= cubeMaxPos.y &&
							rayPoint.z >= cubeMinPos.z && rayPoint.z <= cubeMaxPos.z) {
							if (cube_tag->type == EntityType::dynamite) {

							holdingObj = true;
							objHolded = cube_ent;
							std::cout << "Collision detected at ray point: " << rayPoint.x << ", " << rayPoint.y << ", " << rayPoint.z << std::endl;
							}
							if (cube_tag->type == EntityType::light) {
								lightScript->openLight();
								mazeScript->changeMaze();
								//cout << "Br" << endl;
							}
							if (cube_tag->type == EntityType::mobile) {
								//Implement features with blocks
							}
							return;
						}
					}
					
				});
					rayPoint += rayDir * stepSize;
					rayCurrentDist += stepSize;
					if (holdingObj) break;
			}
	}
	if (holdingObj) {
			ComponentHandle<Transform3D> holded_transform = objHolded->get<Transform3D>();
			holded_transform->position = cam->position + cam->orientation * holdingZoom; // -glm::vec3(0.0f, 1.f, 0.f);
			holded_transform->position.y = 1.f;

			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) holdingZoom += 0.1f;
			else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) holdingZoom -= 0.1f;
			holdingZoom = glm::clamp(holdingZoom, 10.0f, 20.0f);

			ComponentHandle<MeshComponent> holded_mesh = objHolded->get<MeshComponent>();
			
			ComponentHandle<CubeCollider> holded_collider = objHolded->get<CubeCollider>();
			bool canDrop = true;
			
			//collision
			world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {
				
				if (other_ent == objHolded) {
					return;
				}
			
				ComponentHandle<Transform3D> other_transform = other_ent->get<Transform3D>();
				

				glm::vec3 otherMinPos = other_transform->position - glm::vec3(other_collider->width, other_collider->length, other_collider->height) * 0.5f;
				glm::vec3 otherMaxPos = other_transform->position + glm::vec3(other_collider->width, other_collider->length, other_collider->height) * 0.5f;
				//code to check collions
				glm::vec3 holdedMinPos = holded_transform->position - glm::vec3(holded_collider->width, holded_collider->length, holded_collider->height) * 0.5f;
				glm::vec3 holdedMaxPos = holded_transform->position + glm::vec3(holded_collider->width, holded_collider->length, holded_collider->height) * 0.5f;
					
				
				if ((otherMinPos.x <= holdedMaxPos.x && otherMaxPos.x >= holdedMinPos.x) &&
					(otherMinPos.y <= holdedMaxPos.y && otherMaxPos.y >= holdedMinPos.y) &&
					(otherMinPos.z <= holdedMaxPos.z && otherMaxPos.z >= holdedMinPos.z)) {
					canDrop = false;
					cout << "mesh RED!" << endl;
					/*
					glUseProgram(holded_mesh->meshFilepath);
					GLint redOnly = glGetUniformLocation(holded_mesh, "redOnly");
					if (redOnly != -1) {
						glUniform1i(redOnly, 1); // Enable red-only mode
					}
					*/
					
					return;
						
				} else {
					//holded_sprite->color = glm::vec3(1.f, 1.f, 1.f);
					
				}
			});

			if (!canDrop) {
				holded_mesh->color = glm::vec3(1., 0., 0.);
			}
			else {
				holded_mesh->color = glm::vec3(1., 1., 1.);
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && canDrop) {
				holded_transform->position.y = 0.f + holded_collider->height * 0.5f;
				holdingObj = false;
				//objHolded = nullptr;
				//float fallSpeed = 0.1f;
				//holded_transform->position.y = glm::mix(holded_transform->position.y, 0.f, 0.1f);
				//holded_transform->position.y -= fallSpeed * deltaTime;
				//if (holded_transform->position.y < 0.1f) {
				//	holded_transform->position.y = 0.f;
				//	holdingObj = false;
				//	//objHolded = nullptr;
				//}


			}

	}

	if (lightScript->brightness == 0.01f && currentPosition.x > 25.5 && currentPosition.x < 71) {
		cam->position = initialPos;
		lightScript->openLight();
		if (holdingObj) {
			ComponentHandle<Transform3D> holded_transform = objHolded->get<Transform3D>();
			holded_transform->position = glm::vec3(80, 2, 45); //restart position of the dynamite
			objHolded = nullptr;
			holdingObj = false;
		}
	}
	if (currentPosition.x > 71) {
		lightScript->openLight();
	}
	
}





