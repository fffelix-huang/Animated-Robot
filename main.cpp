#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool animation = true;

struct Component {
    const Model& model;
    Component* parent = nullptr;
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);

    Component(const Model& model, Component* parent = nullptr) : model(model), parent(parent) {}

    void setTranslate(glm::vec3 vec) { translate = glm::translate(glm::mat4(1.0f), vec); }

    void setScale(glm::vec3 vec) { scale = glm::scale(glm::mat4(1.0f), vec); }

    void setRotate(float degree, glm::vec3 vec) { rotate = glm::rotate(glm::mat4(1.0f), glm::radians(degree), vec); }

    glm::mat4 getTransform() const {
        glm::mat4 parTransform = (parent == nullptr ? glm::mat4(1.0f) : parent->getTransform());
        return translate * rotate * scale * parTransform;
    }

    void Draw(Shader& shader) {
        shader.setMat4("model", getTransform());
        model.Draw(shader);
    }
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shader.vs", "shader.fs");

    Model cube("assets/objects/Cube.obj");
    Model sphere("assets/objects/Sphere.obj");

    Component torso(cube);
    torso.setTranslate(glm::vec3(0.0f, 0.0f, -5.0f));
    torso.setScale(glm::vec3(1.0f, 2.0f, 1.0f));

    Component head(sphere, &torso);
    head.setTranslate(glm::vec3(0.0f, 1.5f, 0.0f));
    head.setScale(glm::vec3(1.0f, 0.5f, 1.0f));

    Component L1arm(cube, &torso);
    L1arm.setTranslate(glm::vec3(-1.0f, 0.5f, 0.0f));
    L1arm.setScale(glm::vec3(0.5f, 0.5f, 1.0f));

    Component L2arm(cube, &L1arm);
    L2arm.setTranslate(glm::vec3(0.0f, -1.1f, 0.0f));
    L2arm.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Component R1arm(cube, &torso);
    R1arm.setTranslate(glm::vec3(1.0f, 0.5f, 0.0f));
    R1arm.setScale(glm::vec3(0.5f, 0.5f, 1.0f));

    Component R2arm(cube, &R1arm);
    R2arm.setTranslate(glm::vec3(0.0f, -1.1f, 0.0f));
    R2arm.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Component L1leg(cube, &torso);
    L1leg.setTranslate(glm::vec3(-0.5f, -2.0f, 0.0f));
    L1leg.setScale(glm::vec3(0.5f, 0.5f, 1.0f));

    Component L2leg(cube, &L1leg);
    L2leg.setTranslate(glm::vec3(0.0f, -1.1f, 0.0f));
    L2leg.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Component R1leg(cube, &torso);
    R1leg.setTranslate(glm::vec3(0.5f, -2.0f, 0.0f));
    R1leg.setScale(glm::vec3(0.5f, 0.5f, 1.0f));

    Component R2leg(cube, &R1leg);
    R2leg.setTranslate(glm::vec3(0.0f, -1.1f, 0.0f));
    R2leg.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (animation) {
            ;
        }

        glClearColor(0.15f, 0.35f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        torso.Draw(shader);
        head.Draw(shader);
        L1arm.Draw(shader);
        L2arm.Draw(shader);
        R1arm.Draw(shader);
        R2arm.Draw(shader);
        L1leg.Draw(shader);
        L2leg.Draw(shader);
        R1leg.Draw(shader);
        R2leg.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        animation = !animation;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}