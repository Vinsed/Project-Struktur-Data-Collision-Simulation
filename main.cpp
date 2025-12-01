#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include <memory>
#include <string>
#include <iostream>

// Particle Structure
struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;
    float radius;
};

// Quadtree Boundary
struct Rect {
    float x, y, w, h;

    bool contains(const Particle& p) const {
        return (p.pos.x >= x && p.pos.x <= x + w && p.pos.y >= y && p.pos.y <= y + h);
    }
    bool intersects(const Rect& r) const {
        return !(r.x > x + w || r.x + r.w < x || r.x + r.h < y || r.y + r.h < y);
    }
};

// Quadtree Class
class QuadTree {
    Rect boundary;
    std::vector<Particle*> items;
    bool divided = false;
    std::unique_ptr<QuadTree> nw, ne, sw, se;

public:
    QuadTree(Rect r) : boundary(r) {}

    void subdivide() {
        float x = boundary.x, y = boundary.y, w = boundary.w / 2, h = boundary.h / 2;
        nw = std::make_unique<QuadTree>(Rect{x, y, w, h});
        ne = std::make_unique<QuadTree>(Rect{x + w, y, w, h});
        sw = std::make_unique<QuadTree>(Rect{x, y + h, w, h});
        se = std::make_unique<QuadTree>(Rect{x + w, y + h, w, h});
        divided = true;
    }

    bool insert(Particle* p) {
        if (!boundary.contains(*p)) return false;

        if (items.size() < 4) {
            items.push_back(p);
            return true;
        }

        if (!divided) subdivide();

        return nw->insert(p) || ne->insert(p) ||
               sw->insert(p) || se->insert(p);
    }

    void query(const Rect& range, std::vector<Particle*>& found) {
        if (!boundary.intersects(range)) return;

        for (auto p : items) {
            if (range.contains(*p)) found.push_back(p);
        }

        if (divided) {
            nw->query(range, found);
            ne->query(range, found);
            sw->query(range, found);
            se->query(range, found);
        }
    }

    void draw(sf::RenderWindow& w) {
        sf::RectangleShape box({boundary.w, boundary.h});
        box.setPosition(boundary.x, boundary.y);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color(0, 255, 0, 150));
        box.setOutlineThickness(1);
        w.draw(box);

        if (divided) {
            nw->draw(w);
            ne->draw(w);
            sw->draw(w);
            se->draw(w);
        }
    }
};

//Extra Functions
sf::Color randomColor() {
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);
}

sf::Vector2f findSafePosition(int w, int h, float r, const std::vector<Particle>& all) {
    sf::Vector2f pos;
    for (int i = 0; i < 100; i++) {
        pos.x = r * 2 + rand() % (w - (int)(r * 4));
        pos.y = r * 2 + rand() % (h - (int)(r * 4));
        bool safe = true;

        for (const auto& p : all) {
            float dx = pos.x - p.pos.x, dy = pos.y - p.pos.y;
            if (sqrt(dx * dx + dy * dy) < (r + p.radius) * 1.5f) {
                safe = false;
                break;
            }
        }
        if (safe) return pos;
    }
    return pos;
}

bool checkCollision(const Particle& a, const Particle& b) {
    float dx = a.pos.x - b.pos.x, dy = a.pos.y - b.pos.y;
    return sqrt(dx * dx + dy * dy) < (a.radius + b.radius);
}

