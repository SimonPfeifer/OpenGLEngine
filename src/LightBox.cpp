#include "LightBox.h"

LightBox::LightBox()
{
  float vertexInput[] = {
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,

   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f
  };

  float normalInput[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,

   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,
   -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
  };

  unsigned int indexInput[36];
  float uvInput[36*2];
  for (int i = 0; i < 6; i++)
  {
    // indices 0, 2, 3, 0, 3, 1
    indexInput[6 * i] = 0 + 6 * i;
    indexInput[1+6 * i] = 1 + 6 * i;
    indexInput[2+6 * i] = 2 + 6 * i;
    indexInput[3+6 * i] = 3 + 6 * i;
    indexInput[4+6 * i] = 4 + 6 * i;
    indexInput[5+6 * i] = 5 + 6 * i;
    
    uvInput[0+6 * i] =  0.0f;
    uvInput[1+6 * i] =  0.0f;
    uvInput[2+6 * i] =  1.0f;
    uvInput[3+6 * i] =  0.0f;
    uvInput[4+6 * i] =  1.0f;
    uvInput[5+6 * i] =  1.0f;
    uvInput[6+6 * i] =  1.0f;
    uvInput[7+6 * i] =  1.0f;
    uvInput[8+6 * i] =  0.0f;
    uvInput[9+6 * i] =  1.0f;
    uvInput[10+6 * i] = 0.0f;
    uvInput[12+6 * i] = 0.0f;
  }

  float colorInput[36 * 4];
  for (int i = 0; i < 36; i++)
  {
    colorInput[0 + 4 * i] = 1.0f;
    colorInput[1 + 4 * i] = 1.0f;
    colorInput[2 + 4 * i] = 1.0f;
    colorInput[3 + 4 * i] = 1.0f;
  }

  unsigned int vertexLength = sizeof(vertexInput) / sizeof(*vertexInput);
  vertexLength = vertexLength / 3;
  unsigned int indexLength = sizeof(indexInput) / sizeof(*indexInput);

  loadVertexData(vertexInput, vertexLength, indexInput, indexLength, 3, 0);
  loadVertexData(colorInput, vertexLength, indexInput, indexLength, 4, 1);
  loadVertexData(uvInput, vertexLength, indexInput, indexLength, 2, 2);
  loadVertexData(normalInput, vertexLength, indexInput, indexLength, 3, 3);
}
