#ifndef MESHSHAPE_H
#define MESHSHAPE_H

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "GL/glew.h"
#include <vector>
#include <memory>
#include <string>

namespace CS123 { namespace GL {

class VAO;

}}

using namespace CS123::GL;

class MeshShape
{
public:
    MeshShape(std::string path);
    ~MeshShape();

    void draw();
    std::string getPath();

private:
    std::string m_path;
    GLfloat *m_data;
    std::vector<float> m_dataVector;
    GLsizeiptr m_size;
    VBO::GEOMETRY_LAYOUT m_drawMode;
    std::vector<VBOAttribMarker> m_markers;
    std::unique_ptr<VAO> m_VAO;
    int m_numVertices;

    void loadMeshFile();
};

#endif // MESHSHAPE_H
