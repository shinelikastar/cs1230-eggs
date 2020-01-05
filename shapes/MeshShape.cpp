#include "MeshShape.h"
#include "ShapeUtils.h"
#include "shapes/MeshUtils.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/datatype/VAO.h"
#include <iostream>

MeshShape::MeshShape(std::string path) :
    m_path(path),
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_VAO(nullptr),
    m_numVertices(0)
{
    loadMeshFile();
    ShapeUtils::setAttribute(m_markers, ShaderAttrib::POSITION, 3, 0,
            VBOAttribMarker::DATA_TYPE::FLOAT, false);
    ShapeUtils::setAttribute(m_markers, ShaderAttrib::NORMAL, 3, 12,
            VBOAttribMarker::DATA_TYPE::FLOAT, false);
    ShapeUtils::setAttribute(m_markers, ShaderAttrib::TEXCOORD0, 2, 24,
            VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_data = m_dataVector.data();
    m_drawMode = VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES;
    ShapeUtils::buildVAO(m_VAO, m_data, m_size, m_markers, m_drawMode, m_numVertices);
}

MeshShape::~MeshShape()
{
}

void MeshShape::loadMeshFile() {
    MeshUtils::loadObjFile(m_path.c_str(), m_dataVector);
    m_numVertices = m_dataVector.size() / 8;
    m_size = m_dataVector.size();
}

/*** Simple draw method for any given shape ***/
void MeshShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

std::string MeshShape::getPath() {
    return m_path;
}
