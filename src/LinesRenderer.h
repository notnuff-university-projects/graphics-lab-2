#ifndef LINESRENDERER_H
#define LINESRENDERER_H
#include "Shader.h"


class LinesRenderer {
public:
    void Init();
    void DrawLine(float x1, float y1, float x2, float y2);

protected:
    unsigned int VBO, VAO;

    Shader linesShader_;

};



#endif //LINESRENDERER_H
