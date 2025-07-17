#include "PerlinNoise.h"

void PerlinNoise::ApplyPerlinNoiseToArray2D(Array2D& lattice, const float& frequency, const float& amplitude, const int& octaves, const float& persistence)
{

    std::vector<float> values;

    int count = 0;
    for (int y = 0; y < lattice.GetYSize(); ++y)
    {
        for (int x = 0; x < lattice.GetXSize(); ++x)
        {
            const double noise = perlin.octave2D_01((x * frequency), (y * frequency), octaves, persistence) * amplitude;
            float noi = (float)noise;
            values.push_back(noi);
         
        }

        
    }


    for (int i = 0; i < values.size(); i++)
    {
        lattice.SetPoint(i, values[i]);

    }




}

glm::vec3 PerlinNoise::Apply3DPerlinNoiseToVec3(glm::vec3& vec3, const float& frequency, const float& amplitude, const int& octaves, const float& persistence)
{

    float elevation = perlin.octave3D_01(vec3.x * frequency, vec3.y * frequency, vec3.z * frequency, octaves, persistence) * amplitude;


    glm::vec3 pointonplanet = vec3 * (1 + elevation);
    
    return pointonplanet;

}

float PerlinNoise::PerlinNoise3D(glm::vec3& vec3, const float& frequency, const float& amplitude, const int& octaves, const float& persistence, const glm::vec3& centre)
{
    float elevation = perlin.octave3D_01(vec3.x * frequency + centre.x, vec3.y * frequency + centre.y, vec3.z * frequency + centre.z, octaves, persistence) * amplitude;

    return elevation;
}

