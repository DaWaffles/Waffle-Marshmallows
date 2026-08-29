#version 330 core
out vec4 FragColor;

void main()
{
    vec2 fragPos = gl_FragCoord.xy;
    FragColor = vec4(fragPos.x / 800.0f, fragPos.y / 600.0f, 0.0f, 1.0f);
}
