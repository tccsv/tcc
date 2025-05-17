#include <TCC/TCC_compute_shader.h>

//#include <stdio.h>

#define array_size 1024

const GLchar* computeShaderSource = "#version 430 core\n"
"layout(local_size_x = 256) in;\n"
"layout(std430, binding = 0) buffer input {\n"
"float data[];\n"
"};\n"
"layout(std430, binding = 1) buffer output {\n"
"float result[];\n"
"};\n"
"void main() {\n"
"uint id = gl_globalinvocationid.x;\n"
"result[0] += data[id];"
"}\0";

GLuint computeshader, shaderprogram;
GLuint inputbuffer, outputbuffer;

float totalsum = 0.0f;

void checkShaderCompile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        //fprintf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}

int test() {
    glewInit();

    // Исходные данные
    float inputData[] = { 1.0f, 2.0f, 3.0f, 4.0f };
    size_t dataSize = sizeof(inputData);

    // Создание буферов
    GLuint inputBuffer, outputBuffer;
    glGenBuffers(1, &inputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, inputData, GL_STATIC_DRAW);

    float resultData = 0.0f;
    glGenBuffers(1, &outputBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float), &resultData, GL_DYNAMIC_DRAW);

    // Создание вычислительного шейдера
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);
    checkShaderCompile(computeShader);

    // Создание программы
    GLuint program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);

    // Установка буферов
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer);

    // Запуск вычислительного шейдера
    glUseProgram(program);
    glDispatchCompute(4, 1, 1); // Запускаем 4 рабочих групп

    // Синхронизация
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Получение результата
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
    float* ptr = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    printf("Sum: %f\n", *ptr);

    // Освобождение ресурсов
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glDeleteProgram(program);
    glDeleteShader(computeShader);
    glDeleteBuffers(1, &inputBuffer);
    glDeleteBuffers(1, &outputBuffer);

    return 0;
}