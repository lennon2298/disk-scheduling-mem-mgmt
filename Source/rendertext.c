#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <kazmath/kazmath.h>

#include "rendertext.h"

//Vertex Shader
const char *vertShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
        "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "TexCoords = vertex.zw;\n"
    "}\0";

//Fragment Shader
const char *fragShaderSource = "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main() {\n" 
        "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
        "color = vec4(textColor, 1.0) * sampled;\n"
    "}\0";


struct Character {
    GLuint TextureID;
    kmVec2 Size;
    kmVec2 Bearing;
    GLuint Advance;
};

struct Character character[128];

GLuint VAOTEXT, VBOTEXT;

unsigned int shaderProgramText;
int vertexShaderText, fragmentShaderText;
int success;
char infoLog[512];

void initTextGlyphs()
{
    // vertex shader
    vertexShaderText = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderText, 1, &vertShaderSource, NULL);
    glCompileShader(vertexShaderText);
    // check for shader compile errors
    success;
    infoLog[512];
    glGetShaderiv(vertexShaderText, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderText, 512, NULL, infoLog);
        printf ("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);

    }
    // fragment shader
    fragmentShaderText = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderText, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShaderText);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderText, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderText, 512, NULL, infoLog);
        printf ("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);
    }

    // link shaders
    shaderProgramText = glCreateProgram();
    glAttachShader(shaderProgramText, vertexShaderText);
    glAttachShader(shaderProgramText, fragmentShaderText);
    glLinkProgram(shaderProgramText);
    // check for linking errors
    glGetProgramiv(shaderProgramText, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramText, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShaderText);
    glDeleteShader(fragmentShaderText);

    glUseProgram(shaderProgramText);

    kmMat4 projection;
    kmMat4OrthographicProjection(&projection, 0.0f, 800.0f, 0.0f, 450.0f, 0.0f, 100.0f);
    float *projPointer = valPtr(&projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramText, "projection"), 1, GL_FALSE, projPointer);

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        {
            printf("ERROR::FREETYPE: Could not init FreeType Library");
        }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "../Source/Resources/fonts/font.ttf", 0, &face))
        {
            printf("ERROR::FREETYPE: Failed to load font");
        }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        kmVec2 size, bearing;
        bearing.x = face->glyph->bitmap_left;
        bearing.y = face->glyph->bitmap_top;
        size.x = face->glyph->bitmap.width;
        size.y = face->glyph->bitmap.rows;
        character[c].TextureID = texture;
        character[c].Size = size;
        character[c].Bearing = bearing;
        character[c].Advance = face->glyph->advance.x;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOTEXT);
    glGenBuffers(1, &VBOTEXT);
    glBindVertexArray(VAOTEXT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEXT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderText( char *text, GLfloat x, GLfloat y, GLfloat scale, kmVec3 color)
{
    glUseProgram(shaderProgramText);
    glUniform3f(glGetUniformLocation(shaderProgramText, "textColor"), color.x, color.y, color.z);
    //glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOTEXT);

    // Iterate through all characters
    //std::string::const_iterator c;
    int c=0;
    while(text[c]) 
    {
        int n = text[c];
        struct Character ch = character[n];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glActiveTexture(GL_TEXTURE0);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOTEXT);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        c++;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float* valPtr(kmMat4 *matrix)
{
    return &(matrix->mat[0]);
}

void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  

void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
} 