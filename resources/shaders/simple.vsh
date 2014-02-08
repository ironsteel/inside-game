#version 100
attribute vec4 position;
attribute vec2 atexcoord;
attribute vec3 normal;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 lighPosition;

varying vec2 v_texCoord;
varying vec3 v_normal;
varying vec3 v_lightPosition;


void main()
{
    v_texCoord = atexcoord;
	vec3 eyePosition = vec3(modelMatrix * position);
	v_normal = normalize(normalMatrix * normal);
	v_lightPosition = normalize(lighPosition - eyePosition);
	gl_Position = mvp * position;
}