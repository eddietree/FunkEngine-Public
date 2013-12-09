varying mediump vec4 color;
varying mediump vec3 normal;
varying mediump vec2 uv;

uniform sampler2D u_tex;
uniform vec3 u_color;

void main()
{
	mediump vec4 tex_color = texture2D(u_tex, vec2(uv.x, 1.0-uv.y));
	mediump vec3 final_color = mix( tex_color.xyz, u_color, 0.2 );
	
	gl_FragColor = vec4(final_color,1.0f);
	//gl_FragColor = vec4(uv.x, uv.y,0.0,1.0);
	//gl_FragColor = vec4(u_color,1.0);
}
