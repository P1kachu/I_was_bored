#include "Annexes.h"

int GetHighScore(string path, string &nameHS)
{
    int r;
    string tmp;
    ifstream read;
    read.open(path.c_str());
    getline(read, tmp);
    getline(read, nameHS);
    r = atoi(tmp.c_str());
    read.close();
    return r;
}

bool SaveScore(string path, int score, string &nameHS)
{
    cout << "Score: " << score << endl;
    string idk;
    int hs = GetHighScore(path, idk);
    if(nameHS == "")
        nameHS = "null";
    if(score > hs)
    {
        cout << "New Highscore !" << endl;
        ofstream write;
        write.open(path.c_str());
        write << score << endl << nameHS;
        write.close();
        return true;
    }
    return false;
}

void MoveBackground(float *xBG,
                    sf::Sprite *bg1,
                    sf::Sprite *bg2,
                    sf::Texture *text,
                    float rot,
                    float speed)
{
    int tmp = (rot <= 37 || rot >= 360- 37) ? 0 : 1;
    *xBG = *xBG - (3*speed) + tmp;
    bg1->setPosition(*xBG, bg1->getPosition().y);
    float tmp2 = bg1->getPosition().x + text->getSize().x - 1;
    bg2->setPosition(tmp2, bg2->getPosition().y);
    if(bg2->getPosition().x <= 0)
    {
        *xBG = 0;
        bg1->setPosition(*xBG, bg1->getPosition().y);
    }
}

bool MoveSprite(float &rot, float &y, float speed)
{
    bool hasMoved = false;
    y = 0;
    float acc = 0;
    rot+=(1*speed);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        hasMoved = true;
        acc = -3;

    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        hasMoved = true;
        acc = 3;
    }

    rot += (acc*speed);
    rot = AngleHelper(rot);
    y = 5*(rot/60.0f)*speed;

    return hasMoved;
}

void Animate(int &frame, int& timer, bool &animation)
{
    if (timer%10 == 0)
    {
        if (animation)
        {
            frame++;
            if (frame > 3)
            {
                frame = 2;
                animation = false;
            }
        }
        else
        {
            frame--;
            if (frame < 0)
            {
                frame = 1;
                animation = true;
            }

        }
    }
}

void Init(sf::RenderWindow *window)
{
    window->setFramerateLimit(60);
    window->setPosition(sf::Vector2i(0, 0));
}

void UpdateCoins(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *coins,
                 float rot,
                 float sinus,
                 float backgroundW,
                 float backgroundH ,
                 float limit,
                 float playerPos)
{
    int tmp = (rot <= 37 || rot >= 360- 37) ? 0 : 1;
    for (int i = 0; i < 3; ++i)
    {
        int tmp2 = (i%2 == 0) ? 1 : -1;
        (*pos)[i] = sf::Vector2f((*pos)[i].x - 3 + tmp, (*pos)[i].y + sin(sinus) * tmp2);
        (*coins)[i].setPosition((*pos)[i]);
        if((*coins)[i].getPosition().x < limit)
        {

            (*pos)[i] = sf::Vector2f((*pos)[i].x + backgroundW + playerPos + rand()%200,
                                     rand()%((int)backgroundH/2) + 15);
            (*coins)[i].setPosition((*pos)[i]);
        }

    }
}

void UpdateObstacles(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *obstacles,
                 float sinus,
                 float backgroundW,
                 float backgroundH ,
                 float limit,
                 float playerPos,
                 float speed)
{
    for (int i = 0; i < 3; ++i)
    {
        int tmp = (i%2 == 0) ? 1 : -1;
        (*pos)[i] = sf::Vector2f((*pos)[i].x - 5 * speed, (*pos)[i].y + 2*sin(sinus) * tmp);
        (*obstacles)[i].setPosition((*pos)[i]);
        if((*obstacles)[i].getPosition().x < limit)
        {

            (*pos)[i] = sf::Vector2f((*pos)[i].x + backgroundW*2 + playerPos + rand()%200,
                                     rand()%((int)backgroundH/2) + 15);
            (*obstacles)[i].setPosition((*pos)[i]);
        }

    }
}

void UpdatePowerUps(vector<sf::Vector2f> *pos,
                 vector<sf::Sprite> *powerUps,
                 float sinus,
                 float backgroundW,
                 float backgroundH ,
                 float limit,
                 float playerPos,
                 float speed)
{
    for (int i = 0; i < 3; ++i)
    {
        int tmp = (i%2 == 0) ? 1 : -1;
        (*pos)[i] = sf::Vector2f((*pos)[i].x - 5 * speed, (*pos)[i].y + 2*sin(sinus) * tmp);
        (*powerUps)[i].setPosition((*pos)[i]);
        if((*powerUps)[i].getPosition().x < limit)
        {

            (*pos)[i] = sf::Vector2f((*pos)[i].x + backgroundW*6 + playerPos + rand()%200,
                                     rand()%((int)backgroundH/2) + 15);
            (*powerUps)[i].setPosition((*pos)[i]);
        }

    }
}

float AngleHelper(float angle)
{
    int r = angle;
    if(r >= 180)
        r = -(360 - angle);
    if(r >= 60)
        return 60;
    if(r <= -60)
        return -60;
    return r;
}
