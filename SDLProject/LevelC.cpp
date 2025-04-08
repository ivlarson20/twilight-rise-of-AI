/**
* Author: [Isabelle Larson]
* Assignment: Rise of the AI
* Date due: 2025-04-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/bella.png";
constexpr char PLATFROM_FILEPATH[] = "assets/tile.png";
constexpr char ENEMY3_FILEPATH[] = "assets/wolf.png";
constexpr char BACKGROUND_FILEPATH[] = "assets/background1.jpg";
constexpr char EDWARD_FILEPATH[] = "assets/edward.png";
GLuint m_font_c;

unsigned int LEVELC_DATA[] =    // CHANGE THIS PATTERN
{
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

LevelC::~LevelC(){
    delete [] m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    delete m_game_state.background;

    
    // add music stuff
    Mix_FreeMusic(m_game_state.bgm);
    
}



void LevelC::initialise(){
    m_font_c = Utility::load_texture("assets/font1.png");
    
    GLuint map_texture_id = Utility::load_texture("assets/tile.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 4, 1);

    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    int player_walking_animation[4][4] = {
        { 1, 5, 9, 13 },  // for George to move to the left,
        { 3, 7, 11, 15 }, // for George to move to the right,
        { 2, 6, 10, 14 }, // for George to move upwards,
        { 0, 4, 8, 12 }   // for George to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f,-4.905f, 0.0f);

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

    m_game_state.player->set_jumping_power(3.0f);

    // enemies

    GLuint enemy_texture_id = Utility::load_texture(ENEMY3_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, WOLF, IDLE);
        m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
        m_game_state.enemies[i].set_position(glm::vec3(4.0f, -3.0f, 0.0f));
    }
    m_game_state.enemies[0].m_left_bound = 2.0f;
    m_game_state.enemies[0].m_right_bound = 6.0f;
    m_game_state.enemies[0].m_wolf_direction = -1;
    m_game_state.enemies[0].m_wolf_speed = 3.0f;
    m_game_state.enemies[0].set_speed(m_game_state.enemies[0].m_wolf_speed);

    m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].update(0, &m_game_state.enemies[0], NULL, 0, m_game_state.map);

    m_game_state.background = new Entity();
  //  m_game_state.background->set_lives(10000);
    m_game_state.background->set_position(glm::vec3(10.5f,-4.0f,0.0f));
    m_game_state.background->set_scale(glm::vec3(21.06f, 11.7f, 0.0f));
    m_game_state.background->m_texture_id = Utility::load_texture(BACKGROUND_FILEPATH);
    m_game_state.background->update(0.0f, m_game_state.background, NULL, 0, m_game_state.map);
    
    
    // edward
    m_game_state.edward = new Entity();
    m_game_state.edward->set_position(glm::vec3(6.0f, 3.0f, 0.0f));
    m_game_state.edward->set_entity_type(EDWARD);
    m_game_state.edward->m_texture_id = Utility::load_texture(EDWARD_FILEPATH);


    // ADD IN SOUND STUFF

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/Night of the Owl.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(10.0f);
    
}

void LevelC::update(float delta_time)
{
    Scene::update(delta_time);

    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);

    
}


void LevelC::render(ShaderProgram *g_shader_program)
{
    m_game_state.background->render(g_shader_program);
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < m_number_of_enemies; i++)
            m_game_state.enemies[i].render(g_shader_program);
    m_game_state.edward->render(g_shader_program);
    
    if (!m_game_state.player->m_is_active) {
        Utility::draw_text(g_shader_program, m_font_c, "Game Over", 1.0f, -0.65f, glm::vec3(-3.2f, 0.0f, 0.0f));
    } else {
        std::string lives_text = "Lives: " + std::to_string(m_game_state.player->m_lives);
     
        Utility::draw_text(g_shader_program, m_font_c, lives_text, 1.0f, -0.65, glm::vec3(7.0f, -0.5f, 0.0f));
    }
    
   
    
}




