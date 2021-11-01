#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    RED = 0;
    GREEN = 0;
    BLUE = 0;
    pozitieBunny.x = 0;
    pozitieBunny.y = 0.5f;
    pozitieBunny.z = -2;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("mesh");
        meshName = "box.obj";
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), meshName);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* meshBunny = new Mesh("bunny");
        meshBunny->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[meshBunny->GetMeshID()] = meshBunny;
    }
    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(RED, GREEN, BLUE, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["mesh"], glm::vec3(1, 0.5f, 0), glm::vec3(0.1f));

    // Render the object again but with different properties
    // RenderMesh(meshes["mesh"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(meshes["bunny"], pozitieBunny,  glm::vec3(0.02f));
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 initpozitieBunny = pozitieBunny;
    degrees += deltaTime;
    if (degrees > 360.f) {
        degrees = 0.f;
    }

    if (window->KeyHold(GLFW_KEY_Z)) {
       /* glm::mat4 ModelMatrix;
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(50.f), pozitieBunny);
        //nu ma prind de unde iau uniform position
        glUniformMatrix4fv(glGetUniformLocation(0, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));*/
        
        // i tried, se invarte in jurul z aici
        pozitieBunny.x = initpozitieBunny.x * cos(degrees) - initpozitieBunny.y * sin(degrees);
        pozitieBunny.y = initpozitieBunny.x * sin(degrees) + initpozitieBunny.y * cos(degrees);
    }
    if (window->KeyHold(GLFW_KEY_X)) {
        /* glm::mat4 ModelMatrix;
         ModelMatrix = glm::rotate(ModelMatrix, glm::radians(50.f), pozitieBunny);

         //nu ma prind de unde iau uniform position

         glUniformMatrix4fv(glGetUniformLocation(0, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));*/

         // se invarte in jurul x aici
        pozitieBunny.y = initpozitieBunny.y * cos(degrees) - initpozitieBunny.z * sin(degrees);
        pozitieBunny.z = initpozitieBunny.y * sin(degrees) + initpozitieBunny.z * cos(degrees);
    }
    if (window->KeyHold(GLFW_KEY_Y)) {
        /* glm::mat4 ModelMatrix;
         ModelMatrix = glm::rotate(ModelMatrix, glm::radians(50.f), pozitieBunny);
         //nu ma prind de unde iau uniform position
         glUniformMatrix4fv(glGetUniformLocation(0, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));*/

         // se invarte in jurul y aici
        pozitieBunny.x = initpozitieBunny.x * cos(degrees) + initpozitieBunny.z * sin(degrees);
        pozitieBunny.z = initpozitieBunny.z * cos(degrees) - initpozitieBunny.x * sin(degrees);
    }
    // Treat continuous update based on input
    if (window->KeyHold(GLFW_KEY_W)) {
        pozitieBunny.y += 0.01f;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        pozitieBunny.x -= 0.01f;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        pozitieBunny.y -= 0.01f;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        pozitieBunny.x += 0.01f;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        pozitieBunny.z += 0.01f;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        pozitieBunny.z -= 0.01f;
    }
    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_R) {
        // TODO(student): Change the values of the color components.
        RED = 1;
        GREEN = 0;
        BLUE = 0;
    }

    if (key == GLFW_KEY_G) {
        // TODO(student): Change the values of the color components.
        GREEN = 1;
        RED = 0;
        BLUE = 0;
    }

    if (key == GLFW_KEY_B) {
        // TODO(student): Change the values of the color components.
        BLUE = 1;
        RED = 0;
        GREEN = 0;
    }

    if (key == GLFW_KEY_UP) {
        pozitieBunny.y += 0.3f;
    }
    

    if (key == GLFW_KEY_ENTER) {
        {
            Mesh* mesh = new Mesh("mesh");
            if (meshName == "box.obj") {
                meshName = "quad.obj";
            }
            else if (meshName == "quad.obj") {
                meshName = "box.obj";
            }
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), meshName);
            meshes[mesh->GetMeshID()] = mesh;
        }
    }
    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
