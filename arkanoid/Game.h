#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
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

    int m_punkty;
    int m_zycia;

    sf::Font m_font;
    sf::Text m_textPunkty;
    sf::Text m_textZycia;

    void zapiszGre(const std::string& nazwa);
    bool wczytajGre(const std::string& nazwa);
    void zapiszWynik(const std::string& nazwa);
};
