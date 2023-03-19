#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

//Constants
const float MAGNITUDE = 0.75f;

//Input
in VS_OUT
{
	vec3 normal;
} gs_in[];

//Outputs
out vec3 fragColor;


vec4 ComputeCenter()
{
	return (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;
}

vec3 ComputeFaceNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	
	return normalize(cross(a, b));
}


void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void GenerateFaceNormal()
{
	vec3 normal;
	vec4 pos;
	
	pos = ComputeCenter();
	normal = ComputeFaceNormal();
	
	gl_Position = pos;
	EmitVertex();
	gl_Position = pos + vec4(normal, 0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	//vertex normals
	fragColor = vec3(1.0f, 1.0f, 0.0f);
	GenerateLine(0); // First vertex normal
	GenerateLine(1); // Second vertex normal
	GenerateLine(2); // Third vertex normal
	
	//face normals
	fragColor = vec3(0, 1, 1.0f);
	GenerateFaceNormal();
}  