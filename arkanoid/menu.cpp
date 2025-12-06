#include "menu.h"
#include <iostream>

int pokazMenu(sf::RenderWindow& okno)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie moge wczytac arial.ttf\n";
    }

    sf::Text title;
    title.setFont(font);
    title.setString("ARKANOID");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setPosition(
        400.f - title.getLocalBounds().width / 2.f,
        130.f
    );

    const sf::Vector2f size(260.f, 60.f);

    sf::RectangleShape btnStart(size);
    sf::RectangleShape btnQuit(size);

    btnStart.setOrigin(size.x / 2.f, size.y / 2.f);
    btnQuit.setOrigin(size.x / 2.f, size.y / 2.f);
    btnStart.setPosition(400.f, 260.f);
    btnQuit.setPosition(400.f, 340.f);

    btnStart.setOutlineThickness(2.f);
    btnQuit.setOutlineThickness(2.f);
    btnStart.setOutlineColor(sf::Color::Black);
    btnQuit.setOutlineColor(sf::Color::Black);

    sf::Text txtStart, txtQuit;
    txtStart.setFont(font);
    txtQuit.setFont(font);
    txtStart.setString("START");
    txtQuit.setString("QUIT");
    txtStart.setCharacterSize(26);
    txtQuit.setCharacterSize(26);
    txtStart.setFillColor(sf::Color::Black);
    txtQuit.setFillColor(sf::Color::Black);

    auto centerTextOnButton = [](sf::Text& t, const sf::RectangleShape& b) {
        t.setPosition(
            b.getPosition().x - t.getLocalBounds().width / 2.f,
            b.getPosition().y - t.getLocalBounds().height / 2.f - 8.f
        );
        };
    centerTextOnButton(txtStart, btnStart);
    centerTextOnButton(txtQuit, btnQuit);

    int sel = 0;

    while (okno.isOpen()) {
        sf::Event e;
        while (okno.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                return 1;

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::W)
                    sel = 0;
                if (e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::S)
                    sel = 1;
                if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space)
                    return sel;
                if (e.key.code == sf::Keyboard::Escape)
                    return 1;
            }

            if (e.type == sf::Event::MouseMoved) {
                sf::Vector2f m((float)e.mouseMove.x, (float)e.mouseMove.y);
                if (btnStart.getGlobalBounds().contains(m)) sel = 0;
                if (btnQuit.getGlobalBounds().contains(m))  sel = 1;
            }

            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)e.mouseButton.x, (float)e.mouseButton.y);
                if (btnStart.getGlobalBounds().contains(m)) return 0;
                if (btnQuit.getGlobalBounds().contains(m))  return 1;
            }
        }

        btnStart.setFillColor(sel == 0 ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));
        btnQuit.setFillColor(sel == 1 ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));

        okno.clear(sf::Color(15, 15, 22));
        okno.draw(btnStart);
        okno.draw(btnQuit);
        okno.draw(title);
        okno.draw(txtStart);
        okno.draw(txtQuit);
        okno.display();
    }

    return 1;
}
