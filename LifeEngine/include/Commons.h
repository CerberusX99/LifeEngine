#pragma once
#include "Prerequisites.h"


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

struct
    LoadData {
    std::string name;
    std::vector <SimpleVertex> vertex;
    std::vector <unsigned int> index;
    int numVertex;
    int numIndex;
};

struct
    Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
