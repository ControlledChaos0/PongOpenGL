//#ifdef CPP_GLSL_INCLUDE
//std::string my_shader_glsl_src = R"(
//#endif

#version 330 core
out vec4 color;

void main() {
	color = vec4(1.0f);
}

//#ifdef CPP_GLSL_INCLUDE
//)";
//#endif