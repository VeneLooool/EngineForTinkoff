#include "core.h"

t_shader vec_shader;

glm::vec3 lightPos(0.8f, 1.0f, 1.0f);

Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 800 / 2.0;
GLfloat lastY = 600 / 2.0;

int WIDTH = 800, HEIGHT = 600; 
bool keys[1024];

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

void main_loop()
{
    init_GL();

    GLFWwindow* window = create_window(WIDTH, HEIGHT, "Engine");

    t_model light;
    light.curent_shader = 0;
    //model.load_obj("C:/Users/panih/source/repos/Engine/Engine/res/models/VideoShip.obj");
    light.load_obj("C:/Users/panih/source/repos/Engine/Engine/res/models/cube.obj");
    light.setup_mesh();

    light.spawnPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    light.curentPosition = light.spawnPosition;

    t_model lamp;
    lamp.curent_shader = 0;
    lamp.load_obj("C:/Users/panih/source/repos/Engine/Engine/res/models/cube.obj");
    lamp.setup_mesh();

	vec_shader.load_shader("C:/Users/panih/source/repos/Engine/Engine/res/shaders/light.vs", "C:/Users/panih/source/repos/Engine/Engine/res/shaders/light.frag");
    vec_shader.load_shader("C:/Users/panih/source/repos/Engine/Engine/res/shaders/lamp.vs", "C:/Users/panih/source/repos/Engine/Engine/res/shaders/lamp.frag");


    GLuint texture1;
    GLuint texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image("C:/Users/panih/source/repos/Engine/Engine/res/materials/united.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = SOIL_load_image("C:/Users/panih/source/repos/Engine/Engine/res/materials/unitedBlack.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vec_shader.vec[0].Use();
        GLint lightPosLoc = glGetUniformLocation(vec_shader.vec[0].Program, "light.position");
        GLint lightAmbientLoc = glGetUniformLocation(vec_shader.vec[0].Program, "light.ambient");
        GLint lightDiffuseLoc = glGetUniformLocation(vec_shader.vec[0].Program, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(vec_shader.vec[0].Program, "light.specular");
        GLint viewPosLoc = glGetUniformLocation(vec_shader.vec[0].Program, "viewPos");
        //GLint ambientLoc = glGetUniformLocation(vec_shader.vec[0].Program, "material.ambient");
        //GLint diffuseLoc = glGetUniformLocation(vec_shader.vec[0].Program, "material.diffuse");
        GLint specularLoc = glGetUniformLocation(vec_shader.vec[0].Program, "material.specular");
        GLint shininessLoc = glGetUniformLocation(vec_shader.vec[0].Program, "material.shininess");

        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f); 
        glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

        //glUniform3f(ambientLoc, 1.0f, 0.5f, 0.31f);
        //glUniform3f(diffuseLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(specularLoc, 0.5f, 0.5f, 0.5f);
        glUniform1f(shininessLoc, 32.0f);

        //PHYSIC{

        GLint spawnPosLoc = glGetUniformLocation(vec_shader.vec[0].Program, "spawnPosition");
        glUniform3f(spawnPosLoc, light.spawnPosition.x, light.spawnPosition.y, light.spawnPosition.z);
        GLint biasLoc = glGetUniformLocation(vec_shader.vec[0].Program, "bias");
        glUniform3f(biasLoc , light.curentPosition.x - light.spawnPosition.x , light.curentPosition.y - light.spawnPosition.y, light.curentPosition.z - light.spawnPosition.z);

        //PHYSIC}


        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(vec_shader.vec[0].Program, "model");
        GLint viewLoc = glGetUniformLocation(vec_shader.vec[0].Program, "view");
        GLint projLoc = glGetUniformLocation(vec_shader.vec[0].Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glm::mat4 model (1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(vec_shader.vec[0].Program, "material.diffuse"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(vec_shader.vec[0].Program, "material.specular"), 1);


        
        glBindVertexArray(light.VAO);
        glDrawElements(GL_TRIANGLES, light.tri.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // Also draw the lamp object, again binding the appropriate shader
        vec_shader.vec[1].Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(vec_shader.vec[1].Program, "model");
        viewLoc = glGetUniformLocation(vec_shader.vec[1].Program, "view");
        projLoc = glGetUniformLocation(vec_shader.vec[1].Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(lamp.VAO);
        glDrawElements(GL_TRIANGLES, lamp.tri.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        

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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
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