#version 450

layout(location = 0) out vec4 outColor;

layout(constant_id = 0) const float brightness = 1.0;
void main() {
    outColor = vec4(brightness, brightness, brightness, 1.0);
}

void a() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}