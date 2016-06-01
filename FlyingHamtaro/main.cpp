#include "Doves.h"
#include "Annexes.h"

int main()
{
    srand(time(NULL));
    sf::Music music;
    if (!music.openFromFile("res/theme3.ogg"))
        cout << "Music issue" << endl;
    music.setVolume(100);
    music.play();

    sf::RenderWindow window(sf::VideoMode(460,320),"Flying Hamster v1.0");
    string nameHS;
    Init(&window);
    int level = IntroScreen(&window, nameHS);

    while(level > -1)
    {
        level = Game(&window, &music, nameHS);
        music.play();
    }

    music.stop();
    return 42;
}
