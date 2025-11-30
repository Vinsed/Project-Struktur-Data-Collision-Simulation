#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <ctime>

// Struktur data untuk partikel
struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;
};

// Fungsi untuk menghasilkan warna acak
sf::Color getRandomColor() {
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);
}

// Random position generator
sf::Vector2f getRandomPosition(int width, int height, float radius) {
    sf::Vector2f pos;
    pos.x = radius + rand() % (width - (int)(radius * 2));
    pos.y = radius + rand() % (height - (int)(radius * 2));
    return pos;
}

int main() {
    srand(time(0));
    
    int width = 1200;
    int height = 800;
    float radius = 20.0f;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "Particle Sim - Left Click to add");
    window.setFramerateLimit(60);
    
    std::vector<Particle> particles;
    
    // bikin partikel awal, jumlahnya random
    int startCount = 5 + rand() % 11;
    for (int i = 0; i < startCount; i++) {
        Particle p;
        p.pos = getRandomPosition(width, height, radius);
        
        // kasih kecepatan random
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 2.0f;
        p.vel.x = cos(angle) * speed;
        p.vel.y = sin(angle) * speed;
        
        p.color = getRandomColor();
        particles.push_back(p);
    }
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // tambah partikel kalo klik kiri mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Particle p;
                    p.pos.x = event.mouseButton.x;
                    p.pos.y = event.mouseButton.y;
                    
                    float angle = (rand() % 360) * 3.14159f / 180.0f;
                    float speed = 2.0f;
                    p.vel.x = cos(angle) * speed;
                    p.vel.y = sin(angle) * speed;
                    
                    p.color = getRandomColor();
                    particles.push_back(p);
                }
            }
        }
        
        // update posisi semua partikel
        for (auto& p : particles) {
            p.pos.x += p.vel.x;
            p.pos.y += p.vel.y;
            
            // pantul kalo nabrak dinding
            if (p.pos.x < radius || p.pos.x > width - radius)
                p.vel.x *= -1;
            if (p.pos.y < radius || p.pos.y > height - radius)
                p.vel.y *= -1;
        }
        
        // gambar semuanya
        window.clear(sf::Color(25, 25, 35));
        
        for (auto& p : particles) {
            sf::CircleShape ball(radius);
            ball.setPosition(p.pos.x - radius, p.pos.y - radius);
            ball.setFillColor(p.color);
            window.draw(ball);
        }
        
        window.display();
    }
    
    return 0;
}