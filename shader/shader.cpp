#include "shader.h"

Shader::Shader(QOpenGLExtraFunctions* functions, const std::string &filename) : m_filepath(filename), m_rendererID(0)
{
    m_functions = functions;
    ShaderProgramSource source = ParseShader(filename);
    m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

uint Shader::GetRendererID() const
{
    return m_rendererID;
}

Shader::~Shader()
{
    m_functions->glDeleteProgram(m_rendererID);
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    enum ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}
unsigned int Shader::CompileShader(const std::string &source, uint type)
{
    unsigned int id = m_functions->glCreateShader(type);
    const char *src = source.c_str();
    m_functions->glShaderSource(id, 1, &src, nullptr);
    m_functions->glCompileShader(id);

    int result;
    m_functions->glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        m_functions->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        m_functions->glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        m_functions->glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = m_functions->glCreateProgram();
    unsigned int vxShader = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fsShader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    m_functions->glAttachShader(program, vxShader);
    m_functions->glAttachShader(program, fsShader);
    m_functions->glLinkProgram(program);
    m_functions->glValidateProgram(program);
    m_functions->glDeleteShader(vxShader);
    m_functions->glDeleteShader(fsShader);

    return program;
}
void Shader::Bind() {
    m_functions->glUseProgram(m_rendererID);
    GLenum error = m_functions->glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error occurred: " << error << std::endl;
    }
}

void Shader::Unbind()
{
    m_functions->glUseProgram(0);
}

void Shader::SetUniformMat4f(const std::string &name, const s21::Matrix4x4 &mat)
{
    m_functions->glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat.GetData());
}

int Shader::GetUniformLocation(const std::string &name)
{
    if (m_UniformLocationCashe.find(name) != m_UniformLocationCashe.end())
        return m_UniformLocationCashe[name];
    int location = m_functions->glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning:" << name << std::endl;
    }

    m_UniformLocationCashe[name] = location;
    return location;
}