#include "TextureUtils.h"
#include <cmath>

namespace TextureUtils {

// ==================================================================
//                    UV COORDINATE CALCULATIONS
// ==================================================================

/** Redirects to correct uv coordinate calculation function given the
 primitive type. **/
glm::vec2 calcUV(PrimitiveType type, const glm::vec3 &intersectPt) {
	glm::vec2 uv = glm::vec2(0.f);

    switch(type) {
        case PrimitiveType::PRIMITIVE_CONE: {
			uv = calcUVCone(intersectPt);
            break;
        }
        case PrimitiveType::PRIMITIVE_CUBE: {
			uv = calcUVCube(intersectPt);
            break;
        }
        case PrimitiveType::PRIMITIVE_CYLINDER: {
			uv = calcUVCylinder(intersectPt);
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE: {
			uv = calcUVSphere(intersectPt);
            break;
        }
        default: {
			// Leave uv as (0,0)
            break;
        }
    }
	
	return uv;
}

/** Given an object intersection point on a cube,
  * calculate the uv-coordinate of the texture. **/
glm::vec2 calcUVCube(const glm::vec3 &intersectPt) {

	float u = 0.f;
	float v = 0.f;

    if (isEqualF(0.5, intersectPt.x)) {
		
		// Right face
        u = 0.5f - intersectPt.z;
        v = 0.5f - intersectPt.y;

    } else if (isEqualF(-0.5f, intersectPt.x)) {

		// Left face
        u = intersectPt.z + 0.5f;
        v = 0.5f - intersectPt.y;

    } else if (isEqualF(0.5f, intersectPt.y)) {

		// Top face
        u = intersectPt.x + 0.5f;
        v = intersectPt.z + 0.5f;

    } else if (isEqualF(-0.5f, intersectPt.y)) {

		// Bottom face
        u = intersectPt.x + 0.5f;
        v = 0.5f - intersectPt.z;

    } else if (isEqualF(0.5f, intersectPt.z)) {
		
		// Front face
        u = intersectPt.x + 0.5f;
        v = 0.5f - intersectPt.y;

    } else {
		
		// Back face
        u = 0.5f - intersectPt.x;
        v = 0.5f - intersectPt.y;

    }

    u = glm::clamp(u, 0.f, 1.f);
    v = glm::clamp(v, 0.f, 1.f);

	return glm::vec2(u, v);
}

/** Given an object intersection point on a sphere,
  * calculate the uv-coordinate of the texture. **/
glm::vec2 calcUVSphere(const glm::vec3 &intersectPt) {

    float latitude = std::asin(std::min(intersectPt.y / 0.5f, 1.f));
    float v = 0.5f - (latitude / M_PI);

    float u = 0.f;

    if (isEqualF(v, 1.f) || isEqualF(v, 0.f)) {
        u = 0.5f;
    } else {
        float theta = std::atan2(intersectPt.z, intersectPt.x);

        if (theta < 0) {
            u = -theta / (2 * M_PI);
        } else {
            u = 1 - (theta / (2 * M_PI));
        }
    }

    u = glm::clamp(u, 0.f, 1.f);
    v = glm::clamp(v, 0.f, 1.f);

	return glm::vec2(u, v);
}

/** Given an object intersection point on a cylinder,
  * calculate the uv-coordinate of the texture. **/
glm::vec2 calcUVCylinder(const glm::vec3 &intersectPt) {
	
	float u = 0.f;
	float v = 0.f;

	if (isEqualF(intersectPt.y, 0.5f)) {

		// Intersected with top cap
        u = intersectPt.x + 0.5f;
        v = intersectPt.z + 0.5f;

	} else if (isEqualF(intersectPt.y, -0.5f)) {

		// Intersected with bottom cap
        u = intersectPt.x + 0.5f;
        v = 0.5f - intersectPt.z;

	} else {

		// Intersected with wrapper
        v = 0.5f - intersectPt.y;

		float theta = std::atan2(intersectPt.z, intersectPt.x);

		if (theta < 0) {
			u = -theta / (2 * M_PI);
		} else {
			u = 1 - (theta / (2 * M_PI));
		}

	}

    u = glm::clamp(u, 0.f, 1.f);
    v = glm::clamp(v, 0.f, 1.f);

	return glm::vec2(u, v);
}

/** Given an object intersection point on a cone,
  * calculate the uv-coordinate of the texture. **/
glm::vec2 calcUVCone(const glm::vec3 &intersectPt) {
	
	float u = 0.f;
	float v = 0.f;

	if (isEqualF(intersectPt.y, -0.5f)) {

		// Intersected with bottom cap
        u = intersectPt.x + 0.5f;
        v = 0.5f - intersectPt.z;

	} else {

		// Intersected with wrapper
        v = 0.5f - intersectPt.y;
		float theta = std::atan2(intersectPt.z, intersectPt.x);

		if (theta < 0) {
			u = -theta / (2 * M_PI);
		} else {
			u = 1 - (theta / (2 * M_PI));
		}

	}

    u = glm::clamp(u, 0.f, 1.f);
    v = glm::clamp(v, 0.f, 1.f);

	return glm::vec2(u, v);
}

// ==================================================================
//                              MISC
// ==================================================================

/** Given the UV coordinate, the repeating factor for both the U
 * and V axes, and the width and height of the texture map, this
 * function returns the cooresponding ST coordinate. **/
glm::vec2 UVtoST(const glm::vec2 &uv, const float repeatU, 
		const float repeatV, const int width, const int height) {
	int s = (static_cast<int>(uv.x * repeatU * width)) % width;
	int t = (static_cast<int>(uv.y * repeatV * height)) % height;

	return glm::vec2(s, t);
}

/** Given a texture map and ST coordinates, calculates the color
 * of the texture at that coordinate. **/
glm::vec3 getTextureColor(const glm::vec2 &st, const QImage &textureMap) {
	int s = static_cast<int>(st.x);
	int t = static_cast<int>(st.y);
	QRgb color = textureMap.pixel(s, t);

	float red = qRed(color) / 255.f;
	float green = qGreen(color) / 255.f;
	float blue = qBlue(color) / 255.f;

	return glm::vec3(red, green, blue);
}

/** Returns a QImage object given a CS123SceneFileMap struct. **/
QImage getTextureImage(std::string path) {
    QImage textureMap = QImage(path.c_str());

	return textureMap;
}

}
