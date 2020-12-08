#include <iostream>
#include "../Windrews/Windrews.hpp"


const Color WHITE = {1.0, 1.0, 1.0};
const Point WHITE_OCTANGLE[] = {
    {-1.0, 1.0},
    {1.0, -0.75},
};
const Point TMP[] = {
    {-1.0, 1.0},
    {0.5, -0.25},
};


const Color BLUE = {0.0, 0.0, 1.0};
const Point BLUE_OCTANGLE[] = {
    {-1.0, -0.75},
    {1.0, -2.0},
};

const Color RED = {1.0, 0.0, 0.0};
const Point RED_OCTANGLE[] = {
    {-1.0, -2.0},
    {1.0, -4.0},
};

class Img: public AbstractWindow {

    const char* img_path;
    Point topLeft;
    Point bottomRight;
    const Shader* shader;

    GLuint VAO;
    GLuint texture;


public:
    Img(const Point& topLeft, const Point& bottomRight, const char* img_path, const Shader* shader):
    topLeft(topLeft),
    bottomRight(bottomRight),
    img_path(img_path),
    shader(shader) {

        Point topRight;
        topRight.x = bottomRight.x;
        topRight.y = topLeft.y;

        Point bottomLeft;
        bottomLeft.x = topLeft.x;
        bottomLeft.y = bottomRight.y;

        GLfloat vertices[] = {
            // Positions                                       // Colors           // Texture Coords
            float(topRight.x),  float(topRight.y), 0.0f,       0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Top Right
            float(bottomRight.x), float(bottomRight.y), 0.0f,  0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Bottom Right
            float(bottomLeft.x), float(bottomLeft.y), 0.0f,    0.0f, 0.0f, 0.0f,   0.0f, 1.0f, // Bottom Left
            float(topLeft.x),  float(topLeft.y), 0.0f,         0.0f, 0.0f, 0.0f,   0.0f, 0.0f  // Top Left 
        };
        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };
        GLuint VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        int width, height;
        unsigned char* image = SOIL_load_image(img_path, &width, &height, 0, SOIL_LOAD_RGB);

        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    virtual void draw(const viewPortState& state) override {

        shader->Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader->Program, "myTexture"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    virtual void callback(const WindowStat& status, const viewPortState& state) {}

    virtual void move(const Point& moveVec, const viewPortState& state) {
        topLeft += moveVec;
        bottomRight += moveVec;
    }

    virtual bool changeViewPort() override {
        return false;
    }

};

int main(int argc, char** argv) {

    Windrew* window = windrewsInit(argc, argv, {0.04, 0.255, 0.29}, 720, 1280);
    if (window == nullptr)
		return -1;  


    WindowNode* view = window->make_underwindow(new UnderScreen({200, 100}, 900, 400, {0, 1, 0}), nullptr);

    // window->make_underwindow(new Octangle(WHITE_OCTANGLE, WHITE), view);
    // window->make_underwindow(new Octangle(BLUE_OCTANGLE, BLUE), view);
    // window->make_underwindow(new Octangle(RED_OCTANGLE, RED), view);

    window->make_underwindow(new Img({-0.7, 0.7}, {0.7, -0.7}, "textures/texture.jpg", &window->textureShader), view);
    window->make_underwindow(new HorizontalScrollBar({0.95, 1.0}, {1.0, -1.0}, 0.1, 1.0, view, {0.95, 0.95, 0.95}, {1,1,0}, {0.6, 0.6, 0}, {0.2, 0.2, 0}), view);

    while (!window->shouldClose()) {

        window->pollEvents();
        window->draw();

    }

    windrewsTerminate();
}