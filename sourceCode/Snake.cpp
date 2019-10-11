#include<SFML/Graphics.hpp>

struct Snake
{
    Snake()
    {
        snakeTexture.loadFromFile("assets/redSquare.png");
        snakeSprite.setTexture(snakeTexture);
    }

    int x;
    int y;

    sf::Texture snakeTexture;
    sf::Sprite snakeSprite;
};

struct Fruit
{
    Fruit()
    {
        body.setRadius(8);
        body.setFillColor(sf::Color::Green);
        cover = false;
    }

    int x;
    int y;

    bool cover;

    sf::CircleShape body;
};
