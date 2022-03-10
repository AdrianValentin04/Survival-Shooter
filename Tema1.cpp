#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Transform2D.h"
#include "lab_m1/Tema1/Object2D.h"
#include <thread>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = -250;       // logic x
    logicSpace.y = -250;       // logic y
    logicSpace.width = 500;    // logic width
    logicSpace.height = 500;   // logic height

    glm::vec3 corner = glm::vec3(0, 0, 0);

    Mesh* base = Object2D::CreateRectangle("base", corner, width + 10, height + 10, glm::vec3(0.00f, 0.00f, 0.61f), true);
    AddMeshToList(base); //baza pentru conturul hartii
    Mesh* map = Object2D::CreateRectangle("map", corner, width, height, glm::vec3(0.2f, 0.21f, 0.26f), true);
    AddMeshToList(map); //harta


    Mesh* body = Object2D::CreateCircle("body", radiusBody, glm::vec3(0, 1, 1));
    AddMeshToList(body); //corpul player-uli
    Mesh* baseBody = Object2D::CreateCircle("baseBody", radiusBody + 2, glm::vec3(0, 0, 0));
    AddMeshToList(baseBody); //baza pentru corpul player-ului

    Mesh* playerhand = Object2D::CreateCircle("hand", radiusHand, glm::vec3(0.647f, 1, 1));
    AddMeshToList(playerhand); //mana player-ului
    Mesh* basePlayerHand = Object2D::CreateCircle("baseHand", radiusHand + 1, glm::vec3(0, 0, 0));
    AddMeshToList(basePlayerHand); // baza pentru mana player-ullui


    Mesh* thinObstacle = Object2D::CreateRectangle("thin",corner , widthThin, heightThin,
        glm::vec3(0.3f, 0.3f, 0.3f), true);
    AddMeshToList(thinObstacle); //obstacol1
    Mesh* thinObstacleBase = Object2D::CreateRectangle("thinBase", corner, widthThin + 5, heightThin + 5, 
        glm::vec3(0.00f, 0.00f, 0.61f), true);
    AddMeshToList(thinObstacleBase); //baza pentru obstacol1

    Mesh* thickObstacle = Object2D::CreateRectangle("thick", corner, widthThick, heightThick, 
        glm::vec3(0.3f, 0.3f, 0.3f), true);
    AddMeshToList(thickObstacle); //obstacol2
    Mesh* thickObstacleBase = Object2D::CreateRectangle("thickBase", corner, widthThick + 5, heightThick + 5, 
        glm::vec3(0.00f, 0.00f, 0.61f), true);
    AddMeshToList(thickObstacleBase); //baza pentru obstacol2


    Mesh* projectile = Object2D::CreateRectangle("PEW", corner, projectileWidth, projectileHeight, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile); //proiectil


    Mesh* enemyBody = Object2D::CreateRectangle("enemyBody", corner, enemyWidth, enemyHeight, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemyBody); //corpul inamicului
    Mesh* baseEnemyBody = Object2D::CreateRectangle("baseEnemyBody", corner, enemyWidth + 2, enemyHeight + 3, glm::vec3(0, 0, 0), true);
    AddMeshToList(baseEnemyBody); //baza pentru corpul inamicului

    Mesh* enemyHorn = Object2D::CreateRectangle("enemyHorn", corner, enemyHornWidth, enemyHornHeight, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyHorn); //coarnele inamicului
    Mesh* enemyHornBase = Object2D::CreateRectangle("baseEnemyHorn", corner, enemyHornWidth + 2, enemyHornHeight + 2, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyHornBase); //baza pentru coarnele inamicului


    Mesh* healthBar = Object2D::CreateRectangle("health", corner, healthWidth , healthHeight, glm::vec3(0, 1, 0), true);
    AddMeshToList(healthBar); //healthbar
    Mesh* healthBar1 = Object2D::CreateRectangle("health1", corner, healthWidth * 2 / 3, healthHeight, glm::vec3(0, 1, 0), true);
    AddMeshToList(healthBar); //healthbar
    Mesh* healthBar2 = Object2D::CreateRectangle("health2", corner, healthWidth / 3, healthHeight, glm::vec3(0, 1, 0), true);
    AddMeshToList(healthBar); //healthbar
    Mesh* baseHealthBar = Object2D::CreateRectangle("baseHealth", corner, healthWidth + 5, healthHeight + 5, glm::vec3(1, 1, 1), true);
    AddMeshToList(baseHealthBar); //chenat pentru healthbar

}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), 
        (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    
    DrawHealthbar(visMatrix);
    DrawEnemy(visMatrix);
    DrawPlayer(visMatrix);
    DrawProjectile(visMatrix);
    UpdateProjectile(deltaTimeSeconds);
    DrawObstacles(visMatrix);
    DrawMap(visMatrix);
  
    if (damage == 3)
    {
        FinalScene();
        DrawDeadPlayer(visMatrix);
        GameOver();
        exit(0);
    }

    if (score == targetScore)
    {
        WinScene();
        DrawDeadPlayer(visMatrix);
        YouWin();
        exit(0);
    }

}

