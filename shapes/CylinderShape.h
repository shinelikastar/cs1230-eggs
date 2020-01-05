#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "Shape.h"

class CylinderShape : public Shape
{
public:
    CylinderShape(int shape_type, int p1, int p2);
    ~CylinderShape();

private:
    void computeVertices();
};

#endif // CYLINDERSHAPE_H
