#version 410 core

uniform vec2 RES;

out vec3 color;

void main()
{
    vec2 uv = gl_FragCoord.xy / RES;

    color = vec3(uv.x, uv.y, 0);
}
