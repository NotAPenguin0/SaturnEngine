#version 430 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoords;

out vec2 TexCoords;

layout(location = 0) uniform vec2 Scale;
layout(location = 1) uniform vec2 Position;
layout(location = 2) uniform vec2 Offset;

float map_range(float val, float input_start, float input_end, float output_start, float output_end) {
    const float slope = (output_end - output_start) / (input_end - input_start);
    float result = output_start + slope * (val - input_start);
    return result;
}

vec2 map_range(vec2 v, float input_start, float input_end, float output_start, float output_end) {
	return vec2(map_range(v.x, input_start, input_end, output_start, output_end),
				map_range(v.y, input_start, input_end, output_start, output_end));
}

void main()
{
	// remap from NDC to allow for easier scaling
	vec2 pos_remap = map_range(vec2(iPos.x, iPos.y), -1, 1, 0, 1);
	pos_remap.x *= Scale.x;
	pos_remap.y *= Scale.y;
	// Make sure the UI canvas is relative to the top left corner
	pos_remap.y += (1 - Scale.y);
	pos_remap += vec2(Position.x, -Position.y);
    pos_remap += vec2(Offset.x, -Offset.y);
	// map back to NDC
	vec2 pos_scaled = map_range(pos_remap, 0, 1, -1, 1);
	gl_Position = vec4(pos_scaled.x, pos_scaled.y, 0.0, 1.0);
	TexCoords = vec2(iTexCoords.x, 1 - iTexCoords.y);
}