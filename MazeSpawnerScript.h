#pragma once

#include "Script.h"

class MazeSpawnerScript : public Script
{

    using Script::Script;

public:
    
    void startScript() override;

    void tickScript(float deltaTime) override;
    MazeSpawnerScript(GLFWwindow* window, World* world, vector<Entity*> wallsPool) {
       this->wallsPool = wallsPool;
    }
    
    void changeMaze();
private:

    float delay = 5000.f;
    float t = 0.;
    int counter = 0;
    vector<Entity*> wallsPool;
    vector<vector<string>> mazeMaps;
    
    vector<string> concatMaps(vector<string> spawn, vector<string> map, vector<string> objective);
};
