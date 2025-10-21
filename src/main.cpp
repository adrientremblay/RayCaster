#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
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

// TODO: modify the code to use a vector library Eigen?
int main()
{
    double position_x = 22, position_y = 12;
    double direction_x = -1, direction_y = 0;
    double plane_x = 0, plane_y = 0.66;

    double time = 0;
    double old_time = 0;

    auto window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
          for (int screen_x = 0 ; screen_x < SCREEN_WIDTH ; screen_x++) {
            // "camera_x is the x-coordinate on the camera plane that the current x-coordinate of the screen represents"
            // far left is -1.0, middle is 0.0, and far right is 1.0
            double camera_x = 2 * screen_x / double(SCREEN_WIDTH) - 1;

            double ray_direction_x = direction_x + plane_x * camera_x;
            double ray_direction_y = direction_y + plane_y * camera_x;

            // position in the map
            int map_x = int(position_x);
            int map_y = int(position_y);

            // distance from one x/y side to another
            double delta_dist_x = std::abs(1/ray_direction_x);
            double delta_dist_y = std::abs(1/ray_direction_y);

            // distance from current position to next
            double side_dist_x, side_dist_y;

            // move in the x direction or the y direction
            int step_x, step_y;

            int hit = 0;
            int side; // Y or X wall
          }

          if (event->is<sf::Event::Closed>())
              window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }
}