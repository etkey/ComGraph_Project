#include "Model.h"
#include "Plane.h"
#include "Mesh.h"
#include "shaderClass.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "Room Scene"
	GLFWwindow* window = glfwCreateWindow(width, height, "Room Scene", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Create light shader
	Shader lightShader("light.vert", "light.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Room dimensions
	float roomWidth = 30.0f;
	float roomHeight = 15.0f;
	float roomDepth = 50.0f;

	// Main ceiling light (using point light)
	glm::vec3 mainLightPos = glm::vec3(0.0f, 4.0f, 0.0f);

	// Additional point lights for better illumination
	glm::vec3 pointLightPositions[] = {
		glm::vec3(-5.0f, 2.0f, -15.0f),  // Back left
		glm::vec3(5.0f, 2.0f, -15.0f),   // Back right
		glm::vec3(-5.0f, 2.0f, 15.0f),   // Front left
		glm::vec3(5.0f, 2.0f, 15.0f)     // Front right
	};

	// Model light (point light at model position)
	glm::vec3 modelLightPos = glm::vec3(0.0f, roomHeight / 2 - 0.01f, 0.0f);
	glm::vec4 modelLightColor = glm::vec4(1.0f, 0.8f, 0.2f, 1.0f); // Warm yellow light

	shaderProgram.Activate();

	// Set main light (point light)
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), modelLightPos.x, modelLightPos.y, modelLightPos.z);

	// Set model light
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "modelLightColor"), modelLightColor.x, modelLightColor.y, modelLightColor.z, modelLightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "modelLightPos"), modelLightPos.x, modelLightPos.y, modelLightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 5.0f));

	Model sceneModel("scene.gltf");

	// Create planes with textures
	Plane floorPlane(
		"Models and Textures/floor/dark_wooden_planks_diff_2k.jpg",
		"Models and Textures/floor/dark_wooden_planks_nor_gl_2k.jpg",
		"Models and Textures/floor/dark_wooden_planks_arm_2k.jpg", 2.0f, 3.0f
	);

	Plane wallPlane(
		"Models and Textures/walls/stone_brick_wall_001_diff_2k.jpg",
		"Models and Textures/walls/stone_brick_wall_001_nor_gl_2k.jpg",
		"Models and Textures/walls/stone_brick_wall_001_rough_2k.jpg", 1.5f, 1.25f
	);

	// Create ceiling plane with corrugated iron textures
	Plane ceilingPlane(
		"Models and Textures/ceiling/corrugated_iron_02_diff_2k.jpg",
		"Models and Textures/ceiling/corrugated_iron_02_nor_gl_2k.jpg",
		"Models and Textures/ceiling/corrugated_iron_02_arm_2k.jpg", 2.0f, 2.0f
	);

	// Create wooden beam plane with weathered planks textures
	Plane beamPlane(
		"Models and Textures/floor/dark_wooden_planks_diff_2k.jpg",
		"Models and Textures/floor/dark_wooden_planks_nor_gl_2k.jpg",
		"Models and Textures/floor/dark_wooden_planks_arm_2k.jpg", 0.5f, 0.3f
	);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draw floor
		glm::mat4 floorTransform = glm::mat4(1.0f);
		floorTransform = glm::translate(floorTransform, glm::vec3(0.0f, -roomHeight / 2, 0.0f));
		floorTransform = glm::rotate(floorTransform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		floorTransform = glm::scale(floorTransform, glm::vec3(roomWidth, roomDepth, 1.0f));
		floorPlane.Draw(shaderProgram, camera, floorTransform);

		// Draw ceiling
		glm::mat4 ceilingTransform = glm::mat4(1.0f);
		ceilingTransform = glm::translate(ceilingTransform, glm::vec3(0.0f, roomHeight / 2 + 2.0f, 0.0f));
		ceilingTransform = glm::rotate(ceilingTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ceilingTransform = glm::scale(ceilingTransform, glm::vec3(roomWidth, roomDepth, 1.0f));
		ceilingPlane.Draw(shaderProgram, camera, ceilingTransform);

		// Draw wooden beams
		float beamWidth = 30.0f;    // Width of the beam (X dimension)
		float beamHeight = 1.0f;   // Height of the beam (Y dimension)
		float beamDepth = 2.0f;    // Depth of the beam (Z dimension)
		float beamSpacing = roomDepth / 4.0f; // Space beams evenly across room depth

		// Function to draw a 3D beam at given position
		auto drawBeam = [&](float zPos) {
			glm::mat4 baseTransform = glm::mat4(1.0f);
			baseTransform = glm::translate(baseTransform, glm::vec3(0.0f, roomHeight / 2 - beamHeight / 2, zPos));

			// Bottom face
			glm::mat4 bottomTransform = baseTransform;
			bottomTransform = glm::translate(bottomTransform, glm::vec3(0.0f, -beamHeight / 2, 0.0f));
			bottomTransform = glm::rotate(bottomTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			bottomTransform = glm::scale(bottomTransform, glm::vec3(beamWidth, beamDepth, 1.0f));
			beamPlane.Draw(shaderProgram, camera, bottomTransform);

			// Top face
			glm::mat4 topTransform = baseTransform;
			topTransform = glm::translate(topTransform, glm::vec3(0.0f, beamHeight / 2, 0.0f));
			topTransform = glm::rotate(topTransform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			topTransform = glm::scale(topTransform, glm::vec3(beamWidth, beamDepth, 1.0f));
			beamPlane.Draw(shaderProgram, camera, topTransform);

			// Front face
			glm::mat4 frontTransform = baseTransform;
			frontTransform = glm::translate(frontTransform, glm::vec3(0.0f, 0.0f, beamDepth / 2));
			frontTransform = glm::scale(frontTransform, glm::vec3(beamWidth, beamHeight, 1.0f));
			beamPlane.Draw(shaderProgram, camera, frontTransform);

			// Back face
			glm::mat4 backTransform = baseTransform;
			backTransform = glm::translate(backTransform, glm::vec3(0.0f, 0.0f, -beamDepth / 2));
			backTransform = glm::rotate(backTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			backTransform = glm::scale(backTransform, glm::vec3(beamWidth, beamHeight, 1.0f));
			beamPlane.Draw(shaderProgram, camera, backTransform);

			// Left face
			glm::mat4 leftTransform = baseTransform;
			leftTransform = glm::translate(leftTransform, glm::vec3(-beamWidth / 2, 0.0f, 0.0f));
			leftTransform = glm::rotate(leftTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			leftTransform = glm::scale(leftTransform, glm::vec3(beamDepth, beamHeight, 1.0f));
			beamPlane.Draw(shaderProgram, camera, leftTransform);

			// Right face
			glm::mat4 rightTransform = baseTransform;
			rightTransform = glm::translate(rightTransform, glm::vec3(beamWidth / 2, 0.0f, 0.0f));
			rightTransform = glm::rotate(rightTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rightTransform = glm::scale(rightTransform, glm::vec3(beamDepth, beamHeight, 1.0f));
			beamPlane.Draw(shaderProgram, camera, rightTransform);
			};

		// Draw the three beams
		drawBeam(-beamSpacing);  // Back beam
		drawBeam(0.0f);          // Middle beam
		drawBeam(beamSpacing);   // Front beam

		// Draw back wall
		glm::mat4 backWallTransform = glm::mat4(1.0f);
		backWallTransform = glm::translate(backWallTransform, glm::vec3(0.0f, 0.0f, -roomDepth / 2));
		backWallTransform = glm::scale(backWallTransform, glm::vec3(roomWidth, roomHeight + 4.0f, 1.0f));
		wallPlane.Draw(shaderProgram, camera, backWallTransform);

		// Draw front wall
		glm::mat4 frontWallTransform = glm::mat4(1.0f);
		frontWallTransform = glm::translate(frontWallTransform, glm::vec3(0.0f, 0.0f, roomDepth / 2));
		frontWallTransform = glm::rotate(frontWallTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		frontWallTransform = glm::scale(frontWallTransform, glm::vec3(roomWidth, roomHeight + 4.0f, 1.0f));
		wallPlane.Draw(shaderProgram, camera, frontWallTransform);

		// Draw left wall
		glm::mat4 leftWallTransform = glm::mat4(1.0f);
		leftWallTransform = glm::translate(leftWallTransform, glm::vec3(-roomWidth / 2, 0.0f, 0.0f));
		leftWallTransform = glm::rotate(leftWallTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		leftWallTransform = glm::scale(leftWallTransform, glm::vec3(roomDepth, roomHeight + 4.0f, 1.0f));
		wallPlane.Draw(shaderProgram, camera, leftWallTransform);

		// Draw right wall
		glm::mat4 rightWallTransform = glm::mat4(1.0f);
		rightWallTransform = glm::translate(rightWallTransform, glm::vec3(roomWidth / 2, 0.0f, 0.0f));
		rightWallTransform = glm::rotate(rightWallTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rightWallTransform = glm::scale(rightWallTransform, glm::vec3(roomDepth, roomHeight + 4.0f, 1.0f));
		wallPlane.Draw(shaderProgram, camera, rightWallTransform);

		// Draw wall sconces
		float sconceHeight = 6.0f;  // Increased height from the floor
		float sconceDepth = 1.0f;   // Increased offset from the wall
		float sconceScale = 4.0f;   // Significantly increased scale of the sconce model

		// Draw scene model under the middle beam
		glm::mat4 sceneModelMatrix = glm::mat4(1.0f);
		sceneModelMatrix = glm::translate(sceneModelMatrix, glm::vec3(0.0f, roomHeight / 2 - beamHeight - 2.5f, 0.0f));
		sceneModelMatrix = glm::rotate(sceneModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		sceneModelMatrix = glm::scale(sceneModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
		sceneModel.SetTransform(sceneModelMatrix);
		sceneModel.Draw(shaderProgram, camera);

		// Update light position to match the lamp
		glm::vec3 modelLightPos = glm::vec3(0.0f, roomHeight / 2 - beamHeight - 2.5f, 0.0f);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), modelLightPos.x, modelLightPos.y, modelLightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "modelLightPos"), modelLightPos.x, modelLightPos.y, modelLightPos.z);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}