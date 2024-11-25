#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 vertPosition;
out vec3 vertNormal;
out vec2 vertUV;
out vec4 vertPositionWorld;

uniform mat4 model;
uniform mat4 modelView;
uniform mat4 projection;


void main()
{
  // Vertex position in clip space.
  gl_Position = projection * modelView * vec4(aPosition, 1.0f);
  
  // Variables needed by the fragment shader.
  vertPosition = vec3(modelView * vec4(aPosition, 1.0f));
  vertNormal = transpose(inverse(mat3(modelView))) * aNormal;
  vertUV = aUV;

  vertPositionWorld = model * vec4(aPosition, 1.0f);
}