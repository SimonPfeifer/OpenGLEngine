#version 330 core
out vec4 fragColor;

in vec3 vertPosition;
in vec3 vertNormal;
in vec2 vertUV;
in vec4 vertPositionLight;

// Constants
#define PI 3.14159f
#define EPSILON 0.000001f

#define SHADOWBIAS 0.0002f //max(0.05 * (1.0 - max(dot(vertLightDir, normal), 0.0f)), 0.005);

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

// Depth map texture
uniform sampler2D textureDepth;

void main()
{
  vec3 normal = normalize(vertNormal);

  // Texture sampling.
  vec3 diffuseCol = texture(textureDiffuse, vertUV).xyz;
  vec3 specularCol = texture(textureSpecular, vertUV).xyz;

  // Quick hack! Force ambient color to be less than 1.0f and more
  // than a minimum.
  float ambientMin = 0.05f;
  vec3 ambient = vec3(ambientMin);
  if ((colorAmbient.x + colorAmbient.y + colorAmbient.z < 3.0f))
  {
    ambient = max(ambient, colorAmbient);
  }

  vec3 outColor = vec3(0.0f, 0.0f, 0.0f);
  for (int i=0; i<numLights; ++i)
  {
    // Value that is checked against to see if vertex is hit by light.
    bool isInLight = true;

    // Transform light into view space.
    vec3 lightPos = (view * vec4(lights[i].position, 1.0f)).xyz;
    vec3 lightDir = normalize((view * vec4(lights[i].direction, 0.0f)).xyz);
    float lightIntensity = lights[i].intensity;

    // Directional light default values. DistanceSquare is chosen such that a
    // direction light intensity between 1 and 100 is reasonable.
    float distanceSqaure = 100.0f;
    vec3 vertLightDir = -lightDir;

    if (lights[i].type == LIGHT_TYPE_POINT ||
        lights[i].type == LIGHT_TYPE_SPOT)
    {
      vertLightDir = normalize(lightPos - vertPosition);
      float distance = length(lightPos - vertPosition);
      distanceSqaure = dot(distance, distance);
      
      // Only evaluate for spotlight.
      if (lights[i].type == LIGHT_TYPE_SPOT)
      {
        float spotLightPhi = dot(vertLightDir, -normalize(lightDir));
        float spotLightTheta = cos(lights[i].spotlightAngle * PI / 180.0f);

        // TODO: These need to be light specific variables.
        float outerCutOff = 0.75f;
        float innerCutOff = 0.95f;
        float spotLightEpsilon = innerCutOff - outerCutOff;
        lightIntensity *= clamp((spotLightPhi - outerCutOff) / spotLightEpsilon, 0.0, 1.0);
        bool inSpotLight = spotLightPhi > spotLightTheta;
        isInLight = isInLight && inSpotLight;
      }
    }

    // Sample depth map to determine if vertex is in light.
    // TODO: replace by depth map texture array.
    // Perspective division.
    vec3 projCoords = vertPositionLight.xyz / vertPositionLight.w;

    // Rescale coordinates to [0,1] for depth map comparison.
    projCoords = projCoords * 0.5f + 0.5f;

    // Sample depth texture using clip space [x,y] and take RED channel value.
    // This value is the closest point (depth) as seen from the light.
    float closestDepth = texture(textureDepth, projCoords.xy).r;

    // If the current vertex depth is further than the closes point seen from
    // the light, the vertex is in shadow.
    isInLight = projCoords.z - SHADOWBIAS > closestDepth ? false : true;  
    
    // Check against small fudge factor instead of zero. Stops lighting glitch
    // when surface is parallel to light direction.
    float lambertian = max(dot(vertLightDir, normal), 0.0f);
    isInLight = isInLight && lambertian > EPSILON;
    
    float specular = 0.0f;
    if (isInLight)
    {
      vec3 viewDir = normalize(-vertPosition);
      vec3 halfDir = normalize(vertLightDir + viewDir);
      float specularAngle = max(dot(halfDir, normal), 0.0f);
      specular = pow(specularAngle, specularStrength);
    }

    outColor += (ambient * diffuseCol) + 
                (lightIntensity / distanceSqaure) * lights[i].color *
                (diffuseCol * lambertian  + specularCol * specular);
  }

  fragColor = vec4(outColor, 1.0f);
}
