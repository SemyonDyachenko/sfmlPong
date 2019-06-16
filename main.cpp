#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <time.h>


using namespace sf;


sf::RenderWindow * window;

const int winSizeX = 1000;
bool gameover;
const int winSizeY = 600;
int playerScore;
int enemyScore;
sf::SoundBuffer buffer;
Sound sound(buffer);


class Player {
private:
    float x, y;
    float  width, height,  dy, speed;
    sf::RectangleShape shape;
    enum DirectionMovement
    {
        STAY,
        UP,
        DOWN
    };
    DirectionMovement state;
public:

    Player(float x)
    {
        dy = 0; speed = 0;
        width = 30;
        height =120;
        this->x = x;
        this->y = winSizeY/2-height;
        this->state = STAY;
        shape.setSize(sf::Vector2f(width,height));
        shape.setFillColor(Color::White);
        shape.setPosition(x,y);

    }

    void movement()
    {
        if(Keyboard::isKeyPressed(Keyboard::W)) {
            this->state = UP; this->speed = 0.3f;
        }
        else if(Keyboard::isKeyPressed(Keyboard::S))
        {
            this->state = DOWN; this->speed = 0.3f;
        }
    }

    void update(float time)
    {
        movement();
        switch (state)
        {
            case UP: dy = -speed;  break;
            case DOWN: dy = speed;  break;
            case STAY: break;
        }
        y += dy * time;
        shape.setPosition(x,y);
        speed = 0;
    }

    sf::RectangleShape getShape()
    {
        return this->shape;
    }

    void render(sf::RenderWindow * window)
    {
        window->draw(shape);
    }
};


class Ball {
public:
    float posX,posY,speedX,speedY;
    int radius;
    sf::CircleShape shape;
public:
    Ball() {
        posX = winSizeX/1.5;
        posY= winSizeY/2-winSizeY/5;
        radius = 15;
        shape.setFillColor(Color::White);
        shape.setRadius(radius);
        shape.setPosition(posX,posY);
        speedX = 0.3f;
        speedY = 0.3f;
    }


    void update(float time,sf::RectangleShape shape,sf::RectangleShape shape1)
    {


        if(posY <= this->radius) {
            this->speedY = -speedY;
            if(rand()%(1-2) == 2) {
                this->speedX = -speedX;
            } else {
                this->speedX = this->speedX;
            }
        }

        if((posY+radius) > (winSizeY-radius-3)) {
            this->speedY = -speedY;
            if(rand()%(1-2) == 2) {
                this->speedX = -speedX;
            } else {
                this->speedX = this->speedX;
            }
        }

        if(FloatRect(this->shape.getPosition().x,this->shape.getPosition().y,this->radius,this->radius).intersects(sf::FloatRect(shape.getPosition().x,shape.getPosition().y,shape.getSize().x,shape.getSize().y)))
        {
            this->speedY = -speedY;
            this->speedX = -speedX;
            sound.play();
        }


        this->posY += speedY * time;
        this->posX += -speedX*time;
        this->shape.setPosition(posX,posY);



    }

    sf::FloatRect getRect () {
        return sf::FloatRect(posX,posY,radius,radius);
    }


    void render(sf::RenderWindow * window)
    {
        window->draw(shape);
    }
};




int main(int argc,char ** argv)
{
    srand(time(0));
    window = new RenderWindow(VideoMode(winSizeX,winSizeY),"Pong");
    gameover= false;
    Player p(10);
    Ball ball;

    playerScore = 0;
    enemyScore = 0;

    //bot
    sf::RectangleShape shape;
    shape.setFillColor(Color::White);
    shape.setSize(sf::Vector2f(30,120));
    float x, y;
    x= winSizeX - 40;
    y = (winSizeY/2) - 130;
    shape.setPosition(x,y);
    float sp = 0.3f; // speed



    //text
    Text enemy;
    sf::Font font;
    font.loadFromFile("../res/fonts/font.ttf");
    Text player;
    std::string enscore = std::to_string(enemyScore);
    std::string plscore = std::to_string(playerScore);
    enemy.setFont(font);
    enemy.setCharacterSize(25);
    enemy.setString(enscore);
    enemy.setPosition((winSizeX/2)-80,20);
    player.setFont(font);
    player.setCharacterSize(25);
    player.setString(plscore);
    player.setPosition((winSizeX/2)+80,20);

    //sound
    buffer.loadFromFile("../res/sounds/beep.ogg");



    Clock clock;
    while(window->isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window->close();
        }

        //clear

        window->clear();

            p.update(time);


        ball.update(time, p.getShape(), shape);


            if(ball.speedY > 0) {
                if(sf::FloatRect(shape.getPosition().x,shape.getPosition().y,shape.getSize().x,shape.getSize().y).intersects(ball.getRect())) {

                    ball.speedX = -ball.speedX;
                    if(rand()%(1-2) == 2) {
                        ball.speedY = -ball.speedY;
                    } else {
                        ball.speedY = ball.speedY;
                    }
                    sound.play();
                } else {

                    y += sp*time;
                    shape.setPosition(x,y);
                }
            } else if(ball.speedY < 0)
            {
                if(!sf::FloatRect(shape.getPosition().x,shape.getPosition().y,shape.getSize().x,shape.getSize().y).intersects(ball.getRect())) {
                    y += -sp*time;
                    shape.setPosition(x,y);
                } else {
                    ball.speedX = -ball.speedX;
                    sound.play();
                    if(rand()%(1-2) == 2) {
                        ball.speedY = -ball.speedY;
                    } else {
                        ball.speedY = ball.speedY;
                    }
                }
            }


            if(ball.posX < (p.getShape().getSize().x)) {
                playerScore+=1;
                player.setString(plscore);
                std::string plscore = std::to_string(playerScore);
                window->draw(player);
            }
            else if((ball.posX+ball.radius) > (winSizeX-shape.getSize().x)) {
                enemyScore+=1;
                std::string enscore = std::to_string(enemyScore);
                enemy.setString(enscore);
                window->draw(enemy);
            }




        //display
            window->draw(player);
            window->draw(enemy);
            window->draw(shape);
            p.render(window);
            ball.render(window);


        window->display();

    }

    return 0;
}