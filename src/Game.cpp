#include "Game.h"
#include "Rendering/SpriteRenderer.h"
#include "Shaders/ShaderProgram.h"
#include "Texture2D.h"
#include "GLM/gtc/matrix_transform.hpp"
#include <random>
#include <math.h>
#include <GLFW/glfw3.h>
#include "Rendering/TextRenderer.h"
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <fstream>
#include <tmx.h>
#include "Rendering/RectRenderer.h"
#include "IMGUI/imgui.h"

Game::Game(int w, int h) {
    width = w;
    height = h;
}

Uint8 *audio_pos;
Uint32 audio_len;

void audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
	//SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
}

void clicked() {
    printf("clicked!\n");
}

void Game::Init() {

    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height,
    0.0f, -1.0f, 1.0f);

    float dist = (float)sqrt(1 / 3.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(dist, dist, dist),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    view = glm::mat4(1.0f);
    // view = glm::rotate(view, 35.264f, glm::vec3(1.0f, 0.0f, 0.0f));
    // view = glm::rotate(view, -45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // ****

    // SDL_Init(SDL_INIT_AUDIO);
    // Uint32 wav_length;
    // Uint8 *wav_buffer;
    // SDL_AudioSpec wav_spec;
    // SDL_LoadWAV("assets/song.wav", &wav_spec, &wav_buffer, &wav_length);
    // wav_spec.callback = audio_callback;
    // wav_spec.userdata = NULL;
    // audio_pos = wav_buffer;
    // audio_len = wav_length;
    // SDL_OpenAudio(&wav_spec, NULL);

    // SDL_PauseAudio(0);

    // ****

    SpriteRenderer::Init();
    TextRenderer::Init(width, height);
    RectRenderer::Init();

    ShaderProgram texture = ShaderProgram("assets/screen.vs", "assets/screen.fs");
    texture.bind();
    texture.setMat4("projection", projection);
    texture.setMat4("view", view);
    RectRenderer::shader = texture;
    SpriteRenderer::shader = texture;

    ShaderProgram text = ShaderProgram("assets/text.vs", "assets/text.fs");
    text.bind();
    text.setMat4("projection", projection);
    text.setInt("text", 0);
    TextRenderer::TextShader = text;
    TextRenderer::Load("assets/Hack-Regular.ttf", 48);


    checker1 = Texture2D::loadTextureFromFile("assets/grass.png", true);
    checker2 = Texture2D::loadTextureFromFile("assets/brick.png", true);
    bot_back = Texture2D::loadTextureFromFile("assets/bot_back.png", true);
    bot_front = Texture2D::loadTextureFromFile("assets/bot_front.png", true);
    bot_right = Texture2D::loadTextureFromFile("assets/bot_right.png", true);
    bot_left = Texture2D::loadTextureFromFile("assets/bot_left.png", true);
    bot_front_left = Texture2D::loadTextureFromFile("assets/bot_front_left.png", true);
    bot_front_right = Texture2D::loadTextureFromFile("assets/bot_front_right.png", true);
    bot_back_right = Texture2D::loadTextureFromFile("assets/bot_back_right.png", true);
    bot_back_left = Texture2D::loadTextureFromFile("assets/bot_back_left.png", true);

    playerPos = glm::vec2(width/2, height/2);
    playerTexture = bot_front;

    m = tmx_load("assets/untitled.tmx");
    printf("%i, %i\n", m->width, m->height);
}

void Game::ProcessInput(GLFWwindow* window) {
    bool keyExit = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
    bool keyRight = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    bool keyLeft = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    bool keyUp = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    bool keyDown = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
    float speed = 3.5f;
    if(keyExit) glfwSetWindowShouldClose(window, true);
    if(keyLeft)                 playerTexture = bot_back_left;
    if(keyUp)                   playerTexture = bot_back_right;
    if(keyRight)                playerTexture = bot_front_right;
    if(keyDown)                 playerTexture = bot_front_left;
    if(keyUp && keyLeft)        playerTexture = bot_back;
    if(keyDown && keyRight)     playerTexture = bot_front;
    if(keyRight && keyUp)       playerTexture = bot_right;
    if(keyLeft && keyDown)      playerTexture = bot_left;
    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    mousePos.x = xpos;
    mousePos.y = ypos;
    glm::vec2 newPos = playerPos;

    if(keyRight && keyUp && !keyLeft) {
        newPos.x += speed/2;
        newPos.y -= speed/2;
    } 
    if(keyRight && keyDown && !keyLeft) {
        newPos.x += speed/2;
        newPos.y += speed/2;
    }
    if(keyLeft && keyUp && !keyRight) {
        newPos.x -= speed/2;
        newPos.y -= speed/2;
    } 
    if(keyLeft && keyDown && !keyRight) {
        newPos.x -= speed/2;
        newPos.y += speed/2;
    }
    if(keyLeft && !keyDown && !keyUp && !keyRight)     newPos.x -= speed;
    if(keyRight && !keyDown && !keyUp && !keyLeft)    newPos.x += speed;
    if(keyUp && !keyDown && !keyLeft && !keyRight)       newPos.y -= speed;
    if(keyDown && !keyLeft && !keyUp && !keyRight)     newPos.y += speed;
    
    playerPos = newPos;
    // float isoX = (newPos.x*TILEWIDTH/2 - newPos.y*TILEHEIGHT/2) + width/2;
    // float isoY = (newPos.x*TILEWIDTH/2 + newPos.y*TILEHEIGHT/2) / 2;
    // float cX = (newPos.x + 2*newPos.y) / TILEWIDTH;
    // float cY =  (2*newPos.y - newPos.x) / TILEHEIGHT;
    // printf("%f, %f\n", cX, cY);
    // int tileX = round(newPos.x/TILEWIDTH)+2;
    // int tileY = round(newPos.y/TILEHEIGHT)+1;
    // unsigned int pgid = (m->ly_head->content.gids[(tileX*m->width)+tileY]) & TMX_FLIP_BITS_REMOVAL;
    // if(m->tiles[pgid] != NULL) {
    //     playerPos = newPos;
    // }
    // if(keyRight)                playerPos.x += speed;
    // if(keyLeft)                 playerPos.x -= speed;
    // if(keyUp)                   playerPos.y -= speed;
    // if(keyDown)                 playerPos.y += speed;
} 

