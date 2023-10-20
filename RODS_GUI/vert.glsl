#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 vertexColor;

void main()
{
    gl_Position = model * projection * view * vec4(position, 1.0);
    float v = length(color);
    float r = 1.0;
    float g = 1.0;
    float b = 1.0;

    float vmin = 0.0;
    float vmax = 1.0;
    float dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
        r = 0.0;
        g = 4.0 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
        r = 0.0;
        b = 1.0 + 4.0 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
        r = 4.0 * (v - vmin - 0.5 * dv) / dv;
        b = 0.0;
    } else {
        g = 1.0 + 4.0 * (vmin + 0.75 * dv - v) / dv;
        b = 0.0;
    }

    vertexColor = vec4(r, g, b, 1.0);
}