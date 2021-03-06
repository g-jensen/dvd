#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

// unsed function
bool intersect(sf::CircleShape &c, sf::Vector2f velocity, sf::RectangleShape t, sf::RectangleShape r, sf::RectangleShape b, sf::RectangleShape l) {
    auto hb1 = t.getGlobalBounds();
    auto hb2 = r.getGlobalBounds();
    auto hb3 = b.getGlobalBounds();
    auto hb4 = l.getGlobalBounds();

    auto pos = c.getPosition();

    if (hb2.contains(pos) || hb4.contains(pos)) {
        c.move(-velocity);
        if (velocity.y / velocity.x > 0) {
            velocity = {-velocity.y,velocity.x};
        } else {
            velocity = {velocity.y,-velocity.x};
        }
        return true;
    } else
    if (hb3.contains(pos) || hb1.contains(pos)) {
        c.move(-velocity);
        if (velocity.y / velocity.x < 0) {
            velocity = {-velocity.y,velocity.x};
        } else {
            velocity = {velocity.y,-velocity.x};
        }
        return true;
    }

    return false;
}

int getFirstCornerHitTime(int length, int width) {
    int max = length * width;
    for (int i = 1; i <= max; i++) {
        int value1 = abs(2 * abs(fmod((i/2.f) + length - (length/2.f),2.f * length) - length) - length);
        if (value1 == 0 || value1 == length) {
            int value2 = abs(2 * abs(fmod((i/2.f) + width - (width/2.f),2.f * width) - width) - width);
            if (value2 == 0 || value2 == width) {
                return i;
            }
        }
    }
    return -1;
}

void drawHIW() {
    ImGui::Text("DVD Bounce with C++/SFML + ImGui");
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Text("The collision is not simulated, its done by a mathematical equation with");
    ImGui::Text("the length and width of the rectangle as parameters.");
    ImGui::NewLine();
    ImGui::Text("In the source code for this program, you can find a function to simulate");
    ImGui::Text("collision which is what I used to figure out the equation but the function");
    ImGui::Text("is never used.");
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Text("Parametric equations for x and y given l = length and w = width:");
    ImGui::NewLine();
    ImGui::Text("x(t) = abs(2 * abs(mod(t + l - (l/2),2 * l) - l) - l)");
    ImGui::Text("y(t) = abs(2 * abs(mod(t + w - (w/2),2 * w) - w) - w)");
    ImGui::NewLine();
    ImGui::Text("This assumes starting at the bottom left of the rectangle at a 45 degree angle.");
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Text("Controls:");
    ImGui::NewLine();
    ImGui::Text("Arrow keys to move the camera around.");
    ImGui::Text("PageUp and PageDown to zoom in and out respectively");
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Text("Credits:");
    ImGui::TreePush();
    ImGui::BulletText("SFML: opening window, drawing stuff");
    ImGui::BulletText("Dear ImGui: GUI boxes");
    ImGui::BulletText("imgui-sfml: Compatibility between SFML and ImGui");
    ImGui::TreePop();
}

void updatePos(sf::CircleShape& dvd, float length, float width, int tick) {
    sf::Vector2f newpos;
    newpos.x = abs(
        2 * abs(
            fmod((tick/2.f) + length - (length/2.f),2.f * length) - length
        ) - length
    );

    newpos.y = abs(
        2 * abs(
            fmod((tick/2.f) + width - (width/2.f),2.f * width) - width
        ) - width
    );

    dvd.setPosition(newpos);
}

int main(int argc, char** argv) {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "DVD");

    sf::View v1 = window.getView();
    v1.move(-330,-230);
    v1.zoom(0.5);
    window.setView(v1);

    sf::Clock dt;

    ImGui::SFML::Init(window);

    sf::RectangleShape left;
    sf::RectangleShape right;
    sf::RectangleShape top;
    sf::RectangleShape bottom;

    sf::Vector2f velocity {1,1};


    float length = 533;
    float width = 300;
    if (argc > 2) {
        length = stoi(std::string(argv[1]));
        width = stoi(std::string(argv[2]));
    }

    int corner = getFirstCornerHitTime(length,width);
    std::cout << corner << std::endl;

    sf::CircleShape dvd(0.5);
    dvd.setPosition(0,0);
    dvd.setFillColor(sf::Color::Green);

    float y = 0.f;
    float x = 0.f;
    int tick = 0;

    bool pause = true;
    bool doLines = true;

    std::vector<sf::VertexArray> lines;

    sf::VertexArray va(sf::Lines,2);

    sf::Vector2f prevPos = {0,0};

    {
        left = sf::RectangleShape({width,1});
        left.rotate(90);
        left.setPosition({0,0});
        right = sf::RectangleShape({width,1});
        right.rotate(90);
        right.setPosition({length+1,0});
        top = sf::RectangleShape({length,1});
        top.setPosition({0,-1});
        bottom = sf::RectangleShape({length,1});
        bottom.setPosition({0,width});
    }

    while (window.isOpen()) {
        x = dvd.getPosition().x;
        y = dvd.getPosition().y;

        window.setFramerateLimit(60);
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                sf::View v = window.getView();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    v.move(-5,0);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    v.move(5,0);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    v.move(0,-5);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    v.move(0,5);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    pause = !pause;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
                    v.zoom(2.f/3.f);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
                    v.zoom(1.5);
                    window.setView(v);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                    tick++;
                    updatePos(dvd,length,width,tick);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                    tick--;
                    updatePos(dvd,length,width,tick);
                }
            }
        }
        ImGui::SFML::Update(window, dt.restart());

        if (!pause) {
            tick += 1;

            sf::Vector2f newpos;
            
            // alternate computation
            /*
            float tmx = fmod(tick,2*length);
            tmx = (tmx < length) ? tmx : length-(tmx-length);
            newpos.x = tmx;
            
            float tmy = fmod(tick,2*width);
            tmy = (tmy < width) ? tmy : width-(tmy-width);
            newpos.y = tmy;
            */

            updatePos(dvd,length,width,tick);
        }

        if ((x == length || x == 0 || y == 0 || y == width) && (tick <= corner+1)) {
            sf::VertexArray va(sf::Lines,2);
            va[0].position = prevPos;
            va[1].position = {x,y};
            prevPos = {x,y};
            lines.push_back(va);
        }

        ImGui::Begin("y");
        ImGui::SliderFloat("y",&y,0.f,width,"%.3f");
        ImGui::End();

        ImGui::Begin("x");
        ImGui::SliderFloat("x",&x,0.f,length,"%.3f");
        ImGui::End();

        ImGui::Begin("t");
        ImGui::InputInt("t",&tick);
        ImGui::End();

        ImGui::Begin("lines");
        ImGui::Checkbox("lines",&doLines);
        ImGui::End();

        ImGui::Begin("How it works (copy of README.md)");
        drawHIW();
        ImGui::End();
        
        window.clear(sf::Color::Black);


        window.draw(left);
        window.draw(right);
        window.draw(top);
        window.draw(bottom);

        if (doLines) {
            for (sf::VertexArray l: lines) {
                window.draw(l);
            }
        }

        window.draw(dvd);

        ImGui::SFML::Render(window);

        window.display();

        dt.restart();
    }

    ImGui::SFML::Shutdown();
    return 0;
}