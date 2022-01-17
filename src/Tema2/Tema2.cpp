#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    projectionType = "perspective";
    fov = RADIANS(60.0f);
    fovSpeed = 5;

    width = 5;
    height = 5;

    zNear = 0.01f;
    zFar = 200.0f;

    timeToLeave = 0;
    timer = 10000;
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    w = 5;
    h = 5;
    H = 2 * h + 1;
    W = 2 * w + 1;
    grid = std::vector<std::vector<int>>(H, std::vector<int>(W, 1));
    generate();

    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(headX, headY + 1, headZ + camera->distanceToTarget), glm::vec3(headX, headY, headZ), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (headX > 5 * H || headX < 0 || headZ < 0 || headZ > 5 * W)
    {
        std::cout << "Game over!\n";
        exit(0);
    }
    timer--;
    std::cout << timer << '\n';
    if (timer == 0)
    {
        std::cout << "Game over!\n";
        exit(0);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (grid[i][j] == 1)
                {
                    modelMatrix *= transform3D::Translate(j * 5, 0, i * 5);
                    modelMatrix *= transform3D::Translate(0, 2.5, 0);
                    modelMatrix *= transform3D::Scale(5, 5, 5);

                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.365, 0.635, 0.443));
                    modelMatrix = glm::mat4(1);
                }
                else if (grid[i][j] == 2) {
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 5, 0, i * 5));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));

                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.89, 0.89, 0.89));
                    modelMatrix = glm::mat4(1);
                }
            }
        }
        modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(27, 0, 27));
        RenderSimpleMesh(meshes["plane50"], shaders["LabShader"], modelMatrix, glm::vec3(0.537, 0.537, 0.537));
    }

    {
        // head
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation);
        modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.831, 0.627, 0.655));

        // body
        modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(0, -0.6, 0);
        modelMatrix *= transform3D::Scale(0.7, 0.9, 0.7);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.784, 0.275, 0.188));

        // hand 1
        modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(- 0.5, - 0.6, 0);
        modelMatrix *= transform3D::Scale(0.3, 0.9, 0.3);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.831, 0.627, 0.655));

        // hand 2
        modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(0.5, - 0.6, 0);
        modelMatrix *= transform3D::Scale(0.3, 0.9, 0.3);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.831, 0.627, 0.655));

        // leg 1
        modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(0.2, - 1.5, 0);
        modelMatrix *= transform3D::Scale(0.3, 0.9, 0.3);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.22, 0.55, 0));

        // leg 2
        modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(headX, headY, headZ);
        modelMatrix *= transform3D::RotateOY(rotation) * transform3D::Translate(- 0.2, - 1.5, 0);
        modelMatrix *= transform3D::Scale(0.3, 0.9, 0.3);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.22, 0.55, 0));
    }

    if (firstPersonProj == true && fireProj == true)
    {
        // proiectile
        timeToLeave++;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(posProj.x, posProj.y, posProj.z);
        posProj += dirProj * deltaTimeSeconds * glm::vec3(10);
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0));
    }

    if (timeToLeave == 200)
    {
        timeToLeave = 0;
        fireProj = false;
        dirProj = camera->forward;
        posProj = camera->GetTargetPosition();
    }

    glm::vec3 cam = camera->GetTargetPosition();
    cam.y = 0;
    headX = cam.x;
    headZ = cam.z;
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, "Model");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "View");


    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    location = glGetUniformLocation(shader->program, "Projection");

    /*
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    */
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    location = glGetUniformLocation(shader->program, "color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 4.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
        if (renderCameraTarget) {
            firstPersonProj = true;
            dirProj = camera->forward;
            posProj = camera->GetTargetPosition();
            camera->distanceToTarget = -2;
            camera->MoveForward(2.0f);
        }
        else {
            fireProj = false;
            firstPersonProj = false;
            camera->distanceToTarget = 2;
            camera->MoveForward(-2.0f);
        }

    }
    if (key == GLFW_KEY_SPACE)
    {
        fireProj = true;
        dirProj = camera->forward;
        posProj = camera->GetTargetPosition();
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        rotation = atan2((camera->forward.x), camera->forward.z);

        if (renderCameraTarget) {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (!renderCameraTarget) {
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

// aldous broder
std::vector<std::pair<int, int>> Tema2::findNeighbors(int r, int c, int isWall)
{
    std::vector<std::pair<int, int>> ns;

    if (r > 1 && grid[r - 2][c] == isWall) {
        ns.push_back({ r - 2, c });
    }

    if (r < H - 2 && grid[r + 2][c] == isWall) {
        ns.push_back({ r + 2, c });
    }
    if (c > 1 && grid[r][c - 2] == isWall) {
        ns.push_back({ r, c - 2 });
    }
    if (c < W - 2 && grid[r][c + 2] == isWall) {
        ns.push_back({ r, c + 2 });
    }

    std::shuffle(std::begin(ns), std::end(ns), default_random_engine(engie));
    return ns;
}

bool Tema2::doExit(int r, int c)
{
    for (auto& exit : exits) {
        if (abs(exit.first - r) <= 1 || abs(exit.second) <= 1) {
            return true;
        }
    }
    return false;
}

void Tema2::generate()
{
    srand((unsigned)time(NULL));

    int crow = 2 * (rand() % h) + 1;
    int ccol = 2 * (rand() % w) + 1;
    headX = ccol * 5;
    headZ = crow * 5;

    grid[crow][ccol] = 0;
    forcedExit++;

    while ((forcedExit < h * w)) {
        std::vector<std::pair<int, int>> neighbors = findNeighbors(crow, ccol, 1);

        if (neighbors.size() == 0) {
            std::vector<std::pair<int, int>> cell = findNeighbors(crow, ccol, 0);
            if (cell.size() != 0) {
                int rIndex = rand() % cell.size();
                crow = cell[rIndex].first;
                ccol = cell[rIndex].second;
            }
        }

        for (std::pair<int, int>& pairs : neighbors) {
            int nrow = pairs.first;
            int ncol = pairs.second;

            if (grid[nrow][ncol] == 1) {
                enemyValue = rand() % 5 == 1 ? 2 : 0;

                grid[abs((nrow + crow) / 2)][abs((ncol + ccol) / 2)] = enemyValue;
                grid[nrow][ncol] = enemyValue;

                if (count > 0) {
                    if (nrow == 1 && !doExit(0, ncol)) {
                        grid[0][ncol] = enemyValue;
                        exits.push_back({ 0 , ncol });
                        count -= 1;
                    }
                    else if (nrow == H - 2 && !doExit(H - 1, ncol)) {
                        grid[H - 1][ncol] = enemyValue;
                        exits.push_back({ H - 1, ncol });
                        count -= 1;
                    }
                    else if (ncol == 1 && !doExit(nrow, 0)) {
                        grid[nrow][0] = enemyValue;
                        exits.push_back({ nrow, 0 });
                        count -= 1;
                    }
                    else if (ncol == W - 2 && !doExit(nrow, W - 1)) {
                        grid[nrow][W - 1] = enemyValue;
                        exits.push_back({ nrow, W - 1 });
                        count -= 1;
                    }
                }

                forcedExit++;
                crow = nrow;
                ccol = ncol;
                break;
            }
        }
    }
}
