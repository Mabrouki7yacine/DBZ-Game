#include "Person.h"

const std::string heart_path  = "/home/yacine/Learning/Game_Dev/Screw_it_lets_make_a_game/Game_V1/Resources/heart.png";
Texture2D Heart_texture;


std::string getCharacterName(const std::string& path) {
    std::string keyword = "/Resources/";
    size_t start = path.find(keyword);
    if (start == std::string::npos) return "";
    start += keyword.length();
    size_t end = path.find('/', start);
    return path.substr(start, end - start);
}

int calc_num_files(std::string &folderPath){
    try {
        int fileCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                fileCount++;
            }
        }
        return fileCount;
    } catch (const std::exception& e) {
        std::cerr << "Error reading directory: " << e.what() << std::endl;
        return 0;
    }
}

Person::Person(const std::string& person_path,const float* resize_fac, bool Hflip, bool Vflip, int init_posx, int init_posy, int window_width, int window_height)
{
    this->init_pos = {init_posx, init_posy};
    this->person_pos = {init_posx, init_posy};
    this->window_width = window_width;
    this->window_height = window_height;

    this->Person_walk.resize_fac  = resize_fac[1];
    this->Person_hit.resize_fac  = resize_fac[2];

    std::string char_name = getCharacterName(person_path);
    std::string prs_img_path = person_path + "/" + char_name + ".png";

    this->Person_name = char_name;

    Image Person_image = LoadImage(prs_img_path.c_str());
    ImageResize(&Person_image, Person_image.width * resize_fac[0], Person_image.height * resize_fac[0]);
    if (Hflip) ImageFlipHorizontal(&Person_image);
    if (Vflip) ImageFlipVertical(&Person_image);

    this->Person_texture = LoadTextureFromImage(Person_image);
    UnloadImage(Person_image);

    std::string folder_path = person_path + "/" + char_name + "_walk/";
    int frame_count = calc_num_files(folder_path);
    this->Person_walk.frame_count = frame_count;

    for (int i = 0; i < frame_count; i++) {
        std::string frame_path = person_path + "/" + char_name + "_walk/" + std::to_string(i) + ".png";
        Image img = LoadImage(frame_path.c_str());
        ImageResize(&img, (img.width)*(this->Person_walk.resize_fac), (img.height)*(this->Person_walk.resize_fac));

        Texture2D tex_right = LoadTextureFromImage(img);
        this->Person_walk.walk_image_right.push_back(tex_right);

        ImageFlipHorizontal(&img);
        Texture2D tex_left = LoadTextureFromImage(img);
        this->Person_walk.walk_image_left.push_back(tex_left);

        UnloadImage(img);
    }

    folder_path = person_path + "/" + char_name + "_hit/";
    frame_count = calc_num_files(folder_path);
    this->Person_hit.frame_count = frame_count;

    for (int i = 0; i < frame_count; i++) {
        std::string frame_path = person_path + "/" + char_name + "_hit/" + std::to_string(i) + ".png";
        Image img = LoadImage(frame_path.c_str());
        ImageResize(&img, (img.width)*(this->Person_hit.resize_fac), (img.height)*(this->Person_hit.resize_fac));

        Texture2D tex = LoadTextureFromImage(img);
        this->Person_hit.hit_image.push_back(tex);

        UnloadImage(img);
    }

    Image Heart_image = LoadImage(heart_path.c_str());
    ImageResize(&Heart_image, 52/2, 44/2);
    Heart_texture = LoadTextureFromImage(Heart_image);
    UnloadImage(Heart_image);
}

Person::~Person()
{
}

void Person::show()
{
    this->show_health();
    switch (this->action)
    {
    case isSTANDING:
            // DrawRectangle(person_pos.x, person_pos.y, Person_texture.height, Person_texture.width, RED);
            DrawTexture(Person_texture, person_pos.x, person_pos.y, this->Person_theme);
        break;
    case isRIGHT:
            this->show_right();
            this->action = isSTANDING;
        break;
    case isLEFT:
            this->show_left();
            this->action = isSTANDING;
        break;
    case isHITING:
            this->show_hit();
        break;
    
    default:
        break;
    }
}

