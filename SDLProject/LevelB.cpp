/**
* Author: [Isabelle Larson]
* Assignment: Rise of the AI
* Date due: 2025-04-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/bella.png";
constexpr char PLATFROM_FILEPATH[] = "assets/tile.png";
constexpr char ENEMY2_FILEPATH[] = "assets/vampire2.png";
constexpr char ENEMY3_FILEPATH[] = "assets/vampire3.png";
constexpr char BACKGROUND_FILEPATH[] = "assets/background1.jpg";
GLuint m_font_b;


unsigned int LEVELB_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

LevelB::~LevelB(){
    delete [] m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    delete m_game_state.background;
    
    // sound stuff
    
}

void LevelB::initialise(){
    m_font_b = Utility::load_texture("assets/font1.png");
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/tile.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 4, 1);
    
    int player_walking_animation[4][4] = {
        { 1, 5, 9, 13 },  // for George to move to the left,
        { 3, 7, 11, 15 }, // for George to move to the right,
        { 2, 6, 10, 14 }, // for George to move upwards,
        { 0, 4, 8, 12 }   // for George to move downwards
    };
    
    glm::vec3 acceleration = glm::vec3(0.0f,-4.905f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    
    m_game_state.player = new Entity(
                                     player_texture_id,         // texture id
                                     5.0f,                      // speed
                                     acceleration,              // acceleration
                                     3.0f,                      // jumping power
                                     player_walking_animation,  // animation index sets
                                     0.0f,                      // animation time
                                     4,                         // animation frame amount
                                     0,                         // current animation index
                                     4,                         // animation column amount
                                     4,                         // animation row amount
                                     0.75f,                      // width
                                     1.0f,                       // height
                                     PLAYER
                                     );
    
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_game_state.player->set_lives(3);
    
    m_game_state.player->set_jumping_power(3.0f);
    
    // enemies
    
    GLuint enemy_texture_id = Utility::load_texture(ENEMY2_FILEPATH);
    GLuint enemy2_texture_id = Utility::load_texture(ENEMY3_FILEPATH);

    
    m_game_state.enemies = new Entity[2];
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] =  Entity();
        m_game_state.enemies[i].set_speed(1.0f);
        m_game_state.enemies[i].set_width(1.0f);
        m_game_state.enemies[i].set_height(1.0f);
        m_game_state.enemies[i].set_entity_type(ENEMY);

        m_game_state.enemies[i].set_ai_state(IDLE);
        m_game_state.enemies[i].set_position(glm::vec3(4.0f, 0.0f, 0.0f));
        m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    }
    m_game_state.enemies[0].set_position(glm::vec3(6.0f, 0.0f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(3.0f, -5.0f, 0.0f));
    m_game_state.enemies[0].set_texture_id(enemy_texture_id); // brown hair
    m_game_state.enemies[1].set_texture_id(enemy2_texture_id); // blondie
    m_game_state.enemies[0].set_ai_type(WALKER);
    m_game_state.enemies[1].set_ai_type(GUARD);
    m_game_state.enemies[1].set_ai_state(WALKING);


  
    
    //background
   
   m_game_state.background = new Entity();
 //  m_game_state.background->set_lives(10000);
   m_game_state.background->set_position(glm::vec3(10.5f,-4.0f,0.0f));
   m_game_state.background->set_scale(glm::vec3(21.06f, 11.7f, 0.0f));
   m_game_state.background->m_texture_id = Utility::load_texture(BACKGROUND_FILEPATH);
   m_game_state.background->update(0.0f, m_game_state.background, NULL, 0, m_game_state.map);

    

    // add sound stuff
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/Night of the Owl.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(14.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/BounceYoFrankie.wav");
}

void LevelB::update(float delta_time){
    
  
    
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, 2, m_game_state.map);
    m_game_state.enemies[0].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    m_game_state.enemies[1].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
        
        if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 2;
    

}

void LevelB::render(ShaderProgram *program)
{
    m_game_state.background->render(program);

    
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < 2; i++)
            m_game_state.enemies[i].render(program);
    
    if (!m_game_state.player->m_is_active) {
        Utility::draw_text(program, m_font_b, "Game Over", 1.0f, -0.5f, glm::vec3(5.2f, -2.0f, 0.0f));
    } else {
        std::string lives_text = "Lives: " + std::to_string(m_game_state.player->m_lives);
     
        Utility::draw_text(program, m_font_b, lives_text, 1.0f, -0.65, glm::vec3(7.0f, -0.5f, 0.0f));
    }
}

