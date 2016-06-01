#include <stdio.h>
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
#include "Annexes.h"
using namespace std;
#ifndef __DOVES__
#define __DOVES__

int IntroScreen(sf::RenderWindow *window, string &nameHS);
int Game(sf::RenderWindow *window, sf::Music *music, string nameHS);

#endif