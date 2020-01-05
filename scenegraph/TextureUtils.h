#ifndef TEXTUREUTILS_H
#define TEXTUREUTILS_H

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "lib/CS123SceneData.h"
#include <string.h>

#include <QImage>

namespace TextureUtils {

glm::vec2 calcUV(PrimitiveType type, const glm::vec3 &intersectPt);

glm::vec2 calcUVCube(const glm::vec3 &intersectPt);
glm::vec2 calcUVSphere(const glm::vec3 &intersectPt);
glm::vec2 calcUVCylinder(const glm::vec3 &intersectPt);
glm::vec2 calcUVCone(const glm::vec3 &intersectPt);

glm::vec2 UVtoST(const glm::vec2 &uv, const float repeatU, 
		const float repeatV, const int width, const int height);

glm::vec3 getTextureColor(const glm::vec2 &st, const QImage &textureMap);
QImage getTextureImage(std::string path);

// Compares equality of floating point numbers given an epsilon
// of 1e-5
inline bool isEqualF(float left, float right) {
    return std::fabs(left - right) < 1e-5;
}
}

#endif // TEXTUREUTILS_H
