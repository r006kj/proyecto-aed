//
// Created by leona on 24/09/2026.
//

#ifndef PROYECTO1_OCTREE_H
#define PROYECTO1_OCTREE_H

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBottom 4
#define TopRightBottom 5
#define BottomRightBack 6
#define BottomLeftBack 7

#include "Shader.h"
#include "Cube.h"

struct Point {
    int x,y,z;
    Point() : x(-1), y(-1), z(-1) {}

    Point(int x, int y, int z) : x(x), y(y), z(z) {}
};


class Octree {
    Point* point;

    Point *topLeftFront, *bottomRightBack;
    std::vector<Octree*> children;

    int level = 0;

    public:
    Octree();
    Octree(int x, int y, int z);
    Octree(int x1, int y1, int z1, int x2, int y2, int z2);

    void insert(int x, int y, int z);

    bool find(int x, int y, int z);

    void draw(const Shader& shader, const Cube& singleCubeMold) const;
};


#endif //PROYECTO1_OCTREE_H