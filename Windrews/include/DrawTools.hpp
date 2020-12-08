#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE

class AbstractWindow {
public:
	viewPortState viewChange;
	virtual void draw(const viewPortState& state, const Shaders& shaders) = 0;
	virtual void callback(const WindowStat& status, const viewPortState& state) = 0;
	virtual void move(const Point& moveVec, const viewPortState& state) = 0;
	virtual bool changeViewPort() = 0;
	viewPortState getViewPortChange() {
		return viewChange;
	}

	AbstractWindow():
	viewChange(0, 0, 0, 0) {}
};

struct Color {
	double red;
	double green;
	double blue;

	Color() = default;
	Color(double red, double green, double blue): red(red), green(green), blue(blue) {}
	Color(const Color& other) = default;

	void set() const {
		glColor3d(red, green, blue);
	}

	void set_as_bg() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(red, green, blue, 1.0);
	}
};

/*
template<typename MeasureType, int MeasureNum, int MeasureFreq, MeasureType MaxResult>
class Graphic: public AbstractWindow {
	MeasureType* MEASURES;
	Point topLeft;
	Point bottomRight;
	Color color;

public:
	Graphic () = delete;
	Graphic (Point topLeft, Point bottomRight, Color color, MeasureType* measures):
	topLeft (topLeft),
	bottomRight (bottomRight),
	color (color),
	MEASURES (measures) {}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		topLeft += moveVec;
		bottomRight += moveVec;
	}

	virtual void draw (const viewPortState& state, const Shaders& shaders) override {

		Point start_point(topLeft.x, bottomRight.y);

		double vertical_length   = topLeft.y - bottomRight.y;
		double horizontal_length = bottomRight.x - topLeft.x;
		int max_size = MeasureNum * MeasureFreq;

		Point current;

		color.set();

		glBegin(GL_LINE_STRIP);

		start_point.set();

		for (int i = 1; i <= MeasureNum; ++i) {
			current.x = start_point.x + horizontal_length * double(i) * double(MeasureFreq) / max_size;
			current.y = start_point.y + vertical_length * double(MEASURES[i]) / MaxResult; 
			if (current.y > topLeft.y) {
				glEnd();
				glBegin(GL_LINE_STRIP);
				continue;
			}
			current.set();
		}

		glEnd();
		glFlush();
	}

	virtual bool changeViewPort() {
		return false;
	}
};
*/



template <int N>
class Polygon: public AbstractWindow {
	Point coordinates[N];
	GLuint VAO, VBO;
	GLfloat vertices[2*N];
	Color color;

	void make_vertices() {
		for (int i = 0; i < N; ++i) {
			vertices[2*i] = coordinates[i].x;
			vertices[2*i + 1] = coordinates[i].y;
		}
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

public:
	Polygon () = delete;

	Polygon (const Point points[N], Color color): color (color) {
		for (int i = 0; i < N; ++i)
			coordinates[i] = points[i];

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		make_vertices();
	}

	Color getColor() {
		return color;
	}
	void setColor(const Color& color) {
		this->color = color;
	}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		for (int i = 0; i < N; ++i)
			coordinates[i] += moveVec;
		make_vertices();
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw (const viewPortState& state, const Shaders& shaders) override {
		GLint vertexColorLocation = glGetUniformLocation(shaders.primitiveShader.Program, "inColor");
		shaders.primitiveShader.Use();
		glUniform3f(vertexColorLocation, color.red, color.green, color.blue);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POLYGON, 0, N);
		glBindVertexArray(0);
	}

	virtual bool changeViewPort() {
		return false;
	}
};



class Octangle: public AbstractWindow {
protected:

	GLfloat vertices[8] = {};
	Point topLeft;
	Point bottomRight;
	Color color;

	GLuint VBO, VAO;

