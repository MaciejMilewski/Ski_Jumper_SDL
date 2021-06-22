#pragma once

#include <iostream>

class Player
{
public:
	Player() : m_name(""), m_weight(0.0), m_mapa(0), m_weather(), m_wind(0), m_score(0) {}
	~Player();

	void setName(std::string name);
	void setWeight(double weight);
	void setMaps(int mapa);
	void setWeather(int weather);
	void setWind(int wind);
	void setScore(int score);

	std::string getName() const;
	double getWeight() const;
	int getMaps() const;
	int getWeather() const;
	int getWind() const;
	int getScore() const;

private:
	std::string  m_name;	

	double m_weight;		// S=50, M=60, L-70
	int    m_mapa;			
	int    m_weather;		// slonce, deszcz, snieg, wiatr
	int	   m_wind;			// wiatr :  stable - 0, fickle - 1,
	int    m_score;
};

