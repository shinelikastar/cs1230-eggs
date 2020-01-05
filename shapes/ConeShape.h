#ifndef CONESHAPE_H
#define CONESHAPE_H

#include "Shape.h"

class ConeShape : public Shape
{
public:
    ConeShape(int shape_type, int p1, int p2);
    ~ConeShape();

private:
    void computeVertices();
};

#endif // CONESHAPE_H
