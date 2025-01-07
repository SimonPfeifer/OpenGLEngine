#version 330 core
out vec4 fragColor;

in vec3 vertPosition;
in vec3 vertNormal;
in vec2 vertUV;

// Constants
#define PI 3.14159f
#define EPSILON 0.000001f

// Lights
#define MAX_NUM_LIGHTS 512

#define LIGHT_TYPE_POINT 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_DIR 2

uniform mat4 view;

struct Light
{
  vec3 position;
  float range;
  // 16 bytes

  vec3 direction;
  float spotlightAngle;
  // 16 bytes

  vec3 color;
  float intensity;
  // 16 bytes

  int type;
  // 16 bytes
}; // total 64 bytes

layout (std140) uniform LightsBlock
{
  int numLights;
  Light lights[MAX_NUM_LIGHTS];
};

// Material
uniform vec3 colorAmbient;
uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;
uniform float specularStrength;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;


void main()
{
  // Texture sampling.
  float diffuseCol = texture(textureDiffuse, vertUV).x;

  fragColor = vec4(diffuseCol, diffuseCol, diffuseCol, 1.0f);
}
