#version 150

uniform mat4 projection;
uniform mat4 modelView;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;

    // Apply all matrix transformations to vert
    gl_Position = projection * modelView * vec4(vert, 1.0);
}