varying mediump vec2 uv;

uniform sampler2D u_tex;
uniform vec2 u_uv_delta;

void main()
{
	gl_FragColor = texture2D(u_tex, uv);

	vec2 uvStep = u_uv_delta;
	vec4 sum = vec4(0.0f);
 
   // 9 samples
   sum += texture2D(u_tex, uv - 4.0f*uvStep) * 0.05f;
   sum += texture2D(u_tex, uv - 3.0f*uvStep) * 0.09f;
   sum += texture2D(u_tex, uv - 2.0f*uvStep) * 0.12f;
   sum += texture2D(u_tex, uv - uvStep) * 0.15f;
   sum += texture2D(u_tex, uv) * 0.16f;
   sum += texture2D(u_tex, uv + uvStep) * 0.15f;
   sum += texture2D(u_tex, uv + 2.0f*uvStep) * 0.12f;
   sum += texture2D(u_tex, uv + 3.0f*uvStep) * 0.09f;
   sum += texture2D(u_tex, uv + 4.0f*uvStep) * 0.05f;
 
   gl_FragColor = sum;
}