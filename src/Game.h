#define MAPHEIGHT 16
#define MAPWIDTH 16
#define TILEHEIGHT 32
#define TILEWIDTH 32

#include "Texture2D.h"
#include <GLFW/glfw3.h>
#include "GLM/glm.hpp"
#include "SoundPlayer.h"
#include <tmx.h>


class Game {
    public:
        Game(int width, int height);
        ~Game();
        void Init();
        void Update();
        void Render();
        void ProcessInput(GLFWwindow* window);
    private:
        float f;
        SoundPlayer player;
        Texture2D checker1, checker2;
        Texture2D bot_front, bot_back, bot_left, bot_right, bot_front_right, bot_front_left,
        bot_back_right, bot_back_left;
        Texture2D playerTexture;
        glm::vec2 playerPos;
        glm::vec2 mousePos;
        bool cur = false;
        int width, height;
        tmx_map *m;
};