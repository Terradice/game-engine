#include "../GLAD/glad.h"
#include "../GLM/glm.hpp"
#include <map>
#include "../Shaders/ShaderProgram.h"

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer {
    public:
        static std::map<GLchar, Character> Characters;
        static void Init(GLuint width, GLuint height);
        static ShaderProgram TextShader;
        static void Load(std::string font, GLuint fontSize);
        static void DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale,
        glm::vec3 color = glm::vec3(1.0f));
        static GLuint VAO, VBO;
};