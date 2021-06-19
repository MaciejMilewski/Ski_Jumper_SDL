#include "Player.h"

void Player::set_Name(std::string name)
{
	m_name = name;
}
void Player::set_Weight(double weight)
{
	m_weight = weight;
}
void Player::set_Maps(int mapa)
{
	m_mapa = mapa;
}
void Player::set_Weather(int weather)
{
	m_weather = weather;
}
void Player::set_Wind(int wind)
{
	m_wind = wind;
}
std::string Player::get_Name() const
{
	return m_name;
}
void Player::set_Score(int score) 
{
	m_score = score;
}
double Player::get_Weight() const
{
	return m_weight;
}
int Player::get_Maps() const
{
	return m_mapa;
}
int Player::get_Weather() const
{
	return m_weather;
}
int Player::get_Wind() const
{
	return m_wind;
}
int Player::get_Score() const
{
	return m_score;
}
Player::~Player()
{

}