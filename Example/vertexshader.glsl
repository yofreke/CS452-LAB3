#version 130

uniform mat4 Matrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_position;

in vec4 in_color;
out vec4 pass_color;

void main(){
  gl_Position=Matrix*viewMatrix*modelMatrix*vec4(in_position,1.0); 
  pass_color=vec4(1.0, (0.5 + in_position.y), 1 - (0.5 + in_position.z), 1.0);
  //pass_color=in_color;
}
