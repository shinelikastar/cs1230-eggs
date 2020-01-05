#include "FaceBuilder.h"
#include <cmath>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate

FaceBuilder::FaceBuilder()
{
}

/*** Returns vector of the vertices (with normal vectors
* interleaved) of a 1x1 square face with a center at (0,0,.5)
* and a tesselation value of tess.
***/
void FaceBuilder::computeSquareFace(std::vector<float> &face, int tess) {
    // Creates strips of vertices for each strip on the front face
    // of the cube.
    for (int i = 0; i < tess; i++) {

        // Degenerate
        if (i != 0) {
            // Vertex
            face.insert(face.end(), {-0.5f,
                                     0.5f - (i * (1.0f/tess)),
                                     0.5f});
            // Normal
            face.insert(face.end(), {0.0f, 0.0f, 1.0f});
        }

        for(int j = 0; j < tess + 1; j++) {
            // High vertex
            face.insert(face.end(), {-0.5f + (j * (1.0f/tess)),
                                     0.5f - (i * (1.0f/tess)),
                                     0.5f});
            // Normal
            face.insert(face.end(), {0.0f, 0.0f, 1.0f});

            // Low vertex
            face.insert(face.end(), {-0.5f + (j * (1.0f/tess)),
                                     0.5f - ((i+1) * (1.0f/tess)),
                                     0.5f});
            // Normal
            face.insert(face.end(), {0.0f, 0.0f, 1.0f});
        }

        // Degenerate
        if (i != tess - 1) {
            // Vertex
            face.insert(face.end(), {0.5f,
                                     0.5f - ((i+1) * (1.0f/tess)),
                                     0.5f});
            // Normal vector
            face.insert(face.end(), {0.0f, 0.0f, 1.0f});
        }
    }
}

void FaceBuilder::computeCircleFace(std::vector<float> &face, int radius_tess, int theta_tess) {
    std::vector<float> normal {0.0f, 1.0f, 0.0f};

    for (int i = 0; i < radius_tess; i++) {
        float r_top = (i+1) * (0.5f / radius_tess);
        float r_base = i * (0.5f / radius_tess);
        for (int j = 0; j < theta_tess + 1; j++) {
            float theta = j * (2.0f * M_PI / theta_tess);

            float x_top = r_top * std::cos(theta);
            float z_top = r_top * std::sin(theta);
            float x_base = r_base * std::cos(theta);
            float z_base = r_base * std::sin(theta);

            face.insert(face.end(), {x_top, 0.5f, z_top});
            face.insert(face.end(), normal.begin(), normal.end());

            // For degenerate triangle
            if (j == 0 && i != 0) {
                face.insert(face.end(), {x_top, 0.5f, z_top});
                face.insert(face.end(), normal.begin(), normal.end());
            }

            face.insert(face.end(), {x_base, 0.5f, z_base});
            face.insert(face.end(), normal.begin(), normal.end());

            // For degenerate triangle
            if (j == theta_tess && i != radius_tess - 1) {
                face.insert(face.end(), {x_base, 0.5f, z_base});
                face.insert(face.end(), normal.begin(), normal.end());
            }
        }
    }
}

void FaceBuilder::computeTubeFace(std::vector<float> &face, int y_tess, int theta_tess) {

    for (int i = 0; i < y_tess; i++) {
        float y_top = -0.5f + ((i+1) * (1.0f / y_tess));
        float y_bot = -0.5f + (i * (1.0f / y_tess));

        // Iterate from 2*pi to 0 for correct winding of triangle strip.
        for (int j = theta_tess; j >= 0; j--) {
            float theta = j * (2.0f * M_PI / theta_tess);

            float normal_x = std::cos(theta);
            float normal_z = std::sin(theta);

            float x = 0.5f * normal_x;
            float z = 0.5f * normal_z;

            face.insert(face.end(), {x, y_top, z});
            face.insert(face.end(), {normal_x, 0.0f, normal_z});

            if (j == theta_tess && i != 0) {
                face.insert(face.end(), {x, y_top, z});
                face.insert(face.end(), {normal_x, 0.0f, normal_z});
            }

            face.insert(face.end(), {x, y_bot, z});
            face.insert(face.end(), {normal_x, 0.0f, normal_z});

            if (j == 0 && i != y_tess-1) {
                face.insert(face.end(), {x, y_bot, z});
                face.insert(face.end(), {normal_x, 0.0f, normal_z});
            }
        }
    }
}

void FaceBuilder::computeConeFace(std::vector<float> &face, int y_tess, int theta_tess) {
    // Slope of wrapper of cone.
    float slope = 1.0f / 0.5f;

    // For each horizontal strip of cone wrapper
    for (int i = 0; i < y_tess; i++) {
        float y_top = -0.5f + ((i+1) * (1.0f / y_tess));
        float y_bot = -0.5f + (i * (1.0f / y_tess));

        // Calculates horizontal strip between y_bot and y_top.
        for (int j = theta_tess; j >= 0; j--) {
            float theta = j * (2.0f * M_PI / theta_tess);
            float x_top = (0.5f - ((y_top + 0.5f) / slope)) * std::cos(theta);
            float x_bot = (0.5f - ((y_bot + 0.5f) / slope)) * std::cos(theta);
            float z_top = (0.5f - ((y_top + 0.5f) / slope)) * std::sin(theta);
            float z_bot = (0.5f - ((y_bot + 0.5f) / slope)) * std::sin(theta);

            float x_norm = std::cos(theta);
            float z_norm = std::sin(theta);
            float y_norm = 1.0f / slope;

            glm::vec3 norm_vec = glm::vec3(x_norm, y_norm, z_norm);
            norm_vec = glm::normalize(norm_vec);

            face.insert(face.end(), {x_top, y_top, z_top});
            face.insert(face.end(), {norm_vec[0], norm_vec[1], norm_vec[2]});

            // For degenerate triangle
            if (j == theta_tess && i != 0) {
                face.insert(face.end(), {x_top, y_top, z_top});
                face.insert(face.end(), {norm_vec[0], norm_vec[1], norm_vec[2]});
            }

            face.insert(face.end(), {x_bot, y_bot, z_bot});
            face.insert(face.end(), {norm_vec[0], norm_vec[1], norm_vec[2]});

            // For degenerate triangle
            if (j == 0 && i != y_tess-1) {
                face.insert(face.end(), {x_bot, y_bot, z_bot});
                face.insert(face.end(), {norm_vec[0], norm_vec[1], norm_vec[2]});
            }
        }
    }
}
