// TODO: Add this into PrimitiveMaker class

#pragma once
#include <fstream>
#include <vector>
#include "Utilities.h"

using namespace std;

class BinaryLoader
{
public:
    void LoadFileIntoVertices()
    {
// we defined a custom binary file format, where
// the first 32 bits = vertex count,
// then each line contains float vert3, uv2, and norm3 numbers

    // tell the stream to open the file in binary mode
        ifstream file(L"Meshes\\cube", std::ios::binary);

        if (file.fail())
        {
            return;
        }

        int vertexCount = 0; // read first 32 bits into vertexCount
        file.read(reinterpret_cast<char*>(&vertexCount), sizeof(int));

        // pack into vector of structs
        vector<VertexPositionUVNormalType> vertList(vertexCount);
        file.read(reinterpret_cast<char*>(vertList.data()), sizeof(VertexPositionUVNormalType)*vertexCount);

        // TODO: Create UTILITY to pack other mesh formats into binary file
    }
};
