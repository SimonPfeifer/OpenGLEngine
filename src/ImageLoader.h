#pragma once

#include <string>

class ImageLoader
{
public:
  ImageLoader() = default;
  ~ImageLoader();

  bool load(const char* filepath, int& width, int& height, int& nChannels);
  bool load(std::string filepath, int& width, int& height, int& nChannels);

  void unload();

  unsigned char* getImageData() const {return m_imageData;}

private:
  unsigned char* m_imageData{};
};