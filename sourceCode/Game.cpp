#include "Game.hpp"
#include "GameState.hpp"

Game::Game():
    fieldSize{30,20},
    window{sf::VideoMode(fieldSize.x*16, fieldSize.y*16), "Snake game!"}, // 480 x 320
    totalScore{0},
    gamePause{false},
    endedByPlayer{false}
{
    font.loadFromFile("assets/vgaSquarepx.ttf");

    states[GameState::GetReady] = new GetReadyState(this);
    states[GameState::Playing] = new PlayingState(this);
    states[GameState::EndGame] = new EndGameState(this);


    changeState(GameState::GetReady);

}
Game::~Game()
{
    for(GameState* state : states)
    {
        delete state;
    }
}
void Game::run()
{
    sf::Clock c;
    sf::Event e;
    while(window.isOpen())
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Space)
                {
                    if(currentState == states[GameState::GetReady])
                        changeState(GameState::Playing);
                }
                if(e.key.code == sf::Keyboard::Enter)
                {
                    if(currentState == states[GameState::EndGame])
                    {
                        resetGame();
                        changeState(GameState::GetReady);
                    }
                }
                if(e.key.code == sf::Keyboard::Escape)
                {
                    if(currentState == states[GameState::EndGame] || currentState == states[GameState::GetReady])
                        window.close();
                    if(currentState == states[GameState::Playing])
                    {
                        endedByPlayer = true;
                        changeState(GameState::EndGame);
                    }
                }

                if(e.key.code == sf::Keyboard::Up)
                    currentState->handleInput(0);
                if(e.key.code == sf::Keyboard::Down)
                    currentState->handleInput(1);
                if(e.key.code == sf::Keyboard::Left)
                    currentState->handleInput(2);
                if(e.key.code == sf::Keyboard::Right)
                    currentState->handleInput(3);
                if(e.key.code == sf::Keyboard::P)
                {
                    if(currentState == states[GameState::Playing])
                        gamePause = !gamePause;
                }
            }
        }

        if(!gamePause)
            currentState->update(c);
        window.clear();
        currentState->draw(window);
        window.display();
    }
}
void Game::changeState(GameState::States state)
{
    currentState = states[state];
}
sf::Font& Game::getFont()
{
    return font;
}
sf::RenderWindow& Game::getWindow()
{
    return window;
}
int& Game::getFieldWidth()
{
    return fieldSize.x;
}
int& Game::getFieldHeight()
{
    return fieldSize.y;
}
void Game::setTotalScore(int& amount)
{
    totalScore = amount;
}
int& Game::getGameScore()
{
    return totalScore;
}
bool& Game::isEndedByPlayer()
{
    return endedByPlayer;
}
void Game::resetGame()
{
    delete states[GameState::Playing];
    states[GameState::Playing] = new PlayingState(this);
}



