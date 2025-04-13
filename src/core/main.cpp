#include "core.hpp"

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
    gluOrtho2D(-780, 780, -420, 420); 
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
} 
  
int main(int argc, char** argv) { 
    objects.push_back(std::make_unique<Circle>(Circle(0, 0, 50)));
    objects.push_back(std::make_unique<Rectangle>(Rectangle(100, 0, 200, 100)));
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
      
    // giving window size in X- and Y- directon 
    glutInitWindowSize(1366, 768); 
    glutInitWindowPosition(0, 0); 
      
    // Giving name to window 
    glutCreateWindow("Game Engine Core Testing"); 
    init(); 
      
    glutDisplayFunc(display); 
    glutMainLoop(); 
    
    return 0;
} 