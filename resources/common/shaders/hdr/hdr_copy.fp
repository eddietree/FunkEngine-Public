varying mediump vec2 uv;

uniform sampler2D u_tex;

void main()
{
	gl_FragColor = texture2D(u_tex, uv);
}
