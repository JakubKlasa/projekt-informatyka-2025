#include "menu.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

static void pokazTabeleWynikow(sf::RenderWindow& okno, sf::Font& font)
{
    std::vector<int> wyniki;
    std::ifstream f("scores.txt");
    if (f) {
        int w;
        while (f >> w) wyniki.push_back(w);
    }
    std::sort(wyniki.begin(), wyniki.end(), std::greater<int>());

    sf::Text title;
    title.setFont(font);
    title.setString("TABELA WYNIKOW");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setPosition(400.f - title.getLocalBounds().width / 2.f, 80.f);

    std::vector<sf::Text> lines;
    for (size_t i = 0; i < wyniki.size() && i < 10; ++i) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(26);
        t.setFillColor(sf::Color::White);
        t.setString(std::to_string(i + 1) + ". " + std::to_string(wyniki[i]));
        t.setPosition(260.f, 150.f + i * 35.f);
        lines.push_back(t);
    }

    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(20);
    info.setFillColor(sf::Color(200, 200, 200));
    info.setString("Nacisnij klawisz aby wrocic");
    info.setPosition(400.f - info.getLocalBounds().width / 2.f, 520.f);

    while (okno.isOpen()) {
        sf::Event e;
        while (okno.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                okno.close();
                return;
            }
            if (e.type == sf::Event::KeyPressed || e.type == sf::Event::MouseButtonPressed)
                return;
        }

        okno.clear(sf::Color(10, 10, 20));
        okno.draw(title);
        for (auto& t : lines) okno.draw(t);
        okno.draw(info);
        okno.display();
    }
}

int pokazMenu(sf::RenderWindow& okno)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text title;
    title.setFont(font);
    title.setString("ARKANOID");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setPosition(400.f - title.getLocalBounds().width / 2.f, 140.f);

    sf::RectangleShape btn[4];
    const sf::Vector2f size(260.f, 60.f);
    for (int i = 0; i < 4; ++i) {
        btn[i].setSize(size);
        btn[i].setOrigin(size.x / 2.f, size.y / 2.f);
        btn[i].setPosition(400.f, 240.f + i * 70.f);
        btn[i].setOutlineThickness(2.f);
        btn[i].setOutlineColor(sf::Color::Black);
    }

    sf::Text txt[4];
    const char* labels[4] = { "Nowa gra", "Wczytaj gre", "Tabela wynikow", "Wyjdz" };
    for (int i = 0; i < 4; ++i) {
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

    while (okno.isOpen()) {
        sf::Event e;
        while (okno.pollEvent(e)) {
            if (e.type == sf::Event::Closed) return 2;
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) selected = (selected + 3) % 4;
                if (e.key.code == sf::Keyboard::Down) selected = (selected + 1) % 4;
                if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space) {
                    if (selected == 2) {
                        pokazTabeleWynikow(okno, font);
                    }
                    else if (selected == 3) {
                        return 2;
                    }
                    else {
                        return selected;
                    }
                }
                if (e.key.code == sf::Keyboard::Escape) return 2;
            }
            if (e.type == sf::Event::MouseMoved) {
                sf::Vector2f m((float)e.mouseMove.x, (float)e.mouseMove.y);
                for (int i = 0; i < 4; ++i)
                    if (btn[i].getGlobalBounds().contains(m))
                        selected = i;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)e.mouseButton.x, (float)e.mouseButton.y);
                for (int i = 0; i < 4; ++i)
                    if (btn[i].getGlobalBounds().contains(m)) {
                        if (i == 2) {
                            pokazTabeleWynikow(okno, font);
                        }
                        else if (i == 3) {
                            return 2;
                        }
                        else {
                            return i;
                        }
                    }
            }
        }

        for (int i = 0; i < 4; ++i)
            btn[i].setFillColor(i == selected ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));

        okno.clear(sf::Color(15, 15, 22));
        okno.draw(title);
        for (int i = 0; i < 4; ++i) {
            okno.draw(btn[i]);
            okno.draw(txt[i]);
        }
        okno.display();
    }

    return 2;
}
