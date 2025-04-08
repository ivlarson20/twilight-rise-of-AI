/**
* Author: [Isabelle Larson]
* Assignment: Rise of the AI
* Date due: 2025-04-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~LevelB();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
