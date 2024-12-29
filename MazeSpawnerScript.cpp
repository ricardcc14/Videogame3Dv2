#include "MazeSpawnerScript.h"

void MazeSpawnerScript::startScript()
{
	
	vector<string> map1 = {
	"#########################",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#-----------------------#",
	"#########################"
	};
		
	vector<string> map2 = {
			"#########################",
			"#-----------------------#",
			"#-----------------------#",
			"#-----------------------#",
			"#-----------------------#",
			"#-----------------------#",
			"#-#####################-#",
			"#-#-#################-#-#",
			"#-#-#---------------#-#-#",
			"#-#-#-###########-###-#-#",
			"#########################",
			"#-#-#---------#-----#-#-#",
			"#-#-#-#######-#####-#-#-#",
			"#-#-#-#-------#-----#-#-#",
			"#-#-###-#######-#####-#-#",
			"#-#-----#---------------#",
			"#-#####-###########-#####",
			"#----------#-----#------#",
			"#-#-#########-#####-#####",
			"#-#-#---------#---------#",
			"#-#-#-#######-#######-###",
			"#-#-#---------#---------#",
			"#-###-#########-#########",
			"#---#-------------------#",
			"###-###-###############-#",
			"#-----------------------#",
			"#-----------------------#",
			"#-----------------------#",
			"#########################"
	};
	mazeMaps.push_back(map1);
	mazeMaps.push_back(map2);
}

void MazeSpawnerScript::tickScript(float deltaTime)
{
	startScript();
	t += deltaTime;

	if (t > delay) {
		cout << "spawn" << endl;
		t = 0;

		int numMax = 5;
		int r = (rand() % numMax) + 1;
		vector<string> currentMap = mazeMaps.at(1);
		
		
		//counter += 1;
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				if (currentMap[i][j] == '#') {
					if (!wallsPool.empty()) {
						Entity* wallGame = wallsPool.back();
						ComponentHandle<Transform3D> wallGame_transform = wallGame->get<Transform3D>();
						wallGame_transform->position = glm::vec3(i * 4, 0, j * 4);
					}
					if(!wallsPool.empty()) wallsPool.pop_back();
				}
			}
		}
	}
}
