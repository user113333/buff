#shader frag
#version 400 core

in vec4 v_color;
in vec2 v_texcoord;
in float v_texindex;

uniform sampler2D u_textures[32];

out vec4 FragColor;

void main() {
	vec4 tex = texture2D(u_textures[int(v_texindex)], vec2(v_texcoord.x, v_texcoord.y));
	// FragColor = tex * v_color;
	FragColor = vec4(v_color.xyz, tex.r);
}

#shader vert
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexPos;
layout (location = 3) in float aTexIndex;

uniform mat4 u_mvp;

out vec4 v_color;
out vec2 v_texcoord;
out float v_texindex;

void main() {
	v_color = aColor;
	v_texcoord = aTexPos;
	v_texindex = aTexIndex;

	gl_Position = u_mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
