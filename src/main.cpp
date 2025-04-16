#include "core/core.hpp"
#include "custom_objects.hpp"

ObjectList objects;

void init() { 
    // background color 
    glClearColor(0.0, 0.0, 0.0, 1.0); 
      
    // making picture color green 
    glColor3f(0.0, 1.0, 0.0); 
      
    // breadth of picture boundary is 1 pixel 
    glPointSize(1.0); 
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
      
    // setting window dimension in X- and Y- direction 
    gluOrtho2D(0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0); 
} 
  
void display() {
    glClear(GL_COLOR_BUFFER_BIT); 
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw();
        if (objects[i]->isColliding(objects, i)) {
            std::cout << "Collision detected with object: " << objects[i]->toString() << std::endl;
        }
    }

    for (auto& object : objects) {
        object->update();
    }
    glFlush();
    glutPostRedisplay();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
} 
  
int main(int argc, char** argv) { 
    objects.push_back(std::make_unique<MyCircle>(MyCircle(RESOLUTION_WIDTH / 3, 0, 50)));
    objects[0]->setCenterY(RESOLUTION_HEIGHT / 2);
    objects.push_back(std::make_unique<Rectangle>(Rectangle(RESOLUTION_WIDTH / 3 * 2, 0, 200, 100)));
    objects[1]->setCenterY(RESOLUTION_HEIGHT / 2);
    objects.push_back(std::make_unique<Rectangle>(Rectangle(0, RESOLUTION_HEIGHT - 50, RESOLUTION_WIDTH, 50)));
    objects[2]->setCenterX(RESOLUTION_WIDTH / 2);
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
      
    // giving window size in X- and Y- directon 
    glutInitWindowSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT); 
    glutInitWindowPosition(0, 0); 
      
    // Giving name to window 
    glutCreateWindow("Game Engine Core Testing"); 
    init(); 
      
    glutDisplayFunc(display); 
    glutMainLoop(); 
    
    return 0;
} 