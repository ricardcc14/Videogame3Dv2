#pragma once

#include "Script.h"

struct FrameData {
    string texturePath;
    int durationFrames;
    FrameData(string path, int duration)
        : texturePath(path), durationFrames(duration) {}
};

class VideoPlayer : public Script
{

    using Script::Script;

public:
    
    void startScript() override;

    void tickScript(float deltaTime) override;



private:

    std::vector<FrameData> framesData;
    
    bool flag = true;
};