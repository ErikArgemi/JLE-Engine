#include "Render.h"
#include "../Engine.h"
#include "../Log.h"
#include "Windows.h"
#include "JSON_FileReader.h"

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

Render::Render() : Module() {
	name = "render";
}

Render::~Render() {

}

bool Render::Awake() {
	SDL_Window* window = Engine::GetInstance().windows->window;
	// OpenGL is context based and thread local
		// Link OpenGL context to SDL, after this you can load OpenGL functions and start rendering
		// Multi-threading -> Define multiple context and make them current
	glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
        std::cout << "render glcontext failed" << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
    LOG("OpenGL linked context to SDL");
	gladLoadGL();

	// Used for mapping NDC coordinates (-1.0f to 1.0f) to pixel coordinates (e.g. 1920x1080)
    JSON_FileReader fileReader;
    renderSettings = fileReader.GetRenderSettings();
	glViewport(0, 0, renderSettings.windowsSizeX, renderSettings.windowsSizeY);

	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    LOG("ImGui created context");
	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();

    // Create & compile vertex and fragment shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Create Program and bind shaders
    shaderProgram = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete shaders since we've created a program already and they are contained there
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    // Create ModelViewProjection matrix
    modelViewProjLocation = glGetUniformLocation(shaderProgram, "modelViewProj");
    isOutlineLocation = glGetUniformLocation(shaderProgram, "isOutline");

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    // Link GL_ARRAY_BUFFER to vertices data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(d20Vertices), d20Vertices, GL_STATIC_DRAW);


    // Link GL_ELEMENT_ARRAY_BUFFER to indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(d20Indices), d20Indices, GL_STATIC_DRAW);


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
    SDL_GetCurrentTime(&Engine::GetInstance().prevTime);

    CreateFBO(Engine::GetInstance().windows->SCREEN_WIDTH, Engine::GetInstance().windows->SCREEN_HEIGHT, frameBufferObject);
    sceneWindowSize.x = Engine::GetInstance().windows->SCREEN_WIDTH;
    sceneWindowSize.y = Engine::GetInstance().windows->SCREEN_HEIGHT;

    // View Matrix
    glm::vec3 position(0.0f, 0.0f, -5.0f);
    glm::vec3 forward(0.0f, 0.0f, 1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMatrix = glm::lookAt(position             // Camera Position
        , position + forward   // Target Position
        , up);                 // Up Vector

    projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(Engine::GetInstance().windows->SCREEN_WIDTH) / static_cast<float>(Engine::GetInstance().windows->SCREEN_HEIGHT), NEAR_PLANE, FAR_PLANE);
    
    primitiveManager = std::make_unique<PrimitiveManager>();
    
    return true;
}

bool Render::Update() {
    // UPDATE
    // Model Matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

    // Perform Rotation
    rotation += SPEED * Engine::GetInstance().dt;
    
    // Clear screen color
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);

    // Clear Color Buffer and Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // RENDER TO TEXTURE
    if (shouldRefreshSceneWindow)
    {
        CreateFBO(sceneWindowSize.x, sceneWindowSize.y, frameBufferObject);
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
    glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, glm::value_ptr(modelViewProj));
    glUniform1f(isOutlineLocation, 0.0f);  // set the value

    glBindVertexArray(VAO);

    // 2. Render the first cube
    //glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0);

    glDrawElements(GL_TRIANGLES, NUM_INDICES_D20, GL_UNSIGNED_INT, 0);


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
    //glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0);

    glDrawElements(GL_TRIANGLES, NUM_INDICES_D20, GL_UNSIGNED_INT, 0);

    // 5. Restore previous state
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    // Unbind frame buffer, back to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //TODO: Render all primitives from primitiveManager

    return true;
}

bool Render::CleanUp() {
    primitiveManager->CleanUp();
    primitiveManager.reset();

    // Delete VAO, VBO and shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Deinit ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(glContext);
    SDL_Quit();
    return true;
}

void Render::CreateFBO(int width, int height, FrameBufferObject& frameBufferObject)
{
    /// Generate Frame buffer textures if not done earlier
    if (frameBufferObject.FBO_ID == 0)
    {
        glGenFramebuffers(1, &frameBufferObject.FBO_ID);
        glGenTextures(1, &frameBufferObject.RENDER_TO_TEXTURE_ID);
        glGenRenderbuffers(1, &frameBufferObject.RBO_DEPTH_STENCIL_ID);
    }
    // Bind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject.FBO_ID);
    // Color texture
    glBindTexture(GL_TEXTURE_2D, frameBufferObject.RENDER_TO_TEXTURE_ID);
    // Setup texture filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // Depth/stencil
    glBindRenderbuffer(GL_RENDERBUFFER, frameBufferObject.RBO_DEPTH_STENCIL_ID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    // Attach
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferObject.RENDER_TO_TEXTURE_ID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferObject.RBO_DEPTH_STENCIL_ID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Frame buffer incomplete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

PrimitiveMesh Render::CreateMesh(GLfloat* vertices, Uint32 vertexBytes, GLuint* indices, GLsizei indexCount)
{
    PrimitiveMesh mesh;
    mesh.n_index = indexCount;

    glGenVertexArrays(1, &mesh.VAO);

    glGenBuffers(1, &mesh.VBO);

    glGenBuffers(1, &mesh.EBO);

    // Bind VAO and VBO
    glBindVertexArray(mesh.VAO);

    // Link GL_ARRAY_BUFFER to vertices data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, vertexBytes, vertices, GL_STATIC_DRAW);


    // Link GL_ELEMENT_ARRAY_BUFFER to indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, indices, GL_STATIC_DRAW);


    // Define Vertex layout and set attribute index
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unlink VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return mesh;
}