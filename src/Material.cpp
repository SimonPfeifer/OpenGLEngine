#include "Material.h"

void Material::activate(Shader& shader)
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
