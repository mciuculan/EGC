#pragma once

#include "components/simple_scene.h"
#include <string>
#include <glm/glm.hpp>


namespace m1
{
    class Shooter : public gfxc::SimpleScene
    {
    public:
        static void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
    };
}