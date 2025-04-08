#include "StartScreen.h"
#include "Utility.h"

unsigned int MENU_DATA[] = { 0 };
GLuint m_font_texture_id;

StartScreen::~StartScreen(){
    delete [] m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    delete m_game_state.background;
}

void StartScreen::initialise() {
    GLuint map_texture_id = Utility::load_texture("assets/tile.png");
    m_game_state.map = new Map(1, 1, MENU_DATA, map_texture_id, 1.0f, 4, 1);
    
    m_font_texture_id = Utility::load_texture("assets/font1.png");
    
    m_game_state.background = new Entity();
  //  m_game_state.background->set_lives(10000);
    m_game_state.background->set_position(glm::vec3(0.0f,0.0f,0.0f));
    m_game_state.background->set_scale(glm::vec3(13.5f, 11.25f, 0.0f));
    m_game_state.background->m_texture_id = Utility::load_texture("assets/backgroundMain.png");
    m_game_state.background->update(0.0f, m_game_state.background, NULL, 0, m_game_state.map);
    
    m_game_state.player = new Entity();
    
    // add music stuff
}

void StartScreen::update(float delta_time) {
    
}

void StartScreen::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    m_game_state.background->render(program);
    
    Utility::draw_text(program, m_font_texture_id, std::string("Press Enter to Start"), 1.0f, -0.65f, glm::vec3(-3.2f, 0.0f, 0.0f));
   
}
