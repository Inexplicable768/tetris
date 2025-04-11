#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>

using namespace std;

const int bx = 10;
const int by = 20;
const int width = 600;
const int height = 800;
const string font_name = "Arial.ttf";

struct Point {int x, y; };

// 4 points of the tetrimino locations
Point tetr_pos[4];
// copy for mapping
Point tetr_pos1[4];

int blocks[7][4] = {
    {1,3,5,7}, // I
    {2,4,5,7}, // Z
    {3,5,4,6}, // S
    {3,5,4,7}, // T
    {2,3,5,7}, // L
    {3,5,7,6}, // J
    {2,3,4,5}, // O
};
int grid[by][bx] = {0};
bool gameOver = false;

bool check() { // check if tile is out of bounds x or y
    for (int i = 0; i < 4; i++)
        if (tetr_pos[i].x < 0 || tetr_pos[i].x >= bx || tetr_pos[i].y >= by || grid[tetr_pos[i].y][tetr_pos[i].x])
            return false;
    return true;
}

void rotate_pieces() { // rotate the matrix
    Point p = tetr_pos[1]; // start of array
    for (int i = 0; i < 4; i++) {
        int x = tetr_pos[i].y - p.y;
        int y = tetr_pos[i].x - p.x;
        tetr_pos[i].x = p.x - x;
        tetr_pos[i].y = p.y + y;
    }
    if (!check()) for (int i = 0; i < 4; i++) tetr_pos[i] = tetr_pos1[i];

}
void placeText(sf::Text& text, string value) {
   // text.setFont(font_name);
    text.setString(value);
    text.setCharacterSize(48);     // in pixels
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f); // x, y coordinates


}

int main(int argc, char* argv[]) { // game loop //
    srand(time(0)); // int rng
    sf::RenderWindow window(sf::VideoMode(width, height), "Tetris game - c++ SFML");
    sf::Texture tiles;
    tiles.loadFromFile("Textures/tetris_tiles.png");
    sf::Sprite s(tiles);
    bool rotate = true;

    int dx = 0, colorNum = 1; // movement x
    float timer = 0, delay = 0.3;
    sf::Clock clock;
    int score = 0;
    
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        sf::Event e;
        
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Left) dx = -1;
                else if (e.key.code == sf::Keyboard::Right) dx = 1;
                else if (e.key.code == sf::Keyboard::Up) rotate_pieces();
                else if (e.key.code == sf::Keyboard::Down) delay = 0.05;
            }
        }

        if (timer > delay) { // tick move down
            for (int i = 0; i < 4; i++) { tetr_pos1[i] = tetr_pos[i]; tetr_pos[i].y += 1; }
            if (!check()) {
                for (int i = 0; i < 4; i++) grid[tetr_pos1[i].y][tetr_pos1[i].x] = colorNum;
                int n = rand() % 7;
                colorNum = 1 + rand() % 7;
                for (int i = 0; i < 4; i++) {
                    // set positions to move down
                    tetr_pos[i].x = blocks[n][i] % 2 + bx / 2 - 1;
                    tetr_pos[i].y = blocks[n][i] / 2;
                }
                if (!check()) gameOver = true;
            }
            timer = 0; // reset timer
        }

        // move peices
        for (int i = 0; i < 4; i++) { tetr_pos1[i] = tetr_pos[i]; tetr_pos[i].x += dx; }
        if (!check()) for (int i = 0; i < 4; i++) tetr_pos[i] = tetr_pos1[i];

        dx = 0; rotate = false; delay = 0.3; // set new before loop again
        window.clear(sf::Color::White);
        // draw peices //
        
        for (int i = 0; i < by; i++) {
            for (int j = 0; j < bx; j++) {
                if (grid[i][j]) {
                    s.setTextureRect(sf::IntRect(grid[i][j] * 18, 0, 18, 18)); // which texture number
                    s.setPosition(j * 18, i * 18);
                    window.draw(s);
                }
            }
        }
        for (int i = 0; i < 4; i++) {
            s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(tetr_pos[i].x * 18, tetr_pos[i].y * 18);
            window.draw(s);
        }
        // clear lines //
        
        sf::Font font; // init font
        if (!font.loadFromFile(font_name)) {
            //cout << "Error: font failed to load";
            return -1;
        }
         sf::Text text;
         placeText(text, "Score:" + to_string(score));
        
        window.display();

        
         if (gameOver) { // game over menu
            sf::RectangleShape bg_rect(sf::Vector2f(width, 100.f));
            bg_rect.setPosition(0.f, width / 2);
            bg_rect.setFillColor(sf::Color::Green);
            
            break; // exit game loop
        }



        }
        window.close();
    
        return 0;

    }