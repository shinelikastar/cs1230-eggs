#include "ShapeUtils.h"

#include "gl/shaders/ShaderAttribLocations.h"

#include <iostream>

namespace ShapeUtils {

/*** Creates and adds an attribute marker to the vector of attribute markers,
* given the parameters index, numElementsPerVertex, offset, type, and normalize.
***/
void setAttribute(std::vector<VBOAttribMarker> &markers,
                              GLint index, GLuint numElementsPerVertex, int offset,
                              VBOAttribMarker::DATA_TYPE type, bool normalize) {
    markers.push_back(
            VBOAttribMarker(
                index,
                numElementsPerVertex,
                offset,
                type,
                normalize));
}

/*** Initializes shape with positions and normals interleaved. This is used
* for all of the shapes I implemented in the Shapes project.
***/
void initPosNormShape(float *data, std::vector<float> &dataVector,
                                  VBO::GEOMETRY_LAYOUT &drawMode, std::unique_ptr<VAO> &vao,
                                  GLsizeiptr &size, std::vector<VBOAttribMarker> &markers,
                                  int &numVertices)  {
    setAttribute(markers, ShaderAttrib::POSITION, 3, 0,
            VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(markers, ShaderAttrib::NORMAL, 3, 12,
            VBOAttribMarker::DATA_TYPE::FLOAT, false);
    data = dataVector.data();
    drawMode = VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP;
    buildVAO(vao, data, size, markers, drawMode, numVertices);
}

/*** Builds a vertex array object given data, size, attribute markers,
* draw mode, and number of vertices.
***/
void buildVAO(std::unique_ptr<VAO> &vao, float *data, GLsizeiptr &size,
                          std::vector<VBOAttribMarker> &markers,
                          VBO::GEOMETRY_LAYOUT &drawMode, int &numVertices) {
    CS123::GL::VBO vbo = VBO(data, size, markers, drawMode);
    vao = std::make_unique<VAO>(vbo, numVertices);
}

// End of namespace
}
