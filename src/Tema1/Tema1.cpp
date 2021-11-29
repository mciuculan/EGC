#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    timePassede1 = 0;
    timePassede2 = 0;
    resolution = window->GetResolution();
    chosen1 = -1;
    chosen2 = -1;
    space = false;
    timeWeapon = 0;
    srand((unsigned)time(NULL));
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

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(-1.5, 0.05, 0);
    glm::vec3 cornerPlayer = glm::vec3(1, 1, 0);
    length = 3.9;
    width = 7;
    
    player.rotate = 0;
    // centrul dreptunghiului din player
    player.x = cornerPlayer.x + 0.25;
    player.y = cornerPlayer.y + 0.125;
    enemy1.x = corner.x + 3;
    enemy1.y = corner.y + 1;
    enemy2.x = corner.x + 3;
    enemy2.y = corner.y + 1;
    {
        Mesh* rectangleNotFilled = Tema1::CreateRectangle("rectangleNotFilled", corner, width, length, glm::vec3(0.6, 0, 0), false);
        AddMeshToList(rectangleNotFilled);
        Mesh* rectangleFilled = Tema1::CreateRectangle("rectangleFilled", corner, width/10, length/10, glm::vec3(0.6, 0, 0), true);
        AddMeshToList(rectangleFilled);
    }
    
    {
        Mesh* player = Tema1::CreateObject("personaj", cornerPlayer, glm::vec3(0.6, 0, 0.6), glm::vec3(0, 0.4, 0.55));
        AddMeshToList(player);
    }

    {
        Mesh* enemy = Tema1::CreateEnemy("enemy", corner, 0.2, glm::vec3(0, 0.4, 0.55));
        AddMeshToList(enemy);
    }

    {
        Mesh* weapon = Tema1::CreateWeapon("weapon", cornerPlayer, 0.03, glm::vec3(0.1, 0.4, 0.55));
        AddMeshToList(weapon);
    }
}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
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

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    resolution = window->GetResolution();

    // The logical viewport

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.6, 0.5, 0), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix);
    DrawPlayer(visMatrix, deltaTimeSeconds);
    DrawEnemy(1, visMatrix, deltaTimeSeconds);
    AnimateEnemy(1, visMatrix, deltaTimeSeconds);
    DrawEnemy(2, visMatrix, deltaTimeSeconds);
    AnimateEnemy(2, visMatrix, deltaTimeSeconds);
    timePassede1++;
    timePassede2++;
    timeWeapon++;
    if (timeWeapon == 50)
    {
        space = false;
    }
    if (space)
    {
        DrawWeapon(visMatrix, deltaTimeSeconds);
        AnimateWeapon(visMatrix, deltaTimeSeconds);
    }
}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(0, 0);
    RenderMesh2D(meshes["rectangleNotFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(0, 0);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(0, 2) * m1::Scale(1, 0.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(2, 0) * m1::Scale(1, 0.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(0, 3) * m1::Scale(0.5, 1.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(1, 1);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(2, 3);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(4, 3.5) * m1::Scale(1, 0.8);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(6, 3.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(4, 2) * m1::Scale(2, 1.3);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(5, 1) * m1::Scale(1.5, 1.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(7.9, 2.3) * m1::Scale(3, 1.5);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * m1::Translate(7.1, 0) * m1::Scale(2, 1.1);
    RenderMesh2D(meshes["rectangleFilled"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawPlayer(glm::mat3 visMatrix, float deltaTimeSeconds) {
        modelMatrix = visMatrix;
        modelMatrix *=  m1::Translate(player.x, player.y) /** m1::Rotate(player.rotate)
                            * m1::Translate(0, 0)*/;

        RenderMesh2D(meshes["personaj"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawEnemy(int enemyno, glm::mat3 visMatrix, float deltaTimeSeconds) {
    modelMatrix = visMatrix;

    if (enemyno == 1)
    {
        modelMatrix *= m1::Scale(0.6, 0.6);
        modelMatrix *= m1::Translate(enemy1.x, enemy1.y) * m1::Rotate(enemy1.rotate);
    }
    else
    {
        modelMatrix *= m1::Scale(0.6, 0.6);
        modelMatrix *= m1::Translate(enemy2.x, enemy2.y) * m1::Rotate(enemy2.rotate);
    }
    RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawWeapon(glm::mat3 visMatrix, float deltaTimeSeconds) {
    modelMatrix = visMatrix;

    modelMatrix *= m1::Translate(positionWeapon.x, positionWeapon.y);

    RenderMesh2D(meshes["weapon"], shaders["VertexColor"], modelMatrix);
}

void Tema1::AnimateEnemy(int enemyno, glm::mat3 visMatrix, float deltaTimeSeconds)
{
    if (enemyno == 1)
    {
        if (enemy1.x > resolution.x || enemy1.y > resolution.y || timePassede1 == 100)
        {
            chosen1 = -1;
            enemy1.rotate = 0;
            timePassede1 = 0;
            enemy1.x = rand() % 7 + 1;
            enemy1.y = rand() % 7 + 1;
        }
        else
        {
            if (chosen1 == -1)
            {
                chosen1 = rand() % 8;
            }
            else
            {
                switch (chosen1)
                {
                case 0:
                    enemy1.rotate = 0;
                    enemy1.x += 0.1 * (rand() % 2 + 1);
                    break;
                case 1:
                    enemy1.rotate = M_PI / 2;
                    enemy1.y += 0.1 * (rand() % 2 + 1);
                    break;
                case 2:
                    enemy1.rotate = M_PI / 6;
                    enemy1.y += 0.1 * (rand() % 3 + 1);
                    enemy1.x += 0.1 * (rand() % 3 + 1);
                    break;
                case 3:
                    enemy1.rotate = M_PI;
                    enemy1.x -= 0.1 * (rand() % 2 + 1);
                    break;
                case 4:
                    enemy1.rotate = 3 * M_PI / 2;
                    enemy1.y -= 0.1 * (rand() % 2 + 1);
                    break;
                case 5:
                    enemy1.rotate = 3 * M_PI / 4;
                    enemy1.x -= 0.1 * (rand() % 3 + 1);
                    enemy1.y += 0.1 * (rand() % 2 + 1);
                    break;
                case 6:
                    enemy1.rotate = 5 * M_PI / 4;
                    enemy1.x -= 0.1 * (rand() % 3 + 1);
                    enemy1.y -= 0.1 * (rand() % 2 + 1);
                    break;
                case 7:
                    enemy1.rotate = 3 * M_PI / 4;
                    enemy1.x += 0.1 * (rand() % 3 + 1);
                    enemy1.y -= 0.1 * (rand() % 2 + 1);
                    break;
                default:
                    break;
                }
            }
        }
    }
    else
    {
        if (enemy2.x > resolution.x || enemy2.y > resolution.y || timePassede2 == 100)
        {
            chosen2 = -1;
            enemy2.rotate = 0;
            timePassede2 = 0;
            enemy2.x = rand() % 7 + 1;
            enemy2.y = rand() % 7 + 1;
        }
        else
        {
            if (chosen2 == -1)
            {
                chosen2 = rand() % 5;
            }
            else
            {
                switch (chosen2)
                {
                    case 0:
                        enemy2.rotate = 0;
                        enemy2.x += 0.05 * (rand() % 3 + 1);
                        break;
                    case 1:
                        enemy2.rotate = M_PI / 2;
                        enemy2.y += 0.05 * (rand() % 3 + 1);
                        break;
                    case 2:
                        enemy2.rotate = M_PI / 6;
                        enemy2.y += 0.05 * (rand() % 3 + 1);
                        enemy2.x += 0.05 * (rand() % 3 + 1);
                        break;
                    case 3:
                        enemy2.rotate = M_PI;
                        enemy2.x -= 0.1 * (rand() % 2 + 1);
                        break;
                    case 4:
                        enemy2.rotate = 3 * M_PI / 2;
                        enemy2.y -= 0.1 * (rand() % 2 + 1);
                        break;
                    default:
                        break;
                    }
                }
            }
    }
    
    
}

void Tema1::AnimateWeapon(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    if (positionWeapon.x > resolution.x || positionWeapon.y > resolution.y || timeWeapon == 50)
    {
        timeWeapon = 0;
        space = false;
    }
    else
    {
        positionWeapon.x += 0.1;
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    auto camera = GetSceneCamera();
    glm::ivec2 resolution = window->GetResolution();

    // Move the logic window with UP, LEFT, DOWN, RIGHT (up, left, down, right)
    if (window->KeyHold(GLFW_KEY_UP))
    {
        logicSpace.y += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        logicSpace.y -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        logicSpace.x += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        logicSpace.x -= deltaTime;
    }

    // Zoom in and zoom out logic window with + and -
    if (window->KeyHold(GLFW_KEY_KP_SUBTRACT))
    {
        GLfloat zoomAmount = deltaTime * 2.f;

        logicSpace.x += logicSpace.width / 2;
        logicSpace.y += logicSpace.height / 2;

        logicSpace.width += zoomAmount;
        logicSpace.height += zoomAmount;

        logicSpace.x -= logicSpace.width / 2;
        logicSpace.y -= logicSpace.height / 2;
    }
    if (window->KeyHold(GLFW_KEY_KP_ADD))
    {
        GLfloat zoomAmount = deltaTime * 2.f;

        logicSpace.x += logicSpace.width / 2;
        logicSpace.y += logicSpace.height / 2;

        logicSpace.width -= zoomAmount;
        logicSpace.height -= zoomAmount;

        logicSpace.x -= logicSpace.width / 2;
        logicSpace.y -= logicSpace.height / 2;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        player.x += 0.05;
        player.edge.x += 0.05;
        player.rotate = 0;
        modelMatrix = visMatrix;
        modelMatrix *= m1::Translate(player.x, player.y);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        player.x -= 0.05;
        player.edge.x -= 0.05;
        player.rotate = M_PI;
        modelMatrix = visMatrix;
        modelMatrix *= m1::Translate(player.x, player.y);
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        player.y += 0.05;
        player.edge.y += 0.05;
        player.rotate = M_PI / 2;
        modelMatrix = visMatrix;
        modelMatrix *= m1::Translate(player.x, player.y);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        player.y -= 0.05;
        player.edge.y -= 0.05;
        player.rotate = 3 * M_PI / 2;
        modelMatrix = visMatrix;
        modelMatrix *= m1::Translate(player.x, player.y);
    }
    RenderMesh2D(meshes["personaj"], shaders["VertexColor"], modelMatrix);
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        space = true;
        timeWeapon = 0;
        positionWeapon.x = player.x + 0.75;
        positionWeapon.y = player.y + 0.12;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    //float x0 = player/*.edge*/.x;
    //float y0 = player/*.edge*/.y;
    //float len = sqrt(pow(x0 - mouseX, 2) + pow(y0 - mouseY, 2));
    //float rad = acos((mouseX - x0) / len);
    //player.rotate = rad;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

Mesh* Tema1::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_WIDTH);
        glLineWidth(8.0f);
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

Mesh* Tema1::CreateEnemy(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;
    const int numberOfVertices = 360;
    std::vector<VertexFormat> verticesFirstCircle;
    std::vector<unsigned int> indicesFirstCircle;
    GLfloat twicePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];

    circleVerticesX[0] = corner.x;
    circleVerticesY[0] = corner.y;
    circleVerticesZ[0] = corner.z;

    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = corner.x + (radius * cos(i * twicePi / numberOfVertices - 2));
        circleVerticesY[i] = corner.y + (radius * sin(i * twicePi / numberOfVertices - 2));
        circleVerticesZ[i] = corner.z;
    }
    
    // cercul
    for (int i = 0; i < numberOfVertices; i++)
    {
        verticesFirstCircle.push_back(VertexFormat(glm::vec3(circleVerticesX[i], circleVerticesY[i], circleVerticesZ[i]), color));
    }
    // ciocul
    verticesFirstCircle.push_back(VertexFormat(corner + glm::vec3(0.5, 0, 0), color));
    verticesFirstCircle.push_back(VertexFormat(corner + glm::vec3(0, 0.1, 0), color));
    verticesFirstCircle.push_back(VertexFormat(corner + glm::vec3(0, 0.05, 0), color));

    // cerc
    for (int i = 1; i < numberOfVertices - 1; i++)
    {
        indicesFirstCircle.push_back(0);
        indicesFirstCircle.push_back(i);
        indicesFirstCircle.push_back(i + 1);
    }
    indicesFirstCircle.push_back(0);
    indicesFirstCircle.push_back(numberOfVertices - 1);
    indicesFirstCircle.push_back(1);

    // cioc
    indicesFirstCircle.push_back(numberOfVertices);
    indicesFirstCircle.push_back(numberOfVertices + 1);
    indicesFirstCircle.push_back(numberOfVertices + 2);

    Mesh* enemy = new Mesh(name);
    enemy->SetDrawMode(GL_TRIANGLE_FAN);

    enemy->InitFromData(verticesFirstCircle, indicesFirstCircle);
    return enemy;
}

Mesh* Tema1::CreateObject(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    glm::vec3 color1,
    glm::vec3 color2)
{
    glm::vec3 corner = leftBottomCorner;

    // dreptunghi
    std::vector<VertexFormat> verticesBody {
        VertexFormat(corner, color1),
        VertexFormat(corner + glm::vec3(0.5, 0, 0), color1),
        VertexFormat(corner + glm::vec3(0.5, 0.25, 0), color1),
        VertexFormat(corner + glm::vec3(0, 0.25, 0), color1),
    };
    vector<unsigned int> indicesBody = {
        0, 1, 2,
        2, 3, 0
    };

    // tun
    verticesBody.push_back(
            VertexFormat(corner + glm::vec3(0.5, 0.1, 0), color2)
        );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.75, 0.1, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.75, 0.15, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.5, 0.15, 0), color2)
    );

    indicesBody.push_back(4);
    indicesBody.push_back(5);
    indicesBody.push_back(6);

    indicesBody.push_back(6);
    indicesBody.push_back(7);
    indicesBody.push_back(4);
    player.edge = corner + glm::vec3(0.75, 0.15, 0);
    player.x = (corner.x + 0.75) / 2;
    player.y = (corner.y + 0.31 + 0.06) / 2;

    // roata patrata 1
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.05, -0.06, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.43, -0.06, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.43, 0, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.05, 0, 0), color2)
    );

    indicesBody.push_back(8);
    indicesBody.push_back(9);
    indicesBody.push_back(10);

    indicesBody.push_back(10);
    indicesBody.push_back(11);
    indicesBody.push_back(8);

    // roata patrata 2
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.05, 0.31, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.43, 0.31, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.43, 0.25, 0), color2)
    );
    verticesBody.push_back(
        VertexFormat(corner + glm::vec3(0.05, 0.25, 0), color2)
    );

    indicesBody.push_back(12);
    indicesBody.push_back(13);
    indicesBody.push_back(14);

    indicesBody.push_back(14);
    indicesBody.push_back(15);
    indicesBody.push_back(12);

    Mesh* object = new Mesh(name);
    object->SetDrawMode(GL_TRIANGLES);

    object->InitFromData(verticesBody, indicesBody);
    return object;
}

