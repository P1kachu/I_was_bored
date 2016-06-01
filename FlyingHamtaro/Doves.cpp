#include "Doves.h"

int IntroScreen(sf::RenderWindow *window, string &nameHS)
{
    sf::Texture introBG, flyingHam;
    sf::Sprite bg, flying;
    sf::Font font;
    sf::Text text, highscore, name;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    string bestName;
    bool flyBool = 0, transition = 0, buttonReverse = 0, flyingAppears = 1,
         nameEntered = 0;
    float floatingFly = 0, textTrsprcy = 0;
    int highScore = GetHighScore("res/highscore.txt", bestName);

    if (!introBG.loadFromFile("res/introBG.png") || !flyingHam.loadFromFile("res/flying.png"))
        cout << "\033[31;1m intro image didn't load\033[0m" << endl;
    if (!font.loadFromFile("res/font.ttf"))
        cout << "\033[31;1m font didn't load\033[0m" << endl;
    if (!buffer.loadFromFile("res/play.ogg"))
        cout << "\033[31;1m play.ogg didn't load\033[0m" << endl;

    sound.setBuffer(buffer);



    bg.setTexture(introBG);
    flying.setTexture(flyingHam);

    float spriteX = (float)window->getSize().x/(float)introBG.getSize().x;
    float spriteY = (float)window->getSize().y/(float)introBG.getSize().y;
    bg.setScale(spriteX, spriteY);
    bg.setColor(sf::Color(255, 255, 255, 0));
    flying.setPosition((float)window->getSize().x*2/3, -(float)window->getSize().y);
    flying.setScale(0.50f, 0.50f);



    highscore.setFont(font);
    stringstream ss;
    ss << "highscore  " << bestName << " - " << highScore;
    highscore.setString(ss.str());
    highscore.setCharacterSize(13);
    highscore.setColor(sf::Color(255,80,0,255));
    highscore.setPosition((float)window->getSize().x*2/3, (float)window->getSize().y*6/7);

    name.setFont(font);
    name.setString("enter your name");
    name.setCharacterSize(26);
    name.setColor(sf::Color(255,160,0,textTrsprcy));
    name.setPosition((float)window->getSize().x/6, (float)window->getSize().y/2);
    stringstream nameStream;

    while (true)
    {
        if(!nameEntered)
        {
            sf::Event nameEvent;
            while(window->pollEvent(nameEvent))
            {
                if (nameEvent.type == sf::Event::TextEntered)
                {
                    if(nameEvent.text.unicode == 13)
                    {
                        nameEntered = true;
                        nameHS = nameStream.str();
                        transition = true ;
                        sound.play();
                    }
                    if(nameEvent.text.unicode == 8)
                    {
                        nameStream.str("");
                        name.setString(nameStream.str());
                    }
                    if (nameEvent.text.unicode >= 97 && nameEvent.text.unicode <= 122)
                    {
                        nameStream << static_cast<char>(nameEvent.text.unicode);
                        name.setString(nameStream.str());
                    }
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            return -1;

        if(!transition)
        {
            if(!buttonReverse)
            {
                if(textTrsprcy > 1)
                    textTrsprcy-=5;
                else
                    buttonReverse = !buttonReverse;
            }
            if(buttonReverse)
            {
                if(textTrsprcy < 254)
                    textTrsprcy+=5;
                else
                    buttonReverse = !buttonReverse;
            }
            name.setColor(sf::Color(255,105,0,textTrsprcy));
        }

        if(bg.getColor().a < 250 && !transition)
            bg.setColor(sf::Color(255, 255, 255, bg.getColor().a + 5));
        else
            flyBool = true;


        if(transition)
        {
            if(bg.getColor().a > 5)
            {
                bg.setColor(sf::Color(255, 255, 255, bg.getColor().a - 3));
                flying.setColor(sf::Color(255, 255, 255, bg.getColor().a - 3));
                if(text.getColor().a > 5)
                    text.setColor(sf::Color(text.getColor().r, text.getColor().g, text.getColor().b, bg.getColor().a - 3));
            }
            else
                return 1;
        }


        window->clear();
        window->draw(bg);
        window->draw(text);
        if(!nameEntered)
            window->draw(name);
        if(!transition && bg.getColor().a > 200)
            window->draw(highscore);
        if(flyBool)
        {
            if(flying.getPosition().y < 7 * sin(floatingFly) && flyingAppears)
                flying.setPosition(flying.getPosition().x, flying.getPosition().y + 3);
            else
                flyingAppears = false;

            if(!flyingAppears)
            {
                if(floatingFly < 6.28f)
                    floatingFly+=0.05f;
                else
                    floatingFly = 0;
                flying.setPosition(flying.getPosition().x, 7 * sin(floatingFly));
            }
            window->draw(flying);
        }
        window->display();
    }

    return 0;
}

int Game(sf::RenderWindow *window, sf::Music *music, string nameHS)
{
    int score = 0,
        frame = 0,
        timer = 0,
        pandaFrame =0,
        pandaTimer = 0,
        powerUpTimer = 360;

    float xBG = 0,
          floatingFly = 0,
          speed = 1.2f;

    float rot = 0,
          y = 0;

    float backgroundW,
          backgroundH,
          viewSizeY,
          viewSizeX,
          viewCenterX,
          viewCenterY;

    float playerPosX = 0,
          playerPosY = 0;

    bool hasMoved = 0,
         animation = 0,
         endOfGame = 0,
         tmpHighScore = 0;

    vector<sf::Vector2f> position(4, sf::Vector2f(0,0));
    vector<sf::Sprite> coins(4);
    vector<sf::Vector2f> positionObs(4, sf::Vector2f(0,0));
    vector<sf::Sprite> obstacles(4);
    vector<sf::Vector2f> positionUp(4, sf::Vector2f(0,0));
    vector<sf::Sprite> powerUps(4);

    sf::Texture background,
                hamtaro,
                seedB,
                seedB2,
                gameOverTexture,
                newHighScore,
                bubble,
                obstacleText,
                powerText;

    sf::Sprite bg,
               bg2,
               player,
               gameOver,
               highScoreSprite,
               bubblePanda,
               powerUp;

    sf::FloatRect hamBox,
                  groundBox,
                  ceilingBox,
                  seedBox,
                  obstacleBox,
                  powerBox;

    sf::Text scoreText;
    sf::Font font;
    sf::SoundBuffer buffer, buffer2, beeS, pandaBuffer;
    sf::Sound lostSound, balloon, beeSound, pandaSound;

    if (!background.loadFromFile("res/bg.png") ||
        !hamtaro.loadFromFile("res/player.png") ||
        !seedB.loadFromFile("res/seed.png") ||
        !seedB2.loadFromFile("res/seed2.png") ||
        !gameOverTexture.loadFromFile("res/gameOver.png") ||
        !newHighScore.loadFromFile("res/hs.png") ||
        !bubble.loadFromFile("res/bubbleHS.png") ||
        !obstacleText.loadFromFile("res/obstacle2.png") ||
        !powerText.loadFromFile("res/powerup.png"))
        cout << "\033[31;1m game image didn't load\033[0m" << endl;
    if (!font.loadFromFile("res/font.ttf"))
        cout << "\033[31;1m font didn't load\033[0m" << endl;
    if (!buffer.loadFromFile("res/lostSound.ogg") ||
        !buffer2.loadFromFile("res/balloon.ogg") ||
        !beeS.loadFromFile("res/beeSound.ogg") ||
        !pandaBuffer.loadFromFile("res/pandaSound.wav"))
        cout << "\033[31;1m Sound didn't load\033[0m" << endl;

    lostSound.setBuffer(buffer);
    balloon.setBuffer(buffer2);
    beeSound.setBuffer(beeS);
    pandaSound.setBuffer(pandaBuffer);
    bg.setTexture(background);
    bg2.setTexture(background);
    player.setTexture(hamtaro);
    gameOver.setTexture(gameOverTexture);
    highScoreSprite.setTexture(newHighScore);
    bubblePanda.setTexture(bubble);

    backgroundW = background.getSize().x;
    backgroundH = background.getSize().y;
    sf::View view(sf::Vector2f(backgroundW*2/3,
                               backgroundH*2/3),
                  sf::Vector2f(backgroundW*2/3,
                               backgroundH*2/3));
    viewSizeX = view.getSize().x;
    viewSizeY = view.getSize().y;
    bg.setPosition(0, 0);
    player.setOrigin(18,24);
    player.setScale(1.3f,1.3f);
    sf::RectangleShape ground(sf::Vector2f(backgroundW, backgroundH/5));
    ground.setFillColor(sf::Color(255,0,0,255));
    ground.setPosition(0, backgroundH*4/5);
    sf::RectangleShape ceiling(sf::Vector2f(backgroundW, 5));
    ceiling.setFillColor(sf::Color(255,0,0,255));
    ceiling.setPosition(0, 0);

    for (int i = 0; i < 3; ++i)
    {
        if(i%2==0)
            coins[i].setTexture(seedB);
        else
            coins[i].setTexture(seedB2);
        position[i] = sf::Vector2f(rand()%100 + backgroundW/2*(i + 1),
                                rand()%((int)backgroundH/2) + 15);
        coins[i].setPosition(position[i]);
        coins[i].setScale(.8f,.8f);

        positionObs[i] = sf::Vector2f(rand()%100 + backgroundW*2*(i + 1),
                                rand()%((int)backgroundH/2) + 15);
        obstacles[i].setPosition(positionObs[i]);
        obstacles[i].setTexture(obstacleText);

        positionUp[i] = sf::Vector2f(rand()%500 + backgroundW*8*(i + 1),
                                rand()%((int)backgroundH/2) + 15);
        powerUps[i].setPosition(positionUp[i]);
        powerUps[i].setTexture(powerText);
        powerUps[i].setScale(0.5f,0.5f);
    }
    player.setPosition(backgroundW/2, backgroundH/2);

    view.setCenter(playerPosX, playerPosY);


    scoreText.setFont(font);
    scoreText.setCharacterSize(15);
    scoreText.setColor(sf::Color(255,80,0,255));



    while(window->isOpen())
    {
        playerPosX = player.getPosition().x;
        playerPosY = player.getPosition().y;
        viewCenterX = view.getCenter().x;
        viewCenterY = view.getCenter().y;

        sf::Event event;
        while (window->pollEvent(event))
            if (event.type == sf::Event::Closed)
                window->close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            return -1;

        timer++;
        powerUpTimer++;
        if(endOfGame == false)
        {
            sf::FloatRect tmp = player.getGlobalBounds();
            hamBox = sf::FloatRect(tmp.left + 2,
                               tmp.top + tmp.height/2,
                               tmp.width - 4,
                               tmp.height/2 - 4);

            tmp = ground.getGlobalBounds();
            groundBox = sf::FloatRect(tmp.left + 2,
                                  tmp.top + 2,
                                  tmp.width - 4,
                                  tmp.height - 4);

            tmp = ceiling.getGlobalBounds();
            ceilingBox = sf::FloatRect(tmp.left + 2,
                                   tmp.top + 2,
                                   tmp.width - 4,
                                   tmp.height - 4);


            rot = player.getRotation();
            hasMoved = MoveSprite(rot, y, speed);
            player.setRotation(rot);
            player.move(sf::Vector2f(0, y));

            if(playerPosY < 1)
                player.setPosition(playerPosX, 1);
            if(hasMoved)
                Animate(frame, timer, animation);
            else
                frame = 1;
            player.setTextureRect(sf::IntRect(frame * 36, 0, 36, 49));

            MoveBackground(&xBG, &bg, &bg2, &background, rot, speed);
            if(playerPosY + viewSizeY/2 < backgroundH &&
               playerPosY - viewSizeY/2 > 0)
            view.setCenter(playerPosX, playerPosY);

            stringstream ss;
            ss << "score  " << score;
            scoreText.setString(ss.str());
            scoreText.setPosition(viewCenterX + viewSizeX * 1/3,
                                  viewCenterY + viewSizeY * 1/3);


            if(floatingFly < 6.28f)
                    floatingFly+=0.05f;
                else
                    floatingFly = 0;
            UpdateCoins(&position,
                        &coins,
                        rot,
                        floatingFly,
                        backgroundW,
                        backgroundH,
                        viewCenterX - viewSizeX*2/3,
                        playerPosX);
            UpdateObstacles(&positionObs,
                        &obstacles,
                        floatingFly,
                        backgroundW,
                        backgroundH,
                        viewCenterX - viewSizeX*2/3,
                        playerPosX,
                        speed);
            UpdatePowerUps(&positionUp,
                        &powerUps,
                        floatingFly,
                        backgroundW,
                        backgroundH,
                        viewCenterX - viewSizeX*2/3,
                        playerPosX,
                        speed);

            bool obstacleTouched = false;
            for (int i = 0; i < 3; ++i)
            {
                seedBox = coins[i].getGlobalBounds();
                if(seedBox.intersects(hamBox))
                {
                    score++;
                    if (powerUpTimer <= 360)
                        score++;
                    balloon.play();
                    position[i] = sf::Vector2f(position[i].x + backgroundW + playerPosX + rand()%200,
                                     rand()%((int)backgroundH/2) + 15);
                    coins[i].setPosition(position[i]);
                }

                sf::FloatRect tmp = obstacles[i].getGlobalBounds();
                obstacleBox = sf::FloatRect(tmp.left + 2,
                               tmp.top + 2,
                               tmp.width - 4,
                               tmp.height - 4);
                if(obstacleBox.intersects(hamBox))
                    obstacleTouched = true;
                float range1 = backgroundW + playerPosX - 5 * speed;
                float range2 = backgroundW + playerPosX + 5 * speed;
                if(obstacles[i].getPosition().x >= range1 && obstacles[i].getPosition().x <= range2)
                    beeSound.play();

                sf::FloatRect tmp2 = powerUps[i].getGlobalBounds();
                powerBox = sf::FloatRect(tmp2.left + 2,
                               tmp2.top + 2,
                               tmp2.width - 4,
                               tmp2.height - 4);
                if(powerBox.intersects(hamBox))
                {
                    powerUpTimer = 0;
                    positionUp[i] = sf::Vector2f(positionUp[i].x + backgroundW*6 + playerPosX + rand()%1000,
                                     rand()%((int)backgroundH/2) + 15);
                    powerUps[i].setPosition(positionUp[i]);
                }
            }
            if((groundBox.intersects(hamBox) || hamBox.intersects(ceilingBox) || obstacleTouched) && !endOfGame)
            {
                endOfGame = true;
                lostSound.play();
                music->stop();
                tmpHighScore = SaveScore("res/highscore.txt", score, nameHS);
            }
        }
        else
        {
            stringstream ss;
            ss << "score  " << score;
            scoreText.setCharacterSize(25);
            scoreText.setString(ss.str());
            scoreText.setPosition(viewCenterX - 50,
                                  viewCenterY + viewSizeY/3);



            gameOver.setOrigin(gameOverTexture.getSize().x/2,
                               gameOverTexture.getSize().y/2);
            gameOver.setPosition(viewCenterX, viewCenterY);
            gameOver.setScale(3,3);


            highScoreSprite.setOrigin(newHighScore.getSize().x/2,
                               newHighScore.getSize().y/2);
            highScoreSprite.setPosition(viewCenterX - 10, viewCenterY - 45);

            bubblePanda.setOrigin(bubble.getSize().x/2, bubble.getSize().y/2);
            bubblePanda.setScale(0.4f,0.3f);
            bubblePanda.setPosition(viewCenterX - 150, viewCenterY - 100);
            bubblePanda.setOrigin(bubble.getSize().x/2 * 0.3f,
                               bubble.getSize().y/2 * 0.2f);

            pandaTimer++;
            if(pandaTimer == 75 && tmpHighScore)
                pandaSound.play();
            if(timer%8 == 0 && pandaTimer > 90)
                pandaFrame++;
            if(pandaFrame > 5)
            {
                pandaFrame = 0;
                pandaTimer = 0;
            }
            highScoreSprite.setTextureRect(sf::IntRect(pandaFrame * 48, 0, 48, 51));
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                return score;
        }

        window->setView(view);
        window->clear();
        window->draw(bg);
        window->draw(bg2);
        if(!endOfGame)
        {
            for (int i = 0; i < 3; ++i)
            {
                window->draw(coins[i]);
                window->draw(obstacles[i]);
                window->draw(powerUps[i]);
            }
            window->draw(player);
        }
        else
        {
            window->draw(gameOver);
            if(tmpHighScore)
            {
                window->draw(bubblePanda);
                window->draw(highScoreSprite);
            }
        }
        window->draw(scoreText);
        window->display();
    }
    return score;
}


