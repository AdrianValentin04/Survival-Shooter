#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct Obstacle
        {
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawHealthbar(glm::mat3 visMatrix);
        void DrawPlayer(glm::mat3 visMatrix);
        void DrawProjectile(glm::mat3 visMatrix);
        void DrawEnemy(glm::mat3 visMatrix);
        void DrawObstacles(glm::mat3 visMatrix);
        void DrawMap(glm::mat3 visMatrix);
        void FinalScene();
        int  CheckObstacleColision(Obstacle obs[]);
        bool CheckEnemyColision();
        bool CheckProjectileEnemyCollision();
        bool CheckProjectileBorderCollision();
        void DrawDeadPlayer(glm::mat3 visMatrix);
        void UpdateProjectile(float deltaTimeSeconds);
        void WinScene();
        void GameOver();
        void YouWin();
        
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        const float PI = 3.1415;
        float width = 1300, height = 1000;        
        float playerX = 0, playerY = 0;
        float playerAngle = 0;
        float radiusBody = 30;
        float radiusHand = 10;
        float enemyX = width / 2, enemyY = height / 2;
        float enemyAngle = 0;
        float enemyWidth = 40, enemyHeight = 40;
        float enemyHornWidth = 4, enemyHornHeight = 4;
        float projectileX = 0, projectileY = 0;        
        float projectileAngle = 0;
        float projectileDistance = 250;
        float projectileWidth = 15, projectileHeight = 10;       
        float widthThin = 75, heightThin = 270;
        float widthThick = 250, heightThick = 175;      
        float healthWidth = 200, healthHeight = 30;
        int finalScene = 0;
        float obsX = 0, obsY = 0;
        Obstacle obs[10];
        int enemyDir = 0;
        int hpDecrease = 0;
        int damage = 0;
        bool trigger = false;
        float projectileSpeed = 300, playerSpeed = 200, enemySpeed = 200;
        int targetScore = 15, score = 0;
    };
}   // namespace m1
