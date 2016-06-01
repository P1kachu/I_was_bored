#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <vector>
#ifndef _ANNEXES_H_
#define _ANNEXES_H_
using namespace std;

void Init(sf::RenderWindow *window);
int GetHighScore(string path, string &nameHS);
bool SaveScore(string path, int score, string &nameHS);
void Animate(int &frame, int& timer, bool &animation);
bool MoveSprite(float &rot, float &y, float speed);
void MoveBackground(float *xBG,
                    sf::Sprite *bg1,
                    sf::Sprite *bg2,
                    sf::Texture *text,
                    float rot,
                    float speed);
void UpdateCoins(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *coins,
                 float rot,
                 float sinus,
                 float backgroundW,
                 float backgroundH,
                 float limit,
                 float playerPos);
void UpdateObstacles(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *obstacles,
                 float sinus,
                 float backgroundW,
                 float backgroundH ,
                 float limit,
                 float playerPos,
                 float speed);
void UpdatePowerUps(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *powerUps,
                 float sinus,
                 float backgroundW,
                 float backgroundH ,
                 float limit,
                 float playerPos,
                 float speed);
float AngleHelper(float angle);
#endif
