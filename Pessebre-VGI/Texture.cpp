#include "Texture.h"

Texture::Texture(const char* img, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;
	unit = slot;

	int widthImg, heightImg, numColChl;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(img, &widthImg, &heightImg, &numColChl, 0);

	if (bytes == nullptr) {
		std::cerr << "Error loading texture: " << img << "\nReason: " << stbi_failure_reason() << std::endl;
		return;
	}

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + unit);
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, (numColChl == 4) ? GL_RGBA : GL_RGB, widthImg, heightImg, 0, (numColChl == 4) ? GL_RGBA : GL_RGB, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	Unbind();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &id);
}

