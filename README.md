# DBZ-Game
A simple 2D game prototype built in **C++** using **Raylib** and **CMake**.
Project Structure
Game_V1/
├── CMakeLists.txt
├── main.cpp
├── src/Person.cpp
├── Include/
│ ├── Button.h
│ ├── Characters.h
│ ├── Collision.h
│ └── Person.h
└── Resources/ (images, sounds, fonts, etc.)

Build & Run (Linux)
```bash
cmake . build/
cmake --build build/
./build/game_v1
