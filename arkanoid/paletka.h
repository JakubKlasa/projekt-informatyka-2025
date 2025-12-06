#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "pilka.h"
#include "stone.h"
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
