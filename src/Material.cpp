#include "Material.h"

Material::Material()
{
  id = 0;

  colorAmbient = glm::vec3(0.0f);
  colorDiffuse = glm::vec3(0.0f);
  colorSpecular = glm::vec3(0.0f);
  specularStrength = 0.0f;
}

void Material::activate(Shader& shader) const
{
  shader.use();

  // Set the material uniforms.
  shader.setUniformVec3f("colorAmbient", colorAmbient);
  shader.setUniformVec3f("colorDiffuse", colorDiffuse);
  shader.setUniformVec3f("colorSpecular", colorSpecular);
  shader.setUniformFloat("specularStrength", specularStrength);
  
  // Bind the textures.
  shader.setUniformInt("textureDiffuse", 0);
  textureDiffuse.bind(0);

  shader.setUniformInt("textureSpecular", 1);
  textureSpecular.bind(1);
}
