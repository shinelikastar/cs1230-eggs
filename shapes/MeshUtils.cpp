#include "MeshUtils.h"
#include <string>
#include <iostream>
#include <fstream>

namespace MeshUtils
{

int loadObjFile(const char* path, std::vector<float> &data) {

    std::vector<glm::vec3> vertexLookup, normalLookup;
    std::vector<glm::vec2> textureLookup;
    std::vector<int> vertexIndices, normalIndices, textureIndices;

    std::ifstream ifStream(path);

    if (!ifStream) {
        fprintf(stderr, "Could not open meshfile.");
        return -1;
    }

    // Iterate line by line through mesh file
    std::string curLine;
    while (std::getline(ifStream, curLine)) {

        if (curLine.length() < 1) {
            continue;
        }

        // Duplicate line into char[] then tokenize
        char* curLineDup = strdup(curLine.c_str());
        char* keyword = strtok(curLineDup, " \t\n");

        if (strcmp(keyword, "v") == 0) {
            // Vertex
            char* v1 = strtok(NULL, " \t\n");
            char* v2 = strtok(NULL, " \t\n");
            char* v3 = strtok(NULL, " \t\n");

            glm::vec3 curVertex = {atof(v1), atof(v2), atof(v3)};

            vertexLookup.push_back(curVertex);

        } else if (strcmp(keyword, "vn") == 0) {
            // Normal
            char* n1 = strtok(NULL, " \t\n");
            char* n2 = strtok(NULL, " \t\n");
            char* n3 = strtok(NULL, " \t\n");

            glm::vec3 curNormal = {atof(n1), atof(n2), atof(n3)};

            normalLookup.push_back(curNormal);
        } else if (strcmp(keyword, "vt") == 0) {
//             Texture
            char* t1 = strtok(NULL, " \t\n");
            char* t2 = strtok(NULL, " \t\n");

            glm::vec2 curTexture = {atof(t1), atof(t2)};

            textureLookup.push_back(curTexture);
        } else if (strcmp(keyword, "f") == 0) {
            // Face
            for (int i = 0; i < 3; i++) {
                char* vertexI = strtok(NULL, " \t\n/");
                char* textureI = strtok(NULL, " \t\n/");
                char* normalI = strtok(NULL, " \t\n/");

                vertexIndices.push_back(atoi(vertexI));
                textureIndices.push_back(atoi(textureI));
                normalIndices.push_back(atoi(normalI));
            }
        }
    }

    if (vertexLookup.size() == 0) {
        fprintf(stderr, "No vertices found.\n");
        return -1;
    }

    if (normalLookup.size() == 0) {
        fprintf(stderr, "No normals found.\n");
        return -1;
    }

    if (vertexIndices.size() != normalIndices.size()) {
        fprintf(stderr, "Mismatch of vertex and normal vectors.\n");
        return -1;
    }

    // Interleave the vertex, normal, and texture coordinates
    // in preperation for VAO
    for (int i = 0; i < vertexIndices.size(); i += 1) {

        unsigned int vIndex = vertexIndices.at(i) - 1;
        unsigned int nIndex = normalIndices.at(i) - 1;
        unsigned int tIndex = textureIndices.at(i) - 1;

        glm::vec3 curVertex = vertexLookup.at(vIndex);
        glm::vec3 curNormal = normalLookup.at(nIndex);
        glm::vec2 curTexture = textureLookup.at(tIndex);

        data.push_back(curVertex.x);
        data.push_back(curVertex.y);
        data.push_back(curVertex.z);

        data.push_back(curNormal.x);
        data.push_back(curNormal.y);
        data.push_back(curNormal.z);

        // Not storing uv-coordinates just yet...
        data.push_back(curTexture.x);
        data.push_back(curTexture.y);

    }

    ifStream.close();

    std::cout << "finished loading mesh: " << path << std::endl;

    return 0;
}

// End of namespace
}
