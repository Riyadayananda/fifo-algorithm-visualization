#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<GL/freeglut.h>

int frames[3] = {0, 0, 0}; //Frames for animation
int page = 0;
int dx = 0, dy = 0;

int page_seq[20]; //= {1,3,1,4,2,3,1,2,5,6}; //Page Reference Sequence
int n; //Page Reference sequence input
int replacement[20]; //Replacement positions 
int page_faults; //Page fault count
int page_hits; //Page hit count

int isHit(int frames[], int page)
{
    for(int i=0;i<3;i++)    
        if(frames[i] == page) 
            return 1;

    return 0;
}

void fifo()
{   
    int frames[3];
    int first = -1;
    page_faults = 0;
    page_hits = 0;

    for(int i=0;i<3;i++)    
        frames[i] = -1;

    for(int i=0;i<n;i++)    
    {
        if(isHit(frames, page_seq[i]))
        {
            replacement[i] = -1;
            page_hits++;
        }
        else
        {   
            page_faults++;
            first = (first + 1) % 3;
            frames[first] =  page_seq[i];
            replacement[i] = first;
        }
    }
}


void draw_string(float x, float y, float z, char *string) {
  glRasterPos3f(x, y, z);

  for (char* c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
  }
}

void draw_square(int startx, int starty, int side)
{
    /*
    Draw a square
    Params:
    startx - x coordinate of bottom left point
    stary - y coordinate of bottom left point
    side - length of side
    */
    glBegin(GL_QUADS);
        glVertex2i(startx, starty);
        glVertex2i(startx + side, starty);
        glVertex2i(startx + side, starty + side);
        glVertex2i(startx, starty + side);
    glEnd();
}

void draw_frames()
{
    //glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.5f, 1.0f);

    draw_square(230, 250, 40);
    draw_square(230, 200, 40);
    draw_square(230, 150, 40);

    glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(245,265);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, frames[0]+'0');

    glRasterPos2f(245,215);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, frames[1]+'0');

    glRasterPos2f(245,165);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, frames[2]+'0');
}

void draw_ui()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    draw_string(100.0,410.0,0.0,"INPUT: ");
    glRasterPos2f(190.0,410.0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, page_seq[page]+'0');


    draw_string(100.0,265.0,0.0,"FRAME 1");
    draw_string(100.0,215.0,0.0,"FRAME 2");
    draw_string(100.0,165.0,0.0,"FRAME 3");

    if(replacement[page] == -1)
        draw_string(100.0,80.0,0.0,"STATUS: HIT!");
    else 
        draw_string(100.0,80.0,0.0,"STATUS: FAULT!");
}

void box_animation()
{
    glColor3f(0.0f, 0.5f, 1.0f);
    draw_square(230, 395, 40);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(245,410);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, page_seq[page]+'0');

    if(replacement[page] == -1) 
    {
        dx += 1;

        if (dx > 150) 
        {
            dx = 0;
            page++;
        } 
        else 
            dx += 1;
    }
    else
    {
        if(dy < -245 && replacement[page] == 2)
        {
            dy = 0;
            frames[2] = page_seq[page];
            page++;
        }
        else if(dy < -195 && replacement[page] == 1 )
        {
            dy = 0;
            frames[1] = page_seq[page];
            page++;
        }
        else if(dy < -145 && replacement[page] == 0)
        {
            dy = 0;
            frames[0] = page_seq[page];
            page++;
        }
        else 
            dy-=1;
    }
    
    glutPostRedisplay();

}

void draw_result()
{   
    draw_string(70.0,410.0,0.0,"FIFO Page Replacement Algorithm");
        
    draw_string(160.0,330.0,0.0,"Page Faults: ");

    glRasterPos2f(290,330);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, page_faults+'0');

    draw_string(160.0,300.0,0.0,"Page Hits: ");

    glRasterPos2f(290,300);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, page_hits+'0');
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(page == n) 
    {
        draw_result();
    }
    else
    {
        // printf("\nPage: %d", page);
        Sleep(20);
        draw_frames();
        draw_ui();

        glPushMatrix();
        glTranslatef(dx,dy,0);
        box_animation();
        glPopMatrix();
    }
    glutSwapBuffers(); 
}

void init (void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

void cli_menu()
{
    printf("Page Replacement Algorithm Visualization");
    
    printf("\n\nEnter length of page reference string(max 20): ");
    scanf("%d", &n);
    if(n > 20) 
    {
        printf("\nPage reference string length should not exceed 20!\n");
        exit(0);
    }
    
    printf("\nEnter page reference string: ");
    for(int i=0;i<n;i++) 
        scanf("%d", &page_seq[i]);
    
    printf("\n\nVisualizing FIFO Page Replacement Algorithm...\n");
    fifo();
}
 
int main (int argc, char** argv)
{
    cli_menu();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("FIFO Visualization");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}