#version 330 core

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	// Create a glowing effect
	vec3 glowColor = vec3(1.0f, 0.8f, 0.2f); // Warm yellow glow
	float intensity = 20.0f;
	FragColor = vec4(glowColor * intensity, 1.0f);
}