void Game::Update() {

}

void Game::Render() {
    std::stringstream str;
    int tileX = round(playerPos.x/TILEWIDTH);
    int tileY = round(playerPos.y/TILEHEIGHT);
    str << "X: " << tileX << " Y: " << tileY;

    int playerX = (playerPos.x - playerPos.y) + width/2;
    int playerY = (playerPos.x + playerPos.y) / 2;

    // SpriteRenderer::DrawSprite(checker2, glm::vec2(round(mousePos.x/TILEWIDTH)*TILEWIDTH, round(mousePos.y/TILEHEIGHT)*TILEHEIGHT),
    // glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);

    float pX = (tileX*TILEWIDTH - tileY*TILEHEIGHT) + width/2;
    float pY = (tileX*TILEWIDTH + tileY*TILEHEIGHT) / 4;
    
    TextRenderer::DrawText(str.str(), 20, 60, 0.5f, glm::vec3(0.0f));
    // unsigned int pgid = (m->ly_head->content.gids[(tileX*m->width)+tileY]) & TMX_FLIP_BITS_REMOVAL;
    // m->tiles[pgid]->id = 0;
    for(int x = 0; x < m->width; x++) {
        for(int y = 0; y < m->height; y++) {
            float isoX = (x*TILEWIDTH/2 - y*TILEHEIGHT/2)+width/2;
            float isoY = (x*TILEWIDTH/2 + y*TILEHEIGHT/2) / 2;
            unsigned int gid = (m->ly_head->content.gids[(x*m->width)+y]) & TMX_FLIP_BITS_REMOVAL;
            if(m->tiles[gid] != NULL) {
                // printf("%i, %i, %i\n", x, y, m->tiles[gid]->id);
                if(m->tiles[gid]->id == 0) {
                    SpriteRenderer::DrawSprite(checker2, glm::vec2(isoX, isoY),
                    glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);
                } else if(m->tiles[gid]->id == 1) {
                    SpriteRenderer::DrawSprite(checker1, glm::vec2(isoX, isoY),
                    glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);     
                } else {
                    // printf("%i\n", tilemap[x][y]);
                }
                
                // if(tileX == x && tileY == y) {
                    // std::stringstream pos;
                    // pos << "X: " << x << " Y: " << y;

                    // TextRenderer::DrawText(pos.str(), 20, 80, 0.5f, glm::vec3(0.0f));
                    // SpriteRenderer::DrawSprite(checker2, glm::vec2(pX, pY),
                    // glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);
                // }

                // if(pX > isoX && pX < isoX+TILEWIDTH/2 &&
                //     pY > isoY && pY < isoY+TILEHEIGHT/2) {
                //     SpriteRenderer::DrawSprite(checker2, glm::vec2(isoX, isoY),
                //     glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);    
                // }
            }

            // if (abs(dx-x*TILEWIDTH)/(TILEWIDTH/2) + abs(dy-y*TILEHEIGHT)/(TILEHEIGHT*0.5) <= 1) { 
                // SpriteRenderer::DrawSprite(checker2, glm::vec2(isoX, isoY),
                // glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);  
            // };
            // if(isoX == tileX && isoY == tileY) { 
            // }
        }
    }

    // SpriteRenderer::DrawSprite(checker2, glm::vec2(pX, pY),
    // glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);

    SpriteRenderer::DrawSprite(playerTexture, glm::vec2(playerX, playerY), glm::vec2(64, 64));

    ImGui::Text("Hello, world %f", ImGui::GetIO().Framerate);
    // ImGui::Checkbox("Save", &t);
    // if (t)
        // ImGui::Text("clicked the button");
        // clicked();
    // ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::Render();
    // unsigned int pgid = (m->ly_head->content.gids[(playerX*m->width)+playerY]) & TMX_FLIP_BITS_REMOVAL;
    // if(m->tiles[pgid] != NULL) {
    //     SpriteRenderer::DrawSprite(checker2, glm::vec2(playerX, isoY),
    //     glm::vec2(TILEWIDTH, TILEHEIGHT), 0.0f);    
    // }
}