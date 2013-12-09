varying mediump vec4 color;
varying mediump vec3 normal;
varying mediump vec2 uv;

void main()
{
	//gl_FragColor =  vec4(1.0, 1.0, 0.0, 1.0);
	gl_FragColor = vec4(normal,1.0);
}
