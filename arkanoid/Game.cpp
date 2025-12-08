#include "Game.h"
#include "menu.h"
#include "pauza.h"
#include <string>

static void pokazGameOver(sf::RenderWindow& window, sf::Font& font, int punkty)
{
    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setPosition(400.f - title.getLocalBounds().width / 2.f, 200.f);

    sf::Text score;
    score.setFont(font);
    score.setString("Punkty: " + std::to_string(punkty));
    score.setCharacterSize(26);
    score.setFillColor(sf::Color::White);
    score.setPosition(400.f - score.getLocalBounds().width / 2.f, 250.f);

    sf::Text info;
    info.setFont(font);
    info.setString("Nacisnij klawisz aby kontynuowac");
    info.setCharacterSize(20);
    info.setFillColor(sf::Color(200, 200, 200));
    info.setPosition(400.f - info.getLocalBounds().width / 2.f, 300.f);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (e.type == sf::Event::KeyPressed || e.type == sf::Event::MouseButtonPressed)
                return;
        }

        window.clear(sf::Color(10, 10, 20));
        window.draw(title);
        window.draw(score);
        window.draw(info);
        window.display();
    }
}

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
    m_punkty = 0;
    m_textPunkty.setString("Punkty: 0");

    m_textZycia.setFont(m_font);
    m_textZycia.setCharacterSize(24);
    m_textZycia.setFillColor(sf::Color::White);
    m_textZycia.setPosition(700.f, 10.f);
    m_zycia = 3;
    m_textZycia.setString("Zycia: 3");
}

void Game::run()
{
    while (m_window.isOpen()) {
        int wynik = pokazMenu(m_window);
        if (wynik == 1) return;

        m_paletka = Paletka();
        m_pilka = Pilka();
        m_bloki.clear();

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

        m_punkty = 0;
        m_textPunkty.setString("Punkty: " + std::to_string(m_punkty));
        m_zycia = 3;
        m_textZycia.setString("Zycia: " + std::to_string(m_zycia));

        bool graTrwa = true;

        while (m_window.isOpen() && graTrwa) {
            sf::Event e;
            while (m_window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) {
                    m_window.close();
                    return;
                }
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Escape) {
                        int p = pokazPause(m_window, m_font);
                        if (p == 0) {
                        }
                        else if (p == 1) {
                            m_paletka = Paletka();
                            m_pilka = Pilka();
                            m_bloki.clear();
                            float szerBloku2 = (800 - 5 * 2) / 6.f;
                            float wysBloku2 = 25;
                            for (int y = 0; y < 7; y++) {
                                for (int x = 0; x < 6; x++) {
                                    float px = x * (szerBloku2 + 2);
                                    float py = y * (wysBloku2 + 2) + 50;
                                    int L;
                                    if (y == 0) L = 3;
                                    else if (y < 3) L = 2;
                                    else L = 1;
                                    m_bloki.emplace_back(sf::Vector2f(px, py), sf::Vector2f(szerBloku2, wysBloku2), L);
                                }
                            }
                            m_punkty = 0;
                            m_textPunkty.setString("Punkty: " + std::to_string(m_punkty));
                            m_zycia = 3;
                            m_textZycia.setString("Zycia: " + std::to_string(m_zycia));
                            continue;
                        }
                        else if (p == 2) {
                            graTrwa = false;
                            break;
                        }
                    }
                }
            }

            if (!graTrwa) break;

            float dx = 0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -8.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 8.f;
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

            if (m_pilka.getY() > 600) {
                m_zycia--;
                m_textZycia.setString("Zycia: " + std::to_string(m_zycia));

                if (m_zycia <= 0) {
                    pokazGameOver(m_window, m_font, m_punkty);
                    graTrwa = false;
                }
                else {
                    m_paletka = Paletka();
                    m_pilka = Pilka();
                    sf::sleep(sf::milliseconds(400));
                }
            }

            m_window.clear(sf::Color(20, 20, 30));
            m_window.draw(m_paletka.getShape());
            m_window.draw(m_pilka.getShape());
            for (auto& b : m_bloki)
                if (!b.isDestroyed())
                    m_window.draw(b.getShape());
            m_window.draw(m_textPunkty);
            m_window.draw(m_textZycia);
            m_window.display();
        }
    }
}
