#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "Shape.h"

class CubeShape : public Shape
{
public:
    CubeShape(int shape_type, int p1, int p2);
    ~CubeShape();

private:
    void computeVertices();
};

#endif // CUBESHAPE_H
