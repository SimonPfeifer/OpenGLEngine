#version 330 core
out vec4 fragColor;

in vec3 vertPosition;
in vec3 vertNormal;
in vec4 vertColor;

uniform sampler2D inputTexture;

uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform vec3 cameraPosition;
uniform float specularStrength;

uniform vec3 testColor;

void main()
{
	// ambient lighting
	vec3 ambient = ambientLight.xyz * ambientLight.w;

	// direct lighting
	// diffuse
	float distance = dot((lightPosition - vertPosition), (lightPosition - vertPosition)) / 1000.0f;
	vec3 normal = normalize(vertNormal);
	vec3 lightDirection = normalize(lightPosition- vertPosition);
	vec3 diffuse = lightColor.xyz * max(dot(normal, lightDirection), 0.0);

	// specular
	vec3 viewDir = normalize(cameraPosition - vertPosition);
	vec3 reflectDir = reflect(-lightDirection, normal);
	vec3 specular = lightColor.xyz * specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 64);

	vec3 combined = ambient + (diffuse + specular) / distance;
  vec3 result = specular;
	fragColor = vec4(result, 1.0f);// vertColor * vec4(combined, 1.0f);
}
