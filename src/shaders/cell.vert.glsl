#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 offset;
layout (location = 2) in float radius;
layout (location = 3) in float status;

out vec2 TextCoord;
out float Status;

void main() {
    gl_Position = vec4(aPos.xy * radius + offset.xy, 0.0, 1.0);
    TextCoord = (aPos + 1) / 2.0;
    Status = status;
}
