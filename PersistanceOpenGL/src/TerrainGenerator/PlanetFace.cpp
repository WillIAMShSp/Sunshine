#include "PlanetFace.h"

PlanetFace::PlanetFace(int resolution, glm::vec3 updirection)
{
	m_resolution = resolution;
	m_updirection = updirection;

	m_axisA = glm::vec3(updirection.y, updirection.z, updirection.x);

	m_axisB = glm::cross(updirection, m_axisA);

}

PlanetFace::PlanetFace(const int& resolution, const FacePlane& faceplane)
{
    m_resolution = resolution;
    
    m_updirection = faceplane.updirection;

    m_axisA = faceplane.xaxis;

    m_axisB = glm::cross(m_updirection, m_axisA);


}

void PlanetFace::InitVertices()
{

	m_vertices.resize(m_resolution * m_resolution);


	for (int y = 0; y < m_resolution; y++)
	{
		

		for (int x = 0; x < m_resolution; x++)
		{

			int i = x + (y * m_resolution);

			glm::vec2 percent(x,y);
			percent /= (m_resolution - 1);
			glm::vec3 vertexunitpositiononcube = m_updirection + (percent.x - 0.5f) * 2 * m_axisA + (percent.y - 0.5f) * 2 * m_axisB;
            glm::vec3 vertexunitpositiononsphere = CubeToSphere(vertexunitpositiononcube);
            //glm::normalize(vertexunitpositiononcube);

			//m_vertices[i] = Vertex(vertexunitpositiononsphere.x, vertexunitpositiononsphere.y, vertexunitpositiononsphere.z, 0, 0 ,0 , 0 ,0);

            
            

            float elevation = 0;

            for (int e = 0; e < m_settings.numoflayers; e++)
            {

                elevation += m_perlin.PerlinNoise3D(vertexunitpositiononsphere, m_settings.perlinlayers[e].roughness, m_settings.perlinlayers[e].strength, m_settings.perlinlayers[e].octaves, m_settings.perlinlayers[e].persistance, m_settings.noisecentre);


            }

            elevation = Max(0, elevation-m_settings.minvalue);

            elevation = (elevation + 1) * m_settings.radius;

            glm::vec3 posonplanet = vertexunitpositiononsphere * elevation;

            m_minmax.AddValue(elevation);
            
            m_vertices[i] = Vertex(posonplanet.x, posonplanet.y, posonplanet.z, 0, 0, 0, 0, 0);

		}




	}


}

glm::vec3 PlanetFace::CubeToSphere(glm::vec3& cubepos)
{
    
    glm::vec3 finalposition = glm::vec3(0.f);

    finalposition.x = (cubepos.x * sqrt(1.f - (((cubepos.y * cubepos.y) + (cubepos.z * cubepos.z)) / 2.f) + ((cubepos.y * cubepos.y) * (cubepos.z * cubepos.z) / 3.f)));
    finalposition.y = (cubepos.y * sqrt(1.f - (((cubepos.x * cubepos.x) + (cubepos.z * cubepos.z)) / 2.f) + ((cubepos.x * cubepos.x) * (cubepos.z * cubepos.z) / 3.f)));
    finalposition.z = (cubepos.z * sqrt(1.f - (((cubepos.x * cubepos.x) + (cubepos.y * cubepos.y)) / 2.f) + ((cubepos.x * cubepos.x) * (cubepos.y * cubepos.y) / 3.f)));
    
    return finalposition;


}

float PlanetFace::Max(const float& firstval, const float& secondval)
{

    if (firstval > secondval)
    {
        return firstval;

    }
    else
    {
        return secondval;

    }


}


void PlanetFace::InitIndices()
{
    int arrpoint = m_resolution - 1;

    int numofindices = 6 * arrpoint * arrpoint;

    m_indices.resize(numofindices);

    int count = 0;
    int x = 0;


    for (int i = 0; i < m_indices.size(); i++)
    {



        m_indices[i] = count;
        m_indices[i + 1] = count + 1 + m_resolution;
        m_indices[i + 2] = count + m_resolution;


        m_indices[i + 3] = count;
        m_indices[i + 4] = count + 1;
        m_indices[i + 5] = count + 1 + m_resolution;
        i += 5;
        count++;
        x++;


        if (x == m_resolution - 1)
        {


            x = 0;
            count++;




        }






    }




}

void PlanetFace::InitNormals()
{
    

    std::vector<glm::vec3> normalcalc;
    normalcalc.resize(m_resolution * m_resolution);



    int count = 0;
    int x = 0;


    for (int i = 0; i < m_indices.size(); i++)
    {



        int ia = m_indices[i];
        int ib = m_indices[i + 2];
        int ic = m_indices[i + 1];

        glm::vec3 e1 = glm::vec3((m_vertices[ia].x), (m_vertices[ia].y), (m_vertices[ia].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        glm::vec3 e2 = glm::vec3((m_vertices[ic].x), (m_vertices[ic].y), (m_vertices[ic].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        glm::vec3 no = glm::cross(e1, e2);

        normalcalc[ia] += no;
        


        normalcalc[ib] += no;
       

        normalcalc[ic] += no;
        

        ia = m_indices[i + 3];
        ib = m_indices[i + 5];
        ic = m_indices[i + 4];


        e1 = glm::vec3((m_vertices[ia].x), (m_vertices[ia].y), (m_vertices[ia].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        e2 = glm::vec3((m_vertices[ic].x), (m_vertices[ic].y), (m_vertices[ic].z)) - glm::vec3((m_vertices[ib].x), (m_vertices[ib].y), (m_vertices[ib].z));
        no = glm::cross(e1, e2);

        normalcalc[ia]+= no;
       


        normalcalc[ib] += no;
     


        normalcalc[ic] += no;
       


        i += 5;


    }









    for (int i = 0; i < normalcalc.size(); i++)
    {


        normalcalc[i] = glm::normalize(normalcalc[i]);




        SetNormal(i, normalcalc[i]);



       /* m_vertices[i].n0 = normalcalc[i].x;
        m_vertices[i].n1 = normalcalc[i].y;
        m_vertices[i].n2 = normalcalc[i].z;*/





    }

}

void PlanetFace::SetNormal(const int& vertexindex, const glm::vec3& normal)
{

    m_vertices[vertexindex].n0 = normal.x;
    m_vertices[vertexindex].n1 = normal.y;
    m_vertices[vertexindex].n2 = normal.z;


}

void PlanetFace::InitMesh()
{

    InitVertices();
    InitIndices();
    InitNormals();

    m_layout.push<float>(3);
    m_layout.push<float>(2);
    m_layout.push<float>(3);


    m_vb.AddBuffer(m_vertices);
    
    m_vao.AddBuffer(m_vb, m_layout);

    m_ib.AddBuffer(m_indices);
    
}


void PlanetFace::SetPlanetSettings(PlanetSettings& settings)
{

    m_settings = settings;


}

void PlanetFace::UpdatePlanet()
{
    
    InitVertices();
    InitIndices();
    InitNormals();

    m_vb.AddBuffer(m_vertices);

    m_vao.AddBuffer(m_vb, m_layout);

}




