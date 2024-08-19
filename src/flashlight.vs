#version 330

out vec2 currPos;

void main() {
  currPos[0] = gl_Position[0];
  currPos[1] = gl_Position[1];
}
