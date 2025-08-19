#version 430 core

layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------
layout(std430, binding = 0) buffer matrices
{
    mat4 mats[];
};



// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------

void main() 
{
	
	// mat4 modelmatrix = mat4(
		
	// 	1, 0, 0, gl_LocalInvocationID.x,
	// 	0, 1, 0, 0,
	// 	0, 0, 1, gl_LocalInvocationID.y,
	// 	0, 0, 0, 1,
	// 	);


	mat4 modelmatrix = mat4(
		
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
		
		);

	mats[gl_LocalInvocationID.x] = modelmatrix;



	// mats[gl_LocalInvocationIndex] = modelmatrix;










}

