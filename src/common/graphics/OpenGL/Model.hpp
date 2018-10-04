struct Model
{
char *name;
GLuint vao;
GLuint vertexBuffer;
GLuint indexBuffer;
uint32_t vertexCount;
uint32_t indexCount;
Shader shader;
};

glEnableVertexAttribArray(a_position);
glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, sizeof (Model), (const GLvoid*) offsetof(Model, position));
