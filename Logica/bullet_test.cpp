//
// Created by j1p2p3a4 on 4/23/2026.
//

#include<SFML/Graphics.hpp>
#include<iostream>
#include <cmath>
#include<vector>
#include<random>
struct par {
    float first, second;
};
int v_size = 2;
void update_coords(const float *v, float *x, const float delta) {
    x[0] = x[0] + delta * v[0];
    x[1] = x[1] + delta * v[1];
}

sf::CircleShape create_bullet(const float *x, const float radius, const sf::Color color) {
    sf::CircleShape shape(radius);
    shape.setFillColor(color);

    shape.setPosition({x[0], x[1]});

    return shape;
}

float cross_product(const float* x , const float* norma) {
    float cross = x[0] * norma[1] - x[1] * norma[0];
    return cross;
}

float dot_product(const float * x, const float* norma) {
    float dot = 0.;
    for (int i = 0; i < 2; i++) {
        dot += x[i] * norma[i];
    }
    return dot;
}
void draw_map(const std::vector<std::vector<int>>& mapa, const  int cell_size, sf::RenderWindow& window) {
    const int N = mapa.size();
    for (int i = 0; i < N ; i++) {
        for (int j = 0; j < N; j++) {
            if (mapa[i][j] == 1) {
                sf::RectangleShape rect(sf::Vector2f((float) cell_size, (float) cell_size));
                rect.setFillColor(sf::Color::White);
                rect.setPosition({(float) (cell_size * i), (float) (cell_size * j)});
                window.draw(rect);
            }
        }
    }
}
void aim(float *v, float *posit, const  sf::RenderWindow & window) {
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    auto x = (float) pos.x;
    auto y =(float) pos.y;

    x = x - posit[0];
    y = y - posit[1];

    const float norm = std::sqrt(x * x + y * y );

    x = x/ norm;
    y = y/norm;

    v[0] = x;
    v[1] = y;
}
int main() {
    //map construction
    sf::RenderWindow window(sf::VideoMode({600, 600}), "My Window");

    constexpr int N = 30;
    int obstacles = 100;
    constexpr int size = 600/N;

    par traces[2] = {{310.0f, 310.0f}, {310.0f, 310.0f}};
    std::vector<std::vector<int>> mapa(N, std::vector<int>(N, 0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10000);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (obstacles > 0) mapa[i][j] = dist(gen) % 10;
            if (mapa[i][j] == 1 )obstacles -= mapa[i][j];
        }
    }

    //velocity and position construction:
    float v[2] = {0.,0.};
    float radius = 3.f;
    float pos[2] = {310.0, 310.0};

    par dir[8] = {
        {radius + 0.2f, 0}, {-radius - 0.2f, 0}, {0, radius +0.2f},
        {0, -radius -0.2f},
        {radius*0.9f, radius*0.9f}, {radius*0.9f, -radius*0.9f},
        {-radius*0.9f, radius*0.9f}, {-radius*0.9f, -radius*0.9f}
    };

    //main cycle:
    sf::CircleShape bullet = create_bullet(pos, radius,  sf::Color::Blue);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>()){
                window.close();
            }

            if (event -> is<sf::Event::MouseButtonPressed>()) {
                aim(v, pos, window);
            }
        }

        window.clear();

        sf::Vertex line[2];
        line[0].position = {traces[0].first, traces[0].second};
        line[1].position = {traces[1].first, traces[1].second};
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window.draw(line, 2 , sf::PrimitiveType::Lines);


        window.draw(bullet);
        draw_map(mapa, size, window );
        window.display();

        bool flip_x = false, flip_y = false;

        for (par& d : dir) {
            float p1 = pos[0] + d.first, p2 = pos[1] + d.second;

            if (p1 <= 0 || p1 >= 600) flip_x = true;
            if (p2 <= 0 || p2 >= 600) flip_y = true;

            int idx = (int)(p1 / size), jdx = (int)(p2 / size);
            if (p1 >= 0 && p1 < 600 && p2 >= 0 && p2 < 600) {
                if (mapa[idx][jdx] == 1) {
                    float cx = idx * size + size / 2.0f;
                    float cy = jdx * size + size / 2.0f;
                    float dx = std::abs(pos[0] - cx);
                    float dy = std::abs(pos[1] - cy);

                    if (dx >= dy) flip_x = true;
                    else flip_y = true;
                }
            }
        }

        if (flip_x) {
            v[0] = -v[0];
            traces[0] = traces[1];
            traces[1] = {pos[0], pos[1]};
        }
        if (flip_y) {
            v[1] = -v[1];
            traces[0] = traces[1];
            traces[1] = {pos[0], pos[1]};

        }

        update_coords(v, pos,  0.2f);
        bullet.setPosition({pos[0], pos[1]});


    }
}
