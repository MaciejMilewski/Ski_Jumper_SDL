#pragma once

#include <iostream>

class Player
{
public:
	Player(): m_name(), m_weight(0.0), m_mapa(0), m_weather(), m_wind(0.0) {}
	~Player();

	void set_Name(std::string name);
	void set_Weight(double weight);
	void set_Mapa(int mapa);
	void set_Weather(int weather);
	void set_Wind(double wind);

   std::string get_Name() const;
  double get_Weight() const;
     int get_Mapa() const;
     int get_Weather() const;
  double get_Wind() const;
  
private:
	std::string  m_name;

	double m_weight;		// S M L
	int    m_mapa;			// domyœlnie ta jedyna
	int    m_weather;		// s³oñce, deszcz, œnieg, wiatr
	double m_wind;			// wiatr : fickle, stable
};

