#pragma once
#include "../xrGame/GameObject.h"
#include "../xrParticles/ParticlesPlayer.h"
#include "../xrEngine/iobjectphysicscollision.h"
#include "../xrphysics/iphysicsshellholder.h"

class CPHDestroyable;
class CPHCollisionDamageReceiver;
class CPHSoundPlayer;
class IDamageSource;
class CPHSkeleton;
class CCharacterPhysicsSupport;
class ICollisionDamageInfo;
class CIKLimbsController;



class CPhysicsShellHolder:  public CGameObject,
							public CParticlesPlayer,
							public IObjectPhysicsCollision,
							public IPhysicsShellHolder
	
{
	bool				b_sheduled;
public:
	void	SheduleRegister		(){if(!IsSheduled())shedule_register();b_sheduled=true;}
	void	SheduleUnregister	(){if(IsSheduled())shedule_unregister();b_sheduled=false;}
IC	bool	IsSheduled			(){return b_sheduled;}	
public:

	using inherited = CGameObject;
	

	IPhysicsShellEx			*m_pPhysicsShell;


			CPhysicsShellHolder							();
	virtual	~CPhysicsShellHolder						();

	virtual bool		ActivationSpeedOverriden (Fvector& dest, bool clear_override) { return false; }

	IC IPhysicsShellEx	*&PPhysicsShell				()		
	{
		return m_pPhysicsShell;
	}

	IC CPhysicsShellHolder*	PhysicsShellHolder	()
	{
		return this;
	}
	virtual	const IObjectPhysicsCollision*physics_collision			()							;
	virtual	const IPhysicsShell			*physics_shell				()const						;
	virtual		  IPhysicsShell			*physics_shell				()							;
	virtual const IPhysicsElement		*physics_character			()const						;
	virtual CPHDestroyable				*ph_destroyable				()							{return nullptr;}
	virtual ICollisionDamageReceiver	*PHCollisionDamageReceiver	()							{return nullptr;}
	virtual CPHSkeleton					*PHSkeleton					()							{return nullptr;}
	virtual CPhysicsShellHolder			*cast_physics_shell_holder	()							{return this;}
	virtual CParticlesPlayer			*cast_particles_player		()							{return this;}
	virtual IDamageSource				*cast_IDamageSource			()							{return nullptr;}
	virtual CPHSoundPlayer				*ph_sound_player			()  						{return nullptr;}
	virtual	CCharacterPhysicsSupport	*character_physics_support	()							{return nullptr;}
	virtual	const CCharacterPhysicsSupport	*character_physics_support	() const				{return nullptr;}
	virtual	CIKLimbsController			*character_ik_controller	()							{return nullptr;}
	virtual ICollisionHitCallback		*get_collision_hit_callback ()							{return nullptr;}
	virtual void						set_collision_hit_callback	(ICollisionHitCallback *cc)	{;}
	virtual void			_BCL			enable_notificate			()							{;}
public:

	virtual void			PHGetLinearVell		(Fvector& velocity);
	virtual void			PHSetLinearVell		(Fvector& velocity);
	virtual void			PHSetMaterial		(LPCSTR m);
	virtual void			PHSetMaterial		(u16 m);
			void			PHSaveState			(NET_Packet &P);
			void			PHLoadState			(IReader &P);
	virtual f32				GetMass				();
	virtual	void			PHHit				(SHit &H);
	virtual	void			Hit					(SHit* pHDS);
///////////////////////////////////////////////////////////////////////
	virtual u16				PHGetSyncItemsNumber();
	virtual CPHSynchronize*	PHGetSyncItem		(u16 item);
	virtual void			PHUnFreeze			();
	virtual void			PHFreeze			();
	virtual float			EffectiveGravity	();
///////////////////////////////////////////////////////////////
	virtual void			create_physic_shell		();
	virtual void			activate_physic_shell	();
	virtual void			setup_physic_shell		();
	virtual void			deactivate_physics_shell ();

	virtual void			net_Destroy			();
	virtual BOOL			net_Spawn			(CSE_Abstract*	DC);
	virtual void			save				(NET_Packet &output_packet);
	virtual void			load				(IReader &input_packet);
			void			init				();

	virtual void			OnChangeVisual		();
	//��� ������������ CParticlesPlayer
	virtual void			UpdateCL			();
			void			correct_spawn_pos	();
protected:
	virtual	bool			has_shell_collision_place( const CPhysicsShellHolder* obj ) const;
	virtual void			on_child_shell_activate	 ( CPhysicsShellHolder* obj );
public:
	virtual bool			register_schedule	() const;

public:
	virtual	void					_BCL					on_physics_disable					();
private://IPhysicsShellHolder
	virtual	Fmatrix&				_BCL					ObjectXFORM							()						;
	virtual	Fvector&				_BCL					ObjectPosition						()						;
	virtual	LPCSTR					_BCL					ObjectName							()		const			;
	virtual	LPCSTR					_BCL					ObjectNameVisual					()		const			;
	virtual	LPCSTR					_BCL					ObjectNameSect						()		const			;
	virtual	bool					_BCL					ObjectGetDestroy					()		const			;
	virtual ICollisionHitCallback*	_BCL 					ObjectGetCollisionHitCallback		()						;
	virtual	u16						_BCL					ObjectID							()		const			;
	virtual	ICollisionForm*			_BCL					ObjectCollisionModel				()						;
	virtual	IKinematics*			_BCL					ObjectKinematics					()						;
	virtual IDamageSource*			_BCL					ObjectCastIDamageSource				()						;
	virtual	void					_BCL					ObjectProcessingDeactivate			()						;
	virtual	void					_BCL					ObjectProcessingActivate			()						;				
	virtual	void					_BCL					ObjectSpatialMove					()						;
	virtual	IPhysicsShellEx*&			_BCL					ObjectPPhysicsShell					()						;
	virtual bool					_BCL					has_parent_object					()						;
	virtual	IPHCapture*				_BCL					PHCapture							()						;
	virtual	bool					_BCL					IsInventoryItem						()						;
	virtual	bool					_BCL					IsActor								()						;
	virtual bool					_BCL					IsStalker							()						;
	virtual	void					_BCL					HideAllWeapons						( bool v )				;//(SetWeaponHideState(INV_STATE_BLOCK_ALL,true))
	virtual	void					_BCL					MovementCollisionEnable				( bool enable )			;
	virtual CPHSoundPlayer*			_BCL					ObjectPhSoundPlayer					()  					{return ph_sound_player();}
	virtual	ICollisionDamageReceiver* _BCL				ObjectPhCollisionDamageReceiver		()						;
	virtual	void					_BCL					BonceDamagerCallback				(float &damage_factor)	;
	virtual	std::string				_BCL					dump								(EDumpType type) const  ;
};
