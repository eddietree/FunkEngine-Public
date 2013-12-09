varying mediump vec4 color;
varying mediump vec3 normal;
varying mediump vec2 uv;

uniform sampler2D u_tex;

void main()
{
	gl_FragColor = texture2D(u_tex, uv) * color;
	//gl_FragColor = vec4(uv,0.0,1.0);
	//gl_FragColor = vec4(normal,1.0);
	//gl_FragColor = vec4(1.0,0.0f,0.0,1.0);
}
