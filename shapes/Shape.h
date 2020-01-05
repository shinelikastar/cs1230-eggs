#ifndef SHAPE_H
#define SHAPE_H

#include "GL/glew.h"

#include <vector>
#include <memory>

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"

namespace CS123 { namespace GL {

class VAO;

}}

using namespace CS123::GL;

class Shape
{
public:
    Shape(int shape_type, int x_tesselate, int y_tesselate);
    virtual ~Shape();

    virtual void draw();

    virtual int getP1();
    virtual int getP2();

protected:
    /*** Pure virtual function which every inherited class
     * must implement. ***/
    virtual void computeVertices() = 0;

    GLfloat *m_data;
    std::vector<float> m_dataVector;
    GLsizeiptr m_size;
    VBO::GEOMETRY_LAYOUT m_drawMode;


    std::vector<VBOAttribMarker> m_markers;
    std::unique_ptr<CS123::GL::VAO> m_VAO;

    int m_numVertices;
    int m_shapeType;
    int m_p1;
    int m_p2;
};

#endif // SHAPE_H
