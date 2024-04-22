#include "Prerequisites.h"
#pragma once

class ModelLoader
{
public:
    ModelLoader() = default;
    ~ModelLoader() = default;

    void
        init();

    void
        update();

    void
        render();

    void
        destroy();

    Mesh
        Load(std::string objFileName);

private:
    //objl::Loader m_loader;
};