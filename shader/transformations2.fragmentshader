#version 330 core
out vec4 FragColor;

in vec2 ourTexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
  vec4 b = texture(ourTexture2, vec2(1.0f - ourTexCoord.s, ourTexCoord.t));
  FragColor = mix(texture(ourTexture1, ourTexCoord), b, vec4(b).a * 0.2f);
}