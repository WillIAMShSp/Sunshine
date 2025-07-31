#pragma once
#include "Texture.h"
#include "FullScreenQuad.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include <math.h>



struct AtmosphereSettings
{

	AtmosphereSettings() = default;

	AtmosphereSettings(const float& atmradius, const uint16_t& inscattpoints, const uint16_t& optdepthpoints, const float& densityfalloff, const float& sclheight, const float& scatterstrength = 1.f, const glm::vec3& wavelength = glm::vec3(700.f, 530.f, 440.f))
		: atmosphereradius(atmradius), inscatterpoints(inscattpoints), outscatterpoints(optdepthpoints), densityfalloff(densityfalloff), wavelengths(wavelength), scaleheight(sclheight)
	{
		invcolorwavelength.x = glm::pow(1 / wavelength.x, 4) * scatterstrength;
		invcolorwavelength.y = glm::pow(1 / wavelength.y, 4) * scatterstrength;
		invcolorwavelength.z = glm::pow(1 / wavelength.z, 4) * scatterstrength;

	}

	void SetSettings(const float& atmradius, const uint16_t& inscattpoints, const uint16_t& optdepthpoints, const float& denfall, const float& sclheight, const float& scatterstrength = 1.f, const glm::vec3& wavelength = glm::vec3(700.f, 530.f, 440.f))
	{
		atmosphereradius = atmradius;
		inscatterpoints = inscattpoints;
		outscatterpoints = optdepthpoints; 
		densityfalloff = denfall;
		scaleheight = sclheight;
		wavelengths = wavelength;


		invcolorwavelength.x = glm::pow(1 / wavelength.x, 4) * scatterstrength;
		invcolorwavelength.y = glm::pow(1 / wavelength.y, 4) * scatterstrength;
		invcolorwavelength.z = glm::pow(1 / wavelength.z, 4) * scatterstrength;


	}

	float atmosphereradius = 0;
	
	unsigned int inscatterpoints = 0;
	uint16_t outscatterpoints = 0;
	
	float densityfalloff = 0;
	
	float scaleheight = 0.25;


	glm::vec3 wavelengths = glm::vec3(1.f);
	glm::vec3 invcolorwavelength = glm::vec3(1.f);



	




};




class AtmospherePostProc
{

public:

	AtmospherePostProc();
	

	void SetScreenTexture(const Texture& screenbuffer);
	
	void SetDepthBuffer(const Texture& depthbuffer);

	void SetSettings(const AtmosphereSettings& newsettings);

	void SetCameraUniforms(Camera& camera);

	void Draw();

private:

	void UpdateUniforms();


	

	Texture m_originalcolor; // this is the original buffer for the screen;
	Texture m_depthbuffer; // this is the camera depth buffer;

	Shader m_shader; //this SHOULD be the shader used to render raytraced sphere and hold all of our calculations;

	AtmosphereSettings m_settings; //this are the atmosphere settings that will be used in the shader;

	FullScreenQuad quad;

};