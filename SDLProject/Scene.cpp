// ADD HEADER

#include "Scene.h"

void Scene::update(float delta_time){
    Entity *player = m_game_state.player;
    
    // If player is out of lives, stop processing
        if (!player->m_is_active) return;

        // Check collisions with enemies
        for (int i = 0; i < m_number_of_enemies; i++) {
            Entity &enemy = m_game_state.enemies[i];

            if ((enemy.get_ai_type() == GUARD ||enemy.get_ai_type() == WALKER || enemy.get_ai_type() == WOLF) &&
                player->check_collision(&enemy)) {
                
                player->lose_life();
                break; // Avoid multiple losses in one frame
            }
        }
}
    
    

