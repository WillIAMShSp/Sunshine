#include "BasePlanet.h"

BasePlanet::BasePlanet(const int& resolution, Shader& shader)
{
	m_resolution = resolution;
	m_shader = shader;
	//m_planetcolor = Texture("res/Textures/PlanetTexture.png");

}

void BasePlanet::SetFaces()
{
	



	for (int i = 0; i < 6; i++)
	{

		m_planetfaces[i] = PlanetFace(m_resolution, m_faceplanes[i]);
		m_planetfaces[i].InitMesh();

	}

}

void BasePlanet::SetPlanetSettings(PlanetSettings& settings)
{

	m_settings = settings;
	for (int i = 0; i < 6; i++)
	{
		m_planetfaces[i].SetPlanetSettings(m_settings);
	}

}

void BasePlanet::UpdateFaces()
{

	if (m_resetminmax)
	{
		ResetMinMax();

	}

	for (int i = 0; i < 6; i++)
	{
		m_planetfaces[i].UpdatePlanet();
				

	}

}

void BasePlanet::UpdateMinMax()
{

	

	for (int i = 0; i < 6; i++)
	{
		
		m_minmax.Add(m_planetfaces[i].GetMinMax());

	}
	m_shader.SetUniform1f("u_min", m_minmax.GetMin());
	m_shader.SetUniform1f("u_max", m_minmax.GetMax());

}

glm::vec3 avrg(glm::vec3& vec1, glm::vec3& vec2)
{
	glm::vec3 res;

	res.x = (vec1.x + vec2.x) / 2;
	res.y = (vec1.y + vec2.y) / 2;
	res.z = (vec1.z + vec2.z) / 2;

	return res;


}


void BasePlanet::SmoothenNormals()
{
	
	PlanetFace& frontface = m_planetfaces[4];
	PlanetFace& rightface = m_planetfaces[2];
	PlanetFace& leftface = m_planetfaces[3];
	PlanetFace& upface = m_planetfaces[0];
	PlanetFace& downface = m_planetfaces[1];
	PlanetFace& backface = m_planetfaces[5];





	SmoothHorizontal(frontface, rightface, leftface, backface);
	SmoothNorthPole(upface, frontface, rightface, leftface, backface);
	SmoothSouthPole(downface,frontface, rightface, leftface, backface);
	
	//SmoothFrontFace(frontface, rightface, leftface, upface, downface);	
	//SmoothRightFace(rightface, backface, frontface, upface, downface);





	for (int i = 0; i < 6; i++)
	{
		
		VertexBuffer& vb = m_planetfaces[i].m_vb;
		vb.AddBuffer(m_planetfaces[i].GetVertices());
		m_planetfaces[i].m_vao.AddBuffer(vb, m_planetfaces[i].GetLayout());

	}
	
}

void BasePlanet::Draw()
{

	m_shader.Bind();
	//m_planetcolor.Bind();
	//glActiveTexture(GL_TEXTURE0);
	
	for(int i = 0; i < 6; i++)
	{

		int size = m_planetfaces[i].GetIndexSize();
	
		m_planetfaces[i].m_vao.Bind();
		m_planetfaces[i].m_ib.Bind();

		//m_shader.SetUniform3f("Mat.Color", colors[i].x, colors[i].y, colors[i].z);
		//m_shader.SetUniform3f("Mat.DiffuseColor", colors[i].x, colors[i].y, colors[i].z);

		//m_shader.SetUniform3f("Mat.Color", 1, 0, 0);
		//m_shader.SetUniform3f("Mat.DiffuseColor", 1, 0, 0);


		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		m_planetfaces[i].m_ib.UnBind();
		m_planetfaces[i].m_vao.UnBind();


	}
	//m_planetcolor.UnBind();
	m_shader.UnBind();

}

void BasePlanet::DrawDepth(Shader& depthshader)
{
	depthshader.Bind();
	
	for (int i = 0; i < 6; i++)
	{
		int size = m_planetfaces[i].GetIndexSize();

		m_planetfaces[i].m_vao.Bind();
		m_planetfaces[i].m_ib.Bind();


		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		m_planetfaces[i].m_ib.UnBind();
		m_planetfaces[i].m_vao.UnBind();

	}



}



void BasePlanet::ResetMinMax()
{
	m_minmax.Reset();
	for (int i = 0; i < 6; i++)
	{
		m_planetfaces[i].GetMinMax().Reset();
	}
	m_resetminmax = false;


}



void BasePlanet::SmoothHorizontal(PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface)
{

	SmoothToTheRight(frontface, rightface);
	SmoothToTheRight(rightface, backface);
	SmoothToTheRight(backface, leftface);
	SmoothToTheRight(leftface, frontface);
}

