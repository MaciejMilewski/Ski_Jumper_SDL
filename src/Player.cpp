#include "Player.h"

void Player::set_Name(std::string name)
{
	m_name = name;
}
void Player::set_Weight(double weight)
{
	m_weight = weight;
}
void Player::set_Mapa(int mapa)
{
	m_mapa = mapa;
}
void Player::set_Weather(int weather)
{
	m_weather = weather;
}
void Player::set_Wind(double wind)
{
	m_wind = wind;
}
std::string Player::get_Name() const
{
	return m_name;
}
double Player::get_Weight() const
{
	return m_weight;
}
int Player::get_Mapa() const
{
	return m_mapa;
}
int Player::get_Weather() const
{
	return m_weather;
}
double Player::get_Wind() const
{
	return m_wind;
}
Player::~Player()
{

}