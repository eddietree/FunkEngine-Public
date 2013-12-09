varying mediump vec2 uv;

uniform sampler2D u_tex;

uniform sampler2D u_tex_blur_0; // sharpest
uniform sampler2D u_tex_blur_1;
uniform sampler2D u_tex_blur_2;
uniform sampler2D u_tex_blur_3; // blurrest

uniform float u_bloom_coeff;

void main()
{
	vec3 raw_color =  texture2D( u_tex, uv ).xyz;

	// 4 bloom textures
	vec3 bloom0 =  texture2D( u_tex_blur_0, uv ).xyz;
	vec3 bloom1 =  texture2D( u_tex_blur_1, uv ).xyz;
	vec3 bloom2 =  texture2D( u_tex_blur_2, uv ).xyz;
	vec3 bloom3 =  texture2D( u_tex_blur_3, uv ).xyz;

	float bloomCoeff = 0.25;
	vec4 bloom_weights = vec4(0.25);
	vec3 bloom = (bloom0*bloom_weights.x + bloom1*bloom_weights.y + bloom2*bloom_weights.z + bloom3*bloom_weights.w);
	
	vec3 final_color = raw_color + bloom * u_bloom_coeff;

	// vignette effect
	float dist_to_center = length(uv*2.0-vec2(1.0));
	float vignette = smoothstep( 0.65, 2.0, dist_to_center);
	vignette = 1.0f-mix( 0.0, 0.8, vignette );

	gl_FragColor = vec4( final_color* vignette, 1.0 );
}
