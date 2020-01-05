#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Shape.h"

class SphereShape : public Shape
{
public:
    SphereShape(int shape_type, int p1, int p2);
    ~SphereShape();

private:
    void computeVertices();
};

#endif // SPHERESHAPE_H