void Tema1::GameOver()
{
    cout << "   _____                         ____                       " << endl;
    cout << "  / ____|                       / __ \\                     " << endl;
    cout << " | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __       " << endl;
    cout << " | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__| " << endl;
    cout << " | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |        " << endl;
    cout << "  \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   " << endl;

    cout << "========================Scor==========================" << endl;
    cout << "                         " << score << "              " << endl;
}

void Tema1::YouWin()
{
    cout << "  __     __         __          __                " << endl;
    cout << "  \\ \\   / /         \\ \\        / /            " << endl;
    cout << "   \\ \\_/ /__  _   _  \\ \\  /\\  / /__  _ __    " << endl;
    cout << "    \\   / _ \\| | | |  \\ \\/  \\/ / _ \\| '_ \\ " << endl;
    cout << "     | | (_) | |_| |   \\  /\\  / (_) | | | |"      << endl;
    cout << "     |_|\\___/ \\__,_|    \\/  \\/ \\___/|_| |_|"   << endl;

    cout << "========================Scor==========================" << endl;
    cout << "                         " << score << "              " << endl;
}


void Tema1::FrameEnd()
{
}

void Tema1::DrawHealthbar(glm::mat3 visMatrix)
{
    switch (damage)
    {
    case 1:
        modelMatrix = visMatrix * transform2D::Translate(playerX + width / 4, playerY + height / 4.5f);
        RenderMesh2D(meshes["health1"], shaders["VertexColor"], modelMatrix);
    case 2:
        modelMatrix = visMatrix * transform2D::Translate(playerX + width / 4, playerY + height / 4.5f);
        RenderMesh2D(meshes["health2"], shaders["VertexColor"], modelMatrix);
    case 0:
        modelMatrix = visMatrix * transform2D::Translate(playerX + width / 4, playerY + height / 4.5f);
        RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
    }
    
    
    modelMatrix = visMatrix * transform2D::Translate(playerX + width / 4, playerY + height / 4.5f);
    RenderMesh2D(meshes["baseHealth"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawPlayer(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle + PI / 6) * radiusBody + radiusHand / 2,
        playerY - cos(playerAngle + PI / 6) * radiusBody);
    RenderMesh2D(meshes["hand"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle - PI / 6) * radiusBody + radiusHand / 2,
        playerY - cos(playerAngle - PI / 6) * radiusBody);
    RenderMesh2D(meshes["hand"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle + PI / 6) * radiusBody,
        playerY - cos(playerAngle + PI / 6) * radiusBody);
    RenderMesh2D(meshes["baseHand"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle - PI / 6) * radiusBody,
        playerY - cos(playerAngle - PI / 6) * radiusBody);
    RenderMesh2D(meshes["baseHand"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);
    modelMatrix *= transform2D::Rotate(playerAngle);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);
    modelMatrix *= transform2D::Rotate(playerAngle);
    RenderMesh2D(meshes["baseBody"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawDeadPlayer(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle + PI / 6) * radiusBody + radiusHand / 2,
        playerY - cos(playerAngle + PI / 6) * radiusBody);
    RenderMesh2D(meshes["hand"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle - PI / 6) * radiusBody + radiusHand / 2,
        playerY - cos(playerAngle - PI / 6) * radiusBody);
    RenderMesh2D(meshes["hand"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle + PI / 6) * radiusBody,
        playerY - cos(playerAngle + PI / 6) * radiusBody);
    RenderMesh2D(meshes["baseHand"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX + sin(playerAngle - PI / 6) * radiusBody,
        playerY - cos(playerAngle - PI / 6) * radiusBody);
    RenderMesh2D(meshes["baseHand"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);   
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);  
    RenderMesh2D(meshes["baseBody"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawProjectile(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(projectileX, projectileY);
    modelMatrix *= transform2D::Rotate(projectileAngle);
    RenderMesh2D(meshes["PEW"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawEnemy(glm::mat3 visMatrix)
{

    modelMatrix = visMatrix * transform2D::Translate(enemyX + sin(enemyAngle + PI / 4 + PI / 2) * (enemyHeight / 2),
        enemyY - cos(enemyAngle + PI / 4 + PI / 2) * (enemyHeight / 2));
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["enemyHorn"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(enemyX + sin(enemyAngle + PI / 4 + PI / 2) * (enemyHeight / 2), 
        enemyY - cos(enemyAngle + PI / 4 + PI / 2) * (enemyHeight / 2));
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["baseEnemyHorn"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(enemyX + sin(enemyAngle - PI / 4 + PI / 2) * (enemyHeight / 2), 
        enemyY - cos(enemyAngle - PI / 4 + PI / 2) * (enemyHeight / 2));
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["enemyHorn"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(enemyX + sin(enemyAngle - PI / 4 + PI / 2) * (enemyHeight / 2), 
        enemyY - cos(enemyAngle - PI / 4 + PI / 2) * (enemyHeight / 2));
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["baseEnemyHorn"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(enemyX, enemyY);
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(enemyX, enemyY);
    modelMatrix *= transform2D::Rotate(enemyAngle);
    RenderMesh2D(meshes["baseEnemyBody"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawObstacles(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(-width / 3, height / 4.2f);
    RenderMesh2D(meshes["thin"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(-width / 3, height / 4.2f);
    RenderMesh2D(meshes["thinBase"], shaders["VertexColor"], modelMatrix);

    obs[0].x = -width / 3;
    obs[0].y = height / 4.2f;
    obs[0].width = widthThin;
    obs[0].height = heightThin;

    modelMatrix = visMatrix * transform2D::Translate(-width / 3, -height / 4.2f);
    RenderMesh2D(meshes["thin"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(-width / 3, -height / 4.2f);
    RenderMesh2D(meshes["thinBase"], shaders["VertexColor"], modelMatrix);

    obs[1].x = -width / 3;
    obs[1].y = -height / 4.2f;
    obs[1].width = widthThin;
    obs[1].height = heightThin;
    
    modelMatrix = visMatrix * transform2D::Translate(width / 3, height / 4.2f);
    RenderMesh2D(meshes["thin"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(width / 3, height / 4.2f);
    RenderMesh2D(meshes["thinBase"], shaders["VertexColor"], modelMatrix);

    obs[2].x = width / 3;
    obs[2].y = height / 4.2f;
    obs[2].width = widthThin;
    obs[2].height = heightThin;

    modelMatrix = visMatrix * transform2D::Translate(width / 3, -height / 4.2f);
    RenderMesh2D(meshes["thin"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(width / 3, -height / 4.2f);
    RenderMesh2D(meshes["thinBase"], shaders["VertexColor"], modelMatrix);

    obs[3].x = width / 3;
    obs[3].y = -height / 4.2f;
    obs[3].width = widthThin;
    obs[3].height = heightThin;

    modelMatrix = visMatrix * transform2D::Translate(-100, height / 3.6f);
    RenderMesh2D(meshes["thick"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(-100, height / 3.6f);
    RenderMesh2D(meshes["thickBase"], shaders["VertexColor"], modelMatrix);

    obs[4].x = -100;
    obs[4].y = height / 3.6f;
    obs[4].width = widthThick;
    obs[4].height = heightThick;

    modelMatrix = visMatrix * transform2D::Translate(100, -height / 3.6f);
    RenderMesh2D(meshes["thick"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(100, -height / 3.6f);
    RenderMesh2D(meshes["thickBase"], shaders["VertexColor"], modelMatrix);

    obs[5].x = 100;
    obs[5].y = -height / 3.6f;
    obs[5].width = widthThick;
    obs[5].height = heightThick;

}

void Tema1::DrawMap(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
    modelMatrix = visMatrix * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["base"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FinalScene()
{
    glClearColor(0.5f, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    finalScene = 1;
}

void Tema1::WinScene()
{
    glClearColor(0, 0.5f, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    finalScene = 1;
}

int Tema1::CheckObstacleColision(Obstacle obs[])
{
    glm::vec2 playerCenter = { playerX, playerY };
    glm::vec2 obsCenter, diff, clamped, closest, cornerObstacle ;

    for (int i = 0; i < 6; i++)
    {
        cornerObstacle = { obs[i].width / 2.0f, obs[i].height / 2.0f };
        obsCenter = { obs[i].x, obs[i].y };

        diff = playerCenter - obsCenter;

        clamped = glm::clamp(diff, -cornerObstacle, cornerObstacle);

        closest = obsCenter + clamped;
        diff = closest - playerCenter;

        if (glm::length(diff) < radiusBody)
        {
            if (closest[1] == (obsCenter[1] + (obs[i].height / 2)))
            {
                return 0; // coliziune cu peretele de sus al obstacolelor
            }
            else if (closest[1] == (obsCenter[1] - (obs[i].height / 2)))
            {
                return 1; // coliziune cu peretele de jos al obstacolelor
            }
            else if (closest[0] == (obsCenter[0] + (obs[i].width / 2)))
            {
                return 2; //coliziune cu peretele din dreapta al obstacolelor
            }
            else if (closest[0] == (obsCenter[0] - (obs[i].width / 2)))
            {
                return 3; //coliziune cu peretele din stanga al obstacolelor
            }

        }

    }

    return 4;
}

bool Tema1::CheckEnemyColision()
{
    glm::vec2 playerCenter = { playerX, playerY };
    glm::vec2 obsCenter, diff, clamped, closest, cornerObstacle;

    cornerObstacle = { enemyWidth / 2.0f, enemyHeight / 2.0f };
    obsCenter = { enemyX, enemyY };

    diff = playerCenter - obsCenter;

    clamped = glm::clamp(diff, -cornerObstacle, cornerObstacle);

    closest = obsCenter + clamped;
    diff = closest - playerCenter;

    if (glm::length(diff) < radiusBody)
    {
        return true;
    }

    return false;
}

bool Tema1::CheckProjectileEnemyCollision()
{
    bool collisionX = projectileX  <= enemyX + enemyWidth * 0.5f && projectileX >= enemyX - enemyWidth * 0.5f;
    bool collisionY = projectileY  <= enemyY + enemyHeight * 0.5f && projectileY  >= enemyY - enemyHeight * 0.5f;

    return collisionX && collisionY;
}

bool Tema1::CheckProjectileBorderCollision()
{
    bool collisionX = projectileX <= (width / 2) * 0.5f && projectileX >= (-width / 2) * 0.5f * 0.5f;
    bool collisionY = projectileY <= (height / 2) * 0.5f && projectileY >= (-height / 2) * 0.5f * 0.5f;

    return collisionX && collisionY;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (deltaTime < 0 || isinf(deltaTime))
        return;
    if (finalScene == 0)
    {
        if (window->KeyHold(GLFW_KEY_W) && Object2D::CheckBorderCollision(playerX, playerY, radiusBody, width, height) != 0) {
            if (Tema1::CheckObstacleColision(obs) != 1)
            {
                logicSpace.y += deltaTime * playerSpeed;
                playerY += deltaTime * playerSpeed;
            }
        }


        if (window->KeyHold(GLFW_KEY_S) && Object2D::CheckBorderCollision(playerX, playerY, radiusBody, width, height) != 1) {
            if (Tema1::CheckObstacleColision(obs) != 0)
            {
                logicSpace.y -= deltaTime * playerSpeed;
                playerY -= deltaTime * playerSpeed;
            }
        }


        if (window->KeyHold(GLFW_KEY_A) && Object2D::CheckBorderCollision(playerX, playerY, radiusBody, width, height) != 3) {
            if (Tema1::CheckObstacleColision(obs) != 2)
            {
                logicSpace.x -= deltaTime * playerSpeed;
                playerX -= deltaTime * playerSpeed;
            }
        }


        if (window->KeyHold(GLFW_KEY_D) && Object2D::CheckBorderCollision(playerX, playerY, radiusBody, width, height) != 2) {
            if (Tema1::CheckObstacleColision(obs) != 3)
            {
                logicSpace.x += deltaTime * playerSpeed;
                playerX += deltaTime * playerSpeed;
            }
        }
    }

    if (!Tema1::CheckEnemyColision())
    {
        enemyAngle = atan2(playerY - enemyY, playerX - enemyX);
        enemyX += cos(enemyAngle) * enemySpeed * deltaTime;
        enemyY += sin(enemyAngle) * enemySpeed * deltaTime;
    }
    else
    {
        if (damage != 3)
        {
            enemyDir = rand() % 4;
            switch (enemyDir)
            {
            case 0:
                enemyY = height / 2;
                break;
            case 1:
                enemyY = -height / 2;
                break;
            case 2:
                enemyX = width / 2;
                break;
            case 3:
                enemyX = -width / 2;
                break;
            }
            damage++;
        }
        
    }

    if (Tema1::CheckProjectileEnemyCollision())
    {
        enemyDir = rand() % 4;
        switch (enemyDir)
        {
        case 0:
            enemyY = height / 2;
            break;
        case 1:
            enemyY = -height / 2;
            break;
        case 2:
            enemyX = width / 2;
            break;
        case 3:
            enemyX = -width / 2;
            break;
        }
        score++;
    }
    
}

void Tema1::UpdateProjectile(float deltaTimeSeconds)
{
    if (trigger)
    {
        projectileX += projectileSpeed * sin(projectileAngle) * deltaTimeSeconds;
        projectileY -= projectileSpeed * cos(projectileAngle) * deltaTimeSeconds;

        if (projectileX > playerX + projectileDistance || projectileY > playerY + projectileDistance
            || projectileX < playerX - projectileDistance || projectileY < playerY - projectileDistance
            || CheckProjectileBorderCollision())
        {
            trigger = false;
            projectileX = playerX;
            projectileY = playerY;
            projectileAngle = playerAngle;
            projectileSpeed = 0;
        }
    }
    else
    {
        projectileX = playerX;
        projectileY = playerY;
        projectileAngle = playerAngle;
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (damage != 3)
    {
        playerAngle = atan2((mouseX - (window->GetResolution().x) / 2), (mouseY - (window->GetResolution().y) / 2));
    }
    
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        projectileSpeed += 500;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        trigger = true;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
