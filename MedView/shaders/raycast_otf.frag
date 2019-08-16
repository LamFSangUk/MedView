#version 330

in vec3 ray_entry;

out vec4 out_color;

// Volume data
uniform sampler2D backtex;
uniform sampler3D voltex;

uniform float screen_width;
uniform float screen_height;

// Windowing
uniform float window_min;
uniform float window_max;

vec4 colour_transfer(float intensity){
	vec3 high = vec3(1.0);
	vec3 low = vec3(0.0);
	float alpha = (exp(intensity) - 1.0)/(exp(1.0) - 1.0);
	return vec4(intensity*high+(1.0-intensity)*low,alpha);
}

float simple_windowing(float intensity){
	if(intensity < window_min) intensity = window_min;
	if(intensity > window_max) intensity = window_max;

	return (intensity-window_min) / (window_max-window_min);
}

void main()
{
	// const for ray-cast
	int nb_samples = 2000;

	vec3 start_pos = ray_entry;
	vec3 exit_pos = texture(backtex, gl_FragCoord.st / vec2(screen_width, screen_height)).xyz;

	if(start_pos == exit_pos){
		out_color = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}

	vec3 ray = exit_pos - start_pos;
	
	float len = length(ray);
	float sample_step_size = len / nb_samples;
	vec3 ray_dir = normalize(ray);
	vec3 ray_delta = ray_dir * sample_step_size;

	vec3 pos = start_pos;

	float max_intensity = 0;
	float intensity;

	out_color = vec4(0.0);

	for(int i = 0; i < nb_samples && len>0; i++, pos+=ray_delta, len-=sample_step_size){
		// Sample intensity from volume data
		intensity = texture(voltex, pos).r;

		intensity = simple_windowing(intensity);

		vec4 color = colour_transfer(intensity);
		
		out_color.rgb = color.a *color.rgb + (1.0 -color.a) *out_color.a*out_color.rgb;
		out_color.a = color.a + (1.0 - color.a) * out_color.a;

		if(out_color.a >= 1.0){
			out_color.a =1.0;
			break;
		}
	}
	// Blend background
    out_color.rgb = out_color.a * out_color.rgb + (1 - out_color.a) * pow(vec3(0.0), vec3(2.2)).rgb;
    out_color.a = 1.0;

    // Gamma correction
    out_color.rgb = pow(out_color.rgb, vec3(1.0 / 2.2));
    out_color.a = out_color.a;
}