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
layout(std430, binding = 1) buffer rotationmatrices
{
	mat4 rotationmats[];	
};



// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------


mat4 translatematrix(float xpos, float ypos, float zpos)
{
	mat4 m;

	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3] = vec4(xpos, ypos, zpos, 1.0);

	return m;





}


mat4 rotationmatrix(float angle)
{
	mat4 m;

	m[0] = vec4(cos(angle),0,sin(angle),0);
	m[1] = vec4(0,1,0,0);
	m[2] = vec4(-sin(angle),0,cos(angle),0);
	m[3] = vec4(0,0,0,1);


	return m;

}







void main() 
{
	
	// mat4 modelmatrix = mat4(
		
	// 	1, 0, 0, gl_LocalInvocationID.x,
	// 	0, 1, 0, 0,
	// 	0, 0, 1, gl_LocalInvocationID.y,
	// 	0, 0, 0, 1,
	// 	);



	
	mats[gl_LocalInvocationIndex] = translatematrix(gl_LocalInvocationID.x, 0, gl_LocalInvocationID.y) * rotationmatrix(90);
	rotationmats[gl_LocalInvocationIndex] = rotationmatrix(90);






	// mats[gl_LocalInvocationIndex] = modelmatrix;










}

