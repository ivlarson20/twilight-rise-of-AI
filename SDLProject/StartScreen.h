
#include "Scene.h"
#include "Utility.h"

class StartScreen : public Scene {
public:
    int ENEMY_COUNT = 0;
    
    ~StartScreen();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
};
