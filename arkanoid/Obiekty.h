#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


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
};


class Paletka {
private:
    sf::RectangleShape shape;
public:
    Paletka(float x = 400, float y = 550, float szer = 100, float wys = 20) {
        shape.setSize({ szer, wys });
        shape.setOrigin(szer / 2.f, wys / 2.f);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void move(float dx) {
        shape.setPosition(shape.getPosition().x + dx, shape.getPosition().y);
        if (shape.getPosition().x - shape.getSize().x / 2.f < 0) shape.setPosition(shape.getSize().x / 2.f, shape.getPosition().y);
        if (shape.getPosition().x + shape.getSize().x / 2.f > 800) shape.setPosition(800 - shape.getSize().x / 2.f, shape.getPosition().y);
    }

    const sf::RectangleShape& getShape() const { return shape; }
};


class Pilka {
private:
    sf::CircleShape shape;
    sf::Vector2f vel;
public:
    Pilka(float x = 400, float y = 500, float vx = 4, float vy = 3, float r = 8) : vel(vx, vy) {
        shape.setRadius(r);
        shape.setOrigin(r, r);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void move() { shape.move(vel); }
    void bounceX() { vel.x = -vel.x; }
    void bounceY() { vel.y = -vel.y; }

    void collideWalls() {
        if (shape.getPosition().x - shape.getRadius() <= 0 || shape.getPosition().x + shape.getRadius() >= 800) bounceX();
        if (shape.getPosition().y - shape.getRadius() <= 0) bounceY();
    }

    void collidePaddle(const Paletka& p) {
        if (shape.getGlobalBounds().intersects(p.getShape().getGlobalBounds()) && vel.y > 0) vel.y = -vel.y;
    }

    const sf::CircleShape& getShape() const { return shape; }
    float getY() const { return shape.getPosition().y; }
};
