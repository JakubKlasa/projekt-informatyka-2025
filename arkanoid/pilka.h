#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "paletka.h"
#include "stone.h"

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
        if (shape.getPosition().x - shape.getRadius() <= 0 ||
            shape.getPosition().x + shape.getRadius() >= 800)
            vel.x = -vel.x;
        if (shape.getPosition().y - shape.getRadius() <= 0)
            vel.y = -vel.y;
    }

    void collidePaddle(const Paletka& p) {
        if (shape.getGlobalBounds().intersects(p.getShape().getGlobalBounds()) && vel.y > 0) {
            float paddleX = p.getShape().getPosition().x;
            float ballX = shape.getPosition().x;
            float half = p.getShape().getSize().x / 2.f;
            float ratio = (ballX - paddleX) / half;
            vel.x = ratio * 6.f;
            vel.y = -vel.y;
        }
    }

    const sf::CircleShape& getShape() const { return shape; }
    float getY() const { return shape.getPosition().y; }

    sf::Vector2f getPosition() const { return shape.getPosition(); }
    void setPosition(const sf::Vector2f& p) { shape.setPosition(p); }

    sf::Vector2f getVelocity() const { return vel; }
    void setVelocity(const sf::Vector2f& v) { vel = v; }
};
