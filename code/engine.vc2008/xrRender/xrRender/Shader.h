// Shader.h: interface for the CShader class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "r_constants.h"
#include "../../xrCore/xr_resource.h"

#include "sh_atomic.h"
#include "sh_texture.h"
#include "sh_matrix.h"
#include "sh_constant.h"
#include "sh_rt.h"

using sh_list = xr_vector<shared_str>;
class CBlender_Compile;
class IBlender;
#define	SHADER_PASSES_MAX 2

#pragma pack(push,4)

//////////////////////////////////////////////////////////////////////////
struct ECORE_API STextureList : public xr_resource_flagged, public xr_vector<std::pair<u32, ref_texture>>
{
	using inherited_vec = xr_vector<std::pair<u32, ref_texture>>;

	~STextureList();

	inline BOOL equal(const STextureList& base) const
	{
		if (size() != base.size())
			return FALSE;

		for (u32 cmp = 0; cmp < size(); cmp++)
		{
			if ((*this)[cmp].first != base[cmp].first)	return FALSE;
			if ((*this)[cmp].second != base[cmp].second)	return FALSE;
		}
		return TRUE;
	}

	virtual void clear();

	//	Avoid using this function.
	//	If possible use precompiled texture list.
	u32 find_texture_stage(const shared_str &TexName) const;
};

using ref_texture_list = resptr_core<STextureList, resptr_base<STextureList>>;

//////////////////////////////////////////////////////////////////////////
struct ECORE_API SMatrixList : public xr_resource_flagged, public svector<ref_matrix, 4> { ~SMatrixList(); };
using ref_matrix_list = resptr_core<SMatrixList, resptr_base<SMatrixList>>;

//////////////////////////////////////////////////////////////////////////
struct ECORE_API SConstantList : public xr_resource_flagged, public svector<ref_constant_obsolette, 4> { ~SConstantList(); };
using ref_constant_list = resptr_core<SConstantList, resptr_base<SConstantList>>;

//////////////////////////////////////////////////////////////////////////
struct ECORE_API SGeometry : public xr_resource_flagged
{
	u32 vb_stride;
	ref_declaration	dcl;
	ID3DVertexBuffer *vb;
	ID3DIndexBuffer *ib;
	~SGeometry();
};

struct 	ECORE_API	resptrcode_geom : public resptr_base<SGeometry>
{
	void create(D3DVERTEXELEMENT9* decl, ID3DVertexBuffer* vb, ID3DIndexBuffer* ib);
	void create(u32 FVF, ID3DVertexBuffer* vb, ID3DIndexBuffer* ib);
	void destroy() { _set(NULL); }
	u32  stride() const { return _get()->vb_stride; }
};
using ref_geom = resptr_core<SGeometry, resptrcode_geom>;

//////////////////////////////////////////////////////////////////////////
struct ECORE_API SPass : public xr_resource_flagged
{
	// Generic state, like Z-Buffering, samplers, etc
	ref_state	state;
	// may be NULL = FFP, in that case "state" must contain TSS setup
	ref_ps		ps;
	// may be NULL = FFP, in that case "state" must contain RS setup, *and* FVF-compatible declaration must be used
	ref_vs		vs;
#if defined(USE_DX10) || defined(USE_DX11)
	ref_gs		gs;			// may be NULL = don't use geometry shader at all
#	ifdef USE_DX11
	ref_hs		hs;			// may be NULL = don't use hull shader at all
	ref_ds		ds;			// may be NULL = don't use domain shader at all
	ref_cs		cs;			// may be NULL = don't use compute shader at all
#	endif
#endif	//	USE_DX10

	ref_ctable constants;	// may be NULL

	ref_texture_list T;
	ref_constant_list C;

#ifdef _EDITOR
	ref_matrix_list M;
#endif

	~SPass();

	BOOL equal(const SPass& other);
};
using ref_pass = resptr_core<SPass, resptr_base<SPass>>;

//////////////////////////////////////////////////////////////////////////
struct 	 ECORE_API	ShaderElement : public xr_resource_flagged
{
public:
	struct Sflags
	{
		u32	iPriority : 2;
		u32	bStrictB2F : 1;
		u32	bEmissive : 1;
		u32	bDistort : 1;
		u32	bWmark : 1;
	};
public:
	Sflags flags;
	svector<ref_pass, SHADER_PASSES_MAX> passes;

	ShaderElement();
	~ShaderElement();
	BOOL equal(ShaderElement& S);
	BOOL equal(ShaderElement* S);
};
using ref_selement = resptr_core<ShaderElement, resptr_base<ShaderElement>>;

//////////////////////////////////////////////////////////////////////////
struct ECORE_API Shader : public xr_resource_flagged
{
public:
	ref_selement E[6];	// R1 - 0=norm_lod0(det),	1=norm_lod1(normal),	2=L_point,		3=L_spot,	4=L_for_models,
										// R2 - 0=deffer,			1=norm_lod1(normal),	2=psm,			3=ssm,		4=dsm
	~Shader();
	BOOL equal(Shader& S);
	BOOL equal(Shader* S);
};

struct ECORE_API resptrcode_shader : public resptr_base<Shader>
{
	void create(LPCSTR s_shader = 0, LPCSTR s_textures = 0, LPCSTR s_constants = 0, LPCSTR s_matrices = 0);
	void create(IBlender*	B, LPCSTR s_shader = 0, LPCSTR s_textures = 0, LPCSTR s_constants = 0, LPCSTR s_matrices = 0);
	void destroy() { _set(NULL); }
};
using ref_shader = resptr_core<Shader, resptrcode_shader>;

enum SE_R1
{
	SE_R1_NORMAL_HQ = 0,	// high quality/detail
	SE_R1_NORMAL_LQ = 1,	// normal or low quality
	SE_R1_LPOINT = 2,	// add: point light
	SE_R1_LSPOT = 3,	// add:	spot light
	SE_R1_LMODELS = 4,	// lighting info for models or shadowing from models
};

//#define		SE_R2_NORMAL_HQ		0	// high quality/detail
//#define		SE_R2_NORMAL_LQ		1	// low quality
//#define		SE_R2_SHADOW		2	// shadow generation
//	E[3] - can use for night vision but need to extend SE_R1. Will need
//	Extra shader element.
//	E[4] - distortion or self illumination(self emission).
//	E[4] Can use for lightmap capturing.

#pragma pack(pop)