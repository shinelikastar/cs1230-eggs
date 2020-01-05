#ifndef FACES_H
#define FACES_H

#include <vector>

class FaceBuilder
{
public:
    FaceBuilder();

    /*** Returns vector of the vertices (with normal vectors
    * interleaved) of a 1x1 square face with a center at (0,0,.5)
    * and a tesselation value of tess.
    ***/
    void computeSquareFace(std::vector<float> &face, int tess);
    void computeCircleFace(std::vector<float> &face, int radius_tess, int theta_tess);
    void computeTubeFace(std::vector<float> &face, int y_tess, int theta_tess);
    void computeConeFace(std::vector<float> &face, int y_tess, int theta_tess);
};

#endif // FACES_H
