#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "glm/glm.hpp"



class Array2D
{

public:
	Array2D()
	{


	};

	void SetSize(int x, int y);
	
	float GetPoint(const int& x, const int& z);
	float GetPoint(const int& i);

	void SetPoint(int x, int z, float value);
	void SetPoint(int i, float value);


	void LoadFromImage(const char* filepath);


	inline int GetXSize()
	{
		return m_xsize;

	}

	inline int GetYSize()
	{
		return m_ysize;
		
	}

	inline int GetArrayDimensions()
	{
		return m_xsize * m_ysize;
	}
	
private:


	std::vector<float> m_values;

	int m_xsize = 0;
	int m_ysize = 0;




};


