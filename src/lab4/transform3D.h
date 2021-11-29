#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Implement the translation matrix
        return glm::mat4(   1, 0, 0, 0,                                 // col 1
                            0, 1, 0, 0,                                 // col 2
                            0, 0, 1, 0,                                 // col 3
                            translateX, translateY, translateZ, 1);     // col 4

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Implement the scaling matrix
        return glm::mat4(   scaleX, 0, 0, 0,      // col 1
                            0, scaleY, 0, 0,      // col 2
                            0, 0, scaleZ, 0,      // col 2
                            0, 0, 0, 1);          // col 4

    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::mat4(   cos(radians), sin(radians), 0, 0,   //col 1
                            -sin(radians), cos(radians), 0, 0,  //col 2
                             0, 0, 1, 0,                        //col 3;
                             0, 0, 0, 1);                       //col 4;

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::mat4(   cos(radians), 0, -sin(radians), 0,      //col 1
                            0, 1, 0, 0,                             //col 2
                            sin(radians), 0, cos(radians), 0,       //col 3;
                            0, 0, 0, 1);                            //col 4;

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::mat4(   1, 0, 0, 0,                             //col 1
                            0, cos(radians), sin(radians), 0,       //col 2
                            0, -sin(radians), cos(radians), 0,      //col 3
                            0, 0, 0, 1);                            //col 4

    }
}   // namespace transform3D
