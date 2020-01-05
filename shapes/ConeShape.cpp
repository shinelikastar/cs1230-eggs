#include "ConeShape.h"
#include "FaceBuilder.h"
#include "ShapeUtils.h"

#include <gl/datatype/VAO.h>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

using namespace CS123::GL;

ConeShape::ConeShape(int shape_type, int p1, int p2)
    : Shape(shape_type, p1, p2)
{
    computeVertices();
    ShapeUtils::initPosNormShape(m_data, m_dataVector, m_drawMode, m_VAO,
                          m_size, m_markers, m_numVertices);
}

ConeShape::~ConeShape()
{
}


/*** Computes the vertices and associated normals of the shape. ***/
void ConeShape::computeVertices() {
    FaceBuilder face_builder = FaceBuilder();

    std::vector<float> cone_face;
    face_builder.computeConeFace(cone_face, m_p1, m_p2+2);

    m_dataVector.insert(m_dataVector.end(), cone_face.begin(), cone_face.end());

    // Duplicate last vertex and normal of cone face for the degenerate triangle.
    m_dataVector.insert(m_dataVector.end(),
            {cone_face.at(cone_face.size()-6),
             cone_face.at(cone_face.size()-5),
             cone_face.at(cone_face.size()-4),
             cone_face.at(cone_face.size()-3),
             cone_face.at(cone_face.size()-2),
             cone_face.at(cone_face.size()-1)});

    std::vector<float> cone_base;
    face_builder.computeCircleFace(cone_base, m_p1, m_p2+2);

    glm::mat4 rot_base_mat = glm::rotate(static_cast<float>(M_PI), glm::vec3(1.0f, 0.0f, 0.0f));

    int coneBaseSize = static_cast<int>(cone_base.size());

    // Rotates top face of cylinder 180 degrees to create base of cone.
    for (int i = 0; i < coneBaseSize; i += 3) {
        glm::vec4 value = glm::vec4(cone_base.at(i),
                                    cone_base.at(i+1),
                                    cone_base.at(i+2),
                                    1.0f);
        glm::vec4 rotated = rot_base_mat * value;
        m_dataVector.insert(m_dataVector.end(), {rotated[0], rotated[1], rotated[2]});

        // For degenerate triangle
        if (i == 3) {
            int data_size = m_dataVector.size();
            m_dataVector.insert(m_dataVector.end(),
                {m_dataVector.at(data_size-6),
                 m_dataVector.at(data_size-5),
                 m_dataVector.at(data_size-4),
                 m_dataVector.at(data_size-3),
                 m_dataVector.at(data_size-2),
                 m_dataVector.at(data_size-1)});
        }
    }

    m_size = m_dataVector.size();
    m_numVertices = m_dataVector.size() / 6;
}
