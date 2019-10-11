#ifndef GAME_HPP
#define GAME_HPP

#include"GameState.hpp"

#include<SFML/Graphics.hpp>
#include<array>

class Game
{
public:
    Game();
    ~Game();
    void run();
    void changeState(GameState::States state);

    sf::Font& getFont();
    sf::RenderWindow& getWindow();
    int& getFieldWidth();
    int& getFieldHeight();
    void setTotalScore(int& amount);
    int& getGameScore();
    bool& isEndedByPlayer();
    void resetGame();

private:
    sf::Font font;
    sf::Vector2i fieldSize;
    sf::RenderWindow window;
    std::array<GameState*, 3> states;
    GameState* currentState;
    int totalScore;
    bool gamePause;
    bool endedByPlayer;
};

#endif // GAME_HPP
