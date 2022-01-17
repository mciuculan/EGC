#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab5/lab_camera.h"
#include <random>
#include <vector>
#include <string>
#include <iostream>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void Tema2::generate();
        bool Tema2::doExit(int r, int c);
        std::vector<std::pair<int, int>> Tema2::findNeighbors(int r, int c, int isWall);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        std::string projectionType;
        float fov, fovSpeed;
        float width, height;
        float zNear, zFar;

        // timer
        int timer;

        // player
        float headX = 1.0; float headY = 2.0; float headZ = 0.0;
        float rotation;

        // proiectil
        bool firstPersonProj = false;;
        bool fireProj = false;
        glm::vec3 posProj, dirProj;
        int timeToLeave;

        // lab
        int row;
        int column;
        int h;
        int w;
        int H;
        int W;
        float latimeCubLabirint;
        int count = 2;
        int enemyValue;

        int forcedExit = 0;;

        std::vector<std::vector<int>> grid;
        std::vector<std::pair<int, int>> exits;
        std::default_random_engine engie = std::default_random_engine();
    };
}   // namespace m1
