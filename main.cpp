#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <ctime>

void rectMove();
void imageLogic(sf::Image& image);
sf::Color mandelbrotPoint(float x, float y);
sf::Color transformNumToColor(unsigned int n);

float zoomerf = 385.f;
float dX = 160.f;
const float PI = 3.141592f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Mandelbrot set");

    sf::Image image;
    image.create(1600, 900, sf::Color(255, 255, 255));

    imageLogic(image);

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    std::string filename = std::to_string(now->tm_year + 1900) + '-'
                         + std::to_string(now->tm_mon) + '-'
                         + std::to_string(now->tm_mday) + '_'
                         + std::to_string(now->tm_hour) + ':'
                         + std::to_string(now->tm_min) + ':'
                         + std::to_string(now->tm_sec) + ".png";

    image.saveToFile("/home/oleg/Изображения/mandelbrot/" + filename);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

void imageLogic(sf::Image& image)
{
    float image_w = image.getSize().x;
    float image_h = image.getSize().y;

    for (unsigned int i = 0; i < image_w; i++)
        for (unsigned int j = 0; j < image_h; j++)
        {
            float x = ((float)i - image_w / 2.f - dX) / zoomerf;
            float y = ((float)j - image_h / 2.f) / zoomerf;

            sf::Color col = mandelbrotPoint(x, y);
            image.setPixel(i, j, col);
        }
}

class ComplexNum {
    public:
        float x;
        float y;

        ComplexNum(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void addComplexNum(const ComplexNum& num)
        {
            this->x += num.x;
            this->y += num.y;
        }

        void square()
        {
            float newX = x*x - y*y;
            y = 2 * x * y;
            x = newX;
        }

        float abs()
        {
            return sqrt(x*x + y*y);
        }
};

sf::Color mandelbrotPoint(float x, float y)
{
    int counter = 0;

    ComplexNum z(0, 0);
    ComplexNum c(x, y);

    do
    {
        z.square();
        z.addComplexNum(c);

        if (z.abs() > 2)
            break;

        counter++;
    } while (counter < 255);

    return transformNumToColor(counter);
}

sf::Color transformNumToColor(unsigned int n)
{
    int b = 200.f * abs(sin(PI / 1.f * (n / 255.f)));
    int g = 255.f * abs(tan(PI / 1.f * (n / 255.f)));
    int r = (255 - n) / 1.5f;

    return sf::Color(r, g, b);
}
