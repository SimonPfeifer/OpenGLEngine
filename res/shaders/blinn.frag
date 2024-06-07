#version 330 core
out vec4 fragColor;

in vec3 vertPosition;
in vec3 vertNormal;
in vec2 vertUV;

uniform vec3 colorAmbient;
uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;
uniform float specularStrength;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;


void main()
{
  vec3 normal = normalize(vertNormal);
  vec3 lightDir = normalize(lightPosition - vertPosition);
  float distance = length(lightPosition - vertPosition);
  float distanceSqaure = dot(distance, distance);

  float lambertian = max(dot(lightDir, normal), 0.0f);
  
  float specular = 0.0;
  if (lambertian > 0.0)
  {
    vec3 viewDir = normalize(-vertPosition);
    vec3 halfDir = normalize(lightDir + viewDir);
    float specularAngle = max(dot(halfDir, normal), 0.0f);
    specular = pow(specularAngle, specularStrength);
  }

  vec3 diffuseCol = texture(textureDiffuse, vertUV).xyz;
  vec3 specularCol = texture(textureSpecular, vertUV).xyz;
  
  // Quick hack! Force ambient color to be less than 1.0f and more
  // than a minimum.
  float ambientMin = 0.1f;
  vec3 ambient = vec3(ambientMin);
  if ((colorAmbient.x + colorAmbient.y + colorAmbient.z < 3.0f))
  {
    ambient = max(ambient, colorAmbient);
  }

  vec3 outColor = (ambient * diffuseCol) + 
                  (lightIntensity / distanceSqaure) * lightColor *
                  (diffuseCol * lambertian  + specularCol * specular);
  fragColor = vec4(outColor, 1.0f);
}
