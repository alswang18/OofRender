#pragma once

namespace Core
{
	int Render();

	unsigned int CompileVertexShader(const char *vertexShaderSource);

	unsigned int CompileFragmentShader(const char *fragmentShaderSource);
}