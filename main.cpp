#include <GL/glut.h>

GLfloat ASPECTO, ANGULO;
GLfloat obsX, obsY, obsZ, rotX, rotY;
GLfloat obsX_ini, obsY_ini, obsZ_ini, rotX_ini, rotY_ini;
GLfloat escalaX, escalaY, escalaZ;
int x_ini,y_ini, bot;
#define SENS_ROT 10.0
#define SENS_OBS 10.0
#define SENS_TRANS 10.0
#define ALTURA 4.0

static int slices = 40;
static int stacks = 40;
static double t = 0;
static double a = 0;

static bool flagS = false;
static bool flagT = false;
static bool flagC = false;
static bool flagK = true;

void init(){
    glClearColor (0.0, 1.0, 1.0, 0.5);
    //glMatrixMode (GL_PROJECTION);
    //glLoadIdentity ();
    //glOrtho(-40.0, 40.0, -40.0, 40.0, -40.0, 40.0);
    ANGULO = 45;
    rotX = rotY = 0;
    obsX = obsY = 0;
    obsZ = 20;//Voltar para 10
    escalaX = escalaY = escalaZ = 1;
}

void ativaIluminacao (void){
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};// "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    //Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    //Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    //Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
    //Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);
}

void desativaIluminacao(void){
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
}

void posicionaObservador (void) {
    glMatrixMode (GL_MODELVIEW);/*Coordenadas na matrix de visualização*/
    glLoadIdentity();
    glTranslatef(-obsX, -obsY, -obsZ);/*Translata a câmera para essas variáveis*/
    glRotatef(rotX,1,0,0);/*Rotacionar a câmera para essas coordenadas*/
    glRotatef(rotY,0,1,0);
}

void motion(int x, int y){
    if(bot == GLUT_LEFT_BUTTON){//Rotação
        int deltaX = x_ini - x;
        int deltaY = y_ini - y;
        rotX = rotX_ini - deltaY/ SENS_ROT;
        rotY = rotY_ini - deltaX/ SENS_ROT;
    }
    else if (bot == GLUT_RIGHT_BUTTON){//Zoom
        int deltaZ = y_ini - y;
        obsZ = obsZ_ini + deltaZ/ SENS_OBS;
    }
    else if (bot == GLUT_MIDDLE_BUTTON){//Correr
        int deltaX = x_ini - x;
        int deltaY = y_ini - y;
        obsX = obsX_ini + deltaX/ SENS_TRANS;
        obsY = obsY_ini + deltaY/ SENS_TRANS;
    }
    posicionaObservador();
    glutPostRedisplay();
}

void especificaParametrosVisuais (void){
    glMatrixMode(GL_PROJECTION);/*Modo de visualização da matriz, Projeção*/
    glLoadIdentity();
    gluPerspective (ANGULO, ASPECTO, 0.5, 500);
    posicionaObservador();
}

void redesenhaPrimitivas(GLsizei largura, GLsizei altura){
    if (altura == 0)
        altura = 1;
    glViewport (0, 0, largura, altura);/*Dimensiona o ViewPort*/
    ASPECTO = (GLfloat) largura/ (GLfloat) altura;/*Calcula a correção de aspecto*/
    especificaParametrosVisuais();
}

void mouse(int botao, int estado, int x, int y){
    if(estado == GLUT_DOWN){
        x_ini = x;
        y_ini = y;
        obsX_ini = obsX;
        obsY_ini = obsY;
        obsZ_ini = obsZ;
        rotX_ini = rotX;
        rotY_ini = rotY;
        bot=botao;
    }
    else
        bot = -1;
}

static void key(unsigned char key, int x, int y){
    switch (key){
    case 27 :
    case 'q':
        exit(0);
        break;
    case '+':
        slices++;
        stacks++;
        break;
    case '-':
        if (slices>3 && stacks>3){
            slices--;
            stacks--;
        }
        break;
    case 's':
        if (flagS){
            flagS = false;
            break;
        }
        else{
            flagS = true;
            break;
        }
    case 't':
        if (flagT){
            flagT = false;
            break;
        }
        else{
            flagT = true;
            break;
        }
    case 'c':
        if (flagC){
            flagC = false;
            break;
        }
        else{
            flagC = true;
            break;
        }
    case 'k':
        if (flagK){
            flagK = false;
            break;
        }
        else{
            flagK = true;
            break;
        }
    }
    glutPostRedisplay();
}


