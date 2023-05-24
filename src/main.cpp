#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900
#define IMAGE_TEXTURE_SIZE 512
#define WALL_TEXTURE_SIZE 128

int map[MAP_WIDTH][MAP_HEIGHT] =
{
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
    {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
    {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
    {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
    {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
    {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
    {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
    {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
    {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
    {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
    {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
    {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
    {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
    {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
    {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
    {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
    {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

struct Player {
    double movSpeed = 15.0f;
    Eigen::Vector2f pos = Eigen::Vector2f(22.0f, 12.0f);
    const Eigen::Vector2f original_dir = Eigen::Vector2f(-1.0f, 0.0f);
    Eigen::Vector2f dir = Eigen::Vector2f(-1.0f, 0.0f);
    const Eigen::Vector2f original_screen = Eigen::Vector2f(0.0f, 0.66f);
    Eigen::Vector2f screen = Eigen::Vector2f(0.0f, 0.66f);
} player;

void rayCast(sf::RenderWindow& window, sf::VertexArray lines, sf::RenderStates renderStates, sf::VertexArray& floor_lines, sf::RenderStates floor_render_states) {
    lines.resize(0);
    floor_lines.resize(0);

    // FLOOR CASTING
    float near = 0.01f;
    float far = 1.0f;

    float floor_tex_sf = 50.f;

    Eigen::Vector2f frustum_top_left = player.pos + ((player.dir - player.screen) * far);
    Eigen::Vector2f frustum_top_right = player.pos + ((player.dir + player.screen) * far);
    Eigen::Vector2f frustum_bottom_left = player.pos + ((player.dir - player.screen) * near);
    Eigen::Vector2f frustum_bottom_right = player.pos + ((player.dir + player.screen) * near);

    for (int y = 0 ; y < SCREEN_HEIGHT / 2 ; y++) {
        float normalized_y = float(y) / ((float) SCREEN_HEIGHT / 2.0f); // between 0 and 1

        Eigen::Vector2f swag = (frustum_top_left - frustum_bottom_left);
        Eigen::Vector2f swag2 = (frustum_top_left - frustum_bottom_left) / normalized_y;
        Eigen::Vector2f scanline_left = (frustum_top_left - frustum_bottom_left) / (normalized_y) + frustum_bottom_left ;
        Eigen::Vector2f scanline_right = (frustum_top_right - frustum_bottom_right) / (normalized_y) + frustum_bottom_right ;

        sf::Color color = sf::Color::White;

        floor_lines.append(sf::Vertex(
                sf::Vector2f(0, y + SCREEN_HEIGHT/2),
                color,
                sf::Vector2f(scanline_left.x() * floor_tex_sf, scanline_left.y() * floor_tex_sf)
        ));
        floor_lines.append(sf::Vertex(
                sf::Vector2f(SCREEN_WIDTH, y + SCREEN_HEIGHT/2),
                color,
                sf::Vector2f(scanline_right.x() * floor_tex_sf, scanline_right.y() * floor_tex_sf)
        ));
    }

    // WALL CASTING
    for (int x = 0 ; x < SCREEN_WIDTH ; x++) {
        double screen_pos = 2 * (x / double(SCREEN_WIDTH)) - 1; // goes from -1 to 1
        Eigen::Vector2f ray = player.dir + (player.screen * screen_pos);

        // Actual x and y of the cell the player is in
        int map_x = int(player.pos.x());
        int map_y = int(player.pos.y());

        // Distance to the next x and y along the ray
        double dist_next_x = ray.x() == 0 ? 1e30 : abs(1 / ray.x());
        double dist_next_y = ray.y() == 0 ? 1e30 : abs(1 / ray.y());

        // Distance to next x and to next y in the ray direction from the starting pos
        double side_dist_x;
        double side_dist_y;
        // If we are increasing or decreasing the x and y values
        int step_x;
        int step_y;
        if (ray.x() < 0) {
            step_x = -1;
            side_dist_x = (player.pos.x() - map_x) * dist_next_x;
        } else {
            step_x = 1;
            side_dist_x = (1.0f + map_x - player.pos.x()) * dist_next_x;
        }
        if (ray.y() < 0) {
            step_y = -1;
            side_dist_y = (player.pos.y() - map_y) * dist_next_y;
        } else {
            step_y = 1;
            side_dist_y = (1.0f + map_y - player.pos.y()) * dist_next_y;
        }

        bool hit = false;
        int side;

        // Doing the DDA algorithm
        while (!hit) {
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
        if (side == 0) {
            wall_distance = side_dist_x - dist_next_x;
        } else {
            wall_distance = side_dist_y - dist_next_y;
        }

        // Calculating line height
        int line_height = SCREEN_HEIGHT / wall_distance;
        int draw_start = SCREEN_HEIGHT/2 - line_height/2;
        int draw_end = SCREEN_HEIGHT/2 + line_height/2;

        // Texture stuff
        int tex_num = map[map_x][map_y] - 1; // so we can use 0 texture
        sf::Vector2i texture_coords(
                tex_num * (int)WALL_TEXTURE_SIZE % (int)IMAGE_TEXTURE_SIZE,
                tex_num * (int)WALL_TEXTURE_SIZE / (int)IMAGE_TEXTURE_SIZE * (int)WALL_TEXTURE_SIZE
        );

        // calculate where the wall was hit //todo: try to understand this???
        float wall_x;
        if (side == 0) {
            wall_x = player.pos.y() + (float)wall_distance * ray.y();
        } else {
            wall_x = player.pos.x() + (float)wall_distance * ray.x();
        }
        wall_x -= floor(wall_x);

        // get x coordinate on the wall texture
        int tex_x = int(wall_x * float(WALL_TEXTURE_SIZE));

        // flip texture if we see it on the other side of us, this prevents a mirrored effect for the texture
        if ((side == 0 && ray.x() <= 0) || (!side == 0 && ray.y() >= 0)) {
            tex_x = WALL_TEXTURE_SIZE - tex_x - 1;
        }
        texture_coords.x += tex_x;

        // illusion of shadows by making horizontal walls darker
        sf::Color line_color = sf::Color::White;
        if (side == 1) {
            line_color.r = line_color.r / 2;
            line_color.g = line_color.g / 2;
            line_color.b = line_color.b / 2;
        }

        lines.append(sf::Vertex(
                sf::Vector2f(x, draw_start),
                line_color,
                sf::Vector2f((float)texture_coords.x, (float)texture_coords.y + 1.0f)
        ));
        lines.append(sf::Vertex(
                sf::Vector2f(x, draw_end),
                line_color,
                sf::Vector2f((float)texture_coords.x, (float)(texture_coords.y + (float)WALL_TEXTURE_SIZE - 1.0f))
        ));
    }

    window.draw(floor_lines, floor_render_states);
    window.draw(lines, renderStates);
}

void drawMiniMap(sf::RenderWindow& window) {

}

int main() {
    // Setting up window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Ray-Caster");
    window.setFramerateLimit(60);

    // Render lines
    sf::VertexArray wall_lines(sf::Lines, SCREEN_WIDTH);

    // Texture for walls
    sf::Texture walls_texture;
    if (!walls_texture.loadFromFile("../res/walls.png")) {
        fprintf(stderr, "Cannot open texture!\n");
        return EXIT_FAILURE;
    }
    sf::RenderStates wall_render_states(&walls_texture);

    // Render lines for floor
    sf::VertexArray floor_lines(sf::Lines, SCREEN_HEIGHT);

    // Texture for floor
    sf::Texture floor_texture;
    if (!floor_texture.loadFromFile("../res/floor.png")) {
        fprintf(stderr, "Cannot open texture!\n");
        return EXIT_FAILURE;
    }
    sf::RenderStates floor_render_states(&floor_texture);

    sf::Vector2i center_screen(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    sf::Mouse::setPosition(center_screen, window);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float deltaTimeSeconds = deltaTime.asSeconds();

        // Camera rotation with mouse
        if (window.hasFocus()) {
            sf::Vector2i rel_mouse_pos = center_screen - sf::Mouse::getPosition(window);
            int mouse_x = rel_mouse_pos.x;
            double theta = (mouse_x / double(SCREEN_WIDTH)) * M_PI;
            Eigen::Rotation2D<float> rot(theta);
            player.dir = rot * player.dir;
            player.screen = rot * player.screen;
            sf::Mouse::setPosition(center_screen, window);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                Eigen::Vector2f potential_new_loc;

                if (event.key.code == sf::Keyboard::W) {
                    potential_new_loc = player.pos + player.dir * player.movSpeed * deltaTimeSeconds;
                } else if (event.key.code == sf::Keyboard::S) {
                    potential_new_loc = player.pos - player.dir * player.movSpeed * deltaTimeSeconds;
                } else if (event.key.code == sf::Keyboard::A) {
                    Eigen::Rotation2D<float> rot(M_PI / 2.0f);
                    potential_new_loc = player.pos + rot * player.dir * player.movSpeed * deltaTimeSeconds;
                } else if (event.key.code == sf::Keyboard::D) {
                    Eigen::Rotation2D<float> rot(-M_PI / 2.0f);
                    potential_new_loc = player.pos + rot * player.dir * player.movSpeed * deltaTimeSeconds;
                }

                if (map[int(potential_new_loc.x())][int(potential_new_loc.y())] == 0) {
                    player.pos = potential_new_loc;
                }
            } else if (event.type == sf::Event::GainedFocus) {
                window.setMouseCursorVisible(false);
            } else if (event.type == sf::Event::LostFocus) {
                window.setMouseCursorVisible(true);
            }
        }

        // Clearing screen
        window.clear(sf::Color::Black);

        // Casting rays
        rayCast(window, wall_lines, wall_render_states, floor_lines, floor_render_states);

        window.display();
    }

    return 0;
}