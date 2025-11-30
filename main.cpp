#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>

const int WIN_WIDTH = 1156;
const int WIN_HEIGHT = 650;
const int JUMLAH_PARTIKEL = 20;

int randomRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

class Particle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;

    // Constructor
    Particle(float r, sf::Vector2f pos, sf::Vector2f vel, sf::Color color) 
        : radius(r), velocity(vel) 
    {
        shape.setRadius(r);
        shape.setOrigin(r, r); 
        shape.setPosition(pos);
        shape.setFillColor(color);
    }

    void update() {
        shape.move(velocity);
        sf::Vector2f pos = shape.getPosition();

        if (pos.x - radius < 0) {
            shape.setPosition(radius, pos.y);
            velocity.x = std::abs(velocity.x);
        } 
        else if (pos.x + radius > WIN_WIDTH) {
            shape.setPosition(WIN_WIDTH - radius, pos.y);
            velocity.x = -std::abs(velocity.x);
        }
        if (pos.y - radius < 0) {
            shape.setPosition(pos.x, radius);
            velocity.y = std::abs(velocity.y);
        } 
        else if (pos.y + radius > WIN_HEIGHT) {
            shape.setPosition(pos.x, WIN_HEIGHT - radius);
            velocity.y = -std::abs(velocity.y);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

void checkCollision(Particle& a, Particle& b) {
    sf::Vector2f posA = a.shape.getPosition();
    sf::Vector2f posB = b.shape.getPosition();

    float dx = posB.x - posA.x;
    float dy = posB.y - posA.y;
    
    float distance = std::sqrt(dx*dx + dy*dy);
    float minDistance = a.radius + b.radius;

    // Overlap Mitigation
    if (distance < minDistance) {
        float overlap = (minDistance - distance) / 2.0f;
        float nx = dx / distance;
        float ny = dy / distance;

        a.shape.move(-nx * overlap, -ny * overlap);
        b.shape.move(nx * overlap, ny * overlap);

        std::swap(a.velocity, b.velocity);
    }
}

// Partikel Generator
Particle createRandomParticle() {
    float r = randomRange(10, 25);
    
    float x = randomRange(r * 2, WIN_WIDTH - r * 2);
    float y = randomRange(r * 2, WIN_HEIGHT - r * 2);
    float vx = (randomRange(-100, 100)) / 25.0f; 
    float vy = (randomRange(-100, 100)) / 25.0f;
    
    sf::Color color(rand() % 255, rand() % 255, rand() % 255);

    return Particle(r, sf::Vector2f(x, y), sf::Vector2f(vx, vy), color);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Particle Collision Simulation");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    for (int i = 0; i < JUMLAH_PARTIKEL; i++) {
        particles.push_back(createRandomParticle());
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                Particle newP = createRandomParticle();
                newP.shape.setPosition(event.mouseButton.x, event.mouseButton.y);
                particles.push_back(newP);
            }
        }

        // Update
        for (auto& p : particles) {
            p.update();
        }
        // Cek collision 
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                checkCollision(particles[i], particles[j]);
            }
        }

        // Rendering
        window.clear(sf::Color(20, 20, 30)); 
        for (auto& p : particles) {
            p.draw(window);
        }
        window.display();
    }

    return 0;
}