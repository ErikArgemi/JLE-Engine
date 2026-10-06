#pragma once

#include "../Module.h"
#include "../Engine.h"
#include "Windows.h"
#include <iostream>
#include <memory>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "ImGuizmo.h"
struct FrameBufferObject
{
    GLuint FBO_ID = 0;
    GLuint RENDER_TO_TEXTURE_ID = 0;
    GLuint RBO_DEPTH_STENCIL_ID = 0;

};

class Render : public Module {
public:
	Render();

	~Render();

	// Called before render is available
	bool Awake();

    // Called each loop iteration
    bool Update();

	// Called before quitting
	bool CleanUp();

    //Create a Frame Buffer Object
    void CreateFBO(int width, int height, FrameBufferObject& frameBufferObject, bool recreate);
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);

    glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

    const float sensitivity = 0.5f;
    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::mat4 viewMatrix = glm::lookAt(position, position + z_axis, y_axis);
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix = 1.0f;

    FrameBufferObject frameBufferObject;
    ImVec2 sceneWindowSize;
    bool shouldRefreshSceneWindow = false;

private:
    SDL_GLContext glContext;

    // Create & compile vertex and fragment shaders
    GLuint vertexShader;

    GLuint fragmentShader;

    // Create Program and bind shaders
    GLuint shaderProgram;

    // Local Space
    //GLfloat cubeVertices[]
    //{
    ////  Position                Color
    //    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,
    //    0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
    //    0.5f, 0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
    //    -0.5f, 0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
    //    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f,
    //    0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 0.0f,
    //    0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 1.0f,
    //    -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,
    //};
    GLfloat d20Vertices[72]
    {
        //  Position                        Color
            0.0f, 1.0f, 0.0f,               1.0f, 0.0f, 0.0f,
            -0.276f, 0.447f, 0.851f,        0.0f, 1.0f, 0.0f,
            0.724f, 0.447f, 0.526f,         0.0f, 0.0f, 1.0f,
            0.724f, 0.447f, -0.526f,       1.0f, 0.0f, 0.0f,
            -0.276f, 0.447f, -0.851f,       0.0f, 1.0f, 1.0f,
            -0.894f, 0.447f, 0.0f,          1.0f, 1.0f, 0.0f,
            0.276f, -0.447f, 0.851f,        1.0f, 0.0f, 1.0f,
            0.894f, -0.447f, 0.0f,          1.0f, 1.0f, 1.0f,
            0.276f, -0.447f, -0.851f,        1.0f, 0.0f, 0.0f,
            -0.724f, -0.447f, -0.526f,       0.0f, 1.0f, 0.0f,
            -0.724f, -0.447f, 0.526f,        0.0f, 0.0f, 1.0f,
            0.0f, -1.0f, 0.0f,              1.0f, 0.0f, 0.0f
    };
    //const unsigned int NUM_INDICES = 36;
    //GLuint cubeIndices[]
    //{
    //    // Top face
    //    3, 2, 6,
    //    6, 7, 3,
    //    // Bottom face
    //    0, 1, 5,
    //    5, 4, 0,
    //    // Left face
    //    0, 4, 7,
    //    7, 3, 0,
    //    // Right face
    //    1, 5, 6,
    //    6, 2, 1,
    //    // Back face
    //    0, 1, 2,
    //    2, 3, 0,
    //    // Front face
    //    4, 5, 6,
    //    6, 7, 4,
    //};
    const unsigned int NUM_INDICES_D20 = 60;
    GLuint d20Indices[60]{
        //top 5 faces
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,5,
        0,5,1,
        //middle 10 faces
        1,6,2,
        2,6,7,
        3,2,7,
        3,7,8,
        4,3,8,
        4,8,9,
        5,4,9,
        5,9,10,
        1,5,10,
        1,10,6,
        //bottom 5 faces
        11,6,7,
        11,7,8,
        11,8,9,
        11,9,10,
        11,10,6
    };
    
    // Projection Matrix
    const float FOV = 45.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 100.0f;


    // Create ModelViewProjection matrix
    GLuint modelViewProjLocation;
    GLint isOutlineLocation;

    // Create VAO & VBO & EBO
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // Rotate the cube over time
    
    float rotation = 0.0f;
    const float SPEED = 10.0f;

    bool isRunning = true;
};