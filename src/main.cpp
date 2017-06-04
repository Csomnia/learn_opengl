#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include "shader.hpp"
#include "camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);


float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

bool wireframe_mode;
float lastFrame = 0.0f;

float lightRadius = 1.5f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


Camera my_camera(glm::vec3(0.0f, 0.0f, -3.0f));

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "learnOpenGl", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "faile to create GLFW windoe" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    std::cout << "OpenGl " << GLVersion.major << "." << GLVersion.minor
              << std::endl;

    // set view posint
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    // set cursor position change call back.
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // set mouse mode.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Gouraud Lighting.
    // Shader cubeShader("../shader/GouraudVertex.vsh", "../shader/GouraudFragment.fsh");

    // lighting in world space
    Shader cubeShader("../shader/vertex.vsh", "../shader/fragment.fsh");

    // lighting in view space.
    // Shader cubeShader("../shader/viewSpaceVertex.vsh", "../shader/viewSpaceFragment.fsh");

    Shader lightShader("../shader/vertex.vsh", "../shader/lightFragment.fsh");

    // =====================
    // loading texture.
    // =====================
    GLuint boxTexture = loadTexture("../resources/container2.png");
    GLuint boxBoarderTexture = loadTexture("../resources/container2_specular.png");
    GLuint emissionTexture = loadTexture("../resources/matrix.jpg");
//    GLuint boxBoarderTexture = loadTexture("../resources/lighting_maps_specular_color.png");

    // ======================
    // bind cube VAO
    // ======================
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // =====================
    // bind lightcube VAO;
    // =====================
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // already have cube vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // enable z_buffer
    glEnable(GL_DEPTH_TEST);

    // config cube texture.
    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    cubeShader.setInt("material.emission", 2);
    //    cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    //    cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        glm::mat4 projection, view;
        projection = glm::perspective(glm::radians(my_camera.Zoom),
                                      (float)window_width / window_height,
                                      0.1f, 100.0f);
        view = my_camera.GetViewMatrix();


        // ===================
        // render light cuble.
        // ===================
        //        lightPos.x = sin(glfwGetTime()) * lightRadius;
        //        lightPos.z = cos(glfwGetTime()) * lightRadius;

        // set light source position.
        glm::mat4 lightModel;
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        lightShader.use();
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===================
        // render color cube.
        // ===================
        glm::mat4 cubeModel;

        cubeShader.use();
        cubeShader.setMat4("model", cubeModel);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        // lighting in world space.
        glm::vec4 lightWorldPos = lightModel * glm::vec4(lightPos, 1.0f);
        cubeShader.setVec3("viewPos", my_camera.Position);

        cubeShader.setFloat("material.shininess", 32.0f);

        cubeShader.setVec3("light.position", glm::vec3(lightWorldPos));
        cubeShader.setVec3("light.ambient", .2f, .2f, .2f);
        cubeShader.setVec3("light.diffuse", .5f, .5f, .5f);
        cubeShader.setVec3("light.specular", 1.f, 1.f, 1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, boxBoarderTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionTexture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    float now_time = glfwGetTime();
    float deltaTime = now_time - lastFrame;
    lastFrame = now_time;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        std::cout << "switch mode" << std::endl;
        if (wireframe_mode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe_mode = false;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe_mode = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        my_camera.ProcessKeyBoard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        my_camera.ProcessKeyBoard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        my_camera.ProcessKeyBoard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        my_camera.ProcessKeyBoard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX, lastY;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //    std::cout << "Last X: " << lastX << "   X pos: " << xpos << std::endl;
    //    std::cout << "Last Y: " << lastY << "   Y pos: " << ypos << std::endl;
    my_camera.ProcessMouseMovement(xpos - lastX , lastY - ypos);
    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    my_camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
