//
// Created by leona on 24/09/2026.
//

#include "Octree.h"

Octree::Octree() {
    point = new Point();
}

Octree::Octree(int x, int y, int z) {
    point = new Point(x,y,z);
}

Octree::Octree(int x1, int y1, int z1, int x2, int y2, int z2, int currentLevel) {
    if (x2 < x1 || y2 < y1 || z2 < z1) {
        std::cout << "boundary points are not valid" << std::endl;
        return;
    }

    this->level = currentLevel;
    point = nullptr;
    topLeftFront = new Point(x1, y1, z1);
    bottomRightBack = new Point(x2, y2, z2);

    int midx = (x1 + x2) / 2;
    int midy = (y1 + y2) / 2;
    int midz = (z1 + z2) / 2;

    children.assign(8, nullptr);
    for (int i = TopLeftFront; i <= BottomLeftBack; ++i) {
        children[i] = new Octree();
        children[i]->level = this->level + 1;
    }

    children[TopLeftFront]->topLeftFront     = new Point(x1, y1, z1);
    children[TopLeftFront]->bottomRightBack = new Point(midx, midy, midz);

    children[TopRightFront]->topLeftFront    = new Point(midx + 1, y1, z1);
    children[TopRightFront]->bottomRightBack= new Point(x2, midy, midz);

    children[BottomRightFront]->topLeftFront    = new Point(midx + 1, midy + 1, z1);
    children[BottomRightFront]->bottomRightBack= new Point(x2, y2, midz);

    children[BottomLeftFront]->topLeftFront     = new Point(x1, midy + 1, z1);
    children[BottomLeftFront]->bottomRightBack = new Point(midx, y2, midz);

    children[TopLeftBottom]->topLeftFront     = new Point(x1, y1, midz + 1);
    children[TopLeftBottom]->bottomRightBack = new Point(midx, midy, z2);

    children[TopRightBottom]->topLeftFront    = new Point(midx + 1, y1, midz + 1);
    children[TopRightBottom]->bottomRightBack= new Point(x2, midy, z2);

    children[BottomRightBack]->topLeftFront    = new Point(midx + 1, midy + 1, midz + 1);
    children[BottomRightBack]->bottomRightBack= new Point(x2, y2, z2);

    children[BottomLeftBack]->topLeftFront     = new Point(x1, midy + 1, midz + 1);
    children[BottomLeftBack]->bottomRightBack = new Point(midx, y2, z2);
}

void Octree::insert(int x, int y, int z) {
    // 1. Evitar duplicados exactos
    if (find(x, y, z)) {
        std::cout << "Point already exist in the tree" << std::endl;
        return;
    }

    // 2. Control de límites geográficos
    if (x < topLeftFront->x || x > bottomRightBack->x || y < topLeftFront->y
        || y > bottomRightBack->y || z < topLeftFront->z || z > bottomRightBack->z) {
        std::cout << "Point is out of bound" << std::endl;
        return;
    }

    int midx = (topLeftFront->x + bottomRightBack->x) / 2;
    int midy = (topLeftFront->y + bottomRightBack->y) / 2;
    int midz = (topLeftFront->z + bottomRightBack->z) / 2;

    int pos = -1;

    // Determinación del cuadrante
    if (x <= midx) {
        if (y <= midy) {
            if (z <= midz) pos = TopLeftFront;
            else           pos = TopLeftBottom;
        } else {
            if (z <= midz) pos = BottomLeftFront;
            else           pos = BottomLeftBack;
        }
    } else {
        if (y <= midy) {
            if (z <= midz) pos = TopRightFront;
            else           pos = TopRightBottom;
        } else {
            if (z <= midz) pos = BottomRightFront;
            else           pos = BottomRightBack;
        }
    }

    // CASO A: El hijo está vacío (aún no tiene un punto asignado)
    if (children[pos]->point == nullptr) {
        children[pos]->insert(x, y, z);
        return;
    }
    // CASO B: El hijo tiene un punto inicializador vacío (x == -1)
    else if (children[pos]->point->x == -1) {
        children[pos]->point->x = x;
        children[pos]->point->y = y;
        children[pos]->point->z = z;
        return;
    }
    // CASO C: El peor caso (Ya hay un punto real en esa posición espacial)
    else {
        // CORRECCIÓN DE SEGURIDAD: Si alcanzamos la profundidad máxima, NO subdividimos más.
        // Sobrescribimos o mantenemos el punto según la lógica de tu simulación para evitar el Stack Overflow.
        if (this->level >= MAX_DEPTH) {
            std::cout << "MAX_DEPTH reached. Cannot subdivide further." << std::endl;
            // Opcional: Actualizar el valor del punto actual en lugar de colgar el programa
            children[pos]->point->x = x;
            children[pos]->point->y = y;
            children[pos]->point->z = z;
            return;
        }

        int x_ = children[pos]->point->x,
            y_ = children[pos]->point->y,
            z_ = children[pos]->point->z;

        // Liberamos el nodo hoja simple anterior
        delete children[pos]->point;
        delete children[pos];

        // Reconstruimos el hijo como un nuevo sub-Octree con sus propios límites espaciales reducidos
        if (pos == TopLeftFront) {
            children[pos] = new Octree(topLeftFront->x, topLeftFront->y, topLeftFront->z, midx, midy, midz,this->level + 1);
        }
        else if (pos == TopRightFront) {
            children[pos] = new Octree(midx + 1, topLeftFront->y, topLeftFront->z, bottomRightBack->x, midy, midz,this->level + 1);
        }
        else if (pos == BottomRightFront) {
            children[pos] = new Octree(midx + 1, midy + 1, topLeftFront->z, bottomRightBack->x, bottomRightBack->y, midz,this->level + 1);
        }
        else if (pos == BottomLeftFront) {
            children[pos] = new Octree(topLeftFront->x, midy + 1, topLeftFront->z, midx, bottomRightBack->y, midz,this->level + 1);
        }
        else if (pos == TopLeftBottom) {
            children[pos] = new Octree(topLeftFront->x, topLeftFront->y, midz + 1, midx, midy, bottomRightBack->z,this->level + 1);
        }
        else if (pos == TopRightBottom) {
            children[pos] = new Octree(midx + 1, topLeftFront->y, midz + 1, bottomRightBack->x, midy, bottomRightBack->z,this->level + 1);
        }
        else if (pos == BottomRightBack) {
            children[pos] = new Octree(midx + 1, midy + 1, midz + 1, bottomRightBack->x, bottomRightBack->y, bottomRightBack->z,this->level + 1);
        }
        else if (pos == BottomLeftBack) {
            children[pos] = new Octree(topLeftFront->x, midy + 1, midz + 1, midx, bottomRightBack->y, bottomRightBack->z,this->level + 1);
        }

        // Insertamos de forma recursiva tanto el punto antiguo como el nuevo
        children[pos]->insert(x_, y_, z_);
        children[pos]->insert(x, y, z);
    }
}

