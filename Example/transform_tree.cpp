#include "initShaders.h"
#include <cstdlib>
using namespace std;

#include <iostream>
#include "Chest.h"
using std::cout;

// Compile with g++ transform_tree.cpp -lSDL2 -lglut -lGL -lGLEW -g ; ./a.out

void rotate(GLuint locate);

GLuint program;

GLfloat pit, yaw;
GLfloat scalar = 24;
glm::vec3 cubeTran;

GLuint vertexBuffer;
GLuint uvbuffer;
GLuint normalbuffer;

void init(){
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ChestVerts), ChestVerts, GL_STATIC_DRAW);

	// VERTS
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_TRUE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );
    // UV
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );
    // NORMALS
	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

	glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ChestTexCoords), ChestTexCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ChestNormals), ChestNormals, GL_STATIC_DRAW);

	ShaderInfo shaders[]={
		{ GL_VERTEX_SHADER , "vertexshader.glsl"},
		{ GL_FRAGMENT_SHADER , "fragmentshader.glsl"}, 
		{ GL_NONE , NULL} 
	};


	program = initShaders(shaders);
}


void display(SDL_Window* screen){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glm::mat4 trans;

	trans=glm::translate(trans, cubeTran);//translate the cube
	trans=glm::rotate(trans, pit, glm::vec3(1,0,0));//rotate the cube around the x axis
	trans=glm::rotate(trans, yaw, glm::vec3(0,1,0));//rotate the cube arround the y axis
	trans=glm::scale(trans, glm::vec3(scalar));//scaling the cube

	GLint tempLoc = glGetUniformLocation(program, "modelMatrix");//Matrix that handle the transformations
	glUniformMatrix4fv(tempLoc, 1, GL_FALSE, &trans[0][0]);

	//glDrawElements(GL_POLYGON,24,GL_UNSIGNED_BYTE,NULL);

    glDrawArrays(GL_TRIANGLES, 0, ChestNumVerts);

	glFlush();
	SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){

	SDL_Event event;

	while (SDL_PollEvent(&event)){//Handling the keyboard
		switch (event.type){
			case SDL_QUIT:exit(0);break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:exit(0);
					case SDLK_w:cubeTran.y+=2;break;
					case SDLK_s:cubeTran.y-=2;break;
					case SDLK_a:cubeTran.x-=2;break;
					case SDLK_d:cubeTran.x+=2;break;
					case SDLK_e:
						scalar *= 0.9f;
						cout << "Scale: " << scalar << std::endl;
						break;
					case SDLK_q:
						scalar *= 1.1f;
						cout << "Scale: " << scalar << std::endl;
						break;
					case SDLK_i:pit+=2;break;
					case SDLK_k:pit-=2;break;
					case SDLK_j:yaw+=2;break;
					case SDLK_l:yaw-=2;break;
				}
		/*case SDL_MOUSEMOTION:
				yaw+=((event.motion.x)-300)/10.0;
				pit+=((event.motion.y)-300)/10.0;
				SDL_WarpMouseInWindow(screen,300,300);
				
			}
			*/
		}
	}
}


int main(int argc, char **argv){
	
	//SDL window and context management
	SDL_Window *window;
	
	if(SDL_Init(SDL_INIT_VIDEO)<0){//initilizes the SDL video subsystem
		fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);//die on error
	}

	//create window
	window = SDL_CreateWindow(
		"Chest", //Window title
		SDL_WINDOWPOS_UNDEFINED, //initial x position
		SDL_WINDOWPOS_UNDEFINED, //initial y position
		800,							//width, in pixels
		600,							//height, in pixels
		SDL_WINDOW_OPENGL	//flags to be had
	);
	
	//check window creation
	if(window==NULL){
		fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
	}
	

	//creates opengl context associated with the window
	SDL_GLContext glcontext=SDL_GL_CreateContext(window);
	
	//initializes glew
	glewExperimental=GL_TRUE;
	if(glewInit()){
		fprintf(stderr, "Unable to initalize GLEW");
		exit(EXIT_FAILURE);
	}

	init();

	while(true){
		input(window);//keyboard controls
		display(window);//displaying
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
 
  return 0;
}
