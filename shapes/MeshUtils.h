#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <vector>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

namespace MeshUtils
{

int loadObjFile(const char* path, std::vector<float> &data);

}

#endif // MESHLOADER_H
