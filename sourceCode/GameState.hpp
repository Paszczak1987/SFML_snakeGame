#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Snake.cpp"

class Game;

////////// GameState //////////

class GameState // abstract class (at least one pure virtual method)
{
public:
    enum States
    {
        GetReady,
        Playing,
        EndGame
    };
    GameState(Game* game);
    virtual ~GameState();   //if there is virtual method it is recommended to have virtual destructor
    virtual void update(sf::Clock& clock) = 0;       // pure virtual
    virtual void draw(sf::RenderWindow& window) = 0; // pure virtual
    virtual void handleInput(int direction) = 0;
    Game& getParent();

private:
    Game* parent;
};

////////// GetReadyState //////////

class GetReadyState : public GameState
{
public:
    GetReadyState(Game* game);
    ~GetReadyState();
    void update(sf::Clock& clock) override;
    void draw(sf::RenderWindow& window) override;
    void handleInput(int direction) override;

private:
    sf::Text gameTitle;
    sf::Text welcome;
};

////////// PlayingState //////////

class PlayingState : public GameState
{
public:
    PlayingState(Game* game);
    ~PlayingState();
    void update(sf::Clock& clock) override;
    void draw(sf::RenderWindow& window) override;
    void handleInput(int direction) override;

private:
    void randFruitPosition();
    void setDacayTxtPosition();

    sf::Text scoreTxt;
    sf::Text decayTimeTxt;
    std::vector<Snake*> snake;
    Fruit fruit;
    int snakeDirection;
    int score;
    int time;
};

////////// EndGameState //////////

class EndGameState : public GameState
{
public:
    EndGameState(Game* game);
    ~EndGameState();
    void update(sf::Clock& clock) override;
    void draw(sf::RenderWindow& window) override;
    void handleInput(int direction) override;
private:
    sf::Text text;
};

#endif // GAMESTATE_HPP
