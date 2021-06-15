#pragma once

#include <iostream>

class Player
{
public:
	Player() : m_name(""), m_weight(0.0), m_mapa(0), m_weather(), m_wind(0) {}
	~Player();

	void set_Name(std::string name);
	void set_Weight(double weight);
	void set_Maps(int mapa);
	void set_Weather(int weather);
	void set_Wind(int wind);

	std::string get_Name() const;
	double get_Weight() const;
	int get_Maps() const;
	int get_Weather() const;
	int get_Wind() const;

private:
	std::string  m_name;	// imie gracza

	double m_weight;		// S=50, M=60, L-70
	int    m_mapa;			// domyœlnie ta jedyna
	int    m_weather;		// s³oñce, deszcz, œnieg, wiatr
	int	   m_wind;			// wiatr :  stable - 0, fickle - 1,
};

