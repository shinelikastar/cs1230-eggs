#include "CylinderShape.h"
#include "FaceBuilder.h"
#include "ShapeUtils.h"

#include <gl/datatype/VAO.h>
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

using namespace CS123::GL;

CylinderShape::CylinderShape(int shape_type, int p1, int p2)
    : Shape(shape_type, p1, p2)
{
    computeVertices();
    ShapeUtils::initPosNormShape(m_data, m_dataVector, m_drawMode, m_VAO,
                          m_size, m_markers, m_numVertices);
}

CylinderShape::~CylinderShape()
{
}


/*** Computes the vertices and associated normals of the shape. ***/
void CylinderShape::computeVertices() {
    FaceBuilder face_builder = FaceBuilder();

    // m_p2+2 passed so that min angular tesselation generates a triangle, not
    // a non-face.
    std::vector<float> top_cap;
    face_builder.computeCircleFace(top_cap, m_p1, m_p2+2);

    // Insert top cap of cylinder.
    m_dataVector.insert(m_dataVector.end(), top_cap.begin(), top_cap.end());

    // Duplicate last vertex and normal of top cap for the
    // degenerate triangle.
    m_dataVector.insert(m_dataVector.end(),
            {top_cap.at(top_cap.size()-6),
             top_cap.at(top_cap.size()-5),
             top_cap.at(top_cap.size()-4),
             top_cap.at(top_cap.size()-3),
             top_cap.at(top_cap.size()-2),
             top_cap.at(top_cap.size()-1)});

    // Rotation matrix for rotating top cap into bottom cap.
    glm::mat4 cap_rot = glm::rotate(static_cast<float>(M_PI), glm::vec3(1.0f, 0.0f, 0.0f));

    for (int i = 0; i < static_cast<int>(top_cap.size()); i += 3) {
        glm::vec4 value = glm::vec4(top_cap.at(i),
                                    top_cap.at(i+1),
                                    top_cap.at(i+2),
                                    1.0f);
        glm::vec4 rotated = cap_rot * value;

        m_dataVector.insert(m_dataVector.end(),
            {rotated[0], rotated[1], rotated[2]});

        // For degenerate triangle
        if (i == 3 || i == static_cast<int>(top_cap.size()-3)) {
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


    std::vector<float> tube;
    face_builder.computeTubeFace(tube, m_p1, m_p2+2);

    // Repeat first vertex and normal for degenerate triangle
    std::vector<float>::iterator it = tube.begin();
    m_dataVector.insert(m_dataVector.end(), it, it+6);
    m_dataVector.insert(m_dataVector.end(), tube.begin(), tube.end());

    m_size = m_dataVector.size();
    m_numVertices = m_dataVector.size() / 6;
}

