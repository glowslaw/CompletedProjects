#pragma once

#include <iostream>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "shaderprogram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};



class TextRenderer
{
public:
    std::map<char, Character> characters;
    unsigned int VAO, VBO;

	TextRenderer();
    void loadGlyphs(FT_Face face);
    void RenderText(ShaderProgram* shader, glm::mat4 projection, std::string text, float x, float y, float scale);

    float texCoords[12] = {
    0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 0.0f,  1.0f, 1.0f,  1.0f, 0.0f
    };
};

