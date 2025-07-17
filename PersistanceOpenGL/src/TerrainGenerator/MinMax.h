#pragma once


struct MinMax
{
public:

	MinMax() = default;

	void AddValue(float val)
	{
		if (val < m_minimum)
		{
			m_minimum = val;
		}

		if (val > m_maximum)
		{
			m_maximum = val;
		}
	};

	void Add(MinMax& minmax)
	{
		
		AddValue(minmax.GetMin());
		AddValue(minmax.GetMax());

	}

	void Reset()
	{
		m_minimum = 1000.f;
		m_maximum = -1.f;


	}


	const float& GetMin()
	{

		return m_minimum;

	}

	const float& GetMax()
	{

		return m_maximum;

	}


private:

	float m_minimum = 1000.f;
	float m_maximum = -1.f;


};