void resolveCollision(Particle& a, Particle& b) {
    float dx = b.pos.x - a.pos.x, dy = b.pos.y - a.pos.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance == 0) return;

    float nx = dx / distance, ny = dy / distance;
    float overlap = (a.radius + b.radius - distance) / 2.0f;

    a.pos.x -= nx * overlap;
    a.pos.y -= ny * overlap;
    b.pos.x += nx * overlap;
    b.pos.y += ny * overlap;

    std::swap(a.vel, b.vel);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int width = 1156, height = 650;
    float minRadius = 10, maxRadius = 30;

    sf::RenderWindow window(sf::VideoMode(width, height), "Particle Collision Simulation");
    window.setFramerateLimit(60);

    sf::Font font;
    bool hasFont = false;
    std::string fontPath = "DejaVuSans.ttf";
    if (font.loadFromFile(fontPath)) hasFont = true;

    std::vector<Particle> particles;

    // Initial Spawn
    int initialCount = 20 + rand() % 11;
    for (int i = 0; i < initialCount; i++) {
        Particle p;
        p.radius = minRadius + rand() % (int)(maxRadius - minRadius + 1);
        p.pos = findSafePosition(width, height, p.radius, particles);
        p.vel.x = (rand() % 200 - 100) / 25.0f;
        p.vel.y = (rand() % 200 - 100) / 25.0f;
        p.color = randomColor();
        particles.push_back(p);
    }

    bool useQuadTree = false;
    bool showGrid = true;
    long long checksBF = 0, checksQT = 0;

    // ======================
    // FPS COUNTER VARIABLES
    // ======================
    sf::Clock fpsClock;
    float fps = 0;
    int frameCount = 0;
    float frameTimer = 0;

    while (window.isOpen()) {
        frameCount++;
        float dt = fpsClock.restart().asSeconds();
        frameTimer += dt;
        if (frameTimer >= 1.0f) {
            fps = frameCount / frameTimer;
            frameCount = 0;
            frameTimer = 0;
        }
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                Particle p;
                p.radius = minRadius + rand() % (int)(maxRadius - minRadius + 1);
                p.pos = { (float)e.mouseButton.x, (float)e.mouseButton.y };
                p.vel.x = (rand() % 200 - 100) / 25.0f;
                p.vel.y = (rand() % 200 - 100) / 25.0f;
                p.color = randomColor();
                particles.push_back(p);
            }

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::T) useQuadTree = !useQuadTree;
                if (e.key.code == sf::Keyboard::G) showGrid = !showGrid;
            }
        }

        // Update Physics
        for (auto& p : particles) {
            p.pos += p.vel;

            if (p.pos.x - p.radius <= 0) { p.pos.x = p.radius + 0.1f; p.vel.x = std::abs(p.vel.x); }
            if (p.pos.x + p.radius >= width) { p.pos.x = width - p.radius - 0.1f; p.vel.x = -std::abs(p.vel.x); }
            if (p.pos.y - p.radius <= 0) { p.pos.y = p.radius + 0.1f; p.vel.y = std::abs(p.vel.y); }
            if (p.pos.y + p.radius >= height) { p.pos.y = height - p.radius - 0.1f; p.vel.y = -std::abs(p.vel.y); }

            float minSpeed = 0.5f;
            if (std::abs(p.vel.x) < minSpeed && std::abs(p.vel.y) < minSpeed) {
                p.vel.x = (rand() % 2 ? 1 : -1) * (minSpeed + (rand() % 100) / 100.0f);
                p.vel.y = (rand() % 2 ? 1 : -1) * (minSpeed + (rand() % 100) / 100.0f);
            }
        }

        checksBF = checksQT = 0;

        if (useQuadTree) {
            QuadTree tree(Rect{0, 0, (float)width, (float)height});
            for (auto& p : particles) tree.insert(&p);

            for (size_t i = 0; i < particles.size(); i++) {
                float r = particles[i].radius;
                Rect searchArea = { particles[i].pos.x - r * 2, particles[i].pos.y - r * 2, r * 4, r * 4 };
                std::vector<Particle*> nearby;

                tree.query(searchArea, nearby);
                checksQT += nearby.size();

                for (auto* other : nearby) {
                    if (&particles[i] < other) {
                        checksBF++;
                        if (checkCollision(particles[i], *other)) {
                            resolveCollision(particles[i], *other);
                        }
                    }
                }
            }

            window.clear(sf::Color(25, 25, 35));
            if (showGrid) tree.draw(window);

        } else {
            for (size_t i = 0; i < particles.size(); i++) {
                for (size_t j = i + 1; j < particles.size(); j++) {
                    checksBF++;
                    if (checkCollision(particles[i], particles[j])) {
                        resolveCollision(particles[i], particles[j]);
                    }
                }
            }
            window.clear(sf::Color(25, 25, 35));
        }

        for (auto& p : particles) {
            sf::CircleShape circle(p.radius);
            circle.setPosition(p.pos.x - p.radius, p.pos.y - p.radius);
            circle.setFillColor(p.color);
            window.draw(circle);
        }
        if (hasFont) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(10, 10);

            std::string info = "Click: Add | T: Toggle Algo | G: Grid\n";
            info += "Particles: " + std::to_string(particles.size()) + "\n";
            info += "Mode: " + std::string(useQuadTree ? "QUADTREE" : "BRUTE FORCE") + "\n";
            info += "Grid: " + std::string(showGrid ? "ON" : "OFF") + "\n";
            info += "QuadTree Checks: " + std::to_string(checksQT) + "\n";
            info += "BruteForce Checks: " + std::to_string(checksBF) + "\n";
            info += "FPS: " + std::to_string((int)fps);

            text.setString(info);

            sf::RectangleShape bg({200, 110});
            bg.setPosition(5, 5);
            bg.setFillColor(sf::Color(0, 0, 0, 150));

            window.draw(bg);
            window.draw(text);
        }

        window.display();
    }

    return 0;
}