	virtual void make_vertices() {
		Point topRight = {bottomRight.x, topLeft.y};
		Point bottomLeft = {topLeft.x, bottomRight.y};

		vertices[0] = topLeft.x;
		vertices[1] = topLeft.y;

		vertices[2] = topRight.x;
		vertices[3] = topRight.y;

		vertices[4] = bottomRight.x;
		vertices[5] = bottomRight.y;

		vertices[6] = bottomLeft.x;
		vertices[7] = bottomLeft.y;

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

public:
	Octangle (const Point points[2], Color color): color (color){
		if (points[0].x >= points[1].x) {
			bottomRight = points[0];
			topLeft = points[1];
		}
		else {
			bottomRight = points[1];
			topLeft = points[0];
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		make_vertices();
	}

	Octangle (const Point& topLeft, const Point& bottomRight, Color color):
	topLeft (topLeft),
	bottomRight (bottomRight),
	color (color) {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		make_vertices();
	}

	void setPoints(const Point& topLeft, const Point& bottomRight) {
		this->topLeft = topLeft;
		this->bottomRight = bottomRight;
		make_vertices();
	}
	
	void getPoints(Point& topLeft, Point& bottomRight) {
		topLeft = this->topLeft;
		bottomRight = this->bottomRight;
	}

	void setColor(const Color& color) {
		this->color = color;
	}

	Color getColor() {
		return color;
	}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		topLeft += moveVec;
		bottomRight += moveVec;
		make_vertices();
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw (const viewPortState& state, const Shaders& shaders) override {
		GLint vertexColorLocation = glGetUniformLocation(shaders.primitiveShader.Program, "inColor");
		shaders.primitiveShader.Use();
		glUniform3f(vertexColorLocation, color.red, color.green, color.blue);

		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, 4);
		glBindVertexArray(0);

	}

	virtual bool changeViewPort() {
		return false;
	}

};

class Picture: public Octangle {
	const char* img_path;

	GLuint EBO;
	GLuint texture;

	void make_vertices() override {
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

public:
	Picture(const Point& topLeft, const Point& bottomRight, const char* img_path):
	Octangle(topLeft, bottomRight, {0.0, 0.0, 0.0}), 
	img_path(img_path)
	{
		glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

		make_vertices();
	}

	Picture(const Point points[2], const char* img_path):
	Octangle(points, {0.0, 0.0, 0.0}), 
	img_path(img_path)
	{
		glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

		make_vertices();
	}

	virtual void draw (const viewPortState& state, const Shaders& shaders) override {
		shaders.textureShader.Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaders.textureShader.Program, "myTexture"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

	}
};

class Arrow: public AbstractWindow {
	static const int POINT_NUM = 4;
	Point coordinates[POINT_NUM];

	GLfloat line_vertices[4];
	GLfloat end_vertices[6];

	GLuint lineVAO, lineVBO, endVAO, endVBO;

	void make_vertices() {
		for (int i = 0; i < 2; ++i) {
			line_vertices[2*i] = coordinates[i].x;
			line_vertices[2*i + 1] = coordinates[i].y;
		}

		for (int i = 0; i < 3; ++i) {
			end_vertices[2*i] = coordinates[i+1].x;
			end_vertices[2*i + 1] = coordinates[i+1].y;
		}

		glBindVertexArray(lineVAO);

		glBindBuffer(GL_ARRAY_BUFFER, lineVBO); 
		glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(endVAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, endVBO); 
		glBufferData(GL_ARRAY_BUFFER, sizeof(end_vertices), end_vertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

public:
	Color color;

	Arrow (const Point points[4], Color color): color (color) {
		for (int i = 0; i < 4; ++i)
			coordinates[i] = points[i];
		
		glGenVertexArrays(1, &lineVAO);
		glGenBuffers(1, &lineVBO);
		glGenVertexArrays(1, &endVAO);
		glGenBuffers(1, &endVBO);

		make_vertices();
	}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		for (int i = 0; i < POINT_NUM; ++i)
			coordinates[i] += moveVec;
	
		make_vertices();
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw(const viewPortState& state, const Shaders& shaders) override {
		GLint vertexColorLocation = glGetUniformLocation(shaders.primitiveShader.Program, "inColor");
		shaders.primitiveShader.Use();
		glUniform3f(vertexColorLocation, color.red, color.green, color.blue);

		glBindVertexArray(lineVAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(endVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	virtual bool changeViewPort() {
		return false;
	}
};

class UnderScreen: public AbstractWindow {
	Point bottomLeft;
	Color color;
	Octangle background;
	int width;
	int height;

public:

	UnderScreen(const Point& bottomLeft, int width, int height, const Color& color):
	bottomLeft(bottomLeft),
	color(color),
	width(width),
	height(height),
	background(Point(-1, 1), Point(1, -1), color) {
		viewChange = viewPortState(bottomLeft, width, height); 	
	}
	
	virtual void callback(const WindowStat& status, const viewPortState& state) override {};
	virtual void move(const Point& moveVec, const viewPortState& state) override {
		bottomLeft += Point(moveVec.x * state.width, moveVec.y * state.height);
	}

	virtual void draw(const viewPortState& state, const Shaders& shaders) override {
		glViewport(int(bottomLeft.x), int(bottomLeft.y), width, height);
		background.draw(state, shaders);
	}

	virtual bool changeViewPort() override {
		return true;
	}
};
#endif