#version 330 core

out vec4 FragColor;

uniform vec4 shapeColor;
uniform bool hasOutline;
uniform float outlineWidth;

void main() {
    FragColor = shapeColor;
    
}