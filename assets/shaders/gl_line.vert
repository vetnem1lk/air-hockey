#version 460 core

layout(location = 0) in vec2 aPos; // Позиция вершины линии

uniform mat4 uProjection;
uniform vec2 uOffset; // Для смещения линий (пока не нужно, но для будущего)
uniform float uScale; // Для масштабирования линий (пока не нужно)

void main() {
    vec2 transformedPos = aPos * uScale + uOffset;
    gl_Position = uProjection * vec4(transformedPos, 0.0, 1.0);
}
