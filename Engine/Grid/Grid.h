#pragma once

#include <iostream>
#include <vector>

class MeshComponent;

class Grid {
public:
    Grid();
    ~Grid();

    void SetData(unsigned int size);

    void Render();

private:
    void Init();

    void Clean();

private:
    std::unique_ptr<MeshComponent> meshComponent = nullptr;

    std::vector<float> vertex;
    std::vector<int> indices;
    const int unit = 1;
    unsigned int N = 50; //default points per line/column
};