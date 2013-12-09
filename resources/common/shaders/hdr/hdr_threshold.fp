varying mediump vec2 uv;

uniform sampler2D u_tex;
uniform vec2 u_thresholds;

void main()
{
	vec4 tex_color = texture2D(u_tex, uv);

	float lumin = dot( tex_color.xyz, vec3( 0.3, 0.59, 0.11 ) );
	float thres_coeff = smoothstep( u_thresholds.x, u_thresholds.y, lumin );

	gl_FragColor = vec4(tex_color.xyz * thres_coeff, 1.0);
}