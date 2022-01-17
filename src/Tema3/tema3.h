#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        struct FloorLight;

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const FloorLight floorLights[9], const bool transparent = false, Texture2D* texture = nullptr);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        FloorLight* getClosestFloorLights(const glm::vec3& pos);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
      
        float timeForDancersSwitch;
        float timeForCeilingLightsSwitch;
        int ceilingLightsDir;

        struct FloorLight {
            glm::vec3 pos, color;
        };

        struct CeilingLight {
            glm::vec3 pos, color, dir;
        };

        struct Dancer {
            glm::vec3 pos;
            int dir;
        };

        FloorLight floorLights[8][8];
        CeilingLight ceilingLights[6];
        Dancer dancers[8];

        float materialShininess;
        float materialKd;
        float materialKs;

        int mode;
        glm::vec3 ballPos;

        float conesAngleX[6], conesAngleZ[6];
    };
}   // namespace m1
