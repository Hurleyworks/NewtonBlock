#pragma once

/*
Copyright (c) 2010, The Cinder Project: http://libcinder.org
All rights reserved.

This code is intended for use with the Cinder C++ library: http://libcinder.org

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and
the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


#define STRINGIFY(A) #A

//======================================= shadowVert ====================================================

static const char * shadowVert = "#version 410\n" STRINGIFY(

in vec4 ciPosition;
in vec3 ciNormal;
in vec4 ciColor;

uniform mat4 ciModelMatrix;
uniform mat4 ciModelView;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelViewProjection;

// ShadowMatrix converts from modeling coordinates to shadow map coordinates.
uniform mat4 uShadowMatrix;

out vec4	vColor;
out vec4	vPosition;
out vec3	vNormal;
out vec4	vModelPosition;
out vec3	vModelNormal;
out vec2	vTexCoord0;
// Coordinate to be used for shadow map lookup
out vec4	vShadowCoord;

/* Bias matrix alters the clip coordinates so that x & y
* lie between 0.0 and 1.0 for texture sampling. */
const mat4 biasMat = mat4(0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);
void main() {
	vColor = ciColor;
	vPosition = ciModelView * ciPosition;
	vModelPosition = ciModelMatrix * ciPosition;
	vModelNormal = (ciModelMatrix * vec4(ciNormal, 0.0)).xyz;
	vNormal = normalize(ciNormalMatrix * ciNormal);

	vShadowCoord = (biasMat * uShadowMatrix * ciModelMatrix) * ciPosition;
	gl_Position = ciModelViewProjection * ciPosition;
}
);

//======================================= shadowFrag ====================================================
static const char * shadowFrag = "#version 410\n" STRINGIFY(

uniform float ciElapsedSeconds;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;
in vec4 vModelPosition;
in vec3 vModelNormal;
in vec4	vShadowCoord;


uniform vec3			uLightPos;
uniform bool			uIsTeapot;

uniform sampler2DShadow	uShadowMap;
uniform mat4			uShadowMatrix;
uniform int				uShadowTechnique;
uniform float			uDepthBias;
uniform bool			uOnlyShadowmap;
uniform float			uRandomOffset;
uniform bool			uEnableNormSlopeOffset;
uniform int				uNumRandomSamples;

out vec4 fragColor;

float rand(vec2 seed) {
	return fract(sin(dot(seed.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(vec4 seed)
{
	float dot_product = dot(seed, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

vec4 getRandomOffset(int i) {
	float indexA = rand(vec4(gl_FragCoord.xyx, ciElapsedSeconds*i)) - 0.5;
	float indexB = rand(vec4(gl_FragCoord.yxy * i, ciElapsedSeconds*i)) - 0.5;
	return vec4(vec2(indexA, indexB), 0, 0);
}

vec2 getNormSlopeBias(vec3 N, vec3 L) {

	float cos_alpha = clamp(dot(N, L), 0.0, 1.0);
	float offset_scale_N = sqrt(1 - cos_alpha * cos_alpha); // sin(acos(L·N))
	float offset_scale_L = offset_scale_N / cos_alpha;    // tan(acos(L·N))
	return vec2(offset_scale_N, min(2.0, offset_scale_L));
}

float sampleBasic(vec4 sc) {
	return textureProj(uShadowMap, sc);
}

float samplePCF3x3(vec4 sc)
{
	const int s = 2;

	float shadow = 0.0;
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, 0));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(0, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(0, 0));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(0, s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, 0));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, s));
	return shadow / 9.0;;
}

float samplePCF4x4(vec4 sc)
{
	const int r = 2;
	const int s = 2 * r;

	float shadow = 0.0;
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-r, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(r, -s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, -s));

	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, -r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-r, -r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(r, -r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, -r));

	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-r, r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(r, r));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, r));

	shadow += textureProjOffset(uShadowMap, sc, ivec2(-s, s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(-r, s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(r, s));
	shadow += textureProjOffset(uShadowMap, sc, ivec2(s, s));

	return shadow / 16.0;
}

float sampleRandom(vec4 sc, vec2 normSlopeBias)
{
	float shadow = 0.0;
	for (int i = 0; i< uNumRandomSamples; i++) {
		vec4 off = getRandomOffset(i);
		off.xy *= normSlopeBias;
		shadow += textureProj(uShadowMap, sc + off);
	}
	return shadow / float(uNumRandomSamples);
}

void main()
{
	// Normal in view space
	vec3	N = normalize(vNormal);
	// Light direction
	vec3	L = normalize(uLightPos - vPosition.xyz);
	// To camera vector
	vec3	C = normalize(-vPosition.xyz);
	// Surface reflection vector
	vec3	R = normalize(-reflect(L, N));

	// Diffuse factor
	float NdotL = max(dot(N, L), 0.0);
	vec3	D = vec3(NdotL);
	// Specular factor
	vec3	S = pow(max(dot(R, C), 0.0), 50.0) * vec3(1.0);

	// Sample the shadowmap to compute the shadow value
	float shadow = 1.0f;
	vec4 sc = vShadowCoord;
	sc.z += uDepthBias;

	if (uShadowTechnique == 0) {
		shadow = sampleBasic(sc);
	}
	else if (uShadowTechnique == 1) {
		shadow = samplePCF3x3(sc);
	}
	else if (uShadowTechnique == 2) {
		shadow = samplePCF4x4(sc);
	}
	else if (uShadowTechnique == 3) {
		vec2 offset = mix(vec2(uRandomOffset), getNormSlopeBias(N, L), float(uEnableNormSlopeOffset));
		shadow = sampleRandom(sc, offset);
	}

	fragColor.rgb = mix(((D + S) * shadow) * vColor.rgb, vec3(shadow), float(uOnlyShadowmap));
	fragColor.a = 1.0;
}
);
