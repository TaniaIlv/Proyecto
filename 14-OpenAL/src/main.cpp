#define _USE_MATH_DEFINES
#include <cmath>

//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

//Shadow box include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>
//#include <GL/glut.h>
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para luces
Shader shaderLighting;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

ShadowBox *shadowBox;

//Models para el proyecto
Model modelLampCity;
Model characterModel;
Model enemyModel;
Model bottleModel;
Model bottle2Model;
Model virusModel;
Model cubreModel;

Terrain terrain(-1, -1, 200, 16, "../Textures/Proy/Blendmap/heightmap.png");

GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/cieloAzul/cielo.tga",
		"../Textures/cieloAzul/cielo.tga",
		"../Textures/cieloAzul/cielo3.tga",
		"../Textures/cieloAzul/cielo.tga",
		"../Textures/cieloAzul/cielo.tga",
		"../Textures/cieloAzul/cielo.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

glm::mat4 modelMatrixCharacter = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy2 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy3 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy4 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy5 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy6 = glm::mat4(1.0f);
glm::mat4 modelMatrixBottle = glm::mat4(1.0f);
glm::mat4 modelMatrixBottle2 = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus2 = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus3 = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus4 = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus5 = glm::mat4(1.0f);
glm::mat4 modelMatrixVirus6 = glm::mat4(1.0f);
glm::mat4 modelMatrixCubre = glm::mat4(1.0f);

glm::mat4 matrixPass = glm::mat4(1.0f);

int animationIndexCh = 0;
int animationIndexEn = 0;
int modelSelected = 1;
bool enableCountSelected = true;
bool dead = false;

float day = 0.6;
int stateDay = 0;
float count = 0.0;

int stateEnemy = 0;
float steps = 0.0;
int turn = 1;

int stateEnemy2 = 0;
float steps2 = 0.0;

int stateEnemy3 = 0;
float steps3 = 0.0;
int turn3 = 1;

int stateEnemy4 = 0;
float steps4 = 0.0;

int stateEnemy5 = 0;
float steps5 = 0.0;

bool touchBottle = false;
bool oneTouch = true;
bool touchBottle2 = false;
bool oneTouch2 = true;
bool touchCubre = false;
bool oneTouch3 = true;

bool oneTouchEnemy = true;
int countObjets = 0;
int countEnemies = 0;

std::vector<glm::vec3> lampCityPosition = { glm::vec3(-30, 0, 80), glm::vec3(2, 0, 48), glm::vec3(17, 0, 4),
											glm::vec3(50, 0,-22), glm::vec3(39, 0, 43), glm::vec3(86, 0, 23),
											glm::vec3(55, 0, 64) , glm::vec3(-55, 0, 37), glm::vec3(-43, 0, 3),
											glm::vec3(-80, 0, -32), glm::vec3(-38, 0, -40), glm::vec3(-22, 0, -69),
											glm::vec3(10, 0, -50), glm::vec3(41, 0, -56) };
std::vector<float> lampCityOrientation = { -17.0, -82.67, 23.70 };

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		/*{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)},
		{"fountain", glm::vec3(5.0, 0.0, -40.0)},
		{"fire", glm::vec3(0.0, 0.0, 7.0)}*/
};

double deltaTime;
double currTime, lastTime;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

// OpenAL Defines
#define NUM_BUFFERS 4
#define NUM_SOURCES 4
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
 //Source 0
ALfloat source0Pos[] = { 0.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };

// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true, true};

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
//void initParticleBuffersFire();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
//void renderDisplay();

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	
	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//model lampaoras
	modelLampCity.loadModel("../modelsProy/lampCity/lamparasCity.obj");
	modelLampCity.setShader(&shaderLighting);

	//Character
	characterModel.loadModel("../modelsProy/cn_shield/character3.fbx");
	characterModel.setShader(&shaderMulLighting);

	//Enemy
	enemyModel.loadModel("../modelsProy/dead_max_obj/infectadoAni.fbx");
	enemyModel.setShader(&shaderMulLighting);

	//bottle
	bottleModel.loadModel("../modelsProy/Bottle/bottle.obj");
	bottleModel.setShader(&shaderMulLighting);

	//bottle2
	bottle2Model.loadModel("../modelsProy/Bottle2/bottle2.obj");
	bottle2Model.setShader(&shaderMulLighting);

	//Virus
	virusModel.loadModel("../modelsProy/coronavirus_textures/corona.obj");
	virusModel.setShader(&shaderMulLighting);

	//Cubrebocas
	cubreModel.loadModel("../modelsProy/cubrebocas/cubrebocas.obj");
	cubreModel.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/Proy/Blendmap/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/Proy/Blendmap/asfalto.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/Proy/Blendmap/grava.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/Proy/Blendmap/agua.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/Proy/Blendmap/blendMapProy.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/juego.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/alert.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/alert.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/alert.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 0.1f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 0);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.3f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 500);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.3f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);

	////Que tan rapido se reproduce
	//alSourcef(source[3], AL_PITCH, 1.0f);
	////Ganancia
	//alSourcef(source[3], AL_GAIN, 1.0f);
	////La posicion
	//alSourcefv(source[3], AL_POSITION, source3Pos);
	////Velocidad
	//alSourcefv(source[3], AL_VELOCITY, source3Vel);
	////Buffer asociado
	//alSourcei(source[3], AL_BUFFER, buffer[3]);
	////Se repite
	//alSourcei(source[3], AL_LOOPING, AL_TRUE);
	////Que tanto se necesita alejar
	//alSourcef(source[3], AL_MAX_DISTANCE, 500);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();

	shaderLighting.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();


	// Terrains objects Delete
	terrain.destroy();

	//// Custom objects animate
	//mayowModelAnimate.destroy();
	characterModel.destroy();
	enemyModel.destroy();
	bottleModel.destroy();
	bottle2Model.destroy();
	virusModel.destroy();
	modelLampCity.destroy();
	cubreModel.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		dead = true;

	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		modelMatrixCharacter = glm::rotate(modelMatrixCharacter, glm::radians(1.0f), glm::vec3(0, 1, 0));
		animationIndexCh = 1;
	}
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		modelMatrixCharacter = glm::rotate(modelMatrixCharacter, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		animationIndexCh = 1;
	}if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		modelMatrixCharacter = glm::translate(modelMatrixCharacter, glm::vec3(0, 0, 0.5));
		animationIndexCh = 1;
	}
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		modelMatrixCharacter = glm::translate(modelMatrixCharacter, glm::vec3(0, 0, -0.5));
		animationIndexCh = 1;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	modelMatrixCharacter = glm::translate(modelMatrixCharacter, glm::vec3(-36.0f, 0.0f, 95.0));
	modelMatrixCharacter = glm::rotate(modelMatrixCharacter, glm::radians(150.0f), glm::vec3(0, 1, 0));
	modelMatrixEnemy = glm::translate(modelMatrixEnemy, glm::vec3(-18.0f, 0.0f, 65.0));
	modelMatrixBottle = glm::translate(modelMatrixBottle, glm::vec3(68.0f, 0.0f, 71.0));
	modelMatrixBottle2 = glm::translate(modelMatrixBottle2, glm::vec3(-50.0f, 0.0f, -26.0));
	modelMatrixVirus = glm::translate(modelMatrixVirus, glm::vec3(-37.0f, 0.0f, 85.0));
	modelMatrixCubre = glm::translate(modelMatrixCubre, glm::vec3(85.0f, 0.0f, -15.0f));

	modelMatrixEnemy2 = glm::translate(modelMatrixEnemy2, glm::vec3(-10.0f, 0.0f, 40.0));
	modelMatrixEnemy3 = glm::translate(modelMatrixEnemy3, glm::vec3(70.0f, 0.0f, 76.0));
	modelMatrixEnemy4 = glm::translate(modelMatrixEnemy4, glm::vec3(-30.0f, 0.0f, 7.0));
	modelMatrixEnemy5 = glm::translate(modelMatrixEnemy5, glm::vec3(-28.0f, 0.0f, -32.0));
	modelMatrixEnemy6 = glm::translate(modelMatrixEnemy6, glm::vec3(9.0f, 0.0f, -66.0));

	modelMatrixVirus2 = glm::translate(modelMatrixVirus2, glm::vec3(38.0f, 0.0f, 44.0));
	modelMatrixVirus3 = glm::translate(modelMatrixVirus3, glm::vec3(28.0f, 0.0f, 10.0));
	modelMatrixVirus4 = glm::translate(modelMatrixVirus4, glm::vec3(-15.0f, 0.0f, -2.0));
	modelMatrixVirus5 = glm::translate(modelMatrixVirus5, glm::vec3(35.0f, 0.0f, -26.0));
	modelMatrixVirus6 = glm::translate(modelMatrixVirus6, glm::vec3(-70.0f, 0.0f, -40.0));

	matrixPass = glm::translate(matrixPass, glm::vec3(0,0,100));

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	//Posicion light, apuntador a la camara, distancia maxima de alcance, plano cercano y el FOV(Field og view)
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1, 45);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		if (modelSelected == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixCharacter));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixCharacter));
			target = modelMatrixCharacter[3];
		}
		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(0.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		

		//Se actualiza la caja usando las dimenciones del viewport
		shadowBox->update(screenWidth, screenHeight);
		shadowBox->getCenter();

		// Projection light shadow mapping
		/*glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 20.0f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));*/
		glm::mat4 lightProjection = glm::mat4(1.0), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0 / shadowBox->getWidth();
		lightProjection[1][1] = 2.0 / shadowBox->getHeight();
		lightProjection[2][2] = -2.0 / shadowBox->getLength();
		lightProjection[3][3] = 1.0;
		lightView = glm::lookAt(shadowBox->getCenter(), shadowBox->getCenter() + glm::normalize(-lightPos), glm::vec3(0, 1.0f, 0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));

		shaderLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(day, day, day)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(day, day, day)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		////Proyecto
		shaderLighting.setInt("pointLightCount", lampCityPosition.size() + lampCityOrientation.size());
		shaderTerrain.setInt("pointLightCount", lampCityPosition.size() + lampCityOrientation.size());

		for (int i = 0; i < lampCityPosition.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lampCityPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lampCityOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for (int i = 0; i < lampCityPosition.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lampCityPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lampCityOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderLighting.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderLighting.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].constant", 1.0);
			shaderLighting.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].linear", 0.09);
			shaderLighting.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lampCityPosition.size() + i) + "].quadratic", 0.02);
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();

		// Lamps1 colliders
		for (int i = 0; i < lampCityPosition.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lampCityPosition[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lampCityOrientation[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 1.0, 0.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampCity.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLampCity.getObb().e * glm::vec3(0.5, 0.5, 0.15);
			std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
		}

		//Collider de Character
		AbstractModel::OBB characterCollider;
		glm::mat4 modelMatrixColliderCharacter = glm::mat4(modelMatrixCharacter);
		characterCollider.u = glm::quat_cast(modelMatrixColliderCharacter);
		modelMatrixColliderCharacter[3][1] = terrain.getHeightTerrain(modelMatrixColliderCharacter[3][0], modelMatrixColliderCharacter[3][2]);
		modelMatrixColliderCharacter = glm::scale(modelMatrixColliderCharacter, glm::vec3(0.006, 0.006, 0.006));
		modelMatrixColliderCharacter = glm::translate(modelMatrixColliderCharacter, characterModel.getObb().c);
		characterCollider.c = glm::vec3(modelMatrixColliderCharacter[3]);
		characterCollider.e = characterModel.getObb().e * glm::vec3(0.019, 0.28, 0.01);
		addOrUpdateColliders(collidersOBB, "character", characterCollider, modelMatrixCharacter);

		//Collider bottle
		AbstractModel::OBB bottleCollider;
		glm::mat4 modelMatrixColliderBottle = glm::mat4(modelMatrixBottle);
		bottleCollider.u = glm::quat_cast(modelMatrixColliderBottle);
		modelMatrixColliderBottle[3][1] = terrain.getHeightTerrain(modelMatrixColliderBottle[3][0], modelMatrixColliderBottle[3][2]);
		modelMatrixColliderBottle = glm::scale(modelMatrixColliderBottle, glm::vec3(0.2, 0.2, 0.2));
		modelMatrixColliderBottle = glm::translate(modelMatrixColliderBottle, bottleModel.getObb().c);
		bottleCollider.c = glm::vec3(modelMatrixColliderBottle[3]);
		bottleCollider.e = bottleModel.getObb().e * glm::vec3(0.2, 0.2, 0.2);
		addOrUpdateColliders(collidersOBB, "bottle", bottleCollider, modelMatrixBottle);

		//Collider bottle2
		AbstractModel::OBB bottle2Collider;
		glm::mat4 modelMatrixColliderBottle2 = glm::mat4(modelMatrixBottle2);
		bottle2Collider.u = glm::quat_cast(modelMatrixColliderBottle2);
		modelMatrixColliderBottle2[3][1] = terrain.getHeightTerrain(modelMatrixColliderBottle2[3][0], modelMatrixColliderBottle2[3][2]);
		modelMatrixColliderBottle2 = glm::scale(modelMatrixColliderBottle2, glm::vec3(0.4, 0.4, 0.4));
		modelMatrixColliderBottle2 = glm::translate(modelMatrixColliderBottle2, bottle2Model.getObb().c);
		bottle2Collider.c = glm::vec3(modelMatrixColliderBottle2[3]);
		bottle2Collider.e = bottle2Model.getObb().e * glm::vec3(0.4, 0.4, 0.4);
		addOrUpdateColliders(collidersOBB, "bottle2", bottle2Collider, modelMatrixBottle2);

		//Collider enemy
		AbstractModel::OBB enemyCollider;
		glm::mat4 modelMatrixColliderEnemy = glm::mat4(modelMatrixEnemy);
		enemyCollider.u = glm::quat_cast(modelMatrixColliderEnemy);
		modelMatrixColliderEnemy[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy[3][0], modelMatrixColliderEnemy[3][2]);
		modelMatrixColliderEnemy = glm::scale(modelMatrixColliderEnemy, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy = glm::translate(modelMatrixColliderEnemy, enemyModel.getObb().c);
		enemyCollider.c = glm::vec3(modelMatrixColliderEnemy[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider.c.x = enemyCollider.c.x + (steps*0.0077);
		enemyCollider.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy", enemyCollider, modelMatrixEnemy);

		//Collider Virus
		AbstractModel::SBB virusCollider;
		glm::mat4 modelMatrixColliderVirus = glm::mat4(modelMatrixVirus);
		modelMatrixColliderVirus = glm::scale(modelMatrixColliderVirus, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus = glm::translate(modelMatrixColliderVirus, virusModel.getSbb().c);
		virusCollider.c = glm::vec3(modelMatrixColliderVirus[3])* glm::vec3(1.0, 2.2, 1.0);
		virusCollider.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus", virusCollider, modelMatrixVirus);
		//**************************************************************************************************************
		//Enemy2
		AbstractModel::OBB enemyCollider2;
		glm::mat4 modelMatrixColliderEnemy2 = glm::mat4(modelMatrixEnemy2);
		enemyCollider2.u = glm::quat_cast(modelMatrixColliderEnemy2);
		modelMatrixColliderEnemy2[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy2[3][0], modelMatrixColliderEnemy2[3][2]);
		modelMatrixColliderEnemy2 = glm::scale(modelMatrixColliderEnemy2, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy2 = glm::translate(modelMatrixColliderEnemy2, enemyModel.getObb().c);
		enemyCollider2.c = glm::vec3(modelMatrixColliderEnemy2[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider2.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy2", enemyCollider2, modelMatrixEnemy2);

		//Enemy3
		AbstractModel::OBB enemyCollider3;
		glm::mat4 modelMatrixColliderEnemy3 = glm::mat4(modelMatrixEnemy3);
		enemyCollider3.u = glm::quat_cast(modelMatrixColliderEnemy3);
		modelMatrixColliderEnemy3[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy3[3][0], modelMatrixColliderEnemy3[3][2]);
		modelMatrixColliderEnemy3 = glm::scale(modelMatrixColliderEnemy3, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy3 = glm::translate(modelMatrixColliderEnemy3, enemyModel.getObb().c);
		enemyCollider3.c = glm::vec3(modelMatrixColliderEnemy3[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider3.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy3", enemyCollider3, modelMatrixEnemy3);

		//Enemy5
		AbstractModel::OBB enemyCollider5;
		glm::mat4 modelMatrixColliderEnemy5 = glm::mat4(modelMatrixEnemy5);
		enemyCollider5.u = glm::quat_cast(modelMatrixColliderEnemy5);
		modelMatrixColliderEnemy5[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy5[3][0], modelMatrixColliderEnemy5[3][2]);
		modelMatrixColliderEnemy5 = glm::scale(modelMatrixColliderEnemy5, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy5 = glm::translate(modelMatrixColliderEnemy5, enemyModel.getObb().c);
		enemyCollider5.c = glm::vec3(modelMatrixColliderEnemy5[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider5.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy5", enemyCollider5, modelMatrixEnemy5);

		//Enemy6
		AbstractModel::OBB enemyCollider6;
		glm::mat4 modelMatrixColliderEnemy6 = glm::mat4(modelMatrixEnemy6);
		enemyCollider6.u = glm::quat_cast(modelMatrixColliderEnemy6);
		modelMatrixColliderEnemy6[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy6[3][0], modelMatrixColliderEnemy6[3][2]);
		modelMatrixColliderEnemy6 = glm::scale(modelMatrixColliderEnemy6, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy6 = glm::translate(modelMatrixColliderEnemy6, enemyModel.getObb().c);
		enemyCollider6.c = glm::vec3(modelMatrixColliderEnemy6[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider6.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy6", enemyCollider6, modelMatrixEnemy6);

		//Enemy4
		AbstractModel::OBB enemyCollider4;
		glm::mat4 modelMatrixColliderEnemy4 = glm::mat4(modelMatrixEnemy4);
		enemyCollider4.u = glm::quat_cast(modelMatrixColliderEnemy4);
		modelMatrixColliderEnemy4[3][1] = terrain.getHeightTerrain(modelMatrixColliderEnemy4[3][0], modelMatrixColliderEnemy4[3][2]);
		modelMatrixColliderEnemy4 = glm::scale(modelMatrixColliderEnemy4, glm::vec3(0.007, 0.007, 0.007));
		modelMatrixColliderEnemy4 = glm::translate(modelMatrixColliderEnemy4, enemyModel.getObb().c);
		enemyCollider4.c = glm::vec3(modelMatrixColliderEnemy4[3]) * glm::vec3(1.0, 2.0, 1.0);
		enemyCollider4.c.x = enemyCollider4.c.x + (steps3 * 0.00715);
		enemyCollider4.e = enemyModel.getObb().e * glm::vec3(0.2, 0.4, 1.0);
		addOrUpdateColliders(collidersOBB, "enemy4", enemyCollider4, modelMatrixEnemy4);
		
		//Virus2
		AbstractModel::SBB virusCollider2;
		glm::mat4 modelMatrixColliderVirus2 = glm::mat4(modelMatrixVirus2);
		modelMatrixColliderVirus2 = glm::scale(modelMatrixColliderVirus2, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus2 = glm::translate(modelMatrixColliderVirus2, virusModel.getSbb().c);
		virusCollider2.c = glm::vec3(modelMatrixColliderVirus2[3]) * glm::vec3(1.0, 2.2, 1.0);
		virusCollider2.c.z = virusCollider2.c.z + (steps2*0.009999);
		virusCollider2.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus2", virusCollider2, modelMatrixVirus2);

		//Virus3
		AbstractModel::SBB virusCollider3;
		glm::mat4 modelMatrixColliderVirus3 = glm::mat4(modelMatrixVirus3);
		modelMatrixColliderVirus3 = glm::scale(modelMatrixColliderVirus3, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus3 = glm::translate(modelMatrixColliderVirus3, virusModel.getSbb().c);
		virusCollider3.c = glm::vec3(modelMatrixColliderVirus3[3]) * glm::vec3(1.0, 2.2, 1.0);
		//virusCollider3.c.z = virusCollider2.c.z + (steps2 * 0.009999);
		virusCollider3.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus3", virusCollider3, modelMatrixVirus3);

		//Virus4
		AbstractModel::SBB virusCollider4;
		glm::mat4 modelMatrixColliderVirus4 = glm::mat4(modelMatrixVirus4);
		modelMatrixColliderVirus4 = glm::scale(modelMatrixColliderVirus4, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus4 = glm::translate(modelMatrixColliderVirus4, virusModel.getSbb().c);
		virusCollider4.c = glm::vec3(modelMatrixColliderVirus4[3]) * glm::vec3(1.0, 2.2, 1.0);
		virusCollider4.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus4", virusCollider4, modelMatrixVirus4);

		//Virus5
		AbstractModel::SBB virusCollider5;
		glm::mat4 modelMatrixColliderVirus5 = glm::mat4(modelMatrixVirus5);
		modelMatrixColliderVirus5 = glm::scale(modelMatrixColliderVirus5, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus5 = glm::translate(modelMatrixColliderVirus5, virusModel.getSbb().c);
		virusCollider5.c = glm::vec3(modelMatrixColliderVirus5[3]) * glm::vec3(1.0, 2.2, 1.0);
		virusCollider5.c.x = virusCollider5.c.x + (steps4 * 0.01);
		virusCollider5.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus5", virusCollider5, modelMatrixVirus5);

		//Virus6
		AbstractModel::SBB virusCollider6;
		glm::mat4 modelMatrixColliderVirus6 = glm::mat4(modelMatrixVirus6);
		modelMatrixColliderVirus6 = glm::scale(modelMatrixColliderVirus6, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderVirus6 = glm::translate(modelMatrixColliderVirus6, virusModel.getSbb().c);
		virusCollider6.c = glm::vec3(modelMatrixColliderVirus6[3]) * glm::vec3(1.0, 2.2, 1.0);
		virusCollider6.c.z = virusCollider6.c.z + (steps5 * 0.009999);
		virusCollider6.ratio = virusModel.getSbb().ratio * 0.35;
		addOrUpdateColliders(collidersSBB, "virus6", virusCollider6, modelMatrixVirus6);

		//Cubrebocas
		AbstractModel::OBB cubreCollider;
		glm::mat4 modelMatrixColliderCubre = glm::mat4(modelMatrixCubre);
		cubreCollider.u = glm::quat_cast(modelMatrixColliderCubre);
		modelMatrixColliderCubre[3][1] = terrain.getHeightTerrain(modelMatrixColliderCubre[3][0], modelMatrixColliderCubre[3][2]);
		modelMatrixColliderCubre = glm::scale(modelMatrixColliderCubre, glm::vec3(2.5, 2.5, 2.5));
		modelMatrixColliderCubre = glm::translate(modelMatrixColliderCubre, cubreModel.getObb().c);
		cubreCollider.c = glm::vec3(modelMatrixColliderCubre[3]);
		cubreCollider.e = cubreModel.getObb().e * glm::vec3(2.0, 2.0, 2.0);
		addOrUpdateColliders(collidersOBB, "cubre", cubreCollider, modelMatrixCubre);

		AbstractModel::OBB passCollider;
		glm::mat4 modelMatrixCollider = glm::mat4(matrixPass);
		passCollider.u = glm::quat_cast(modelMatrixCollider);
		modelMatrixCollider = glm::scale(modelMatrixCollider, glm::vec3(100, 5, 5));
		modelMatrixCollider = glm::translate(modelMatrixCollider, glm::vec3(0,0,0));
		passCollider.u = glm::vec3(modelMatrixCollider[3]);
		passCollider.e = glm::vec3(1.0,1.0,1.0);
		addOrUpdateColliders(collidersOBB,"pass1",passCollider,matrixPass);

		/*******************************************
		 * Render de colliders
		 *******************************************/
//for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
//	collidersOBB.begin(); it != collidersOBB.end(); it++) {
//	glm::mat4 matrixCollider = glm::mat4(1.0);
//	matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
//	matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
//	matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
//	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
//	boxCollider.enableWireMode();
//	boxCollider.render(matrixCollider);
//}
//
//for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
//	collidersSBB.begin(); it != collidersSBB.end(); it++) {
//	glm::mat4 matrixCollider = glm::mat4(1.0);
//	matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
//	matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
//	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
//	sphereCollider.enableWireMode();
//	sphereCollider.render(matrixCollider);
//}

/*******************************************
 * Test Colisions
 *******************************************/
for (std::map<std::string,
	std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
	collidersOBB.begin(); it != collidersOBB.end(); it++) {
	bool isCollision = false;
	for (std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
		collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
		if (it != jt
			&& testOBBOBB(std::get<0>(it->second),
				std::get<0>(jt->second))) {
			std::cout << "Colision " << it->first << " with "
				<< jt->first << std::endl;
			if (it->first == "character" && jt->first == "bottle" && oneTouch) {
				touchBottle = true;
				oneTouch = false;
				countObjets++;
			}
			else if (it->first == "character" && jt->first == "bottle2" && oneTouch2) {
				touchBottle2 = true;
				oneTouch2 = false;
				countObjets++;
			}
			else if (it->first == "character" && jt->first == "cubre" && oneTouch3) {
				touchCubre = true;
				oneTouch3 = false;
				countObjets++;
			}
			else if ((it->first == "character" && oneTouchEnemy && jt->first == "enemy") ||
				(it->first == "character" && oneTouchEnemy && jt->first == "enemy2") ||
				(it->first == "character" && oneTouchEnemy && jt->first == "enemy3") ||
				(it->first == "character" && oneTouchEnemy && jt->first == "enemy4") ||
				(it->first == "character" && oneTouchEnemy && jt->first == "enemy5") ||
				(it->first == "character" && oneTouchEnemy && jt->first == "enemy6")) {
				dead = true;
				oneTouchEnemy = false;
			}
			isCollision = true;
		}
	}
	addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
}

for (std::map<std::string,
	std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
	collidersSBB.begin(); it != collidersSBB.end(); it++) {
	bool isCollision = false;
	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
		collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
		if (it != jt
			&& testSphereSphereIntersection(std::get<0>(it->second),
				std::get<0>(jt->second))) {
			std::cout << "Colision " << it->first << " with "
				<< jt->first << std::endl;
			isCollision = true;
		}
	}
	addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
}

for (std::map<std::string,
	std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
	collidersSBB.begin(); it != collidersSBB.end(); it++) {
	bool isCollision = false;
	std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
		collidersOBB.begin();
	for (; jt != collidersOBB.end(); jt++) {
		if (testSphereOBox(std::get<0>(it->second),
			std::get<0>(jt->second))) {
			std::cout << "Colision " << it->first << " with "
				<< jt->first << std::endl;

			if ((it->first == "virus" && oneTouchEnemy && jt->first == "character") ||
				(it->first == "virus2" && oneTouchEnemy && jt->first == "character") ||
				(it->first == "virus3" && oneTouchEnemy && jt->first == "character") ||
				(it->first == "virus4" && oneTouchEnemy && jt->first == "character") ||
				(it->first == "virus5" && oneTouchEnemy && jt->first == "character") ||
				(it->first == "virus6" && oneTouchEnemy && jt->first == "character")){
				dead = true;
				oneTouchEnemy = false;
			}
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
				colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("character") == 0)
						modelMatrixCharacter = std::get<1>(jt->second);
				}
			}
		}

		if(dead != true)
			animationIndexCh = 0;
		if (dead)
			animationIndexCh = 5;

		if (countObjets > 2)
			animationIndexCh = 3;
			

		switch (stateDay) {
		case 0:
			count += 0.5;
			if (count == 800) {
				day = 0.0;
				stateDay = 1;
			}
			break;
		case 1:
			count -= 0.5;
			if (count == 0) {
				day = 0.6;
				stateDay = 0;
			}
			break;
		}
		//steps = 0.002;
		switch (stateEnemy)
		{
		case 0:
			steps += 7.5;
			if (steps > 1000) {
				stateEnemy = 1;
				turn = -1;
			}
			break;
		case 1:
			steps -= 7.5;
			if (steps < -700) {
				stateEnemy = 0;
				turn = 1;
			}
			break;
		}

		switch (stateEnemy2)
		{
		case 0:
			steps2 += 5.5;
			if (steps2 > 1150) {
				stateEnemy2 = 1;
			}
			break;
		case 1:
			steps2 -= 5.5;
			if (steps2 < -150) {
				stateEnemy2 = 0;
			}
			break;
		}

		switch (stateEnemy3)
		{
		case 0:
			steps3 -= 7.5;
			if (steps3 < -2500) {
				stateEnemy3 = 1;
				turn3 = -1;
			}
			break;
		case 1:
			steps3 += 7.5;
			if (steps3 > 30) {
				stateEnemy3 = 0;
				turn3 = 1;
			}
			break;
		}

		switch (stateEnemy4)
		{
		case 0:
			steps4 += 7.5;
			if (steps4 > 1350) {
				stateEnemy4 = 1;
			}
			break;
		case 1:
			steps4 -= 7.5;
			if (steps4 < 100) {
				stateEnemy4 = 0;
			}
			break;
		}

		switch (stateEnemy5)
		{
		case 0:
			steps5 += 7.5;
			if (steps5 > 1600) {
				stateEnemy5 = 1;
			}
			break;
		case 1:
			steps5 -= 7.5;
			if (steps5 < 300) {
				stateEnemy5 = 0;
			}
			break;
		}

		if (touchBottle) {
			alSourceStop(source[1]);
		}
		else if (touchBottle2) {
			alSourceStop(source[2]);
		}
		else if (touchCubre) {
			alSourceStop(source[3]);
		}

		glfwSwapBuffers(window);

		source0Pos[0] = 0.0;
		source0Pos[1] = 0.0;
		source0Pos[2] = 0.0;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source1Pos[0] = modelMatrixBottle[3].x;
		source1Pos[1] = modelMatrixBottle[3].y;
		source1Pos[2] = modelMatrixBottle[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		source2Pos[0] = modelMatrixBottle2[3].x;
		source2Pos[1] = modelMatrixBottle2[3].y;
		source2Pos[2] = modelMatrixBottle2[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		source3Pos[0] = modelMatrixCubre[3].x;
		source3Pos[1] = modelMatrixCubre[3].y;
		source3Pos[2] = modelMatrixCubre[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);
		/****************************+
		 * Open AL sound data
		 */

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixCharacter[3].x;
		listenerPos[1] = modelMatrixCharacter[3].y;
		listenerPos[2] = modelMatrixCharacter[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixCharacter[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixCharacter[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		/*listenerPos[0] = camera->getPosition().x;
		listenerPos[1] = camera->getPosition().y;
		listenerPos[2] = camera->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);
		listenerOri[0] = camera->getFront().x;
		listenerOri[1] = camera->getFront().y;
		listenerOri[2] = camera->getFront().z;
		listenerOri[3] = camera->getUp().x;
		listenerOri[4] = camera->getUp().y;
		listenerOri[5] = camera->getUp().z;*/
		//alListenerfv(AL_ORIENTATION, listenerOri);

		/*sourcesPlay[0] = false;
		alSourcePlay(source[0]);*/
		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

void prepareScene(){

	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	modelLampCity.setShader(&shaderLighting);

	characterModel.setShader(&shaderMulLighting);
	enemyModel.setShader(&shaderMulLighting);
	bottleModel.setShader(&shaderMulLighting);
	bottle2Model.setShader(&shaderMulLighting);
	virusModel.setShader(&shaderMulLighting);
	cubreModel.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	modelLampCity.setShader(&shaderDepth);

	characterModel.setShader(&shaderDepth);
	enemyModel.setShader(&shaderDepth);
	bottleModel.setShader(&shaderDepth);
	bottle2Model.setShader(&shaderDepth);
	virusModel.setShader(&shaderDepth);
	cubreModel.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/

	for (int i = 0; i < lampCityPosition.size(); i++) {
		lampCityPosition[i].y = terrain.getHeightTerrain(lampCityPosition[i].x, lampCityPosition[i].z);
		modelLampCity.setPosition(lampCityPosition[i]);
		modelLampCity.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLampCity.setOrientation(glm::vec3(0, lampCityOrientation[i], 0));

		modelLampCity.render();

	}

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/

	modelMatrixCharacter[3][1] = terrain.getHeightTerrain(modelMatrixCharacter[3][0], modelMatrixCharacter[3][2]);
	glm::mat4 modelMatrixCharacterBody = glm::mat4(modelMatrixCharacter);
	modelMatrixCharacterBody = glm::scale(modelMatrixCharacterBody, glm::vec3(0.006, 0.006, 0.006));
	characterModel.setAnimationIndex(animationIndexCh);
	characterModel.render(modelMatrixCharacterBody);

	modelMatrixEnemy[3][1] = terrain.getHeightTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2]);
	glm::mat4 modelMatrixEnemyBody = glm::mat4(modelMatrixEnemy);
	modelMatrixEnemyBody = glm::scale(modelMatrixEnemyBody, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody = glm::translate(modelMatrixEnemyBody, glm::vec3(steps, 0.0, 0.0));
	modelMatrixEnemyBody = glm::rotate(modelMatrixEnemyBody, glm::radians(turn * 60.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(1);
	enemyModel.render(modelMatrixEnemyBody);

	modelMatrixBottle[3][1] = terrain.getHeightTerrain(modelMatrixBottle[3][0], modelMatrixBottle[3][2]);
	glm::mat4 modelMatrixBottleBody = glm::mat4(modelMatrixBottle);
	modelMatrixBottleBody = glm::scale(modelMatrixBottleBody, glm::vec3(0.2, 0.2, 0.2));
	bottleModel.render(modelMatrixBottleBody);

	modelMatrixBottle2[3][1] = terrain.getHeightTerrain(modelMatrixBottle2[3][0], modelMatrixBottle2[3][2]);
	glm::mat4 modelMatrixBottle2Body = glm::mat4(modelMatrixBottle2);
	modelMatrixBottle2Body = glm::scale(modelMatrixBottle2Body, glm::vec3(0.4, 0.4, 0.4));
	bottle2Model.render(modelMatrixBottle2Body);

	modelMatrixVirus[3][1] = terrain.getHeightTerrain(modelMatrixVirus[3][0], modelMatrixVirus[3][2]);
	glm::mat4 modelMatrixVirusBody = glm::mat4(modelMatrixVirus);
	modelMatrixVirusBody = glm::scale(modelMatrixVirusBody, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody = glm::translate(modelMatrixVirusBody, glm::vec3(0.0, 2.0, 0.0));
	virusModel.render(modelMatrixVirusBody);
	//********************************************************************************************************
	modelMatrixEnemy2[3][1] = terrain.getHeightTerrain(modelMatrixEnemy2[3][0], modelMatrixEnemy2[3][2]);
	glm::mat4 modelMatrixEnemyBody2 = glm::mat4(modelMatrixEnemy2);
	modelMatrixEnemyBody2 = glm::scale(modelMatrixEnemyBody2, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody2 = glm::translate(modelMatrixEnemyBody2, glm::vec3(0.0, 0.0, 0.0));
	modelMatrixEnemyBody2 = glm::rotate(modelMatrixEnemyBody2, glm::radians(60.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(0);
	enemyModel.render(modelMatrixEnemyBody2);

	modelMatrixVirus2[3][1] = terrain.getHeightTerrain(modelMatrixVirus2[3][0], modelMatrixVirus2[3][2]);
	glm::mat4 modelMatrixVirusBody2 = glm::mat4(modelMatrixVirus2);
	modelMatrixVirusBody2 = glm::scale(modelMatrixVirusBody2, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody2 = glm::translate(modelMatrixVirusBody2, glm::vec3(0.0, 2.0, steps2 * 0.016));
	virusModel.render(modelMatrixVirusBody2);

	modelMatrixEnemy3[3][1] = terrain.getHeightTerrain(modelMatrixEnemy3[3][0], modelMatrixEnemy3[3][2]);
	glm::mat4 modelMatrixEnemyBody3 = glm::mat4(modelMatrixEnemy3);
	modelMatrixEnemyBody3 = glm::scale(modelMatrixEnemyBody3, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody3 = glm::translate(modelMatrixEnemyBody3, glm::vec3(0.0, 0.0, 0.0));
	modelMatrixEnemyBody3 = glm::rotate(modelMatrixEnemyBody3, glm::radians(-70.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(0);
	enemyModel.render(modelMatrixEnemyBody3);

	modelMatrixVirus3[3][1] = terrain.getHeightTerrain(modelMatrixVirus3[3][0], modelMatrixVirus3[3][2]);
	glm::mat4 modelMatrixVirusBody3 = glm::mat4(modelMatrixVirus3);
	modelMatrixVirusBody3 = glm::scale(modelMatrixVirusBody3, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody3 = glm::translate(modelMatrixVirusBody3, glm::vec3(0.0, 2.0, 0.0));
	virusModel.render(modelMatrixVirusBody3);

	modelMatrixEnemy4[3][1] = terrain.getHeightTerrain(modelMatrixEnemy4[3][0], modelMatrixEnemy4[3][2]);
	glm::mat4 modelMatrixEnemyBody4 = glm::mat4(modelMatrixEnemy4);
	modelMatrixEnemyBody4 = glm::scale(modelMatrixEnemyBody4, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody4 = glm::translate(modelMatrixEnemyBody4, glm::vec3(steps3, 0.0, 0.0));
	modelMatrixEnemyBody4 = glm::rotate(modelMatrixEnemyBody4, glm::radians(turn3 * -60.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(1);
	enemyModel.render(modelMatrixEnemyBody4);

	modelMatrixVirus4[3][1] = terrain.getHeightTerrain(modelMatrixVirus4[3][0], modelMatrixVirus4[3][2]);
	glm::mat4 modelMatrixVirusBody4 = glm::mat4(modelMatrixVirus4);
	modelMatrixVirusBody4 = glm::scale(modelMatrixVirusBody4, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody4 = glm::translate(modelMatrixVirusBody4, glm::vec3(0.0, 2.0, 0.0));
	virusModel.render(modelMatrixVirusBody4);

	modelMatrixVirus5[3][1] = terrain.getHeightTerrain(modelMatrixVirus5[3][0], modelMatrixVirus5[3][2]);
	glm::mat4 modelMatrixVirusBody5 = glm::mat4(modelMatrixVirus5);
	modelMatrixVirusBody5 = glm::scale(modelMatrixVirusBody5, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody5 = glm::translate(modelMatrixVirusBody5, glm::vec3(steps4 * 0.0164, 2.0, 0.0));
	virusModel.render(modelMatrixVirusBody5);

	modelMatrixEnemy5[3][1] = terrain.getHeightTerrain(modelMatrixEnemy5[3][0], modelMatrixEnemy5[3][2]);
	glm::mat4 modelMatrixEnemyBody5 = glm::mat4(modelMatrixEnemy5);
	modelMatrixEnemyBody5 = glm::scale(modelMatrixEnemyBody5, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody5 = glm::translate(modelMatrixEnemyBody5, glm::vec3(0.0, 0.0, 0.0));
	modelMatrixEnemyBody5 = glm::rotate(modelMatrixEnemyBody5, glm::radians(20.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(0);
	enemyModel.render(modelMatrixEnemyBody5);

	modelMatrixEnemy6[3][1] = terrain.getHeightTerrain(modelMatrixEnemy6[3][0], modelMatrixEnemy[3][2]);
	glm::mat4 modelMatrixEnemyBody6 = glm::mat4(modelMatrixEnemy6);
	modelMatrixEnemyBody6 = glm::scale(modelMatrixEnemyBody6, glm::vec3(0.007, 0.007, 0.007));
	modelMatrixEnemyBody6 = glm::translate(modelMatrixEnemyBody6, glm::vec3(0.0, 0.0, 0.0));
	modelMatrixEnemyBody6 = glm::rotate(modelMatrixEnemyBody6, glm::radians(20.0f), glm::vec3(0, 1, 0));
	enemyModel.setAnimationIndex(0);
	enemyModel.render(modelMatrixEnemyBody6);

	modelMatrixVirus6[3][1] = terrain.getHeightTerrain(modelMatrixVirus6[3][0], modelMatrixVirus6[3][2]);
	glm::mat4 modelMatrixVirusBody6 = glm::mat4(modelMatrixVirus6);
	modelMatrixVirusBody6 = glm::scale(modelMatrixVirusBody6, glm::vec3(0.6, 0.6, 0.6));
	modelMatrixVirusBody6 = glm::translate(modelMatrixVirusBody6, glm::vec3(0.0, 2.0, steps5 * 0.0164));
	virusModel.render(modelMatrixVirusBody6);

	modelMatrixCubre[3][1] = terrain.getHeightTerrain(modelMatrixCubre[3][0], modelMatrixCubre[3][2]);
	glm::mat4 modelMatrixCubreBody = glm::mat4(modelMatrixCubre);
	modelMatrixCubreBody = glm::scale(modelMatrixCubreBody, glm::vec3(2.5, 2.5, 2.5));
	cubreModel.render(modelMatrixCubreBody);

	/**********
	 * Update the position with alpha objects
	 */
	 // Update the aircraft
	 //blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	 //// Update the lambo
	 //blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	 //// Update the helicopter
	 //blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	 /**********
	  * Sorter with alpha objects
	  */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
