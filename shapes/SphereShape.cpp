#include "SphereShape.h"
#include "FaceBuilder.h"
#include "ShapeUtils.h"

#include <gl/datatype/VAO.h>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include <cmath>

using namespace CS123::GL;

SphereShape::SphereShape(int shape_type, int p1, int p2)
    : Shape(shape_type, p1, p2)
{
    computeVertices();
    ShapeUtils::initPosNormShape(m_data, m_dataVector, m_drawMode, m_VAO,
                          m_size, m_markers, m_numVertices);
}

SphereShape::~SphereShape()
{
}


/*** Computes the vertices and associated normals of the shape. ***/
void SphereShape::computeVertices() {
    int y_tess = m_p1 + 1;
    int theta_tess = m_p2 + 2;

    // Builds sphere vertices.
    for (int i = 0; i < y_tess; i++) {
        float y_top = -0.5f + ((i+1) * (1.0f / y_tess));
        float y_bot = -0.5f + (i * (1.0f / y_tess));
        y_top = 0.5f * std::sin(M_PI * y_top);
        y_bot = 0.5f * std::sin(M_PI * y_bot);
        float r_top = std::sqrt(std::pow(0.5f, 2.0f) - std::pow(y_top, 2.0f));
        float r_bot = std::sqrt(std::pow(0.5f, 2.0f) - std::pow(y_bot, 2.0f));

        // Creates horizontal strip of sphere from y_bot to y_top.
        for (int j = theta_tess; j >= 0; j--) {
            float theta = j * (2.0f * M_PI / theta_tess);
            float x_top = r_top * std::cos(theta);
            float x_bot = r_bot * std::cos(theta);
            float z_top = r_top * std::sin(theta);
            float z_bot = r_bot * std::sin(theta);

            // Normal vector is just coordinates normalized.
            glm::vec3 norm_vec_top = glm::vec3(x_top, y_top, z_top);
            glm::vec3 norm_vec_bot = glm::vec3(x_bot, y_bot, z_bot);
            norm_vec_top = glm::normalize(norm_vec_top);
            norm_vec_bot = glm::normalize(norm_vec_bot);

            m_dataVector.insert(m_dataVector.end(), {x_top, y_top, z_top});
            m_dataVector.insert(m_dataVector.end(), {norm_vec_top[0], norm_vec_top[1], norm_vec_top[2]});

            // Repeat add for degenerate triangles.
            if (j == theta_tess && i != 0) {
                m_dataVector.insert(m_dataVector.end(), {x_top, y_top, z_top});
                m_dataVector.insert(m_dataVector.end(), {norm_vec_top[0], norm_vec_top[1], norm_vec_top[2]});
            }

            m_dataVector.insert(m_dataVector.end(), {x_bot, y_bot, z_bot});
            m_dataVector.insert(m_dataVector.end(), {norm_vec_bot[0], norm_vec_bot[1], norm_vec_bot[2]});

            // Repeat add for degenerate triangles.
            if (j == 0 && i != y_tess-1) {
                m_dataVector.insert(m_dataVector.end(), {x_bot, y_bot, z_bot});
                m_dataVector.insert(m_dataVector.end(), {norm_vec_bot[0], norm_vec_bot[1], norm_vec_bot[2]});
            }
        }
    }

    m_size = m_dataVector.size();
    m_numVertices = m_dataVector.size() / 6;
}