bool Octree::find(int x, int y, int z) {
    if (x < topLeftFront->x || x > bottomRightBack->x || y < topLeftFront->y
        || y > bottomRightBack->y || z < topLeftFront->z || z > bottomRightBack->z)
        return false;

    int midx = (topLeftFront->x + bottomRightBack->x) / 2;
    int midy = (topLeftFront->y + bottomRightBack->y) / 2;
    int midz = (topLeftFront->z + bottomRightBack->z) / 2;

    int pos = -1;

    if (x <= midx) {
        if (y <= midy) {
            if (z <= midz) pos = TopLeftFront;
            else           pos = TopLeftBottom;
        } else {
            if (z <= midz) pos = BottomLeftFront;
            else           pos = BottomLeftBack;
        }
    } else {
        if (y <= midy) {
            if (z <= midz) pos = TopRightFront;
            else           pos = TopRightBottom;
        } else {
            if (z <= midz) pos = BottomRightFront;
            else           pos = BottomRightBack;
        }
    }

    // Validación de seguridad para evitar accesos a punteros nullptr
    if (children[pos] == nullptr) {
        return false;
    }

    if (children[pos]->point == nullptr) {
        return children[pos]->find(x, y, z);
    }

    if (children[pos]->point->x == -1) {
        return false;
    }

    return (x == children[pos]->point->x &&
            y == children[pos]->point->y &&
            z == children[pos]->point->z);
}

void Octree::draw(int colorLoc, int modelLoc, const Cube &singleCubeMold) const {
    if (point != nullptr && point->x == -1 && point->y == -1 && point->z == -1)
        return;

    if (topLeftFront != nullptr && bottomRightBack != nullptr) {
        glm::vec3 center(
            (topLeftFront->x + bottomRightBack->x) / 2.0f,
            (topLeftFront->y + bottomRightBack->y) / 2.0f,
            (topLeftFront->z + bottomRightBack->z) / 2.0f
        );

        glm::vec3 size(
            abs(bottomRightBack->x - topLeftFront->x),
            abs(bottomRightBack->y - topLeftFront->y),
            abs(bottomRightBack->z - topLeftFront->z)
        );

        size -= (float)this->level * 0.05f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, center);
        model = glm::scale(model, size); // Redimensiona el cubo molde al tamaño de la región
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        float factor = (float)this->level / (float)(MAX_DEPTH+1);
        glm::vec3 colorRaiz(0.2f, 0.5f, 0.8f);
        glm::vec3 colorHoja(1.0f, 0.3f, 0.3f);
        glm::vec3 finalColor = glm::mix(colorRaiz, colorHoja, factor);

        glUniform3f(colorLoc, finalColor.x, finalColor.y, finalColor.z);

        glBindVertexArray(singleCubeMold.getVAO());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    for (const auto& child : children) {
        if (child != nullptr) {
            child->draw(colorLoc, modelLoc, singleCubeMold);
        }
    }
}

bool Octree::isFull() const {
    if (children.empty() || children[0] == nullptr) {
        return (point != nullptr && point->x != -1);
    }
    for (const auto& child : children) {
        if (child != nullptr) {
            if (!child->isFull()) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}



