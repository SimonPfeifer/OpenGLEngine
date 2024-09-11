#version 330 core
out vec4 fragColor;
  
in vec2 UV;

uniform sampler2D screenTexture;

void main()
{
  float depth = texture(screenTexture, UV).r;
  fragColor = vec4(depth, depth, depth, 1.0f);
}
