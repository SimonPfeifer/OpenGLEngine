#version 330 core
out vec4 fragColor;
  
in vec2 UV;

uniform sampler2DArray screenTexture;

void main()
{
  int currentLayer = 0;
  float depth = texture(screenTexture, vec3(UV, currentLayer)).r;
  fragColor = vec4(depth, depth, depth, 1.0f);
}
