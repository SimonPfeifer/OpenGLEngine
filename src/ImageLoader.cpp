#include "ImageLoader.h"

#include "stb_image.h"

#include <iostream>
#include <string>

ImageLoader::~ImageLoader()
{
  unload();
}

bool ImageLoader::load(const char* filepath, int& width, int& height,
                       int& nChannels)
{
  // TODO: This could be user defined.
	stbi_set_flip_vertically_on_load(true);

	m_imageData = stbi_load(filepath, &width, &height, &nChannels, 0);
  if (!m_imageData)
  {
    std::cerr << "Failed to load image or image is empty" << std::endl;
    return false;
  }
  return true;
}

bool ImageLoader::load(std::string filepath, int& width, int& height,
                       int& nChannels)
{
  return load(filepath.c_str(), width, height, nChannels);
}

void ImageLoader::unload()
{
  stbi_image_free(m_imageData);
}
