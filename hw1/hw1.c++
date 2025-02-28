#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include <algorithm>

using namespace std;

class Shape {
    protected:
        float x, y, z_order;
        string name;

    public:
        Shape(float x, float y, string name, float z_order = 0.0) : x(x), y(y), name(name), z_order(z_order) {}

        virtual void draw() const = 0;
        virtual ~Shape() {}

        float getZOrder() const {
            return z_order;
        }
};

class Circle : public Shape {
    private:
        float radius;

    public:
        Circle(float x, float y, float radius, float z_order = 0.0) : Shape(x, y, "Circle", z_order), radius(radius) {}

        void draw() const override {
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(x, y);
                for (int i = 0; i <= 360; i++) {
                    float theta = i * 3.14159 / 180;
                    glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
                }
            glEnd();
        }
};

class Triangle : public Shape {
    private:
        float size;

    public:
        Triangle(float x, float y, float size, float z_order = 0.0) : Shape(x, y, "Triangle", z_order), size(size) {}

        void draw() const override {
            float halfSize = size / 2;
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y + halfSize);
                glVertex2f(x - halfSize, y - halfSize);
                glVertex2f(x + halfSize, y - halfSize);
            glEnd();
        }
};

class Square : public Shape {
    private:
        float side;

    public:
        Square(float x, float y, float side, float z_order = 0.0) : Shape(x, y, "Square", z_order), side(side) {}

        void draw() const override {
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + side, y);
                glVertex2f(x + side, y + side);
                glVertex2f(x, y + side);
            glEnd();
        }
};

vector<Shape*> shapes;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    sort(shapes.begin(), shapes.end(), [](Shape* a, Shape* b) {
        return a->getZOrder() < b->getZOrder();
    });

    for (const auto& shape : shapes) {
        shape->draw();
    }

    glFlush();
}


void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

void reshape(int width, int height) {
    if (width != 500 || height != 500) {
        glutReshapeWindow(500, 500);
    }
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL Shapes");

    initOpenGL();

    shapes.push_back(new Circle(200, 200, 50, 1.0));
    shapes.push_back(new Triangle(300, 300, 80, 0.5));
    shapes.push_back(new Square(400, 400, 60, 0.0));

    cout << "Number of shapes: " << shapes.size() << endl;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    for (auto& shape : shapes) {
        delete shape;
    }

    return 0;
}
