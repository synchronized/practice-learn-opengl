#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
  FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, vec2(1.0f - ourTexCoord.s, ourTexCoord.t)), vec4(texture(ourTexture2, ourTexCoord)).a * 0.2);
}