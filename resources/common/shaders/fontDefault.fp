uniform sampler2D u_tex;
uniform vec4 u_color;

varying mediump vec2 uv;

void main()
{
	mediump vec4 color = texture2D(u_tex, uv);
	gl_FragColor = vec4( color.xyz, color.w ) * u_color;
}
