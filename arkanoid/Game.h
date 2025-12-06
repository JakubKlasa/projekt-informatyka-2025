#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "pilka.h"
#include "paletka.h"
#include "stone.h"

using namespace std;

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow m_window;
    Paletka m_paletka;
    Pilka m_pilka;
    vector<Stone> m_bloki;
    int m_punkty = 0;
    sf::Font m_font;
    sf::Text m_textPunkty;
};
