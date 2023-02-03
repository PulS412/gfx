#version 330 core
out vec4 FragColor;

in vec3 ourPos;
uniform float colorAmplitude;
void main()
{
    FragColor = vec4(ourPos*colorAmplitude+1, 1.0f);
}