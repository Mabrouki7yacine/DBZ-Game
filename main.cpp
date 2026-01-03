#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include "raylib.h"
#include "Person.h"
#include "Button.h"
#include "Characters.h"

#define ArenafontSize 28

std::string resource_path = "/home/yacine/Learning/Game_Dev/Screw_it_lets_make_a_game/Game_V1/Resources/";

typedef enum{
    WELCOME,
    GAME,
    GAME_OVER,
    CHARACTERS,
    ARENA,
    FINISH
}events;

int centerTextX(const char* text, int rectX, int rectWidth, int fontSize) {
    int textWidth = MeasureText(text, fontSize);
    return rectX + (rectWidth - textWidth) / 2;
}

int main() {
    // Initialize window
    int width = 640;
    int height = 480;
    InitWindow(width, height, "DBZ Game");
    InitAudioDevice();
    SetTargetFPS(60);

    
    // Load your existing resources
    const std::string menu_path     = resource_path + "menu.png";
    const std::string char_path     = resource_path + "Characters.png";
    const std::string aren_path     = resource_path + "arena.png";

    const std::string warrior_path  = resource_path + "Warrior.png";
    const std::string nature_path   = resource_path + "Nature.jpg";
    const std::string namek_path    = resource_path + "Namek.png";
    const std::string gc_path       = resource_path + "Grand_canyon.png";

    const std::string swarrior_path  = resource_path + "smallw.png";
    const std::string snature_path   = resource_path + "smallt.png";
    const std::string snamek_path    = resource_path + "smalln.png";
    const std::string sgc_path       = resource_path + "smallgc.png";
    
    Texture2D character_menu = LoadTexture(char_path.c_str());
    Texture2D arena_menu     = LoadTexture(aren_path.c_str());
    Texture2D menu           = LoadTexture(menu_path.c_str());
    Texture2D arena          = LoadTexture(nature_path.c_str());

    Texture2D sGrand_canyon = LoadTexture(sgc_path.c_str());
    Texture2D sWarrior      = LoadTexture(swarrior_path.c_str());
    Texture2D sNature       = LoadTexture(snature_path.c_str());
    Texture2D sNamek        = LoadTexture(snamek_path.c_str());
    
    const std::string click_sound_path = resource_path + "click.wav";
    const std::string jump_sound_path = resource_path + "hit.wav";
    const std::string game_music_path = resource_path + "game.wav";

    Sound clickSound = LoadSound(click_sound_path.c_str());
    Music GameMusic = LoadMusicStream(game_music_path.c_str()); // Load your music file

    Person goku(goku_path, goku_resize, false, false, 24, 300);
    // Person trapking(trapking_path, trapking_resize, true, false, 550, 320);
    // Menu state
    events state = WELCOME;
    bool startHovered = false, charHovered = false, arenaHovered = false, exitHovered = false;
    
    // Define button rectangles
    Rectangle startBtn  = { 200, 180, 200, 50 };
    Rectangle charBtn   = { 200, 250, 200, 50 };
    Rectangle arenaBtn  = { 200, 320, 200, 50 };
    Rectangle exitBtn   = { 200, 390, 200, 50 };
    
    bool jumping = false;
    bool finished = false;

    int GOWidth = MeasureText("GAME IS OVER", 48);
    int GOcounter = 0;
    int arena_chosen = 0;

    PlayMusicStream(GameMusic);
    bool gameMusicPlaying = false;

    while (!WindowShouldClose() & !finished) {
        BeginDrawing();

        if (state == WELCOME) {
            // Draw menu background
            DrawTexture(menu, 0, 0, WHITE);
            // Draw menu buttons
            if (DrawPixelButton(startBtn, "START", startHovered)) {
                goku.health = 3;
                goku.person_pos = {24, 300};
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                state = GAME;
            }
            if (DrawPixelButton(charBtn, "CHARACTER", charHovered)) {
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                state = CHARACTERS;
            }
            if (DrawPixelButton(arenaBtn, "ARENA", arenaHovered)) {
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                state = ARENA;
            }
            if (DrawPixelButton(exitBtn, "EXIT", exitHovered)) {
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                state = FINISH;
            }
            
        } else if (state == GAME) {
            if (!gameMusicPlaying) {
                PlayMusicStream(GameMusic);
                gameMusicPlaying = true;
            }            
            UpdateMusicStream(GameMusic); // Update music buffer with new stream data
            DrawTexture(arena, 0, 0, WHITE);
            
            if (IsKeyDown(KEY_LEFT)) {
                goku.move_left(3);
            } else if (IsKeyDown(KEY_RIGHT)) {
                goku.move_right(3);
            } else if (IsKeyDown(KEY_X)) {
                goku.hit();
            }
            if (IsKeyDown(KEY_SPACE)) {                
                goku.jump();
            } 
            if (IsKeyDown(KEY_F)) {
                gameMusicPlaying = false;
                StopMusicStream(GameMusic);
                state = WELCOME;
            } 
            if (IsKeyPressed(KEY_DOWN)) {
                goku.health--;
            }
            goku.show();
            if(goku.dead()){
                gameMusicPlaying = false;
                state = GAME_OVER;
                StopMusicStream(GameMusic);
            }
            // trapking.show();

        } else if (state == GAME_OVER) {
            DrawTexture(arena, 0, 0, WHITE);
            DrawText("GAME IS OVER",  320 - GOWidth/2, 100, 48, WHITE);
            GOcounter++;
            if (GOcounter == 60){
               state = WELCOME; 
               GOcounter = 0;
            }
        } else if (state == CHARACTERS) {
            DrawTexture(character_menu, 0, 0, WHITE);
            if (DrawPixelButton({ 220, 390, 200, 50 }, "Finish", exitHovered)) {
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                state = WELCOME;
            }
        } else if (state == ARENA) {
            DrawTexture(arena_menu, 0, 0, GRAY);

            if (IsKeyPressed(KEY_LEFT)) {
                arena_chosen =  (arena_chosen - 1);
            } else if (IsKeyPressed(KEY_RIGHT)) {
                arena_chosen =  (arena_chosen + 1);
            } else if (IsKeyPressed(KEY_UP)){
                arena_chosen =  (arena_chosen - 2);
            } else if (IsKeyPressed(KEY_DOWN)) {
                arena_chosen =  (arena_chosen + 2);
            }

            arena_chosen = std::clamp(arena_chosen, 0, (int) 4 -1);
            // Nature
            DrawRectangle(75, 20, 210, 160, WHITE);
            DrawTexture(sNature, 80, 25, GRAY);
            DrawText("Nature", centerTextX("Nature", 75, 210, ArenafontSize), 190, ArenafontSize, WHITE);

            // Namek
            DrawRectangle(355, 20, 210, 160, WHITE);
            DrawTexture(sNamek, 360, 25, GRAY);
            DrawText("Namek", centerTextX("Namek", 355, 210, ArenafontSize), 190, ArenafontSize, WHITE);

            // Grand Canyon
            DrawRectangle(75, 215, 210, 160, WHITE);
            DrawTexture(sGrand_canyon, 80, 220, GRAY);
            DrawText("Grand Canyon", centerTextX("Grand Canyon", 75, 210, ArenafontSize), 385, ArenafontSize, WHITE);

            // Warrior
            DrawRectangle(355, 215, 210, 160, WHITE);
            DrawTexture(sWarrior, 360, 220, GRAY);
            DrawText("Warrior", centerTextX("Warrior", 355, 210, ArenafontSize), 385, ArenafontSize, WHITE);
            switch (arena_chosen)
            {
            case 0:
                DrawTexture(sNature,  80,  25, WHITE);
                break;
            case 1:
                DrawTexture(sNamek, 360,  25, WHITE);
                break;
            case 2:
                DrawTexture(sGrand_canyon,  80, 220, WHITE);
                break;
            case 3:
                DrawTexture(sWarrior, 360, 220, WHITE);
                break;
            default:
                break;
            }
            
            if (DrawPixelButton({ 220, 415, 200, 50 }, "Finish", exitHovered) or IsKeyDown(KEY_ENTER)) {
                PlaySound(clickSound);
                std::this_thread::sleep_for(std::chrono::milliseconds(370));
                goku.set_theme(WHITE);
                switch (arena_chosen)
                {
                case 0:
                    arena = LoadTexture(nature_path.c_str());
                    break;
                case 1:
                    arena = LoadTexture(namek_path.c_str());
                    break;
                case 2:
                    arena = LoadTexture(gc_path.c_str());
                    break;
                case 3:
                    arena = LoadTexture(warrior_path.c_str());
                    goku.set_theme(LIGHTGRAY);
                    break;
                default:
                    break;
                }
                state = WELCOME;
            }

        } else if (state == FINISH) {
            finished = true;
        }

        EndDrawing();
    }
    
    // Cleanup
    UnloadTexture(menu);
    UnloadTexture(arena);
    UnloadTexture(arena_menu);
    UnloadTexture(character_menu);

    UnloadTexture(sGrand_canyon);
    UnloadTexture(sWarrior);
    UnloadTexture(sNature);
    UnloadTexture(sNamek);

    UnloadSound(clickSound);
    StopMusicStream(GameMusic); 
    UnloadMusicStream(GameMusic); 

    goku.free_resources();
    // trapking.free_resources();
    CloseWindow();
    CloseAudioDevice();
    
    return 0;
}