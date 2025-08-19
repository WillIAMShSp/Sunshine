#include "ComputeShader.h"
ComputeShader::ComputeShader()
{

}

ComputeShader::ComputeShader(const std::string& ComputeShaderSource)
{

    m_Shader = CreateShader(ParseShader(ComputeShaderSource));

    



}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::Bind()
{
    glUseProgram(m_Shader);

}

void ComputeShader::UnBind()
{
    glUseProgram(0);
}

void ComputeShader::SetUniform4f(const std::string& name, float value1, float value2, float value3, float value4)
{

    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniform4f(location, value1, value2, value3, value4);


    if (location == -1)
    {
        std::cout << name << " uniform does not exist \n";

    }
    UnBind();

}

void ComputeShader::SetUniform3f(const std::string& name, float value1, float value2, float value3)
{
    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniform3f(location, value1, value2, value3);

    UnBind();

}

void ComputeShader::SetUniform3f(const std::string& name, const glm::vec3& vector)
{
    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniform3f(location, vector.x, vector.y, vector.z);

    UnBind();


}

void ComputeShader::ComputeShaderDispatch(const uint32_t& valueX, const uint32_t& valueY, const uint32_t& valueZ)
{

    Bind();

    glDispatchCompute(valueX, valueY, valueZ);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);




}


void ComputeShader::SetUniform1f(const std::string& name, float value)
{
    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniform1f(location, value);

    if (location == -1)
    {
        std::cout << name << " uniform does not exist \n";

    }
    UnBind();

}

void ComputeShader::SetUniform1i(const std::string& name, int value)
{
    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniform1i(location, value);

    if (location == -1)
    {
        std::cout << name << " uniform does not exist \n";

    }

    UnBind();

}

void ComputeShader::SetUniformMat4fv(const std::string& name, const glm::mat4& matrix)
{
    Bind();
    int location = glGetUniformLocation(m_Shader, name.c_str());

    glUniformMatrix4fv(location, 1, 0, &matrix[0][0]);

    UnBind();

}





std::string ComputeShader::ParseShader(const std::string& path)
{
    std::ifstream stream(path);
    std::string line;
    std::stringstream ss;



    while (getline(stream, line))
    {


        ss << line << "\n";




    }

    return ss.str();




}
unsigned int ComputeShader::CompileShader(const std::string& source)
{
    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    const char* src = source.c_str();



    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    int shaderscompiled; //did they work? umm idk, lets check!

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderscompiled);

    if (shaderscompiled == 0)
    {

        int length = GL_INFO_LOG_LENGTH;




        char* message = (char*)alloca(sizeof(char) * length);

        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << "shit\n";
        std::cout << message;

        glDeleteShader(shader);
        return 0;

    }





    return shader;



}
unsigned int ComputeShader::CreateShader(const std::string& ComputeShaderSource)
{
    unsigned int program = glCreateProgram();
    unsigned int cs = CompileShader(ComputeShaderSource);
    
    

    glAttachShader(program, cs);
    
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(cs);



    return program;






}