void displayCube(int i){

    //fundo
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0 + (ALTURA * i),-4.0);
    glVertex3f(0.0,4.0 + (ALTURA * i),-4.0);
    glVertex3f(4.0,4.0 + (ALTURA * i),-4.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),-4.0);
    glEnd();
    glPopMatrix();

    //janela
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.5,2.0 + (ALTURA * i),0.1);
    glVertex3f(0.5,3.0 + (ALTURA * i),0.1);
    glVertex3f(3.5,3.0 + (ALTURA * i),0.1);
    glVertex3f(3.5,2.0 + (ALTURA * i),0.1);
    glEnd();
    glPopMatrix();

    //linha da porta
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_LINES);
    glVertex3f(2.0, 0.0 + (ALTURA * i), 0.2);
    glVertex3f(2.0, 1.0 + (ALTURA * i), 0.2);
    glEnd();
    glPopMatrix();

    //linha da janela vertical
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_LINES);
    glVertex3f(2.0, 2.0 + (ALTURA * i), 0.2);
    glVertex3f(2.0, 3.0 + (ALTURA * i), 0.2);
    glEnd();
    glPopMatrix();

    if (i != 0) {
        //sacada chão
        glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glTranslated(-2,-2,0);
        glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0 + (ALTURA * i),0.0);
        glVertex3f(4.0,0.0 + (ALTURA * i),0.0);
        glVertex3f(4.0,0.0 + (ALTURA * i),1.0);
        glVertex3f(0.0,0.0 + (ALTURA * i),1.0);
        glEnd();
        glPopMatrix();

        //sacada frente
        glPushMatrix();
        //color red 0.647059 green 0.164706 blue 0.164706
        glColor4f(0.647059, 0.164706, 0.164706, 0.5);
        glTranslated(-2,-2,0);
        glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0 + (ALTURA * i),1.0);
        glVertex3f(0.0,0.5 + (ALTURA * i),1.0);
        glVertex3f(4.0,0.5 + (ALTURA * i),1.0);
        glVertex3f(4.0,0.0 + (ALTURA * i),1.0);
        glEnd();
        glPopMatrix();

        //lateral sacada
        glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glTranslated(-2,-2,0);
        glBegin(GL_POLYGON);
        glVertex3f(0.0,0.5 + (ALTURA * i),1.0);
        glVertex3f(0.0,0.5 + (ALTURA * i),0.0);
        glVertex3f(0.0,0.0 + (ALTURA * i),0.0);
        glVertex3f(0.0,0.0 + (ALTURA * i),1.0);
        glEnd();
        glPopMatrix();

        //lateral sacada
        glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glTranslated(-2,-2,0);
        glBegin(GL_POLYGON);
        glVertex3f(4.0,0.5 + (ALTURA * i),1.0);
        glVertex3f(4.0,0.5 + (ALTURA * i),0.0);
        glVertex3f(4.0,0.0 + (ALTURA * i),0.0);
        glVertex3f(4.0,0.0 + (ALTURA * i),1.0);
        glEnd();
        glPopMatrix();
    }

    //linha da janela horizontal
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_LINES);
    glVertex3f(0.5, 2.5 + (ALTURA * i), 0.2);
    glVertex3f(3.5, 2.5 + (ALTURA * i), 0.2);
    glEnd();
    glPopMatrix();

    //porta
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(1.5,0.0 + (ALTURA * i),0.1);
    glVertex3f(1.5,1.0 + (ALTURA * i),0.1);
    glVertex3f(2.5,1.0 + (ALTURA * i),0.1);
    glVertex3f(2.5,0.0 + (ALTURA * i),0.1);
    glEnd();
    glPopMatrix();

    //frente
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0 + (ALTURA * i),0.0);
    glVertex3f(0.0,4.0 + (ALTURA * i),0.0);
    glVertex3f(4.0,4.0 + (ALTURA * i),0.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),0.0);

    glEnd();
    glPopMatrix();


    //base
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0 + (ALTURA * i),0.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),0.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),-4.0);
    glVertex3f(0.0,0.0 + (ALTURA * i),-4.0);
    glEnd();
    glPopMatrix();

   /* //topo
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0,4.0 + (ALTURA * i),-4.0);
        glVertex3f(0.0,4.0 + (ALTURA * i),0.2);
        glVertex3f(4.0,4.0 + (ALTURA * i),0.2);
        glVertex3f(4.0,4.0 + (ALTURA * i),-4.0);
    glEnd();
    glPopMatrix();*/

    //lateral esquerda
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,4.0 + (ALTURA * i),-4.0);
    glVertex3f(0.0,4.0 + (ALTURA * i),0.0);
    glVertex3f(0.0,0.0 + (ALTURA * i),0.0);
    glVertex3f(0.0,0.0 + (ALTURA * i),-4.0);
    glEnd();
    glPopMatrix();

    //lateral direita
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(4.0,4.0 + (ALTURA * i),-4.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),-4.0);
    glVertex3f(4.0,0.0 + (ALTURA * i),0.0);
    glVertex3f(4.0,4.0 + (ALTURA * i),0.0);
    glEnd();
    glPopMatrix();
}

void display(void){
    //Rotina principal de desenho
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ativaIluminacao();

    //chão
    glPushMatrix();
    glColor3f(0.647059, 0.164706, 0.164706);
    glTranslated(-2,-2,0);
    glBegin(GL_POLYGON);
    glVertex3f(-1000.0,-0.000001,1000.0);
    glVertex3f(1000.0,-0.000001,1000.0);
    glVertex3f(1000.0,-0.000001,-1000.0);
    glVertex3f(-1000.0,-0.000001,-1000.0);
    glEnd();
    glPopMatrix();

    if(flagK){
        for (int i = 0; i < 10; i++) {
             displayCube(i);
        }
    }
    glutSwapBuffers();
}

static void idle(void){
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //Tempo randomico
    a = t*90.0; //angulo randomico
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    //glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(0,0);
    glutCreateWindow("simple");
    glutDisplayFunc(display);
    glutReshapeFunc(redesenhaPrimitivas);/*Redesenho na tela*/
    glutMouseFunc(mouse);/*Rotina do mouse*/
    glutMotionFunc(motion);/*Rotina do movimento*/
    glutIdleFunc(idle); /*Rotina de idle*/
    glutKeyboardFunc(key); /*Rotina de teclado*/
    init();
    glutMainLoop();
    return 0;
}
