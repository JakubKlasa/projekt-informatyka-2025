#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "pilka.h"
#include "paletka.h"

class Stone : public sf::RectangleShape {
private:
    int m_punktyZycia;
    bool m_jestZniszczony;
public:
    Stone(sf::Vector2f p, sf::Vector2f s, int L)
        : m_punktyZycia(L), m_jestZniszczony(false)
    {
        setPosition(p);
        setSize(s);
        setOutlineThickness(2);
        setOutlineColor(sf::Color::Black);
        if (m_punktyZycia >= 3) setFillColor(sf::Color::Blue);
        else if (m_punktyZycia == 2) setFillColor(sf::Color::Yellow);
        else if (m_punktyZycia == 1) setFillColor(sf::Color::Red);
    }

    void trafienie() {
        if (m_jestZniszczony) return;
        m_punktyZycia--;
        if (m_punktyZycia <= 0) m_jestZniszczony = true;
        if (m_jestZniszczony) return;
        if (m_punktyZycia >= 3) setFillColor(sf::Color::Blue);
        else if (m_punktyZycia == 2) setFillColor(sf::Color::Yellow);
        else if (m_punktyZycia == 1) setFillColor(sf::Color::Red);
    }

    bool isDestroyed() const { return m_jestZniszczony; }
    const sf::RectangleShape& getShape() const { return *this; }

    int getHp() const { return m_punktyZycia; }
    void setHp(int L) {
        m_punktyZycia = L;
        m_jestZniszczony = (m_punktyZycia <= 0);
        if (m_jestZniszczony) return;
        if (m_punktyZycia >= 3) setFillColor(sf::Color::Blue);
        else if (m_punktyZycia == 2) setFillColor(sf::Color::Yellow);
        else if (m_punktyZycia == 1) setFillColor(sf::Color::Red);
    }
};
