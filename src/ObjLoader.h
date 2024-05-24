#include "Mesh.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

class ObjLoader
{
public:
  std::vector<Mesh> load(const char* filepath);

private:
  std::vector<std::vector<int>> indexVector;
  std::vector<std::vector<glm::vec3>> positionVector;
  std::vector<std::vector<glm::vec3>> normalVector;
  std::vector<std::vector<glm::vec2>> uvVector;

  // Keeps track of vertex indices of the faces that have already been seen.
  // The hash map speeds up look-ups of previous indices and the vector keeps
  // track of face index to vertex index correspondence,
  // for example vertexFaceIndices[n][vertexIndex] = faceIndex.
  std::unordered_map<std::string, int> faceIndexMap;
  std::vector<std::vector<std::string>> vertexFaceIndices;

  void parseFile(const char* filepath);
  std::vector<Mesh> constructMesh();

  std::vector<std::string> splitString(std::string inputString,
                                       char delimitor);
};