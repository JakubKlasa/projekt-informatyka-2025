#include "Game.h"
#include "menu.h"
#include <string>

static int pokazGameOver(sf::RenderWindow& window, sf::Font& font, int punkty)
{
    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setPosition(400.f - title.getLocalBounds().width / 2.f, 140.f);

    sf::Text score;
    score.setFont(font);
    score.setString("Punkty: " + std::to_string(punkty));
    score.setCharacterSize(26);
    score.setFillColor(sf::Color::White);
    score.setPosition(400.f - score.getLocalBounds().width / 2.f, 190.f);

    sf::RectangleShape btn[3];
    const sf::Vector2f size(260.f, 60.f);
    for (int i = 0; i < 3; ++i) {
        btn[i].setSize(size);
        btn[i].setOrigin(size.x / 2.f, size.y / 2.f);
        btn[i].setPosition(400.f, 260.f + i * 70.f);
        btn[i].setOutlineThickness(2.f);
        btn[i].setOutlineColor(sf::Color::Black);
    }

    sf::Text txt[3];
    const char* labels[3] = { "Zagraj ponownie", "Powrot do menu", "Wyjdz" };
    for (int i = 0; i < 3; ++i) {
        txt[i].setFont(font);
        txt[i].setString(labels[i]);
        txt[i].setCharacterSize(24);
        txt[i].setFillColor(sf::Color::Black);
        txt[i].setPosition(
            btn[i].getPosition().x - txt[i].getLocalBounds().width / 2.f,
            btn[i].getPosition().y - txt[i].getLocalBounds().height / 2.f - 8.f
        );
    }

    int selected = 0;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) return 2;
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) selected = (selected + 2) % 3;
                if (e.key.code == sf::Keyboard::Down) selected = (selected + 1) % 3;
                if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space) return selected;
                if (e.key.code == sf::Keyboard::Escape) return 2;
            }
        }

        for (int i = 0; i < 3; ++i)
            btn[i].setFillColor(i == selected ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));

        window.clear(sf::Color(15, 15, 22));
        window.draw(title);
        window.draw(score);
        for (int i = 0; i < 3; ++i) {
            window.draw(btn[i]);
            window.draw(txt[i]);
        }
        window.display();
    }

    return 2;
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
        m_textPunkty.setString("Punkty: 0");

        bool graTrwa = true;

        while (m_window.isOpen() && graTrwa) {
            sf::Event e;
            while (m_window.pollEvent(e))
                if (e.type == sf::Event::Closed) {
                    m_window.close();
                    return;
                }

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
                int opt = pokazGameOver(m_window, m_font, m_punkty);
                if (opt == 0) {
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
                    m_textPunkty.setString("Punkty: 0");
                    continue;
                }
                else if (opt == 1) {
                    graTrwa = false;
                    break;
                }
                else {
                    m_window.close();
                    return;
                }
            }

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
}
