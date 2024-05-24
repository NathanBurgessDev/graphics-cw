#include "SkyBox.h"

SkyBox::SkyBox(GLuint skyBoxShaderProgram) {

    shaderProgram = skyBoxShaderProgram;


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (skyboxVertices.size() * 9), skyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
    {
        "objs/skybox/right.jpg",
        "objs/skybox/left.jpg",
        "objs/skybox/top.jpg",
        "objs/skybox/bottom.jpg",
        "objs/skybox/front.jpg",
        "objs/skybox/back.jpg"
    };

    textureID = loadCubeMap(faces);
}

void SkyBox::RenderSkyBox(glm::mat4 view, glm::mat4 projection) {
    glDepthMask(GL_FALSE);
    glUseProgram(shaderProgram);

    glm::mat4 newView = glm::mat4(glm::mat3(view));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(newView));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE); 
}