#include "MazeSpawnerScript.h"

void MazeSpawnerScript::startScript()
{
	vector<string> spawn = {
		"#########################",
		"#-----------------------#",
		"#-----------------------#",
		"#-----------------------#",
		"#-----------------------#",
		"#-----------------------#",
	};
	vector<string> objective = {
		"#-----------------------#",
		"#-----------------------#",
		"#-----------------------#",
		"#########################"
	};
	vector<string> lab_map1 = {
		"#-----------------------#",
		"#-#######-#############-#",
		"#-#-----#-#-----------#-#",
		"#-#-###-#-#-#######-###-#",
		"#-#---#-#-#-------#---#-#",
		"#-###-#-#-#######-#-###-#",
		"#---#-#-#-------#-#-----#",
		"#-###-###########-#######",
		"#-#---------------------#",
		"#-###-###########-#####-#",
		"#-----#---------#-------#",
		"#-#######-#####-#######-#",
		"#-----------------------#",
	};
	vector<string> map1 = concatMaps(spawn, lab_map1, objective);
	
	vector<string> lab_map2 = {
		"#-----------------------#",
		"#-###########-#########-#",
		"#-#---------#-#---------#",
		"#-###-#####-#-#-#######-#",
		"#---#---------#---------#",
		"#-###-#####-#-#########-#",
		"#-----#-----#-----------#",
		"#################-#######",
		"#-#---------------------#",
		"#-#-###############-###-#",
		"#-----------#---------#-#",
		"#-###-#####-#-#######-###",
		"#-----------------------#",
	};
	vector<string> map2 = concatMaps(spawn, lab_map2, objective);
	
	vector<string> lab_map3 = {
		"#-----------------------#",
		"#-###-###########-#####-#",
		"#-#-----#-------#-#-----#",
		"#-###-###-#####-#-------#",
		"#---#---#-----#-###-###-#",
		"#-#####-###-###-#-------#",
		"#-----#---#---------#---#",
		"#-###########-#####-#####",
		"#---------#-------------#",
		"###-#####-###-###-#####-#",
		"#-#---#-------#-----#---#",
		"#-#####-#####-###-###---#",
		"#-----------------------#",
	};
	vector<string> map3 = concatMaps(spawn, lab_map3, objective);

	vector<string> lab_map4 = {
		"#-----------------------#",
		"#-#####-###########-###-#",
		"#-#---------#-----------#",
		"#-#####-#####-#####-###-#",
		"#-----#-----#-----#-----#",
		"#-###-#####-###-#####-###",
		"###-------#---#---------#",
		"#-#########-#####-#####-#",
		"#-----------------------#",
		"#-#####-###-###-#######-#",
		"#-#---#---#-#-----------#",
		"#-#####-###-###-###-###-#",
		"#-----------------------#",
	};
	vector<string> map4 = concatMaps(spawn, lab_map4, objective);

	vector<string> lab_map5 = {
		"#-----------------------#",
		"#-#########-###########-#",
		"#-#---------#-------#---#",
		"#---#######-#######-###-#",
		"###-#---#-------#-------#",
		"#-###-#####-#####-#####-#",
		"#-----#---#-----#-------#",
		"#-###########-#########-#",
		"#-----------------------#",
		"#-###-#########-###-###-#",
		"#---#-----#-----#-------#",
		"#-#####-###-#####-#####-#",
		"#-----------------------#",
	};
	
	vector<string> map5 = concatMaps(spawn, lab_map5, objective);

	mazeMaps.push_back(map1);
	mazeMaps.push_back(map2);
	mazeMaps.push_back(map3);
	mazeMaps.push_back(map4);
	mazeMaps.push_back(map5);
}

void MazeSpawnerScript::tickScript(float deltaTime)
{
	startScript();
	t += deltaTime;

	
	if (t > delay) {
		cout << "spawn" << endl;
		t = 0;
	}
}
vector<string> MazeSpawnerScript::concatMaps(vector<string> spawn, vector<string> map, vector<string> objective) {
	vector<string> concatMap = spawn;
	concatMap.insert(concatMap.end(), map.begin(), map.end());
	concatMap.insert(concatMap.end(), objective.begin(), objective.end());
	return concatMap;
}
void MazeSpawnerScript::changeMaze() {
	int numMax = 5;
	int r = (rand() % numMax) + 1;
	vector<string> currentMap = mazeMaps.at(r);

	cout << "CurrentMap:" << r << endl;
	for (Entity* wallsGame : wallsPool) {
		ComponentHandle<Transform3D> wallGame_transform = wallsGame->get<Transform3D>();
		wallGame_transform->position = glm::vec3(-10.f);
		ComponentHandle<MeshComponent> wallGame_mesh = wallsGame->get<MeshComponent>();
		wallGame_mesh->visible = false;
	}
	int wallsIndex = 0;
	for (int i = 0; i < 23; i++) {
		for (int j = 0; j < 25; j++) {
			if (currentMap[i][j] == '#') {
				if (!wallsPool.empty()) {
					Entity* wallGame = wallsPool[wallsIndex];
					ComponentHandle<Transform3D> wallGame_transform = wallGame->get<Transform3D>();
					wallGame_transform->position = glm::vec3(i * 4, 0, j * 4);
					ComponentHandle<MeshComponent> wallGame_mesh = wallGame->get<MeshComponent>();
					wallGame_mesh->visible = true;
					wallsIndex++;
				}
				
			}
		}
	}
	
	for (int k = wallsIndex; k < wallsPool.size(); k++) {
		Entity* wallGame = wallsPool[k];
		ComponentHandle<Transform3D> wallGame_transform = wallGame->get<Transform3D>();
		wallGame_transform->position = glm::vec3(-10.f, -10.f, -10.f); // Move off-screen
		ComponentHandle<MeshComponent> wallGame_mesh = wallGame->get<MeshComponent>();
		wallGame_mesh->visible = false;
	}
	
}