Mesh* Tema1::CreateWeapon(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color) {

    glm::vec3 corner = leftBottomCorner;
    const int numberOfVertices = 360;
    std::vector<VertexFormat> verticesFirstCircle;
    std::vector<unsigned int> indicesFirstCircle;
    GLfloat twicePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];

    circleVerticesX[0] = corner.x;
    circleVerticesY[0] = corner.y;
    circleVerticesZ[0] = corner.z;

    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = corner.x + (radius * cos(i * twicePi / numberOfVertices - 2));
        circleVerticesY[i] = corner.y + (radius * sin(i * twicePi / numberOfVertices - 2));
        circleVerticesZ[i] = corner.z;
    }

    for (int i = 0; i < numberOfVertices; i++)
    {
        verticesFirstCircle.push_back(VertexFormat(glm::vec3(circleVerticesX[i], circleVerticesY[i], circleVerticesZ[i]), color));
    }

    for (int i = 1; i < numberOfVertices - 1; i++)
    {
        indicesFirstCircle.push_back(0);
        indicesFirstCircle.push_back(i);
        indicesFirstCircle.push_back(i + 1);
    }
    indicesFirstCircle.push_back(0);
    indicesFirstCircle.push_back(numberOfVertices - 1);
    indicesFirstCircle.push_back(1);

    Mesh* weapon = new Mesh(name);
    weapon->SetDrawMode(GL_TRIANGLES);

    weapon->InitFromData(verticesFirstCircle, indicesFirstCircle);
    return weapon;
}
