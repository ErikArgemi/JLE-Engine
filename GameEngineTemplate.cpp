#include <iostream>

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

//Additional C++ Headers
#include <vector>

// Setup VS and PS in GLSL
const char* vertexShaderSource = "\n"
"#version 460 core\n"
"layout (location = 0) in vec3 inPos;\n"
"layout (location = 1) in vec3 inColor;\n"
"out vec3 vertexColor;\n"
"uniform mat4 modelViewProj;\n"
"void main()\n"
"{\n"
"gl_Position = modelViewProj * vec4(inPos, 1.0);\n"
"vertexColor = inColor;\n"
"}\0";

const char* fragmentShaderSource = "\n"
"#version 460 core\n"
"in vec3 vertexColor;\n"
"uniform float isOutline;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f) + vec4(isOutline, isOutline, isOutline, isOutline);\n"
"}\0";

struct FrameBufferObject
{
    GLuint FBO_ID = 0;
    GLuint RENDER_TO_TEXTURE_ID = 0;
    GLuint RBO_DEPTH_STENCIL_ID = 0;

};

void CreateFBO(int width, int height, FrameBufferObject& frameBufferObject, bool recreate)
{
    // Create frame buffer object if it didnt exist
    if (frameBufferObject.FBO_ID == 0)
    {
        // Create FrameBuffer objects
        glGenFramebuffers(1, &frameBufferObject.FBO_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject.FBO_ID);

        glGenTextures(1, &frameBufferObject.RENDER_TO_TEXTURE_ID);
        glBindTexture(GL_TEXTURE_2D, frameBufferObject.RENDER_TO_TEXTURE_ID);

        glGenRenderbuffers(1, &frameBufferObject.RBO_DEPTH_STENCIL_ID);
        glBindRenderbuffer(GL_RENDERBUFFER, frameBufferObject.RBO_DEPTH_STENCIL_ID);

        // Setup texture filtering params
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach color + depth&stencil buffers
        // We can attach more than one color fragment shader output simultanously. For our purpose, we only attach one. 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferObject.RENDER_TO_TEXTURE_ID, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferObject.RBO_DEPTH_STENCIL_ID);
    }

    // Bind Render to Texture for resizing
    glBindTexture(GL_TEXTURE_2D, frameBufferObject.RENDER_TO_TEXTURE_ID);
    // glTexImage2D only reallocates color data within GPU memory inside existing ID
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Bind Depth&Stencil for resizing
    glBindRenderbuffer(GL_RENDERBUFFER, frameBufferObject.RBO_DEPTH_STENCIL_ID);
    // glRenderbufferStorage only reallocates depth&stencil data within GPU memory inside existing ID
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // Validation for creation/reuse of frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject.FBO_ID);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Frame buffer incomplete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main()
{
    // Window Resolution
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    const ImVec2 windowSizesValues[] = {
                ImVec2(1080, 720),
                ImVec2(1920, 1080),
                ImVec2(2560, 1440),
                ImVec2(3840, 2160)
    };
    const char* windowSizes[] = {
        "1080x720",
        "1920x1080",
        "2560x1440",
        "3840x2160"
    };
    // Init SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return -1;
    }

    // Setup Min/Major version for using OpenGL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // Set Core Profile Mode
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create OpenGL window using SDL
    SDL_Window* window = SDL_CreateWindow("EnginishGL",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    bool fullscreen = false;
    bool resizable = false;
    bool borderless = false;
    // Early out if window not valid
    if (window == nullptr)
    {
        SDL_Quit();
        return -1;
    }

    // OpenGL is context based and thread local
    // Link OpenGL context to SDL, after this you can load OpenGL functions and start rendering
    // Multi-threading -> Define multiple context and make them current
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Init all OpenGL function pointers at runtime (not linked at compile time)
    gladLoadGL();

    // Used for mapping NDC coordinates (-1.0f to 1.0f) to pixel coordinates (e.g. 1920x1080)
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();

    // Create & compile vertex and fragment shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create Program and bind shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete shaders since we've created a program already and they are contained there
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Local Space
    GLfloat cubeVertices[]
    {
    //  Position                Color
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,
    };

    const unsigned int NUM_INDICES = 36;
    GLuint cubeIndices[]
    {
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Bottom face
        0, 1, 5,
        5, 4, 0,
        // Left face
        0, 4, 7,
        7, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        0, 1, 2,
        2, 3, 0,
        // Front face
        4, 5, 6,
        6, 7, 4,
    };

    // View Matrix
    glm::vec3 position(0.0f, 0.0f, -5.0f);
    glm::vec3 forward(0.0f, 0.0f, 1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::mat4 viewMatrix = glm::lookAt(position             // Camera Position
                                     , position + forward   // Target Position
                                     , up);                 // Up Vector
    // Projection Matrix
    const float FOV = 45.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 100.0f;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), NEAR_PLANE, FAR_PLANE);


    // Create ModelViewProjection matrix
    GLuint modelViewProjLocation = glGetUniformLocation(shaderProgram, "modelViewProj");
    GLint isOutlineLocation = glGetUniformLocation(shaderProgram, "isOutline");

    // Create VAO & VBO & EBO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    // Link GL_ARRAY_BUFFER to vertices data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Link GL_ELEMENT_ARRAY_BUFFER to indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Define Vertex layout and set attribute index
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unlink VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Rotate the cube over time
    SDL_Time prevTime;
    SDL_GetCurrentTime(&prevTime);
    float rotation = 0.0f;
    const float SPEED = 100.0f;

    bool isRunning = true;
    FrameBufferObject frameBufferObject;
    CreateFBO(SCREEN_WIDTH, SCREEN_HEIGHT, frameBufferObject, false);
    ImVec2 sceneWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    bool shouldRefreshSceneWindow = false;

    //FPS
    float fps;
    int max_fps = 60;
    std::vector<float> fps_log;
    std::vector<float> ms_log;

    //Brightness
    /*FrameBufferObject brightnessFBO;*/

    while (isRunning)
    {
        // INPUT
        // SDL EVENTS
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP 
                && event.button.button == SDL_BUTTON_RIGHT)
            {
            }

            if ((event.type == SDL_EVENT_MOUSE_BUTTON_DOWN 
                && event.button.button == SDL_BUTTON_RIGHT))
            {
            }

        }

        // SDL KEYS
        int numKeys;
        const bool* keys = SDL_GetKeyboardState(&numKeys);
        if (keys[SDL_SCANCODE_W])
        {
        }

        // UPDATE
        // Model Matrix
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

        // Perform Rotation
        SDL_Time currentTime;
        SDL_GetCurrentTime(&currentTime);

        const float dt = (currentTime - prevTime) / 1000000000.0f;
        rotation += SPEED * dt;
        prevTime = currentTime;

        //FPS Update
        fps = 1.0f / dt;
        if (ms_log.size() > 59) {
            ms_log.erase(ms_log.begin());
        }
        ms_log.emplace_back(dt*1000);

        if (fps_log.size() > 59) {
            fps_log.erase(fps_log.begin());
        }
        fps_log.emplace_back(fps);

        // Clear screen color
        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);

        // Clear Color Buffer and Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // RENDER TO TEXTURE
        if (shouldRefreshSceneWindow)
        {
            CreateFBO(sceneWindowSize.x, sceneWindowSize.y, frameBufferObject, true);
            /*CreateFBO(sceneWindowSize.x, sceneWindowSize.y, brightnessFBO, true);*/
            shouldRefreshSceneWindow = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject.FBO_ID);
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);

        glViewport(0, 0, sceneWindowSize.x, sceneWindowSize.y);
        
        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Enable Stencil test
        glEnable(GL_STENCIL_TEST);

        // Clear screen color from render to texture
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

        // Clear Color Buffer and Depth Buffer from render to texture
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // RENDER
        // 1. We write value 1 to stencil buffer for all fragments that pass
        // It will only write if depth test passes, this is why we use stencil 
        // for outlining to avoid wrong visuals when other objects are in front
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        // Enable write to stencil
        glStencilMask(0xFF);

        // Use shader program & bind VAO
        glUseProgram(shaderProgram);
        glUniformMatrix4fv (modelViewProjLocation, 1, GL_FALSE, glm::value_ptr(modelViewProj));
        glUniform1f(isOutlineLocation, 0.0f);  // set the value

        glBindVertexArray(VAO);

        // 2. Render the first cube
        glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0);

        // 3. We will compare when rendering 2nd cube if there's value 1 to stencil buffer for all fragments that pass
        // Only write outline to value != 1
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        // Disable write to stencil, we don't need to do that for 2nd cube
        glStencilMask(0x00);

        // 4. Draw Second Cube with higher scale, and update uniform so that its color is white
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.1f, 1.1f, 1.1f));

        modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

        glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, glm::value_ptr(modelViewProj));
        glUniform1f(isOutlineLocation, 1.0f);  // set the value
        glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0);

        // 5. Restore previous state
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);

        // Unbind frame buffer, back to default
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        // Header Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
					exit(0);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("Github Documentation")) {
                    SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine#gameenginetemplate");
                }
                if (ImGui::MenuItem("Report a Bug")) {
                    SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine/issues");
                }
                if (ImGui::MenuItem("Download Latest:")) {
                    SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine");
                }
                if (ImGui::MenuItem("About")) {
                    
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        const bool gizmoActive = ImGuizmo::IsOver() || ImGuizmo::IsUsing();
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (gizmoActive)
        {
            flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::Begin("Scene", nullptr, flags);
        ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
        ImVec2 newSceneWindowSize = ImGui::GetContentRegionAvail();
        shouldRefreshSceneWindow = (newSceneWindowSize.x != sceneWindowSize.x || newSceneWindowSize.y != sceneWindowSize.y);
        sceneWindowSize = newSceneWindowSize;
        ImGui::Image(frameBufferObject.RENDER_TO_TEXTURE_ID, newSceneWindowSize, ImVec2(0, 1), ImVec2(1, 0));

        glDisable(GL_DEPTH_TEST);
        ImGuizmo::SetRect(cursorScreenPos.x, cursorScreenPos.y, newSceneWindowSize.x, newSceneWindowSize.y);
        ImGuizmo::SetDrawlist();
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(modelMatrix));
        glEnable(GL_DEPTH_TEST);
        ImGui::End();

        ImGui::ShowDemoWindow();

        //Configuration window
        ImGui::Begin("Configuration window", nullptr, flags);
        if (ImGui::CollapsingHeader("Application"))
        {
            ImGui::Text("JLE-Engine");
            ImGui::Text("UPC CITM");
            ImGui::SliderInt("Max FPS", &max_fps, 0, 120, ImGuiSliderFlags(ImGuiSliderFlags_None));

            char title[25];
            sprintf_s(title, 25, "Framerate %.f", fps_log[fps_log.size()-1]);
            ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 120.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Milliseconds %.f", ms_log[ms_log.size() - 1]);
            ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
        }
        if (ImGui::CollapsingHeader("Window"))
        {

            //ImGui::SliderFloat("Brightness", ); TODO: Search how

            
            //Windows size TODO: Keep proportions
            static int currentSize = 1;
            if (ImGui::Combo("Window sizes", &currentSize, windowSizes, GLM_COUNTOF(windowSizes))) {
                SDL_SetWindowSize(window, windowSizesValues[currentSize].x, windowSizesValues[currentSize].y);
                glViewport(0, 0, windowSizesValues[currentSize].x, windowSizesValues[currentSize].y);
            }

            ImGui::Text("Refresh rate: %i", (int)fps);

            if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
                SDL_SetWindowFullscreen(window, fullscreen);
            }

            if (ImGui::Checkbox("Resizable", &resizable)) {
                SDL_SetWindowResizable(window, resizable);
            }

            if (ImGui::Checkbox("Borderless", &borderless)) {
                SDL_SetWindowBordered(window, !borderless);
            }
        }
        if (ImGui::CollapsingHeader("Hardware")) {
            ImGui::Text("CPUs: %i (Cache: %ikb)", SDL_GetNumLogicalCPUCores(), SDL_GetCPUCacheLineSize());
            ImGui::Text("System RAM: %i Mb", SDL_GetSystemRAM());
            /*ImGui::Text("Caps: %s", BuildCapsString());*/
            ImGui::NewLine();
            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Brand: %s", glGetString(GL_RENDERER));
        }
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap window
        SDL_GL_SwapWindow(window);

        // FRAME CONTROL
        SDL_GetCurrentTime(&currentTime);
        float current_dt = (currentTime - prevTime) / 1000000000.0f;
        prevTime = currentTime;
        float max_dt;
        if (max_fps != 0) {
            max_dt = 1000 / max_fps;
        }
        else max_dt = 0;

        if (current_dt < max_dt && max_dt != 0.0f) {
            SDL_Delay(max_dt - current_dt);
        }
    }

    // Delete VAO, VBO and shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Deinit ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // DeInit SDL
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
