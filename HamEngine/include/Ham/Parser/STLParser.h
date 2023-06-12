#pragma once

#include "Ham/Core/Base.h"

#include "glm/glm.hpp"

#include <fstream>
#include <sstream>
#include <vector>

namespace Ham
{
    template <typename T>
    static void ReadSTLFile(const std::string &filePath, std::vector<T> &vertices, std::vector<unsigned int> &indices)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file)
        {
            // Failed to open file
            HAM_CORE_ERROR("Failed to open file '{0}'", filePath);
            return;
        }

        // Check if the file starts with "solid" keyword (ASCII format)
        std::string firstLine;
        std::getline(file, firstLine);
        file.seekg(0); // Reset the file position

        bool isBinary = true;
        if (firstLine.find("solid") != 0)
        {
            // File does not start with "solid" keyword, assume binary format
            isBinary = true;
        }
        else
        {
            // File starts with "solid" keyword, assume ASCII format
            isBinary = false;
        }

        if (isBinary)
        {
            // Binary format
            // Skip STL header (80 bytes)
            file.seekg(80);

            // Read the number of triangles
            unsigned int numTriangles;
            file.read(reinterpret_cast<char *>(&numTriangles), sizeof(unsigned int));

            vertices.clear();
            indices.clear();

            // Reserve memory to avoid frequent reallocation
            vertices.reserve(numTriangles * 3);
            indices.reserve(numTriangles * 3);

            // Read triangle data
            for (unsigned int i = 0; i < numTriangles; ++i)
            {
                // Read normal vector (ignored in this example)
                file.seekg(3 * sizeof(float), std::ios::cur);

                // Read vertex positions
                for (unsigned int j = 0; j < 3; ++j)
                {
                    glm::vec3 position;
                    file.read(reinterpret_cast<char *>(&position), 3 * sizeof(float));

                    // Add the vertex to the list
                    vertices.push_back({position, glm::vec3(0.0f)});
                    indices.push_back(indices.size());
                }

                // Skip attribute byte count (ignored in this example)
                file.seekg(sizeof(unsigned short), std::ios::cur);
            }
        }
        else
        {
            // ASCII format
            std::string line;
            std::vector<glm::vec3> positions;

            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string keyword;
                ss >> keyword;

                if (keyword == "facet")
                {
                    // Read normal vector (ignored in this example)
                    glm::vec3 normal;
                    ss >> keyword >> normal.x >> normal.y >> normal.z;
                }
                else if (keyword == "vertex")
                {
                    // Read vertex position
                    glm::vec3 position;
                    ss >> position.x >> position.y >> position.z;
                    positions.push_back(position);
                }
                else if (keyword == "endfacet")
                {
                    // Add the triangle vertices to the list
                    vertices.push_back({positions[0], glm::vec3(0.0f)});
                    vertices.push_back({positions[1], glm::vec3(0.0f)});
                    vertices.push_back({positions[2], glm::vec3(0.0f)});
                    indices.push_back(indices.size());
                    indices.push_back(indices.size());
                    indices.push_back(indices.size());

                    // Clear the positions vector for the next triangle
                    positions.clear();
                }
            }
        }
    }

    template <typename T>
    static void ReadOBJFile(const std::string &filePath, std::vector<T> &vertices, std::vector<unsigned int> &indices)
    {
        std::ifstream file(filePath);
        if (!file)
        {
            // File not found or unable to open
            HAM_CORE_ERROR("Failed to open file '{0}'", filePath);
            return;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "v")
            {
                glm::vec3 position;
                iss >> position.x >> position.y >> position.z;
                positions.push_back(position);
            }
            else if (token == "vn")
            {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (token == "f")
            {
                std::string faceData;
                while (iss >> faceData)
                {
                    std::istringstream faceIss(faceData);
                    std::string vertexIndexStr;
                    std::getline(faceIss, vertexIndexStr, '/');

                    unsigned int vertexIndex = std::stoi(vertexIndexStr) - 1;

                    T vertex;
                    vertex.Position = positions[vertexIndex];
                    vertex.Normal = normals[vertexIndex];
                    vertices.push_back(vertex);
                    indices.push_back(indices.size());
                }
            }
        }

        file.close();
    }
} // namespace Ham
