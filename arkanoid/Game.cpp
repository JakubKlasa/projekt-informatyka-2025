#include "Game.h"
#include "menu.h"  

Game::Game() {
    m_window.create(sf::VideoMode(800, 600), "Gra informatyka");
    m_window.setFramerateLimit(60);
    m_paletka = Paletka();
    m_pilka = Pilka();

    float szerBloku = (800 - 5 * 2) / 6.f;
    float wysBloku = 25;

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 6; x++) {
            float px = x * (szerBloku + 2);
            float py = y * (wysBloku + 2) + 50;
            int L;
            if (y == 0) L = 3;
            else if (y < 3) L = 2;
            else L = 1;
            m_bloki.emplace_back(sf::Vector2f(px, py), sf::Vector2f(szerBloku, wysBloku), L);
        }
    }

    m_font.loadFromFile("arial.ttf");
    m_textPunkty.setFont(m_font);
    m_textPunkty.setCharacterSize(24);
    m_textPunkty.setFillColor(sf::Color::White);
    m_textPunkty.setPosition(10.f, 10.f);
    m_textPunkty.setString("Punkty: 0");
}

void Game::run() {

    int wynik = pokazMenu(m_window);
    if (wynik == 1) return;  

    while (m_window.isOpen()) {

        sf::Event e;
        while (m_window.pollEvent(e))
            if (e.type == sf::Event::Closed)
                m_window.close();

        float dx = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -8.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 8.f;

        m_paletka.move(dx);

        m_pilka.move();
        m_pilka.collideWalls();
        m_pilka.collidePaddle(m_paletka);

        for (auto& b : m_bloki) {
            if (!b.isDestroyed() &&
                m_pilka.getShape().getGlobalBounds().intersects(b.getShape().getGlobalBounds()))
            {
                b.trafienie();
                m_pilka.bounceY();

                if (b.isDestroyed()) {
                    m_punkty += 10;
                    m_textPunkty.setString("Punkty: " + std::to_string(m_punkty));
                }

                break;
            }
        }



        if (m_pilka.getY() > 600)
            m_window.close();

        m_window.clear(sf::Color(20, 20, 30));
        m_window.draw(m_paletka.getShape());
        m_window.draw(m_pilka.getShape());

        for (auto& b : m_bloki)
            if (!b.isDestroyed())
                m_window.draw(b.getShape());

        m_window.draw(m_textPunkty);

        m_window.display();
    }
}
