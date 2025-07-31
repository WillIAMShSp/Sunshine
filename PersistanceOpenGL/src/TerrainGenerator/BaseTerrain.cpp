#include "BaseTerrain.h"

void BaseTerrain::SetSize(const unsigned int& xzsize)
{
    m_xzsize = xzsize;

    InitVertices(m_xzsize);

}

void BaseTerrain::SetShader(const Shader& shader)
{
    m_shader = shader;

}

void BaseTerrain::SetHeightMap(const Array2D& heightmap)
{
	m_heightmap = heightmap;


}

void BaseTerrain::InitHeightMap()
{
    
    m_heightmap.SetSize(m_xzsize, m_xzsize);


}

void BaseTerrain::InitMesh()
{


    m_layout.push<float>(3);// vertices
    m_layout.push<float>(2);// uv
    m_layout.push<float>(3);// normals


    
    
   
    m_vb = VertexBuffer(m_vertices);
    m_ib = IndexBuffer(m_indices);

    m_vao.AddBuffer(m_vb, m_layout);

    m_ib.UnBind();
    
}

void BaseTerrain::UpdateHeight()
{

    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i].y = m_heightmap.GetPoint(i);





    }
}

void BaseTerrain::UpdateNormals()
{
    struct normalcalculationunit
    {
        glm::vec3 normal = glm::vec3(0, 0, 0);
    
        int count = 0;


        void Normalize()
        {
            float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

            normal.x = normal.x / length;
            normal.y = normal.y / length;
            normal.z = normal.z / length;


        }

        



    
    };

    std::vector<normalcalculationunit> normalcalc;
    normalcalc.resize(m_xzsize*m_xzsize);

    
    
    int count = 0;
    int x = 0;


    for (int i = 0; i < m_indices.size(); i++)
    {



        int ia = m_indices[i];
        int ib = m_indices[i + 1];
        int ic = m_indices[i + 2];


        glm::vec3 e1 = glm::vec3((m_vertices[ia].x) , (m_vertices[ia].y) , (m_vertices[ia].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        glm::vec3 e2 = glm::vec3((m_vertices[ic].x) , (m_vertices[ic].y) , (m_vertices[ic].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        glm::vec3 no = glm::cross(e1, e2);

        normalcalc[ia].normal += no;
        normalcalc[ia].count++;


        normalcalc[ib].normal += no;
        normalcalc[ib].count++;


        normalcalc[ic].normal += no;
        normalcalc[ic].count++;

        ia = m_indices[i + 3];
        ib = m_indices[i + 4];
        ic = m_indices[i + 5];

        e1 = glm::vec3((m_vertices[ia].x), (m_vertices[ia].y), (m_vertices[ia].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        e2 = glm::vec3((m_vertices[ic].x), (m_vertices[ic].y), (m_vertices[ic].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        no = glm::cross(e1, e2);

        normalcalc[ia].normal += no;
        normalcalc[ia].count++;


        normalcalc[ib].normal += no;
        normalcalc[ib].count++;


        normalcalc[ic].normal += no;
        normalcalc[ic].count++;


        i += 5;


    }






    


    for (int i = 0; i < normalcalc.size(); i++)
    {

        
        normalcalc[i].Normalize();


        



        m_vertices[i].n0 = normalcalc[i].normal.x;
        m_vertices[i].n1 = normalcalc[i].normal.y;
        m_vertices[i].n2 = normalcalc[i].normal.z;





    }




    







}

void BaseTerrain::Draw()
{

    m_shader.Bind();

    m_vao.Bind();
    m_ib.Bind();


    glDrawElements(GL_TRIANGLES, m_indices.size() , GL_UNSIGNED_INT, 0);

    m_ib.UnBind();
    m_vao.UnBind();
    
    
    m_shader.UnBind();
    




}

void BaseTerrain::InitVertices(const int& meshsize)
{

    m_vertices.resize(meshsize * meshsize);
    

    int x = 0;
    int z = 0;


    for (int i = 0; i < m_vertices.size(); i++)
    {
        if (x >= meshsize)
        {
            x = 0;
            z++;

        }

        m_vertices[i] = Vertex(x, 0, z, 0, 0, 0, 0, 0);

        x++;




    }


    InitIndices(meshsize);

}

void BaseTerrain::InitIndices(const int& pointamount)
{
    int arrpoint = pointamount - 1;

    int numofindices = 6 * arrpoint * arrpoint;

    m_indices.resize(numofindices);

    int count = 0;
    int x = 0;


    for (int i = 0; i < m_indices.size(); i++)
    {



        m_indices[i] = count;
        m_indices[i + 1] = count + 1 + pointamount;
        m_indices[i + 2] = count + pointamount;


        m_indices[i + 3] = count;
        m_indices[i + 4] = count + 1;
        m_indices[i + 5] = count + 1 + pointamount;
        i += 5;
        count++;
        x++;


        if (x == pointamount - 1)
        {


            x = 0;
            count++;




        }






    }

}
