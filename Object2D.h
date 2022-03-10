#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Object2D
{
    Mesh* CreateRectangle(const std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
    Mesh* CreateCircle(std::string name, float radius, glm::vec3 color);
    int  CheckBorderCollision(float playerX, float playerY, float radius, float width, float height);
    //bool CheckEnemyColision(float playerX, float playerY, float enemyX, float enemyY, float enemyWidth, float enemyHeight, float radius);
}   // namespace Object2D

