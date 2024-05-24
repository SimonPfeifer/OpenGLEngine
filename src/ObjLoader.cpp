#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

#include "ObjLoader.h"


std::vector<Mesh> ObjLoader::load(const char* filepath)
{
  // Load the file data into object variables.
  parseFile(filepath);

  // Construct attribute vectors that are consistent with the vertex index
  // vector.

  return constructMesh();
}

void ObjLoader::parseFile(const char* filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "ObjLoader::parseFile() Failed to open file: " << filepath
      << std::endl;;
  }

  // Keeps track of the vertex indices that have already been assigned.
  int maxVertexIndex = 0;

  // Used to check if a new mesh is being parsed.
  bool hasParcedFaces = false;

  // Initialise the vectors with empty vectors to push the parsed data into.
  int meshIndex = 0;
  indexVector.push_back(std::vector<int>());
  positionVector.push_back(std::vector<glm::vec3>());
  normalVector.push_back(std::vector<glm::vec3>());
  uvVector.push_back(std::vector<glm::vec2>());
  vertexFaceIndices.push_back(std::vector<std::string>());

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream lineStream(line);

    std::string type;
    lineStream >> type;

    // Triggers if we have a new mesh.
    if (type == "v" && hasParcedFaces)
    {
      hasParcedFaces = false;

      ++meshIndex;
      indexVector.push_back(std::vector<int>());
      positionVector.push_back(std::vector<glm::vec3>());
      normalVector.push_back(std::vector<glm::vec3>());
      uvVector.push_back(std::vector<glm::vec2>());
      vertexFaceIndices.push_back(std::vector<std::string>());

      maxVertexIndex = 0;

      faceIndexMap.clear();
    }

    if (type == "v")
    {
      glm::vec3 vert;
      lineStream >> vert.x >> vert.y >> vert.z;
      positionVector[meshIndex].push_back(vert);
    }
    else if (type == "vn")
    {
      glm::vec3 norm;
      lineStream >> norm.x >> norm.y >> norm.z;
      normalVector[meshIndex].push_back(norm);
    }
    else if (type == "vt")
    {
      glm::vec2 tex;
      lineStream >> tex.x >> tex.y;
      uvVector[meshIndex].push_back(tex);
    }
    else if (type == "f")
    {
      hasParcedFaces = true;

      // Copy the face indices as strings directly into a vector.
      std::vector<std::string> faceIndices((std::istream_iterator<std::string>(lineStream)),
                                            std::istream_iterator<std::string>());


      // Uses a hash table to keep track of already seen face verteces.
      for (size_t i=0; i<faceIndices.size(); ++i)
      {
        if (faceIndexMap.contains(faceIndices[i]))
          indexVector[meshIndex].push_back(faceIndexMap[faceIndices[i]]);
        else
          {
            indexVector[meshIndex].push_back(maxVertexIndex);
            vertexFaceIndices[meshIndex].push_back(faceIndices[i]);
            faceIndexMap[faceIndices[i]] = maxVertexIndex;
            maxVertexIndex++;
          }
      }

      // If we are dealing with quad faces, add the first and third vertex
      // index of the quad again to make a second tri.
      if (faceIndices.size() == 4)
      {
        size_t vecSize = indexVector[meshIndex].size();
        indexVector[meshIndex].push_back(indexVector[meshIndex][vecSize-4]);
        indexVector[meshIndex].push_back(indexVector[meshIndex][vecSize-2]);
      }
    }
    else if(type == "l" || type == "o" || type == "s" || type == "g"
            || type == "mtllib" || type == "usemtl")
    {
      continue;
    }
    else if(type == "#" || type == "")
    {
      continue;
    }
    else
    {
      std::cout << "Ignored line, unknown type: " << type << std::endl;
    }
  }
}

std::vector<Mesh> ObjLoader::constructMesh()
{
  // Keeps track of the number of previous entries in the OBJ file to allow
  // for correct indexing within each v/vt/vn chunck. 
  // Is initialised at 1 to account for OBJ indices starting from 1.
  // Ordering of offsets is: v, vt, vn.
  int indexOffset[3] = {1, 1, 1};

  std::vector<Mesh> meshVector;
  for (size_t n=0; n<positionVector.size(); ++n)
  {
    Vertex vertex;
    std::vector<Vertex> vertexVector;
    // Load each face index in order of vertex indices.
    for (size_t vertIdx=0; vertIdx<vertexFaceIndices[n].size(); ++vertIdx)
    {
      std::string faceIdx = vertexFaceIndices[n][vertIdx];
      std::vector<std::string> vertAttribIdx = splitString(faceIdx, '/');

      // For each index type, generate a new vector of vertex attributes.
      // It is assumed that the face index contains 3 vertex attributes.
      // Ordering of attribute indices is: v, vt, vn.
      vertex.position = positionVector[n][std::stoi(vertAttribIdx[0])-
                                                    indexOffset[0]];
      vertex.uv = uvVector[n][std::stoi(vertAttribIdx[1])-
                                        indexOffset[1]];
      vertex.normal = normalVector[n][std::stoi(vertAttribIdx[2])-
                                                indexOffset[2]];

      vertexVector.push_back(vertex);
    }
    meshVector.push_back(Mesh(vertexVector, indexVector[n]));

    indexOffset[0] += positionVector[n].size();
    indexOffset[1] += uvVector[n].size();
    indexOffset[2] += normalVector[n].size();
  }
  return meshVector;
}

std::vector<std::string> ObjLoader::splitString(std::string inputString,
                                                char delimitor)
{
  std::string token;
  std::vector<std::string> result;
  std::stringstream inputStream(inputString);

  while (std::getline(inputStream, token, delimitor))
  {
    result.push_back(token);
  }
  return result;
}
