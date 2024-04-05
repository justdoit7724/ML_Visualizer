#pragma once


#define SHADER_REG_CB_DIRECTIONAL_LIGHT 0
#define SHADER_REG_CB_POINT_LIGHT 1
#define SHADER_REG_CB_SPOT_LIGHT 2
#define SHADER_REG_CB_EYE 3
#define SHADER_REG_CB_UNLIT 4
#define SHADER_REG_CB_MATERIAL 5
#define SHADER_REG_CB_COLOR 6

#define SHADER_REG_SRV_CM 0
#define SHADER_REG_SRV_DCM 1
#define SHADER_REG_SRV_DIFFUSE 2
#define SHADER_REG_SRV_NORMAL 3
#define SHADER_REG_SRV_SSAO 4
#define SHADER_REG_SRV_SHADOW 5
#define SHADER_REG_SRV_SHADOW_TRANSP 6
#define SHADER_REG_SRV_DISPLACE 7
#define SHADER_REG_SRV_ROUGHNESS 8
#define SHADER_REG_SRV_METALLIC 9
#define SHADER_REG_SRV_DEPTH 10

#define SHADER_REG_SAMP_POINT 0
#define SHADER_REG_SAMP_LINEAR 1
#define SHADER_REG_SAMP_LINEAR_POINT 3
#define SHADER_REG_SAMP_ANISOTROPIC 4
#define SHADER_REG_SAMP_CMP_POINT 5
#define SHADER_REG_SAMP_CMP_LINEAR_POINT 6