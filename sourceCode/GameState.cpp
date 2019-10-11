#include "Game.hpp"
#include "GameState.hpp"
#include <string>
#include <cstdlib>
#include <ctime>

template<typename T>
void centerOrigin(T& drawable)
{
    sf::FloatRect bound = drawable.getLocalBounds();
    drawable.setOrigin(bound.width/2,bound.height/2);
}

//////////////////// GameState ////////////////////

GameState::GameState(Game* game):
    parent{game}
{
}
GameState::~GameState()
{
}
Game& GameState::getParent()
{
    return *parent;
}
//////////////////// GetReadyState ////////////////////

GetReadyState::GetReadyState(Game* game):
    GameState{game}
{
    gameTitle.setFont(getParent().getFont());
    gameTitle.setCharacterSize(48);
    gameTitle.setString("================\n THE SNAKE GAME\n================\n");
    gameTitle.setFillColor(sf::Color(255,255,85));
    centerOrigin(gameTitle);
    gameTitle.setPosition(getParent().getFieldWidth()/2*16, getParent().getFieldHeight()/4*16);

    welcome.setFont(getParent().getFont());
    welcome.setCharacterSize(24);
    welcome.setString("!!!! Welcome to the Snake Game !!!!\nHit [SPACE] if You are ready to play.");
    centerOrigin(welcome);
    welcome.setPosition(getParent().getFieldWidth()/2*16,getParent().getFieldHeight()/2*16);
}
GetReadyState::~GetReadyState()
{
}
void GetReadyState::update(sf::Clock& clock)
{
}
void GetReadyState::draw(sf::RenderWindow& window)
{
    window.draw(gameTitle);
    window.draw(welcome);
}
void GetReadyState::handleInput(int direction)
{
}
//////////////////// PlayingState ////////////////////

PlayingState::PlayingState(Game* game):
    GameState(game),
    snakeDirection{rand()%4},
    score{0},
    time{50}
{
    snake.push_back(new Snake);

    snake[0]->x = rand()%getParent().getFieldWidth();
    snake[0]->y = rand()%getParent().getFieldHeight();

    scoreTxt.setFont(getParent().getFont());
    scoreTxt.setCharacterSize(24);
    scoreTxt.setFillColor(sf::Color(0,170,170));
    scoreTxt.setPosition(0, getParent().getWindow().getSize().y - scoreTxt.getCharacterSize());

    randFruitPosition();

    decayTimeTxt.setFont(getParent().getFont());
    decayTimeTxt.setCharacterSize(18);
    decayTimeTxt.setFillColor(sf::Color(85,255,255));

}
PlayingState::~PlayingState()
{
    for(Snake* item : snake)
    {
        delete item;
    }
}
void PlayingState::update(sf::Clock& clock)
{

    if(clock.getElapsedTime().asSeconds() > 0.1)
    {
        time-=1;
        clock.restart();

        //Snake body position

        if(snake.size() > 0)
        {
            for(unsigned i = snake.size() -1 ; i > 0; i--)
            {
                snake[i]->x = snake[i - 1]->x;
                snake[i]->y = snake[i - 1]->y;
            }
        }

        //Handle snake direction

        if(snakeDirection == 0)
        {
            snake[0]->y -= 1;
            if(snake[0]->y < 0 )
                snake[0]->y = getParent().getFieldHeight() - 1;
        }
        else if(snakeDirection == 1)
        {
            snake[0]->y += 1;
            if(snake[0]->y > getParent().getFieldHeight() - 1)
                snake[0]->y = 0;
        }
        else if(snakeDirection == 2)
        {
            snake[0]->x -= 1;
            if(snake[0]->x < 0)
                snake[0]->x = getParent().getFieldWidth() - 1;
        }
        else if(snakeDirection == 3)
        {
            snake[0]->x += 1;
            if(snake[0]->x > getParent().getFieldWidth() - 1)
                snake[0]->x = 0;
        }

        //When snake bite himself

        for(unsigned i = 3; i < snake.size(); i++)
        {
            if(snake[0]->x == snake[i]->x && snake[0]->y == snake[i]->y)
            {
               getParent().changeState(GameState::EndGame);
            }
        }

        //When snake eats fruit

        if(snake[0]->x == fruit.x && snake[0]->y == fruit.y)
        {
            if(time > 40)
                score+=5;
            else if(time > 30 && time < 40)
                score+=3;
            else if(time > 20 && time < 30)
                score+=2;
            else if( time < 20)
                score+=1;

            time = 50;

            snake.push_back(new Snake);
            randFruitPosition();
            getParent().setTotalScore(score);
        }

        //When snake is late

            if(time <= 50 && time > 40)
                fruit.body.setFillColor(sf::Color(0,255,0));
            if(time > 30 && time < 40)
                fruit.body.setFillColor(sf::Color(0,170,0));
            else if(time > 20 && time < 30)
                fruit.body.setFillColor(sf::Color(204,102,0));
            else if( time < 20)
                fruit.body.setFillColor(sf::Color(170,0,0));
            if(time <= 0)
            {
                randFruitPosition();
                time = 50;
            }
    }
    scoreTxt.setString("Score: "+ std::to_string(score));
    decayTimeTxt.setString(std::to_string(time/10)+"."+std::to_string(time%10));
    setDacayTxtPosition();
}

