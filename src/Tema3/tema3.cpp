#include "tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create light cone
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        float r = 0;
        int div = 100;

        vertices.push_back(glm::vec3(0, 1, 0));

        for (int i = 0; i <= div; i++) {
            vertices.push_back(glm::vec3(cos(r), 0, sin(r)));
            r += 2 * glm::pi<float>() / div;
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i == div ? 1 : i + 1);
        }

        CreateMesh("cone", vertices, indices);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("myShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create random texture
    {
        mapTextures["random"] = CreateRandomTexture(16, 16);
    }

    // Initialise floor lights
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            floorLights[i][j].pos = glm::vec3(i, 0, j);
            floorLights[i][j].color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        }

    // Initialise ceiling lights
    {
        CeilingLight light;

        light.pos = glm::vec3(0.5, 4, 0.75);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[0] = light;
    }

    {
        CeilingLight light;

        light.pos = glm::vec3(3, 4, 0.75);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[1] = light;
    }

    {
        CeilingLight light;

        light.pos = glm::vec3(5.5, 4, 0.75);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[2] = light;
    }

    {
        CeilingLight light;

        light.pos = glm::vec3(0.5, 4, 6.25);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[3] = light;
    }

    {
        CeilingLight light;

        light.pos = glm::vec3(3, 4, 6.25);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[4] = light;
    }

    {
        CeilingLight light;

        light.pos = glm::vec3(5.5, 4, 6.25);
        light.color = glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
        light.dir = glm::vec3(0, -1, 0);

        ceilingLights[5] = light;
    }

    // Initialise dancers
    for (int i = 0; i < 8; i++) {
        dancers[i].pos = glm::vec3(rand() % 8, 0.5, rand() % 8);
        dancers[i].dir = rand() % 8;
    }

    timeForDancersSwitch = 0;
    materialShininess = 30;
    materialKd = 0.5;
    materialKs = 0.5;
    mode = 0;
    ballPos = glm::vec3(3.5);
    timeForCeilingLightsSwitch = 0;
    ceilingLightsDir = 1;

    for (int i = 0; i < 6; i++) {
        conesAngleX[i] = 0;
        conesAngleZ[i] = 0;
    }

    srand(time(NULL));
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    // Update dancers
    {
        timeForDancersSwitch += deltaTimeSeconds;

        float speed = 2.5;

        for (int i = 0; i < 8; i++) {
            glm::vec3 pos = dancers[i].pos;
            int dir = dancers[i].dir;

            switch (dir) {
            case 0:
                pos.x += speed * deltaTimeSeconds;;
                break;

            case 1:
                pos.x -= speed * deltaTimeSeconds;
                break;

            case 2:
                pos.z += speed * deltaTimeSeconds;
                break;

            case 3:
                pos.z -= speed * deltaTimeSeconds;
                break;

            case 4:
                pos.x += speed * deltaTimeSeconds;;
                pos.z += speed * deltaTimeSeconds;
                break;

            case 5:
                pos.x -= speed * deltaTimeSeconds;
                pos.z -= speed * deltaTimeSeconds;
                break;

            case 6:
                pos.x -= speed * deltaTimeSeconds;
                pos.z += speed * deltaTimeSeconds;
                break;

            default:
                pos.x += speed * deltaTimeSeconds;;
                pos.z -= speed * deltaTimeSeconds;
                break;
            }

            if (pos.x >= 0 && pos.z >= 0 && pos.x < 7 && pos.z < 7) {
                if (timeForDancersSwitch >= 1) {
                    dancers[i].dir = rand() % 8;
                    timeForDancersSwitch = 0;
                } else
                    dancers[i].pos = pos;
            } else
                dancers[i].dir = rand() % 8;
        }
    }

    // Update ceiling lights
    timeForCeilingLightsSwitch += deltaTimeSeconds;

    if (timeForCeilingLightsSwitch > 2.5) {
        timeForCeilingLightsSwitch = 0;
        ceilingLightsDir *= -1;
    }

    for (int i = 0; i < 6; i++) {
        float randomAngle = (float)(1 + rand() % 15) / 100;
        glm::mat4 modelMatrix = glm::mat4(1);

        if (i % 2 == 0) {
            modelMatrix = glm::rotate(modelMatrix, ceilingLightsDir * RADIANS(randomAngle), glm::vec3(1, 0, 0));
            conesAngleX[i] += ceilingLightsDir * RADIANS(randomAngle);

        } else {
            modelMatrix = glm::rotate(modelMatrix, -ceilingLightsDir * RADIANS(randomAngle), glm::vec3(1, 0, 0));
            conesAngleX[i] += -ceilingLightsDir * RADIANS(randomAngle);
        }

        if (i % 3 == 0) {
            modelMatrix = glm::rotate(modelMatrix, ceilingLightsDir * RADIANS(randomAngle), glm::vec3(0, 0, 1));
            conesAngleZ[i] += ceilingLightsDir * RADIANS(randomAngle);
        } else {
            modelMatrix = glm::rotate(modelMatrix, -ceilingLightsDir * RADIANS(randomAngle), glm::vec3(0, 0, 1));
            conesAngleZ[i] += -ceilingLightsDir * RADIANS(randomAngle);
        }

        ceilingLights[i].dir = modelMatrix * glm::vec4(ceilingLights[i].dir, 1);
    }
}


