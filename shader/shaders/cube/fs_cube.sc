$input v_color0

#include "bgfx_shader.sh"

void main() {
	gl_FragData[0] = v_color0;
    gl_FragData[1] = vec4(1,1,1,1);
}