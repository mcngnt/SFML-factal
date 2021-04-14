#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <string>

#include "utils.h"
#include "vector.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, 32), "Raymarch", sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    sf::Sprite renderSprt;
    sf::RenderTexture renderTex;
    renderTex.create(SCREEN_W, SCREEN_H);
    renderSprt.setTexture(renderTex.getTexture());
    renderTex.clear();

    sf::Vector2f camPos(0.f, 0.f);
    float zoom = 1.f;
    int tick = 0;
    int N = 10;
    bool useGrad = false;
    int functionID = 0;

    sf::Shader* raymarch = new sf::Shader;
    raymarch->loadFromFile("res/raymarch.frag", sf::Shader::Fragment);
    raymarch->setUniform("texSize", sf::Vector2f((float)SCREEN_W, (float)SCREEN_H));

    raymarch->setUniform("useGrad", useGrad);

    sf::Image colorsImg;
    colorsImg.create(CSIZE, 1, sf::Color::White);
    for (int x = 0; x < CSIZE; ++x)
    {
        colorsImg.setPixel(x, 0, sf::Color(rand()%255, rand()%255, rand()%255, 255));
    }

    sf::Texture colorsTex;
    colorsTex.loadFromImage(colorsImg);
    raymarch->setUniform("colorsTex", colorsTex);

    renderTex.clear();

    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            if ((event.type == sf::Event::KeyPressed))
            {
                if (event.key.code == sf::Keyboard::Z)
                {
                    camPos.y += .1f/zoom;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    camPos.y -= .1f/zoom;
                }
                if (event.key.code == sf::Keyboard::Q)
                {
                    camPos.x -= .1f/zoom;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    camPos.x += .1f/zoom;
                }
                if (event.key.code == sf::Keyboard::Space)
                {
                    zoom += .1f*zoom;
                }
                if (event.key.code == sf::Keyboard::Tab)
                {
                    zoom -= .1f*zoom;
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    N += 1;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    N -= 1;
                }
                if (event.key.code == sf::Keyboard::R)
                {
                    for (int x = 0; x < CSIZE; ++x)
                    {
                        float grad = (float)x/(float)CSIZE;
                        colorsImg.setPixel(x, 0, sf::Color(rand()%255, rand()%255, rand()%255, 255));
                    }
                
                    colorsTex.loadFromImage(colorsImg);
                    raymarch->setUniform("colorsTex", colorsTex);

                }
                if (event.key.code == sf::Keyboard::X)
                {
                    useGrad = !useGrad;
                    raymarch->setUniform("useGrad", useGrad);
                }
                if (event.key.code == sf::Keyboard::T)
                {
                    functionID++;
                    functionID = functionID % 3;
                }
            }
        }

        tick++;

        raymarch->setUniform("camPos", camPos);
        raymarch->setUniform("zoom", zoom);
        raymarch->setUniform("time", (float)tick/100.f);
        raymarch->setUniform("N", N);
        raymarch->setUniform("functionID", functionID);
        raymarch->setUniform("mousePos", mousePos);
        window.draw(renderSprt, raymarch);
        window.display();
    }
    return EXIT_SUCCESS;
}
