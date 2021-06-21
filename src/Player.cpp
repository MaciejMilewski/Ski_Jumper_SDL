#include "Player.h"

void Player::setName(std::string name)
{
	m_name = name;
}
void Player::setWeight(double weight)
{
	m_weight = weight;
}
void Player::setMaps(int mapa)
{
	m_mapa = mapa;
}
void Player::setWeather(int weather)
{
	m_weather = weather;
}
void Player::setWind(int wind)
{
	m_wind = wind;
}
std::string Player::getName() const
{
	return m_name;
}
void Player::setScore(int score) 
{
	m_score = score;
}
double Player::getWeight() const
{
	return m_weight;
}
int Player::getMaps() const
{
	return m_mapa;
}
int Player::getWeather() const
{
	return m_weather;
}
int Player::getWind() const
{
	return m_wind;
}
int Player::getScore() const
{
	return m_score;
}
Player::~Player()
{

}