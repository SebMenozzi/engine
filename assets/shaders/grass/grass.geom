#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 36) out;

uniform mat4 view;
uniform mat4 projection;
uniform float grassMinHeight;

in vec3 position[];
in vec3 normal[];

out vec2 uv;

const float grassMinSize = 1.0;
float grassSize;

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void generateVertex(int i, vec4 grassPosition, vec2 grassUV, mat4 model)
{
    if (grassMinHeight < position[i].y)
    {
        gl_Position = projection * view * (gl_in[0].gl_Position + model * grassPosition * grassSize);
	    uv = grassUV;
	    EmitVertex();
    }
}

void generateGrass(mat4 model)
{
    vec4 grassPosition[4];
    float margin = 0.1;
    grassPosition[0] = vec4(-margin, 0.0, 0.0, 0.0); 	// Down left
    grassPosition[1] = vec4(margin, 0.0, 0.0, 0.0);  // Down right
    grassPosition[2] = vec4(-margin, margin, 0.0, 0.0);	// Up left
    grassPosition[3] = vec4(margin, margin, 0.0, 0.0);  // Up right

    vec2 grassUV[4];
    grassUV[0] = vec2(0.0, 0.0); // Down left
    grassUV[1] = vec2(1.0, 0.0); // Down right
    grassUV[2] = vec2(0.0, 1.0); // Up left
    grassUV[3] = vec2(1.0, 1.0); // Up right

    for (int i = 0; i < 4; i++)
    {
        grassSize = random(gl_in[0].gl_Position.xz * i) * (1.0 - grassMinSize) + grassMinSize; 
        generateVertex(i, grassPosition[i], grassUV[i], model);
    }
    EndPrimitive();
}

mat4 rotationY(float angle)
{
	return mat4(
        cos(angle),	 0,   sin(angle), 0,
		0,		     1,   0,	      0,
		-sin(angle), 0,	  cos(angle), 0,
		0,           0,	  0,	      1
    );
}

void main()
{
    mat4 model0, model45, modelm45;
	model0 = mat4(1.0f);
	model45 = rotationY(radians(45));
	modelm45 = rotationY(-radians(45));

    generateGrass(model0);
    generateGrass(model45);
    generateGrass(modelm45);
}
