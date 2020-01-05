#include "Shape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/datatype/VAO.h"

#include <iostream>

using namespace CS123::GL;

Shape::Shape(int shape_type, int p1, int p2):
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_VAO(nullptr),
    m_numVertices(0),
    m_shapeType(shape_type),
    m_p1(p1),
    m_p2(p2)
{
}

Shape::~Shape()
{
}

/*** Simple draw method for any given shape ***/
void Shape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

int Shape::getP1() {
    return m_p1;
}

int Shape::getP2() {
    return m_p2;
}
