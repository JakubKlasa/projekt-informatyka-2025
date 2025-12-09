#include "pauza.h"

int pokazPause(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text title;
    title.setFont(font);
    title.setString("PAUZA");
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
    const char* labels[4] = { "Wznow", "Zapisz gre", "Restart", "Wyjdz do menu" };
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

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) return 3;
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) selected = (selected + 3) % 4;
                if (e.key.code == sf::Keyboard::Down) selected = (selected + 1) % 4;
                if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space) return selected;
                if (e.key.code == sf::Keyboard::Escape) return 0;
            }
        }

        for (int i = 0; i < 4; ++i)
            btn[i].setFillColor(i == selected ? sf::Color(230, 210, 70) : sf::Color(70, 70, 80));

        window.clear(sf::Color(15, 15, 22));
        window.draw(title);
        for (int i = 0; i < 4; ++i) { window.draw(btn[i]); window.draw(txt[i]); }
        window.display();
    }

    return 3;
}
