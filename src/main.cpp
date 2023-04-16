#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

int map[MAP_WIDTH][MAP_HEIGHT] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };


int main() {
    // Setting up window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ray-Caster");
    window.setFramerateLimit(60);

    // Initializing important vectors
    Eigen::Vector2f pos = Eigen::Vector2f(22.0f, 12.0f);
    Eigen::Vector2f dir = Eigen::Vector2f(-1.0f, 0.0f);
    Eigen::Vector2f screen = Eigen::Vector2f(0.0f, 0.66f);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float deltaTimeSeconds = deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clearing screen
        window.clear(sf::Color::Black);

        // Casting rays
        for (int x = 0 ; x < SCREEN_WIDTH ; x++) {
            double screen_pos = 2 * (x / double(SCREEN_WIDTH)) - 1; // goes from -1 to 1
            Eigen::Vector2f ray = dir + (screen * screen_pos);

            // Actual x and y of the cell the player is in
            int map_x = int(pos.x());
            int map_y = int(pos.y());

            // Distance to the next and y along the ray
            double dist_next_x = ray.x() == 0 ? 1e30 : abs(1 / ray.x());
            double dist_next_y = ray.y() == 0 ? 1e30 : abs(1 / ray.y());
            double perWallDist;

            // Distance to next x and to next y in the ray direction from the starting pos
            double side_dist_x;
            double side_dist_y;
            // If we are increasing or decreasing the x and y values
            int step_x;
            int step_y;
            if (ray.x() < 0) {
                step_x = -1;
                side_dist_x = (pos.x() - map_x) * dist_next_x;
            } else {
                step_x = 1;
                side_dist_x = (1.0f + map_x - pos.x()) * dist_next_x;
            }
            if (ray.y() < 0) {
                step_y = -1;
                side_dist_y = (pos.y() - map_y) * dist_next_y;
            } else {
                step_y = 1;
                side_dist_y = (1.0f + map_y - pos.y()) * dist_next_y;
            }

            bool hit = false;
            int side;

            // Doing the DDA algorithm
            while (hit == 0) {
                if (side_dist_x < side_dist_y) {
                    side_dist_x += dist_next_x;
                    map_x += step_x;
                    side = 0;
                } else {
                    side_dist_y += dist_next_y;
                    map_y += step_y;
                    side = 1;
                }

                if (map[map_x][map_y] > 0)
                    hit = true;
            }

            // Calculate distance to the screen
            // todo: actually understand the derivation of this https://lodev.org/cgtutor/raycasting.html#Untextured_Raycaster_
            double wall_distance;
            if (side == 0)
                wall_distance = (side_dist_x - dist_next_x);
            else
                wall_distance = (side_dist_y - dist_next_y);

            // Calculating line height
            int line_height = SCREEN_HEIGHT / wall_distance;
            int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
            if (draw_start < 0)
                draw_start = 0;
            int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
            if (draw_end >= SCREEN_HEIGHT)
                draw_end = SCREEN_HEIGHT - 1;

            // Determining line color
            sf::Color line_color;
            switch (map[map_x][map_y]) {
                case 1:  line_color = sf::Color::Red;  break; //red
                case 2:  line_color = sf::Color::Green;  break; //green
                case 3:  line_color = sf::Color::Blue;   break; //blue
                case 4:  line_color = sf::Color::White;  break; //white
                default: line_color = sf::Color::Yellow; break; //yellow
            }
            if (side == 1) {
                line_color.r = line_color.r / 2;
                line_color.g = line_color.g / 2;
                line_color.b = line_color.b / 2;
            }

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, draw_start), line_color),
                sf::Vertex(sf::Vector2f(x, draw_end), line_color)
            };

            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }

    return 0;
}