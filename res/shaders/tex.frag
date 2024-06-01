#version 330 core
out vec4 fragColor;

in vec3 vertPosition;
in vec2 texCoord;

uniform sampler2D textureDiffuse;
uniform vec3 colorDiffuse;


void main()
{
  vec4 diffuseCol = texture(textureDiffuse, texCoord);

  fragColor = diffuseCol;
  // fragColor = vec4(colorDiffuse, 1.0f);
}
