#version 460 core

layout( vertices = 25 ) out;

uniform float uOuter0, uOuter1, uOuter2, uOuter3, uInner0, uInner1;

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	// set tessellation levels
	gl_TessLevelOuter[0] = uOuter0;
    gl_TessLevelOuter[1] = uOuter1;
    gl_TessLevelOuter[2] = uOuter2;
    gl_TessLevelOuter[3] = uOuter3;
    gl_TessLevelInner[0] = uInner0;
    gl_TessLevelInner[1] = uInner1;
}