#include "menu.h"
#include <SFML/Graphics.hpp>

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

    sf::RectangleShape btn[3];
    const sf::Vector2f size(260.f, 60.f);
    for (int i = 0; i < 3; ++i) {
        btn[i].setSize(size);
        btn[i].setOrigin(size.x / 2.f, size.y / 2.f);
        btn[i].setPosition(400.f, 240.f + i * 70.f);
        btn[i].setOutlineThickness(2.f);
        btn[i].setOutlineColor(sf::Color::Black);
    }

    sf::Text txt[3];
    const char* labels[3] = { "Nowa gra", "Wczytaj gre", "Wyjdz" };
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

    while (okno.isOpen()) {
        sf::Event e;
        while (okno.pollEvent(e)) {
            if (e.type == sf::Event::Closed) return 2;
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) selected = (selected + 2) % 3;
                if (e.key.code == sf::Keyboard::Down) selected = (selected + 1) % 3;
                if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space) return selected;
                if (e.key.code == sf::Keyboard::Escape) return 2;
            }
            if (e.type == sf::Event::MouseMoved) {
                sf::Vector2f m((float)e.mouseMove.x, (float)e.mouseMove.y);
                for (int i = 0; i < 3; ++i)
                    if (btn[i].getGlobalBounds().contains(m))
                        selected = i;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)e.mouseButton.x, (float)e.mouseButton.y);
                for (int i = 0; i < 3; ++i)
                    if (btn[i].getGlobalBounds().contains(m))
                        return i;
            }
        }

        for (int i = 0; i < 3; ++i)
            btn[i].setFillColor(i == selected ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));

        okno.clear(sf::Color(15, 15, 22));
        okno.draw(title);
        for (int i = 0; i < 3; ++i) {
            okno.draw(btn[i]);
            okno.draw(txt[i]);
        }
        okno.display();
    }

    return 2;
}
