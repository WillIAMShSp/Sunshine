#include "AtmospherePostProc.h"

AtmospherePostProc::AtmospherePostProc()
{
	const char* vertexshader = "res/Shaders/Screenvs.shader";
	const char* fragmentshader = "res/Shaders/Atmosphere/RaySphere.shader";

	m_shader = Shader(vertexshader, fragmentshader);

	m_shader.SetUniform1i("u_screentexture", 0);
	m_shader.SetUniform1i("u_depthtexture", 1);

}

void AtmospherePostProc::SetScreenTexture(const Texture& screenbuffer)
{
	m_originalcolor = screenbuffer;
}

void AtmospherePostProc::SetDepthBuffer(const Texture& depthbuffer)
{
	m_depthbuffer = depthbuffer;
}

void AtmospherePostProc::SetSettings(const AtmosphereSettings& newsettings)
{
	m_settings = newsettings;
}

void AtmospherePostProc::SetCameraUniforms(Camera& camera)
{

	

	m_shader.SetUniform3f("u_pos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	m_shader.SetUniformMat4fv("u_inverseview", camera.GetInverseViewMatrix());
	m_shader.SetUniformMat4fv("u_inverseproj", camera.GetInverseProjectionMatrix());

}

void AtmospherePostProc::Draw()
{

	m_shader.Bind();
	UpdateUniforms();
	m_originalcolor.Bind(0);
	m_depthbuffer.Bind(1);


	quad.Draw();

	m_originalcolor.UnBind();
	m_depthbuffer.UnBind();


}

void AtmospherePostProc::UpdateUniforms()
{


	m_shader.SetUniform3f("u_scatteringcoefficients", m_settings.invcolorwavelength);
	m_shader.SetUniform3f("u_wavelengths", m_settings.wavelengths);


	m_shader.SetUniform1f("u_atmosphereradius", m_settings.atmosphereradius);
	//m_shader.SetUniform1f("u_densityfalloff", m_settings.densityfalloff);

	m_shader.SetUniform1i("u_inscatterpoints", m_settings.inscatterpoints);
	m_shader.SetUniform1i("u_opticaldepthpoints", m_settings.outscatterpoints);

}
