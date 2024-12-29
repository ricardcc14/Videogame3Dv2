#include "VideoPlayer.h"
#include <fstream>
#include <iostream>
#include <iomanip>
//#include <sstream>


void VideoPlayer::startScript()
{
    string generalDirectory = "Textures/videogame_intro/";
    
    //std::vector<std::string> pathStorage;
    int totalFrames = 626; //51; //626
    float currentFrameDuration = 0;
    for (int i = 0; i < totalFrames; i++) {

        std::ostringstream oss;
        oss << generalDirectory << "frame_" << std::setw(4) << std::setfill('0') << i << ".png";
        std::string frameDir = oss.str();

       
        if (std::ifstream(frameDir)) {
        
            if (!framesData.empty() && currentFrameDuration > 0) {
                framesData.back().durationFrames += currentFrameDuration;
                currentFrameDuration = 0;
            }
            //pathStorage.push_back(frameDir);
           
            framesData.emplace_back(frameDir, 1);
        }
        else {
            currentFrameDuration++;
        }
    }
    if (!framesData.empty() && currentFrameDuration > 0) {
        framesData.back().durationFrames += currentFrameDuration;
    }
    for (int i = 0; i < framesData.size(); i++) {
        FrameData& frame = framesData[i];
        std::cout << "Frame " << i << " -> Path: " << frame.texturePath
            << ", Duration: " << frame.durationFrames << std::endl;
    }
}

void VideoPlayer::tickScript(float deltaTime)
{
    if (flag) {
        startScript();
        flag = false;
    }
    static int currentFramePos = 0;
    static int frameCounter = 0;
    static float frameTimeAccumulator = 0.0f;
    const float frameRate = 1.0f / 25.0f;
    
    frameTimeAccumulator += deltaTime*0.001;
    
    while (frameTimeAccumulator >= frameRate) {
        frameTimeAccumulator -= frameRate;
        
        if (currentFramePos < framesData.size()) {
            FrameData& currentFrame = framesData[currentFramePos];
            if (currentFrame.durationFrames > 1) {
                currentFrame.durationFrames--;
            }
            else {
                currentFramePos++;
            }
            
        }
    }
    if (currentFramePos < framesData.size()) {
        FrameData& currentFrame = framesData[currentFramePos];
        //cout << "Current path: " << currentFrame.texturePath << endl;
        ComponentHandle<Sprite> ent_sprite = entity->get<Sprite>();
        ent_sprite->filepath = currentFrame.texturePath.c_str();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        //entity->get<Sprite>()->visible = false;
        world->destroy(entity);
    }
}

