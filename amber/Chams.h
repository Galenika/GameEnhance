#pragma once
#include "SDK.h"

// Texture groups
#define TEXTURE_GROUP_LIGHTMAP                      "Lightmaps"
#define TEXTURE_GROUP_WORLD                         "World textures"
#define TEXTURE_GROUP_MODEL                         "Model textures"
#define TEXTURE_GROUP_VGUI                          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE                      "Particle textures"
#define TEXTURE_GROUP_DECAL                         "Decal textures"
#define TEXTURE_GROUP_SKYBOX                        "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS                "ClientEffect textures"
#define TEXTURE_GROUP_OTHER                         "Other textures"
#define TEXTURE_GROUP_PRECACHED                     "Precached"             
#define TEXTURE_GROUP_CUBE_MAP                      "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET                 "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED                   "Unaccounted textures"  
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER           "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP     "Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR    "Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD    "World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS   "Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER    "Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER          "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER         "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER                  "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL                    "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS                 "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS                "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE         "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS                 "Morph Targets"

void InitKeyValues(KeyValues* keyValues, char* name);
void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer);
void ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit = true, bool isWireframe = false);