#include "Array2D.h"

void Array2D::SetSize(int x, int y)
{
	m_values.resize(x * y);
	m_xsize = x;
	m_ysize = y;
}

float Array2D::GetPoint(const int& x, const int& z)
{
	return m_values[x + (x*z)];
}

float Array2D::GetPoint(const int& i)
{
	return m_values[i];
}

void Array2D::SetPoint(int x, int z, float value)
{


	m_values[x + (x*z)] = value;


}

void Array2D::SetPoint(int i, float value)
{
	m_values[i] = value;

}

void Array2D::LoadFromImage(const char* filepath)
{




}







