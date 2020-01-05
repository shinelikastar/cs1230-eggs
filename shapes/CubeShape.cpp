#include "CubeShape.h"
#include "FaceBuilder.h"
#include "ShapeUtils.h"

#include <gl/datatype/VAO.h>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

using namespace CS123::GL;

CubeShape::CubeShape(int shape_type, int p1, int p2)
    : Shape(shape_type, p1, p2)
{
    computeVertices();
    ShapeUtils::initPosNormShape(m_data, m_dataVector, m_drawMode, m_VAO,
                          m_size, m_markers, m_numVertices);
}

CubeShape::~CubeShape()
{
}


/*** Computes the vertices and associated normals of the shape. ***/
void CubeShape::computeVertices() {
    FaceBuilder face_builder = FaceBuilder();
    std::vector<float> front_face;
    face_builder.computeSquareFace(front_face, m_p1);

    // Insert front face into cube
    m_dataVector.insert(m_dataVector.end(), front_face.begin(), front_face.end());

    // Duplicate last vertex and normal of front face to create
    // degenerate triangle.
    m_dataVector.insert(m_dataVector.end(),
            {front_face.at(front_face.size()-6),
             front_face.at(front_face.size()-5),
             front_face.at(front_face.size()-4),
             front_face.at(front_face.size()-3),
             front_face.at(front_face.size()-2),
             front_face.at(front_face.size()-1)});

    float pi = static_cast<float>(M_PI);
    // Rotations for rotating front face to each face of the cube.
    glm::mat4 rotations[5] = {
        glm::rotate(pi / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::rotate(-pi / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::rotate(pi, glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::rotate(pi / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::rotate(-pi / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f))
    };

    // Apply all rotations (create all faces of cube).
    for (int i = 0; i < 5; i++) {

        // Rotate vertices/normals then add to vertices vector.
        for (int j = 0; j < static_cast<int>(front_face.size()); j += 3) {
            glm::vec4 value = glm::vec4(
                    front_face.at(j),
                    front_face.at(j+1),
                    front_face.at(j+2),
                    1.0f);
            glm::vec4 rotated = rotations[i] * value;

            m_dataVector.insert(m_dataVector.end(),
                    {rotated[0], rotated[1], rotated[2]});

            // Repeat insertion for degenerate triangle.
            if (j == 3 || (j == static_cast<int>(front_face.size() - 3) && i != 4)) {
                m_dataVector.insert(m_dataVector.end(),
                    {m_dataVector.at(m_dataVector.size()-6),
                     m_dataVector.at(m_dataVector.size()-5),
                     m_dataVector.at(m_dataVector.size()-4),
                     m_dataVector.at(m_dataVector.size()-3),
                     m_dataVector.at(m_dataVector.size()-2),
                     m_dataVector.at(m_dataVector.size()-1)});
            }
        }

    }

    m_size = m_dataVector.size();
    m_numVertices = m_dataVector.size() / 6;
}
