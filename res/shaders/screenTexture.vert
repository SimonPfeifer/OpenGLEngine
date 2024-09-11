#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aUV;

out vec2 UV;

void main()
{
  // Vertex position in clip space.
  gl_Position = vec4(aPosition, 1.0f);
  UV = aUV;
}
