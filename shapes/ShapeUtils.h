#ifndef SHAPEUTILS_H
#define SHAPEUTILS_H

#include "GL/glew.h"
#include <vector>

#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"

using namespace CS123::GL;

namespace ShapeUtils {

/*** Creates and adds an attribute marker to the vector of attribute markers,
 * given the parameters index, numElementsPerVertex, offset, type, and normalize.
 ***/
void setAttribute(std::vector<VBOAttribMarker> &markers, GLint index,
                  GLuint numElementsPerVertex, int offset,
                  VBOAttribMarker::DATA_TYPE type, bool normalize);

/*** Initializes shape with positions and normals interleaved. This is used
 * for all of the shapes I implemented in the Shapes project.
 ***/
void initPosNormShape(float *data, std::vector<float> &dataVector,
                      VBO::GEOMETRY_LAYOUT &drawMode, std::unique_ptr<VAO> &vao,
                      GLsizeiptr &size, std::vector<VBOAttribMarker> &markers,
                      int &numVertices);

/*** Builds a vertex array object given data, size, attribute markers,
 * draw mode, and number of vertices.
***/
void buildVAO(std::unique_ptr<VAO> &vao, float *data, GLsizeiptr &size,
              std::vector<VBOAttribMarker> &markers,
              VBO::GEOMETRY_LAYOUT &drawMode, int &numVertices);

// End of namespace
}

#endif // SHAPEUTILS_H