void Tema3::FrameEnd()
{
    // Draw floor lights
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            FloorLight* floorLights = new FloorLight[9];

            for (int i = 0; i < 9; i++) {
                floorLights[i].pos = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
                floorLights[i].color = glm::vec3(0, 0, 0);
            }

            modelMatrix = glm::translate(modelMatrix, this->floorLights[i][j].pos);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.01, 1));

            if (mode == 2)
                RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);
            else
                RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, this->floorLights[i][j].color, floorLights);

            delete[] floorLights;
        }

    // Draw dancers
    for (int i = 0; i < 8; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        FloorLight* floorLights = getClosestFloorLights(glm::vec3(dancers[i].pos));

        modelMatrix = glm::translate(modelMatrix, dancers[i].pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5));
        RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);

        delete[] floorLights;
    }

    // Draw walls
    for (int i = 0; i < 8; i++)
    {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            FloorLight* floorLights = getClosestFloorLights(glm::vec3(i, 2, -0.5));

            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 2, -0.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4, 0.01));
            RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);

            free(floorLights);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            FloorLight* floorLights = getClosestFloorLights(glm::vec3(i, 2, 7.5));

            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 2, 7.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4, 0.01));
            RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);

            delete[] floorLights;
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            FloorLight* floorLights = getClosestFloorLights(glm::vec3(7.5, 2, i));

            modelMatrix = glm::translate(modelMatrix, glm::vec3(7.5, 2, i));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.001, 4, 1));
            RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);

            delete[] floorLights;
        }
    }

    // Draw ceiling
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        FloorLight* floorLights = new FloorLight[9];

        for (int i = 0; i < 9; i++)
        {
            floorLights[i].pos = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
            floorLights[i].color = glm::vec3(0, 0, 0);
        }

        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.5, 4, 3.5));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(8, 0.001, 8));
        RenderSimpleMesh(meshes["box"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights);

        delete[] floorLights;
    }

    // Draw disco ball
    if (mode == 0 || mode == 3) {
        glm::mat4 modelMatrix = glm::mat4(1);
        FloorLight* floorLights = new FloorLight[9];

        for (int i = 0; i < 9; i++)
        {
            floorLights[i].pos = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
            floorLights[i].color = glm::vec3(0, 0, 0);
        }

        modelMatrix = glm::translate(modelMatrix, ballPos);
        RenderSimpleMesh(meshes["sphere"], shaders["myShader"], modelMatrix, glm::vec3(0, 0, 0), floorLights, false, mapTextures["random"]);

        delete[] floorLights;
    }

    if (mode == 0 || mode == 2) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int i = 0; i < 6; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            FloorLight* floorLights = new FloorLight[9];

            for (int i = 0; i < 9; i++)
            {
                floorLights[i].pos = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
                floorLights[i].color = glm::vec3(0, 0, 0);
            }

            modelMatrix = glm::translate(modelMatrix, ceilingLights[i].pos);
            modelMatrix = glm::rotate(modelMatrix, conesAngleX[i], glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, conesAngleZ[i], glm::vec3(0, 0, 1));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -ceilingLights[i].pos.y, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(RADIANS(15.0f)) * ceilingLights[i].pos.y, ceilingLights[i].pos.y, tan(RADIANS(15.0f)) * ceilingLights[i].pos.y));
            RenderSimpleMesh(meshes["cone"], shaders["myShader"], modelMatrix, ceilingLights[i].color, floorLights, true);
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const FloorLight floorLights[9], const bool transparent, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Send data used for light computation
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(GetSceneCamera()->m_transform->GetWorldPosition()));
    glUniform1f(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform1i(glGetUniformLocation(shader->program, "mode"), mode);

    // Send color
    int loc_color = glGetUniformLocation(shader->program, "color");
    glUniform3f(loc_color, color.x, color.y, color.z);

    glUniform1i(glGetUniformLocation(shader->program, "transparent"), transparent);

    // Send texture
    if (texture) {
        glActiveTexture(GL_TEXTURE0);

        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
        glUniform1i(glGetUniformLocation(shader->program, "textured"), true);
    } else
        glUniform1i(glGetUniformLocation(shader->program, "textured"), false);

    glUniform1f(glGetUniformLocation(shader->program, "elapsedTime"), Engine::GetElapsedTime());

    // Send lights
    glm::vec3 floorLightsPos[9];
    glm::vec3 floorLightsColor[9];

    for (int i = 0; i < 9; i++) {
        floorLightsPos[i] = floorLights[i].pos;
        floorLightsColor[i] = floorLights[i].color;
    }

    GLuint location_floorLightsPos = glGetUniformLocation(shader->program, "floorLightsPos");
    glUniform3fv(location_floorLightsPos, 9, glm::value_ptr(floorLightsPos[0]));

    GLuint location_floorLightsColor = glGetUniformLocation(shader->program, "floorLightsColor");
    glUniform3fv(location_floorLightsColor, 9, glm::value_ptr(floorLightsColor[0]));

    glm::vec3 ceilingLightsPos[6];
    glm::vec3 ceilingLightsColor[6];
    glm::vec3 ceilingLightsDir[6];

    for (int i = 0; i < 6; i++) {
        ceilingLightsPos[i] = ceilingLights[i].pos;
        ceilingLightsColor[i] = ceilingLights[i].color;
        ceilingLightsDir[i] = ceilingLights[i].dir;
    }

    GLuint location_ceilingLightsPos = glGetUniformLocation(shader->program, "ceilingLightsPos");
    glUniform3fv(location_ceilingLightsPos, 9, glm::value_ptr(ceilingLightsPos[0]));

    GLuint location_ceilingLightsColor = glGetUniformLocation(shader->program, "ceilingLightsColor");
    glUniform3fv(location_ceilingLightsColor, 9, glm::value_ptr(ceilingLightsColor[0]));

    GLuint location_ceilingLightsDir = glGetUniformLocation(shader->program, "ceilingLightsDir");
    glUniform3fv(location_ceilingLightsDir, 9, glm::value_ptr(ceilingLightsDir[0]));

    glUniform3f(glGetUniformLocation(shader->program, "ballPos"), ballPos.x, ballPos.y, ballPos.z);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % (UCHAR_MAX + 1);
    }
    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


Tema3::FloorLight* m1::Tema3::getClosestFloorLights(const glm::vec3& pos)
{
    FloorLight* floorLights = new FloorLight[9];
    std::vector<FloorLight> allFloorLights;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            allFloorLights.push_back(this->floorLights[i][j]);

    sort(allFloorLights.begin(), allFloorLights.end(), [&pos](const FloorLight& floorLight1, const FloorLight& floorLight2) {
        return glm::distance(pos, floorLight1.pos) < glm::distance(pos, floorLight2.pos);
    });

    for (int i = 0; i < 9; i++)
        floorLights[i] = allFloorLights[i];

    return floorLights;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_U)
        mode = (mode + 1) % 4;
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