void BasePlanet::SmoothToTheRight(PlanetFace& selectedface, PlanetFace& rightface)
{


	for (int i = 0; i < selectedface.GetResolution(); i++)
	{


		//change when implementing LOD;
		int t = m_resolution - 1 + (i * m_resolution); // this takes the right border of vertices for the selected planet face
		int r = i * m_resolution; // this takes the left border of vertices for the planet face right of the selected face

		glm::vec3 normalthis = glm::vec3(selectedface.GetVertices()[t].n0, selectedface.GetVertices()[t].n1, selectedface.GetVertices()[t].n2);
		glm::vec3 normalright = glm::vec3(rightface.GetVertices()[r].n0, rightface.GetVertices()[r].n1, rightface.GetVertices()[r].n2);



		glm::vec3 result = avrg(normalthis, normalright);




		result = glm::normalize(result);




		selectedface.SetNormal(t, result);
		rightface.SetNormal(r, result);
	}



}

void BasePlanet::SmoothNorthPole(PlanetFace& selectedface, PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface)
{
	for (int i = 0; i < selectedface.GetResolution(); i++)
	{


		int uf = i;
		int t = i + ((m_resolution - 1) * m_resolution); //this is the updirection for all the side faces. 

		glm::vec3 normalfront = glm::vec3(frontface.GetVertices()[t].n0, frontface.GetVertices()[t].n1, frontface.GetVertices()[t].n2);
		glm::vec3 normalup = glm::vec3(selectedface.GetVertices()[uf].n0, selectedface.GetVertices()[uf].n1, selectedface.GetVertices()[uf].n2);

		glm::vec3 result = avrg(normalfront, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(uf, result);
		frontface.SetNormal(t, result);

		
		////////////////////////
		uf = m_resolution - 1 + (i * m_resolution);
		
		//t = m_resolution * m_resolution - 1 - i;

		glm::vec3 normalright = glm::vec3(rightface.GetVertices()[t].n0, rightface.GetVertices()[t].n1, rightface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[uf].n0, selectedface.GetVertices()[uf].n1, selectedface.GetVertices()[uf].n2);

		result = avrg(normalright, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(uf, result);
		
		rightface.SetNormal(t, result);

		/////////////////////////////

		uf = i + ((m_resolution - 1) * m_resolution);

		t = m_resolution * m_resolution - 1 - i;

		glm::vec3 normalback = glm::vec3(backface.GetVertices()[t].n0, backface.GetVertices()[t].n1, backface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[uf].n0, selectedface.GetVertices()[uf].n1, selectedface.GetVertices()[uf].n2);

		result = avrg(normalback, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(uf, result);

		backface.SetNormal(t, result);
		//////////////////////////////////
		
		uf = i * m_resolution;

		glm::vec3 normalleft = glm::vec3(leftface.GetVertices()[t].n0, leftface.GetVertices()[t].n1, leftface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[uf].n0, selectedface.GetVertices()[uf].n1, selectedface.GetVertices()[uf].n2);

		result = avrg(normalleft, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(uf, result);

		leftface.SetNormal(t, result);










	}






}

void BasePlanet::SmoothSouthPole(PlanetFace& selectedface, PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface)
{
	for (int i = 0; i < selectedface.GetResolution(); i++)
	{


		int d = m_resolution - 1 - i;
		int t = i; //this is the updirection for all the side faces. 

		glm::vec3 normalfront = glm::vec3(frontface.GetVertices()[t].n0, frontface.GetVertices()[t].n1, frontface.GetVertices()[t].n2);
		glm::vec3 normalup = glm::vec3(selectedface.GetVertices()[d].n0, selectedface.GetVertices()[d].n1, selectedface.GetVertices()[d].n2);

		glm::vec3 result = avrg(normalfront, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(d, result);
		frontface.SetNormal(t, result);


		////////////////////////
		d = i * m_resolution;

		//t = m_resolution * m_resolution - 1 - i;

		glm::vec3 normalright = glm::vec3(rightface.GetVertices()[t].n0, rightface.GetVertices()[t].n1, rightface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[d].n0, selectedface.GetVertices()[d].n1, selectedface.GetVertices()[d].n2);

		result = avrg(normalright, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(d, result);

		rightface.SetNormal(t, result);

		/////////////////////////////

		d = i + ((m_resolution - 1) * m_resolution);

		

		glm::vec3 normalback = glm::vec3(backface.GetVertices()[t].n0, backface.GetVertices()[t].n1, backface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[d].n0, selectedface.GetVertices()[d].n1, selectedface.GetVertices()[d].n2);

		result = avrg(normalback, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(d, result);

		backface.SetNormal(t, result);
		////////////////////////////////////

		d = m_resolution * m_resolution -1 - (i * m_resolution);

		glm::vec3 normalleft = glm::vec3(leftface.GetVertices()[t].n0, leftface.GetVertices()[t].n1, leftface.GetVertices()[t].n2);
		normalup = glm::vec3(selectedface.GetVertices()[d].n0, selectedface.GetVertices()[d].n1, selectedface.GetVertices()[d].n2);

		result = avrg(normalleft, normalup);

		result = glm::normalize(result);

		selectedface.SetNormal(d, result);

		leftface.SetNormal(t, result);










	}
}
