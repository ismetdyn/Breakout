#include <resource_manager.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb_image.h>

std::map<std::string, Texture2D>	ResourceManager::Textures;
std::map<std::string, Shader>		ResourceManager::Shaders;

Shader ResourceManager::LoadShader(
	const GLchar* vShaderFile,
	const GLchar* fShaderFile,
	const GLchar* gShaderFile,
	std::string name)
{
	Shaders[name] = loadShaderFromFile(
		vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(
	const GLchar* file,
	GLboolean alpha,
	std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// ensure ifstream objects can throws exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		if (!vShaderFile.is_open() || !fShaderFile.is_open())
		{
			throw std::runtime_error("ERROR::SHADER: Failed to open shader files");
		}

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// Read file's buffer contents GLinto stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream GLinto string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			if (!gShaderFile.is_open())
			{
				throw std::runtime_error("ERROR::SHADER: Failed to open geometry shader file");
			}
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = nullptr;
	if (geometryPath != nullptr) gShaderCode = geometryCode.c_str();
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(
	const GLchar* file, GLboolean alpha)
{
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	GLint width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	texture.Generate(width, height, data);
	
	stbi_image_free(data);
	return texture;
}