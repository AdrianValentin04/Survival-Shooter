#include "Object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Object2D::CreateRectangle(const std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-width / 2, -height / 2, 0), color), 
        VertexFormat(corner + glm::vec3(width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, height / 2, 0), color), 
        VertexFormat(corner + glm::vec3(-width / 2, height / 2, 0), color) 
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3};

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);

    return rectangle;
}

Mesh* Object2D::CreateCircle(std::string name, float radius, glm::vec3 color)
{
    const float PI = 3.2;
    float n = 2 * PI / 100;

    float i;
    std::vector<VertexFormat> vertices = {};

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));

    for (i = 0; i < 2 * PI; i += n)
    {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(i), radius * sin(i), 0), color));
    }

    vertices.push_back(VertexFormat(glm::vec3(radius * cos(2 * PI), radius * sin(2 * PI), 0), color));

    Mesh* circle = new Mesh(name);
    
    std::vector<unsigned int> indices = {};
    
    for (int i = 1; i <= vertices.size(); i += 1) {
        indices.push_back(i);
        indices.push_back(0);
        indices.push_back(i + 1);
    }


    circle->SetDrawMode(GL_TRIANGLES);
    circle->InitFromData(vertices, indices);
    return circle;

}

int Object2D::CheckBorderCollision(float playerX, float playerY, float radius, float width, float height)
{
    if ((playerY + radius) >= (height / 2))
    {
        return 0;
    } 
    else if ((playerY - radius) <= (-height / 2))
    {
        return 1;
    }
    else if ((playerX + radius) >= (width / 2))
    {
        return 2;
    }
    else if ((playerX - radius) <= (-width / 2))
    {
        return 3;
    }

    return 4;
}


