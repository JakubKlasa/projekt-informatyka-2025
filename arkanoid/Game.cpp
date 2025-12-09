#include "Game.h"
#include "menu.h"
#include "pauza.h"
#include <string>
#include <fstream>

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

static void pokazWygrana(sf::RenderWindow& window, sf::Font& font, int punkty)
{
    sf::Text title;
    title.setFont(font);
    title.setString("WYGRANA");
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
    float wysBloku = 25.f;

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

void Game::zapiszGre(const std::string& nazwa)
{
    std::ofstream f(nazwa);
    if (!f) return;

    f << m_punkty << " " << m_zycia << "\n";

    sf::Vector2f pp = m_paletka.getPosition();
    f << pp.x << " " << pp.y << "\n";

    sf::Vector2f bp = m_pilka.getPosition();
    sf::Vector2f bv = m_pilka.getVelocity();
    f << bp.x << " " << bp.y << "\n";
    f << bv.x << " " << bv.y << "\n";

    for (auto& b : m_bloki) {
        f << b.getHp() << " ";
    }
    f << "\n";
}

bool Game::wczytajGre(const std::string& nazwa)
{
    std::ifstream f(nazwa);
    if (!f) return false;

    int punkty, zycia;
    if (!(f >> punkty >> zycia)) return false;

    float px, py, bx, by, vx, vy;
    if (!(f >> px >> py)) return false;
    if (!(f >> bx >> by)) return false;
    if (!(f >> vx >> vy)) return false;

    m_punkty = punkty;
    m_zycia = zycia;
    m_textPunkty.setString("Punkty: " + std::to_string(m_punkty));
    m_textZycia.setString("Zycia: " + std::to_string(m_zycia));

    m_paletka.setPosition(sf::Vector2f(px, py));
    m_pilka.setPosition(sf::Vector2f(bx, by));
    m_pilka.setVelocity(sf::Vector2f(vx, vy));

    for (auto& b : m_bloki) {
        int hp;
        if (!(f >> hp)) break;
        b.setHp(hp);
    }

    return true;
}

void Game::zapiszWynik(const std::string& nazwa)
{
    std::ofstream f(nazwa, std::ios::app);
    if (!f) return;
    f << m_punkty << "\n";
}

void Game::run()
{
    while (m_window.isOpen()) {
        int wyborMenu = pokazMenu(m_window);
        if (wyborMenu == 2) return;

        bool wczytaj = (wyborMenu == 1);

        m_paletka = Paletka();
        m_pilka = Pilka();
        m_bloki.clear();

        float szerBloku = (800 - 5 * 2) / 6.f;
        float wysBloku = 25.f;
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
        m_zycia = 3;
        m_textPunkty.setString("Punkty: 0");
        m_textZycia.setString("Zycia: 3");

        if (wczytaj) {
            wczytajGre("save.txt");
        }

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
                            zapiszGre("save.txt");
                        }
                        else if (p == 2) {
                            m_paletka = Paletka();
                            m_pilka = Pilka();
                            m_bloki.clear();
                            float szerBloku2 = (800 - 5 * 2) / 6.f;
                            float wysBloku2 = 25.f;
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
                            m_zycia = 3;
                            m_textPunkty.setString("Punkty: 0");
                            m_textZycia.setString("Zycia: 3");
                            continue;
                        }
                        else if (p == 3) {
                            graTrwa = false;
                            break;
                        }
                    }
                }
            }

            if (!graTrwa) break;

            float dx = 0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                dx = -8.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                dx = 8.f;
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

            bool wszystkieZbite = true;
            for (auto& b : m_bloki) {
                if (!b.isDestroyed()) {
                    wszystkieZbite = false;
                    break;
                }
            }
            if (wszystkieZbite) {
                zapiszWynik("scores.txt");
                pokazWygrana(m_window, m_font, m_punkty);
                graTrwa = false;
            }

            if (!graTrwa) break;

            if (m_pilka.getY() > 600) {
                m_zycia--;
                m_textZycia.setString("Zycia: " + std::to_string(m_zycia));

                if (m_zycia <= 0) {
                    zapiszWynik("scores.txt");
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
