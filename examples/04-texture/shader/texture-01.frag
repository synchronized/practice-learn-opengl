#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
  //FragColor = vec4(ourColor, 1.0f);
  //FragColor = texture(ourTexture1, ourTexCoord);
  //FragColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), texture(ourTexture2, ourTexCoord), vec4(texture(ourTexture2, ourTexCoord)).a * 1.0f);
  FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, ourTexCoord), vec4(texture(ourTexture2, ourTexCoord)).a * 0.2);
}