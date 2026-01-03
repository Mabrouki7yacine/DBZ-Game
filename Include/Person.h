#ifndef PERSON_H
#define PERSON_H

#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <atomic>


#define isSTANDING 0
#define isJUMPING  1
#define isHITING   2
#define isRIGHT    3
#define isLEFT     4

class Person {
private:
    struct Pos {
        int x;
        int y;
    };

    struct Walking {
        int delay = 0;
        int frame_count = 0;
        int current_frame = 0;
        float resize_fac = 0.85f;
        std::vector<Texture2D> walk_image_left;
        std::vector<Texture2D> walk_image_right;
    };

    struct Hiting {
        int delay;
        int frame_count = 0;
        int current_frame = 0;
        float resize_fac = 0.40f;
        std::vector<Texture2D> hit_image;
    };

    int action = isSTANDING;
    std::mutex action_mtx;

    std::string Person_name;

    float vel = 10.0f;
    float acc = 0.7f;
    float duration = 0.0;
    int movy = 0;

    Pos init_pos;
    int window_width;
    int window_height;

    Texture2D Person_texture;
    Walking Person_walk;
    Hiting Person_hit;

    std::atomic<bool> jumping = false;
    std::thread jumpThread;

    std::atomic<bool> hitting = false;
    std::thread hitThread;

    Color Person_theme =  WHITE;

    // Rectangle person_box = {this->person_pos.x, this->person_pos.y, this->Person_image.width, this->Person_image.height};

    void show_right();
    void show_left();
    void show_hit();
    void show_health();

public:
    Person(const std::string& person_path, const float* resize_fac, bool Hflip, bool Vflip, int init_posx, int init_posy, int window_width = 640, int window_height = 480);
    ~Person();

    Pos person_pos;
    int health = 3;

    void jump();
    void move_right(int speed);
    void move_left(int speed);
    void move_up(int speed);
    void move_down(int speed);
    void show();
    void hit();
    bool dead();
    void set_theme(Color theme);
    void free_resources();
    int person_image_x();
    int person_image_y();
};

#endif
