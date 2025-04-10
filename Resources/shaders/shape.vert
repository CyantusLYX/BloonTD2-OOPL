#version 330 core

layout(location = 0) in vec2 aPos;

uniform Matrices {
    mat4 m_Model;
    mat4 m_Projection;
};

void main() {
    gl_Position = m_Projection * m_Model * vec4(aPos, 0.0, 1.0);
}