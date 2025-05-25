#include "core/core.hpp"
#include "custom_objects.hpp"

ObjectList objects;

void init() { 
    // background color 
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClearStencil(0);
      
    // making picture color white 
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
    glPointSize(1.0); 

    textShader = initShaders("src/core/shaders/shader.vert", "src/core/shaders/text.frag");
    imageShader = initShaders("src/core/shaders/shader.vert", "src/core/shaders/image.frag");

    roboto = new Font("assets/Roboto.ttf");
} 
  
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
    for (size_t i = 0; i < objects.size(); i++) {
        if (Collision* collision = dynamic_cast<Collision*>(objects[i].get())) {
            for (size_t j = 0; j < objects.size(); j++) {
                if (i != j && objects[i]->isColliding(*objects[j])) {
                    collision->setCollisionDirection(*objects[i], *objects[j]);
                }
            }
        }
        objects[i]->update();
    }

    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }
 
    roboto->renderSentence("Hello World", 48, Vector(0, 0), GREEN);

    glutSwapBuffers();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
} 
 
int main(int argc, char** argv) { 
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL | GLUT_DEPTH); 
      
    // giving window size in X- and Y- directon 
    glutInitWindowSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT); 
    glutInitWindowPosition(0, 0); 
    glutCreateWindow("Game Engine Core Testing"); 

    glewInit();
    init(); 

    Texture* rickroll = new Texture(loadBMPTexture("assets/rickroll.bmp"));

    objects.push_back(std::make_unique<MyCircle>(MyCircle(RESOLUTION_WIDTH / 3, 0, 50, rickroll)));
    objects[0]->setCenterY(RESOLUTION_HEIGHT / 2);
    objects.push_back(std::make_unique<Rectangle>(Rectangle(RESOLUTION_WIDTH / 3 * 2, 0, 200, 100, rickroll)));
    objects[1]->setCenterY(RESOLUTION_HEIGHT / 2);
    objects.push_back(std::make_unique<MyRectangle>(MyRectangle(0, RESOLUTION_HEIGHT - 50, RESOLUTION_WIDTH, 50, RED)));
    objects[2]->setCenterX(RESOLUTION_WIDTH / 2);
    objects[2]->setCenterY(RESOLUTION_HEIGHT - 200);
     
    glutDisplayFunc(display); 
    glutMainLoop(); 

    delete roboto;
    delete rickroll;
    
    return 0;
} 
