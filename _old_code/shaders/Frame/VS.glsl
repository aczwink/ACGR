#version 330 core

//Inputs
layout (location = 0) in vec2 position;

//Output
out vec2 UV;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    UV = vec2((position.x + 1) / 2, (position.y + 1) / 2);
}