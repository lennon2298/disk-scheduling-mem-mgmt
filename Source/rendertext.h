#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <kazmath/kazmath.h>

void renderText(char *, GLfloat, GLfloat, GLfloat, kmVec3);
float* valPtr(kmMat4 *);
void itoa(int, char *);
void reverse(char *);
void initTextGlyphs();