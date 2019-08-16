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

		// Maximum Intensity Projection
		if(intensity > max_intensity){
			max_intensity = intensity;
		}
	}

	out_color = vec4(vec3(max_intensity), 1.0);
}