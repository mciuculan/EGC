#pragma once

#include "components/simple_scene.h"
#include <string>
#include <glm/glm.hpp>


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

        struct Player
        {
            float x, y;
            float rotate;
            glm::vec3 edge;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

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

        Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill);
        Mesh* CreateEnemy(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color);
        Mesh* CreateObject(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 color1, glm::vec3 color2);
        void DrawPlayer(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawEnemy(int enemyno, glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawWeapon(glm::mat3 visMatrix, float deltaTimeSeconds);
        void AnimateEnemy(int enemyno, glm::mat3 visMatrix, float deltaTimeSeconds);
        void AnimateWeapon(glm::mat3 visMatrix, float deltaTimeSeconds);
        Mesh* CreateWeapon(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color);

    protected:
        glm::vec3 centerPositionPersonaj;
        float translateXPersonaj, translateYPersonaj;
        float length, width;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        Player player;
        Player enemy1, enemy2;
        glm::vec3 positionWeapon;
        bool space;
        int timePassede1, timeWeapon, timePassede2;
        glm::ivec2 resolution;
        int chosen1, chosen2;
    };

    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::mat3(1, 0, 0,                       // col 1
            0, 1, 0,                       // col 2
            translateX, translateY, 1);    // col 3
    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::mat3(scaleX, 0, 0,      // col 1
            0, scaleY, 0,      // col 2
            0, 0, 1);          // col 3
    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::mat3(cos(radians), sin(radians), 0, //col 1
            -sin(radians), cos(radians), 0, //col 2
            0, 0, 1);                      //col 3

    }
}