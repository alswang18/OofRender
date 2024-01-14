#include "Window.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";
int success;
char infoLog[512];

namespace Core
{
	unsigned int CompileVertexShader(const char *vertexShaderSource)
	{
		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
		}
		return vertexShader;
	}

	unsigned int CompileFragmentShader(const char *fragmentShaderSource)
	{
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
		}
		return fragmentShader;
	}

	unsigned int LinkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
	{
		// link shaders
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return shaderProgram;
	}
	int Render()
	{
		Window wnd = Window(SCR_WIDTH, SCR_HEIGHT, "OofRender");

		unsigned int vertexShader = CompileVertexShader(vertexShaderSource);
		unsigned int fragmentShader = CompileFragmentShader(fragmentShaderSource);

		// link shaders
		unsigned int shaderProgram = LinkShaderProgram(vertexShader, fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// first triangle
			-0.9f, -0.5f, 0.0f, // left
			-0.0f, -0.5f, 0.0f, // right
			-0.45f, 0.5f, 0.0f	// top
		};

		float vertices2[] = {
			0.0f, -0.5f, 0.0f, // left
			0.9f, -0.5f, 0.0f, // right
			0.45f, 0.5f, 0.0f  // top
		};

		unsigned int indices[] = {
			// note that we start from 0!
			0, 1, 3, // first triangle
			1, 2, 3	 // second triangle
		};

		unsigned int VBO[2];
		unsigned int VAO[2];
		unsigned int EBO;
		glGenVertexArrays(2, VAO);
		glGenBuffers(2, VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO[0]);
		// glGenBuffers(1, &EBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(VAO[1]);
		// glGenBuffers(1, &EBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		// the index in the code below is the index of the vertex
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		// 			 GL_STATIC_DRAW);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		// uncomment this call to draw in wireframe polygons.
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		// -----------
		while (!wnd.shouldClose())
		{
			// input
			// -----
			wnd.processInput();

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// draw our first triangle
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(VAO[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// glDrawElements(GL_TRINGLES, 6, GL_UNSIGNED_INT, 0);
			// glBindVertexArray(0); // no need to unbind it every time

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			wnd.swapBuffers();
			wnd.pollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO[0]);
		glDeleteBuffers(1, &VBO[0]);
		glDeleteVertexArrays(1, &VAO[1]);
		glDeleteBuffers(1, &VBO[1]);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}

}