void Person::show_right()
{
    Person_walk.delay = (Person_walk.delay + 1) % 6;
    if (Person_walk.delay == 0) {
        Person_walk.current_frame = (Person_walk.current_frame + 1) % Person_walk.frame_count;
    }

    DrawTexture(Person_walk.walk_image_right[Person_walk.current_frame], person_pos.x, person_pos.y, this->Person_theme);
}

void Person::show_left()
{
    Person_walk.delay = (Person_walk.delay + 1) % 6;
    if (Person_walk.delay == 0) {
        Person_walk.current_frame = (Person_walk.current_frame + 1) % Person_walk.frame_count;
    }

    DrawTexture(Person_walk.walk_image_left[Person_walk.current_frame], person_pos.x, person_pos.y, this->Person_theme);
}

void Person::show_hit(){
    DrawTexture(Person_hit.hit_image[Person_hit.current_frame], person_pos.x , person_pos.y, this->Person_theme);
}

void Person::jump() {
    if (jumping) return; // Already jumping, ignore

    jumping = true;
    jumpThread = std::thread([this]() {
        int duration = 0;
        const int max_duration = 30; // ~0.5 sec
        float y;

        while (duration <= max_duration) {
            y = (vel * duration) - (0.5f * acc * duration * duration);
            this->person_pos.y = static_cast<int>(init_pos.y - y);
            this->person_pos.y = std::clamp(this->person_pos.y, 0, init_pos.y);
            duration++;

            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Simulate ~60 FPS physics
        }

        this->person_pos.y = init_pos.y;
        jumping = false;
    });
    jumpThread.detach(); // Let it run independently
}


void Person::hit()
{
    if(this->hitting) return;

    this->action = isHITING;
    this->hitting = true;
    hitThread = std::thread([this]() {

        int duration = 0;
        const int max_duration = this->Person_hit.frame_count;

        while (duration <= max_duration * 6 ) {
            duration++;
            // std::cout << this->Person_hit.current_frame << std::endl;
            this->Person_hit.delay = (this->Person_hit.delay + 1) % 6;
            if (this->Person_hit.delay == 0) {
                this->Person_hit.current_frame = (this->Person_hit.current_frame + 1) % this->Person_hit.frame_count;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Simulate ~60 FPS physics
        }
        this->hitting = false;
        this->action = isSTANDING;
    });
    hitThread.detach(); // Let it run independently
}

void Person::move_right(int speed)
{   
    this->action = isRIGHT;
    person_pos.x += speed;
    person_pos.x = std::clamp(person_pos.x, 0, window_width - Person_texture.width);

}

void Person::move_left(int speed)
{
    this->action = isLEFT;
    person_pos.x -= speed;
    person_pos.x = std::clamp(person_pos.x, 0, window_width - Person_texture.width);
}

void Person::move_down(int speed)
{
    person_pos.y += speed;
    person_pos.y = std::clamp(person_pos.y, 0, init_pos.y);
    DrawTexture(Person_texture, person_pos.x, person_pos.y, WHITE);
}

void Person::move_up(int speed)
{
    person_pos.y -= speed;
    person_pos.y = std::clamp(person_pos.y, 0, init_pos.y);
    DrawTexture(Person_texture, person_pos.x, person_pos.y, WHITE);
}

void Person::free_resources()
{
    UnloadTexture(Person_texture);
    UnloadTexture(Heart_texture);

    for (auto& tex : Person_walk.walk_image_right) {
        UnloadTexture(tex);
    }
    for (auto& tex : Person_walk.walk_image_left) {
        UnloadTexture(tex);
    }
    for (auto& tex : Person_hit.hit_image) {
        UnloadTexture(tex);
    }

    Person_walk.walk_image_right.clear();
    Person_walk.walk_image_left.clear();
    Person_hit.hit_image.clear();
}

int Person::person_image_x()
{
    return Person_texture.width;
}

int Person::person_image_y()
{
    return Person_texture.height;
}

void Person::show_health()
{
    DrawText(this->Person_name.c_str(), 10, 2, 30, WHITE);
    for (int i = 0; i <this->health; i++){
        DrawTexture(Heart_texture, 10 + i * Heart_texture.width, 40, WHITE);
    }
}

bool Person::dead()
{
    if(this->health == 0){
        return true;
    }
    return false;
}

void Person::set_theme(Color theme){
    this->Person_theme = theme;
}