void PlayingState::draw(sf::RenderWindow& window)
{
    for(unsigned i = 0; i < snake.size(); i++)
    {
        snake[0]->snakeSprite.setPosition(snake[i]->x*16, snake[i]->y*16);
        window.draw(snake[0]->snakeSprite);
    }
    fruit.body.setPosition(fruit.x*16, fruit.y*16);
    if(time < 49)
    {
        window.draw(decayTimeTxt);
        window.draw(fruit.body);
    }

    window.draw(scoreTxt);
}
void PlayingState::handleInput(int direction)
{
    if(snakeDirection == 0 && direction == 1){}
    else if(snakeDirection == 1 && direction == 0){}
    else if(snakeDirection == 2 && direction == 3){}
    else if(snakeDirection == 3 && direction == 2){}
    else
        snakeDirection = direction;
}
void PlayingState::randFruitPosition()
{
    do
    {
        fruit.x = rand()%getParent().getFieldWidth();
        fruit.y = rand()%getParent().getFieldHeight();

        for(unsigned i = 0; i < snake.size(); i++)
        {
            if(snake[i]->x == fruit.x && snake[i]->y == fruit.y)
            {
                fruit.cover = true;
                break;
            }
            else
                fruit.cover = false;
        }
    }
    while(fruit.cover);
}
void PlayingState::setDacayTxtPosition()
{
    if(fruit.y == 0 && fruit.x == getParent().getFieldWidth()-1)
        decayTimeTxt.setPosition(fruit.x*16 - 16, fruit.y*16 + 8);
    else if(fruit.y == 0)
        decayTimeTxt.setPosition(fruit.x*16 + 16, fruit.y*16 + 8);
    else if(fruit.x == getParent().getFieldWidth()-1)
        decayTimeTxt.setPosition(fruit.x*16 - 16, fruit.y*16 - 16);
    else
        decayTimeTxt.setPosition(fruit.x*16 + 16,fruit.y*16 - 16);
}

//////////////////// EndGameState /////////////////////

EndGameState::EndGameState(Game* game):
    GameState{game}
{
    text.setFont(getParent().getFont());
    text.setCharacterSize(24);
}
EndGameState::~EndGameState()
{
}
void EndGameState::update(sf::Clock& clock)
{
    int score = getParent().getGameScore();

    if(!getParent().isEndedByPlayer())
    {
        text.setString("You bit yourself :(\n\nYour score: "+std::to_string(score)+"\n\nHit [ESC] to Quit.\nHit [ENTER] to play again.");
    }
    else
        text.setString("You ended the game.\n\nYour score: "+std::to_string(score)+"\n\nHit [ESC] to Quit.\nHit [ENTER] to play again.");
    centerOrigin(text);
    text.setPosition(getParent().getFieldWidth()/2*16, getParent().getFieldHeight()/2*16);
}
void EndGameState::draw(sf::RenderWindow& window)
{
    window.draw(text);
}
void EndGameState::handleInput(int direction)
{
}






















