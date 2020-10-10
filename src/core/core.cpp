#include "core.h"

t_scene scene;

//glm::vec3 lightPos(0.8f, 0.8f, 0.8f);

Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = 800 / 2.0;
GLfloat lastY = 600 / 2.0;

int WIDTH = 800, HEIGHT = 600; 
bool keys[1024];

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

timer time;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
unsigned int loadCubemap(vector<std::string> faces);

stack <t_model> stack_of_model;
stack <t_model> stack_of_moving_model;

void main_loop()
{
    init_GL();

    GLFWwindow* window = create_window(WIDTH, HEIGHT, "Engine");
    
    /*scene.Model.add_3d_model(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "res/models/cube.obj", 32.0f, -1,
        "res/shaders/light.vs", "res/shaders/light.frag", scene.Shaders, "res/materials/united.png", "res/materials/unitedBlack.png");
    scene.Model.add_3d_model(glm::vec3(-0.8f, 0.8f, -0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), "res/models/cube.obj", 0.0f, -1,
        "res/shaders/lamp.vs", "res/shaders/lamp.frag", scene.Shaders, "", "");
    scene.Model.add_3d_model(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.2f, 20.0f), "res/models/plane.obj", 32.0f, -1,
        "res/shaders/shadow_mapping.vs", "res/shaders/shadow_mapping.frag", scene.Shaders, "res/materials/united.png", "");

    scene.Light.add_dirLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f), 
        glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
    scene.Light.add_pointLight(glm::vec3(-0.8f, 0.8f, -0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.05f, 0.05f, 0.05f), 
        glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045, 0.0075);*/

    scene.SkyBox.addSkybox("res/shaders/skybox.vs","res/shaders/skybox.frag","res/maps/skybox/right.jpg", "res/maps/skybox/left.jpg", "res/maps/skybox/top.jpg", "res/maps/skybox/bottom.jpg", "res/maps/skybox/front.jpg", "res/maps/skybox/back.jpg");

    scene.Model.add_3d_model(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "res/models/cube.obj", 32.0f, -1,
        "res/shaders/shadow/shadow_debug_mapping.vs", "res/shaders/shadow/shadow_debug_mapping.frag", scene.Shaders, "res/materials/wood.png", ""/*"res/materials/unitedBlack.png"*/); //��� �������� ������ �� �������� �������� �����������
    scene.Model.add_3d_model(glm::vec3(-0.8f, 0.8f, -0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), "res/models/cube.obj", 0.0f, -1,
        "res/shaders/lamp.vs", "res/shaders/lamp.frag", scene.Shaders, "", "");
    scene.Model.add_3d_model(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.2f, 20.0f), "res/models/plane.obj", 32.0f, -1,
        "res/shaders/shadow/shadow_debug_mapping.vs", "res/shaders/shadow/shadow_debug_mapping.frag", scene.Shaders, "res/materials/wood.png", "");

    scene.Light.add_dirLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
    scene.Light.add_pointLight(glm::vec3(-0.8f, 0.8f, -0.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045, 0.0075);

    int suc;
    //Shader simpleDepthShader("res/shaders/shadow_debug_mapping.vs", "res/shaders/shadow_debug_mapping.frag", suc);
    Shader simpleDepthShader("res/shaders/shadow/shadow_debug_depth.vs", "res/shaders/shadow/shadow_debug_depth.frag", suc, "res/shaders/shadow/shadow_debug_depth.gs");

    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    //const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    /*unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------

    scene.Shaders.vec[0].Use();
    scene.Shaders.vec[0].setInt("diffuseTexture", 0);
    scene.Shaders.vec[0].setInt("depthMap", 1);

    //debugDepthQuad.use();
    //debugDepthQuad.setInt("depthMap", 0);

    while (!glfwWindowShouldClose(window))
    {
        time.calc_time();

        glfwPollEvents();
        do_movement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        /*
        //debug shadow

        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);*/

        scene.draw_scene(camera, WIDTH, HEIGHT, depthMapFBO, depthCubemap, simpleDepthShader);

        glfwSwapBuffers(window);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, time.delta_sec);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, time.delta_sec);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, time.delta_sec);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, time.delta_sec);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
