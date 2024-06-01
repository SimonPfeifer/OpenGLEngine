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

void main()
{
	// ambient lighting
	vec3 ambient = ambientLight.xyz * ambientLight.w;

	// direct lighting
  vec3 distanceVector = lightPosition - vertPosition;
	float distanceSquared = dot(distanceVector, distanceVector);

	// diffuse
	vec3 normal = normalize(vertNormal);
	vec3 lightDirection = normalize(lightPosition-vertPosition);
	vec3 diffuse = lightColor.w * lightColor.xyz * max(0.0f, dot(normal, lightDirection));

	// specular
	vec3 viewDir = normalize(cameraPosition-vertPosition);
	vec3 reflectDir = reflect(-lightDirection, normal);
	vec3 specular = lightColor.w * lightColor.xyz * pow(max(0.0f, dot(viewDir, reflectDir)), 64);

	vec3 combined = ambient + (specular + diffuse) / distanceSquared;
	fragColor = vertColor * vec4(combined, 1.0f);
}
