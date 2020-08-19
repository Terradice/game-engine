#pragma once
#include "../Texture2D.h"
#include "../GLM/glm.hpp"
#include "../Shaders/ShaderProgram.h"

class RectRenderer {
    public:
        static void Init();
        static void DrawRect(glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
        float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f),
        bool flipped = false);
        static ShaderProgram shader;
    private:
        static GLuint quadVAO;
};