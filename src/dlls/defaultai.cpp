/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// Default behaviors.
//=========================================================
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"soundent.h"
#include	"nodes.h"
#include	"scripted.h"

//=========================================================
// Fail
//=========================================================
Task_t	tlFail[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE) },
	{ TASK_WAIT,				2.f		},
	{ TASK_WAIT_PVS,			0.f		},
};

Schedule_t	slFail[] =
{
	{
		tlFail,
		VARRAYSIZE ( tlFail ),
		bits_COND_CAN_ATTACK,
		0,
		"Fail"
	},
};

//=========================================================
//	Idle Schedules
//=========================================================
Task_t	tlIdleStand1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE) },
	{ TASK_WAIT,				5.f		},// repick IDLESTAND every five seconds. gives us a chance to pick an active idle, fidget, etc.
};

Schedule_t	slIdleStand[] =
{
	{
		tlIdleStand1,
		VARRAYSIZE ( tlIdleStand1 ),
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"IdleStand"
	},
};

Schedule_t	slIdleTrigger[] =
{
	{
		tlIdleStand1,
		VARRAYSIZE ( tlIdleStand1 ),
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE,
		0,
		"Idle Trigger"
	},
};


Task_t	tlIdleWalk1[] =
{
	{ TASK_WALK_PATH,			9999.f },
	{ TASK_WAIT_FOR_MOVEMENT,	0.f	},
};

Schedule_t	slIdleWalk[] =
{
	{
		tlIdleWalk1,
		VARRAYSIZE ( tlIdleWalk1 ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"Idle Walk"
	},
};

//=========================================================
// Ambush - monster stands in place and waits for a new
// enemy, or chance to attack an existing enemy.
//=========================================================
Task_t	tlAmbush[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE) },
	{ TASK_WAIT_INDEFINITE,		0.f		},
};

Schedule_t	slAmbush[] =
{
	{
		tlAmbush,
		VARRAYSIZE ( tlAmbush ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED,

		0,
		"Ambush"
	},
};

//=========================================================
// ActiveIdle schedule - !!!BUGBUG - if this schedule doesn't
// complete on its own, the monster's HintNode will not be
// cleared, and the rest of the monster's group will avoid
// that node because they think the group member that was
// previously interrupted is still using that node to active
// idle.
///=========================================================
Task_t tlActiveIdle[] =
{
	{ TASK_FIND_HINTNODE,			0.f	},
	{ TASK_GET_PATH_TO_HINTNODE,	0.f	},
	{ TASK_STORE_LASTPOSITION,		0.f	},
	{ TASK_WALK_PATH,				0.f	},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f	},
	{ TASK_FACE_HINTNODE,			0.f	},
	{ TASK_PLAY_ACTIVE_IDLE,		0.f	},
	{ TASK_GET_PATH_TO_LASTPOSITION,0.f	},
	{ TASK_WALK_PATH,				0.f	},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f	},
	{ TASK_CLEAR_LASTPOSITION,		0.f	},
	{ TASK_CLEAR_HINTNODE,			0.f	},
};

Schedule_t slActiveIdle[] =
{
	{
		tlActiveIdle,
		VARRAYSIZE( tlActiveIdle ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED		|
		bits_COND_HEAR_SOUND,

		bits_SOUND_COMBAT		|
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER,
		"Active Idle"
	}
};

//=========================================================
//	Wake Schedules
//=========================================================
Task_t tlWakeAngry1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE) },
	{ TASK_SOUND_WAKE,			0.f	},
	{ TASK_FACE_IDEAL,			0.f	},
};

Schedule_t slWakeAngry[] =
{
	{
		tlWakeAngry1,
		VARRAYSIZE ( tlWakeAngry1 ),
		0,
		0,
		"Wake Angry"
	}
};

//=========================================================
// AlertFace Schedules
//=========================================================
Task_t	tlAlertFace1[] =
{
	{ TASK_STOP_MOVING,				0.f				},
	{ TASK_SET_ACTIVITY,			static_cast<float>(ACT_IDLE)	},
	{ TASK_FACE_IDEAL,				0.f		},
};

Schedule_t	slAlertFace[] =
{
	{
		tlAlertFace1,
		VARRAYSIZE ( tlAlertFace1 ),
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED,
		0,
		"Alert Face"
	},
};

//=========================================================
// AlertSmallFlinch Schedule - shot, but didn't see attacker,
// flinch then face
//=========================================================
Task_t	tlAlertSmallFlinch[] =
{
	{ TASK_STOP_MOVING,				0.f						},
	{ TASK_REMEMBER,				static_cast<float>(bits_MEMORY_FLINCHED) },
	{ TASK_SMALL_FLINCH,			0.f				},
	{ TASK_SET_SCHEDULE,			static_cast<float>(SCHED_ALERT_FACE)	},
};

Schedule_t	slAlertSmallFlinch[] =
{
	{
		tlAlertSmallFlinch,
		VARRAYSIZE ( tlAlertSmallFlinch ),
		0,
		0,
		"Alert Small Flinch"
	},
};

//=========================================================
// AlertIdle Schedules
//=========================================================
Task_t	tlAlertStand1[] =
{
	{ TASK_STOP_MOVING,			0.f						 },
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE)			 },
	{ TASK_WAIT,				20.f				 },
	{ TASK_SUGGEST_STATE,		static_cast<float>(MONSTERSTATE_IDLE) },
};

Schedule_t	slAlertStand[] =
{
	{
		tlAlertStand1,
		VARRAYSIZE ( tlAlertStand1 ),
		bits_COND_NEW_ENEMY				|
		bits_COND_SEE_ENEMY				|
		bits_COND_SEE_FEAR				|
		bits_COND_LIGHT_DAMAGE			|
		bits_COND_HEAVY_DAMAGE			|
		bits_COND_PROVOKED				|
		bits_COND_SMELL					|
		bits_COND_SMELL_FOOD			|
		bits_COND_HEAR_SOUND,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scent flags
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"Alert Stand"
	},
};

//=========================================================
// InvestigateSound - sends a monster to the location of the
// sound that was just heard, to check things out.
//=========================================================
Task_t tlInvestigateSound[] =
{
	{ TASK_STOP_MOVING,				0.f				},
	{ TASK_STORE_LASTPOSITION,		0.f				},
	{ TASK_GET_PATH_TO_BESTSOUND,	0.f				},
	{ TASK_FACE_IDEAL,				0.f				},
	{ TASK_WALK_PATH,				0.f				},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f				},
	{ TASK_PLAY_SEQUENCE,			static_cast<float>(ACT_IDLE)			},
	{ TASK_WAIT,					10.f				},
	{ TASK_GET_PATH_TO_LASTPOSITION,0.f				},
	{ TASK_WALK_PATH,				0.f				},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f				},
	{ TASK_CLEAR_LASTPOSITION,		0.f				},
};

Schedule_t	slInvestigateSound[] =
{
	{
		tlInvestigateSound,
		VARRAYSIZE ( tlInvestigateSound ),
		bits_COND_NEW_ENEMY			|
		bits_COND_SEE_FEAR			|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"InvestigateSound"
	},
};

//=========================================================
// CombatIdle Schedule
//=========================================================
Task_t	tlCombatStand1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_SET_ACTIVITY,		static_cast<float>(ACT_IDLE) },
	{ TASK_WAIT_INDEFINITE,		0.f		},
};

Schedule_t	slCombatStand[] =
{
	{
		tlCombatStand1,
		VARRAYSIZE ( tlCombatStand1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_CAN_ATTACK,
		0,
		"Combat Stand"
	},
};

//=========================================================
// CombatFace Schedule
//=========================================================
Task_t	tlCombatFace1[] =
{
	{ TASK_STOP_MOVING,				0.f				},
	{ TASK_SET_ACTIVITY,			static_cast<float>(ACT_IDLE)	},
	{ TASK_FACE_ENEMY,				0.f		},
};

Schedule_t	slCombatFace[] =
{
	{
		tlCombatFace1,
		VARRAYSIZE ( tlCombatFace1 ),
		bits_COND_CAN_ATTACK			|
		bits_COND_NEW_ENEMY				|
		bits_COND_ENEMY_DEAD,
		0,
		"Combat Face"
	},
};

//=========================================================
// Standoff schedule. Used in combat when a monster is
// hiding in cover or the enemy has moved out of sight.
// Should we look around in this schedule?
//=========================================================
Task_t	tlStandoff[] =
{
	{ TASK_STOP_MOVING,				0.f					},
	{ TASK_SET_ACTIVITY,			static_cast<float>(ACT_IDLE)				},
	{ TASK_WAIT_FACE_ENEMY,			2.f					},
};

Schedule_t slStandoff[] =
{
	{
		tlStandoff,
		VARRAYSIZE ( tlStandoff ),
		bits_COND_CAN_RANGE_ATTACK1		|
		bits_COND_CAN_RANGE_ATTACK2		|
		bits_COND_ENEMY_DEAD			|
		bits_COND_NEW_ENEMY				|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Standoff"
	}
};

//=========================================================
// Arm weapon (draw gun)
//=========================================================
Task_t	tlArmWeapon[] =
{
	{ TASK_STOP_MOVING,		0				},
	{ TASK_PLAY_SEQUENCE,	static_cast<float>(ACT_ARM) }
};

Schedule_t slArmWeapon[] =
{
	{
		tlArmWeapon,
		VARRAYSIZE ( tlArmWeapon ),
		0,
		0,
		"Arm Weapon"
	}
};

//=========================================================
// reload schedule
//=========================================================
Task_t	tlReload[] =
{
	{ TASK_STOP_MOVING,			0					},
	{ TASK_PLAY_SEQUENCE,		float(ACT_RELOAD)	},
};

Schedule_t slReload[] =
{
	{
		tlReload,
		VARRAYSIZE ( tlReload ),
		bits_COND_HEAVY_DAMAGE,
		0,
		"Reload"
	}
};

//=========================================================
//	Attack Schedules
//=========================================================

// primary range attack
Task_t	tlRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_RANGE_ATTACK1,		0.f		},
};

Schedule_t	slRangeAttack1[] =
{
	{
		tlRangeAttack1,
		VARRAYSIZE ( tlRangeAttack1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Range Attack1"
	},
};

// secondary range attack
Task_t	tlRangeAttack2[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_RANGE_ATTACK2,		0.f		},
};

Schedule_t	slRangeAttack2[] =
{
	{
		tlRangeAttack2,
		VARRAYSIZE ( tlRangeAttack2 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Range Attack2"
	},
};

// primary melee attack
Task_t	tlPrimaryMeleeAttack1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_MELEE_ATTACK1,		0.f		},
};

Schedule_t	slPrimaryMeleeAttack[] =
{
	{
		tlPrimaryMeleeAttack1,
		VARRAYSIZE ( tlPrimaryMeleeAttack1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED,
		0,
		"Primary Melee Attack"
	},
};

// secondary melee attack
Task_t	tlSecondaryMeleeAttack1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_MELEE_ATTACK2,		0.f		},
};

Schedule_t	slSecondaryMeleeAttack[] =
{
	{
		tlSecondaryMeleeAttack1,
		VARRAYSIZE ( tlSecondaryMeleeAttack1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED,
		0,
		"Secondary Melee Attack"
	},
};

// special attack1
Task_t	tlSpecialAttack1[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_SPECIAL_ATTACK1,		0.f		},
};

Schedule_t	slSpecialAttack1[] =
{
	{
		tlSpecialAttack1,
		VARRAYSIZE ( tlSpecialAttack1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Special Attack1"
	},
};

// special attack2
Task_t	tlSpecialAttack2[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_ENEMY,			0.f		},
	{ TASK_SPECIAL_ATTACK2,		0.f		},
};

Schedule_t	slSpecialAttack2[] =
{
	{
		tlSpecialAttack2,
		VARRAYSIZE ( tlSpecialAttack2 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Special Attack2"
	},
};

// Chase enemy schedule
Task_t tlChaseEnemy1[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	static_cast<float>(SCHED_CHASE_ENEMY_FAILED)	},
	{ TASK_GET_PATH_TO_ENEMY,	0.f		},
	{ TASK_RUN_PATH,			0.f		},
	{ TASK_WAIT_FOR_MOVEMENT,	0.f		},
};

Schedule_t slChaseEnemy[] =
{
	{
		tlChaseEnemy1,
		VARRAYSIZE ( tlChaseEnemy1 ),
		bits_COND_NEW_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"Chase Enemy"
	},
};


// Chase enemy failure schedule
Task_t	tlChaseEnemyFailed[] =
{
	{ TASK_STOP_MOVING,				0.f					},
	{ TASK_WAIT,					0.2f					},
	{ TASK_FIND_COVER_FROM_ENEMY,	0.f					},
	{ TASK_RUN_PATH,				0.f					},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f					},
	{ TASK_REMEMBER,				static_cast<float>(bits_MEMORY_INCOVER)	},
//	{ TASK_TURN_LEFT,				179.f					},
	{ TASK_FACE_ENEMY,				0.f					},
	{ TASK_WAIT,					1.f					},
};

Schedule_t	slChaseEnemyFailed[] =
{
	{
		tlChaseEnemyFailed,
		VARRAYSIZE ( tlChaseEnemyFailed ),
		bits_COND_NEW_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"tlChaseEnemyFailed"
	},
};


//=========================================================
// small flinch, played when minor damage is taken.
//=========================================================
Task_t tlSmallFlinch[] =
{
	{ TASK_REMEMBER,			static_cast<float>(bits_MEMORY_FLINCHED) },
	{ TASK_STOP_MOVING,			0.f	},
	{ TASK_SMALL_FLINCH,		0.f	},
};

Schedule_t slSmallFlinch[] =
{
	{
		tlSmallFlinch,
		VARRAYSIZE ( tlSmallFlinch ),
		0,
		0,
		"Small Flinch"
	},
};

//=========================================================
// Die!
//=========================================================
Task_t tlDie1[] =
{
	{ TASK_STOP_MOVING,			0				 },
	{ TASK_SOUND_DIE,		0.f			 },
	{ TASK_DIE,				0.f			 },
};

Schedule_t slDie[] =
{
	{
		tlDie1,
		VARRAYSIZE( tlDie1 ),
		0,
		0,
		"Die"
	},
};

//=========================================================
// Victory Dance
//=========================================================
Task_t tlVictoryDance[] =
{
	{ TASK_STOP_MOVING,			0.f							},
	{ TASK_PLAY_SEQUENCE,		static_cast<float>(ACT_VICTORY_DANCE)	},
	{ TASK_WAIT,				0.f					},
};

Schedule_t slVictoryDance[] =
{
	{
		tlVictoryDance,
		VARRAYSIZE( tlVictoryDance ),
		0,
		0,
		"Victory Dance"
	},
};

//=========================================================
// BarnacleVictimGrab - barnacle tongue just hit the monster,
// so play a hit animation, then play a cycling pull animation
// as the creature is hoisting the monster.
//=========================================================
Task_t	tlBarnacleVictimGrab[] =
{
	{ TASK_STOP_MOVING,		0.f				},
	{ TASK_PLAY_SEQUENCE,	static_cast<float>(ACT_BARNACLE_HIT)	 },
	{ TASK_SET_ACTIVITY,	static_cast<float>(ACT_BARNACLE_PULL) },
	{ TASK_WAIT_INDEFINITE,	0.f				 },// just cycle barnacle pull anim while barnacle hoists.
};

Schedule_t slBarnacleVictimGrab[] =
{
	{
		tlBarnacleVictimGrab,
		VARRAYSIZE ( tlBarnacleVictimGrab ),
		0,
		0,
		"Barnacle Victim"
	}
};

//=========================================================
// BarnacleVictimChomp - barnacle has pulled the prey to its
// mouth. Victim should play the BARNCLE_CHOMP animation
// once, then loop the BARNACLE_CHEW animation indefinitely
//=========================================================
Task_t	tlBarnacleVictimChomp[] =
{
	{ TASK_STOP_MOVING,		0.f				},
	{ TASK_PLAY_SEQUENCE,	static_cast<float>(ACT_BARNACLE_CHOMP) },
	{ TASK_SET_ACTIVITY,	static_cast<float>(ACT_BARNACLE_CHEW)  },
	{ TASK_WAIT_INDEFINITE,	0.f				  },// just cycle barnacle pull anim while barnacle hoists.
};

Schedule_t slBarnacleVictimChomp[] =
{
	{
		tlBarnacleVictimChomp,
		VARRAYSIZE ( tlBarnacleVictimChomp ),
		0,
		0,
		"Barnacle Chomp"
	}
};


//	Universal Error Schedule
Task_t	tlError[] =
{
	{ TASK_STOP_MOVING,			0.f },
	{ TASK_WAIT_INDEFINITE,		0.f },
};

Schedule_t	slError[] =
{
	{
		tlError,
		VARRAYSIZE ( tlError ),
		0,
		0,
		"Error"
	},
};

Task_t tlScriptedWalk[] =
{
	{ TASK_WALK_TO_TARGET,		static_cast<float>(TARGET_MOVE_SCRIPTED) },
	{ TASK_WAIT_FOR_MOVEMENT,	0.f		},
	{ TASK_PLANT_ON_SCRIPT,		0.f		},
	{ TASK_FACE_SCRIPT,			0.f		},
	{ TASK_FACE_IDEAL,			0.f		},
	{ TASK_ENABLE_SCRIPT,		0.f		},
	{ TASK_WAIT_FOR_SCRIPT,		0.f		},
	{ TASK_PLAY_SCRIPT,			0.f		},
};

Schedule_t slWalkToScript[] =
{
	{
		tlScriptedWalk,
		VARRAYSIZE ( tlScriptedWalk ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"WalkToScript"
	},
};


Task_t tlScriptedRun[] =
{
	{ TASK_RUN_TO_TARGET,		static_cast<float>(TARGET_MOVE_SCRIPTED) },
	{ TASK_WAIT_FOR_MOVEMENT,	0.f		},
	{ TASK_PLANT_ON_SCRIPT,		0.f		},
	{ TASK_FACE_SCRIPT,			0.f		},
	{ TASK_FACE_IDEAL,			0.f		},
	{ TASK_ENABLE_SCRIPT,		0.f		},
	{ TASK_WAIT_FOR_SCRIPT,		0.f		},
	{ TASK_PLAY_SCRIPT,			0.f		},
};

Schedule_t slRunToScript[] =
{
	{
		tlScriptedRun,
		VARRAYSIZE ( tlScriptedRun ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"RunToScript"
	},
};

Task_t tlScriptedWait[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_WAIT_FOR_SCRIPT,		0.f		},
	{ TASK_PLAY_SCRIPT,			0.f		},
};

Schedule_t slWaitScript[] =
{
	{
		tlScriptedWait,
		VARRAYSIZE ( tlScriptedWait ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"WaitForScript"
	},
};

Task_t tlScriptedFace[] =
{
	{ TASK_STOP_MOVING,			0.f				},
	{ TASK_FACE_SCRIPT,			0.f		},
	{ TASK_FACE_IDEAL,			0.f		},
	{ TASK_WAIT_FOR_SCRIPT,		0.f		},
	{ TASK_PLAY_SCRIPT,			0.f		},
};

Schedule_t slFaceScript[] =
{
	{
		tlScriptedFace,
		VARRAYSIZE ( tlScriptedFace ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"FaceScript"
	},
};

//=========================================================
// Cower - this is what is usually done when attempts
// to escape danger fail.
//=========================================================
Task_t	tlCower[] =
{
	{ TASK_STOP_MOVING,			0.f					},
	{ TASK_PLAY_SEQUENCE,		static_cast<float>(ACT_COWER)	},
};

Schedule_t	slCower[] =
{
	{
		tlCower,
		VARRAYSIZE ( tlCower ),
		0,
		0,
		"Cower"
	},
};

//=========================================================
// move away from where you're currently standing.
//=========================================================
Task_t	tlTakeCoverFromOrigin[] =
{
	{ TASK_STOP_MOVING,					0.f					},
	{ TASK_FIND_COVER_FROM_ORIGIN,		0.f					},
	{ TASK_RUN_PATH,					0.f					},
	{ TASK_WAIT_FOR_MOVEMENT,			0.f					},
	{ TASK_REMEMBER,					static_cast<float>(bits_MEMORY_INCOVER)	},
	{ TASK_TURN_LEFT,					179.f					},
};

Schedule_t	slTakeCoverFromOrigin[] =
{
	{
		tlTakeCoverFromOrigin,
		VARRAYSIZE ( tlTakeCoverFromOrigin ),
		bits_COND_NEW_ENEMY,
		0,
		"TakeCoverFromOrigin"
	},
};

//=========================================================
// hide from the loudest sound source
//=========================================================
Task_t	tlTakeCoverFromBestSound[] =
{
	{ TASK_STOP_MOVING,					0.f					},
	{ TASK_FIND_COVER_FROM_BEST_SOUND,	0.f					},
	{ TASK_RUN_PATH,					0.f					},
	{ TASK_WAIT_FOR_MOVEMENT,			0.f					},
	{ TASK_REMEMBER,					static_cast<float>(bits_MEMORY_INCOVER)	},
	{ TASK_TURN_LEFT,					179.f					},
};

Schedule_t	slTakeCoverFromBestSound[] =
{
	{
		tlTakeCoverFromBestSound,
		VARRAYSIZE ( tlTakeCoverFromBestSound ),
		bits_COND_NEW_ENEMY,
		0,
		"TakeCoverFromBestSound"
	},
};

//=========================================================
// Take cover from enemy! Tries lateral cover before node
// cover!
//=========================================================
Task_t	tlTakeCoverFromEnemy[] =
{
	{ TASK_STOP_MOVING,				0.f					},
	{ TASK_WAIT,					0.2f					},
	{ TASK_FIND_COVER_FROM_ENEMY,	0.f					},
	{ TASK_RUN_PATH,				0.f					},
	{ TASK_WAIT_FOR_MOVEMENT,		0.f					},
	{ TASK_REMEMBER,				static_cast<float>(bits_MEMORY_INCOVER)	},
//	{ TASK_TURN_LEFT,				179.f					},
	{ TASK_FACE_ENEMY,				0.f					},
	{ TASK_WAIT,					1.f					},
};

Schedule_t	slTakeCoverFromEnemy[] =
{
	{
		tlTakeCoverFromEnemy,
		VARRAYSIZE ( tlTakeCoverFromEnemy ),
		bits_COND_NEW_ENEMY,
		0,
		"tlTakeCoverFromEnemy"
	},
};

Schedule_t *CBaseMonster::m_scheduleList[] =
{
	slIdleStand,
	slIdleTrigger,
	slIdleWalk,
	slAmbush,
	slActiveIdle,
	slWakeAngry,
	slAlertFace,
	slAlertSmallFlinch,
	slAlertStand,
	slInvestigateSound,
	slCombatStand,
	slCombatFace,
	slStandoff,
	slArmWeapon,
	slReload,
	slRangeAttack1,
	slRangeAttack2,
	slPrimaryMeleeAttack,
	slSecondaryMeleeAttack,
	slSpecialAttack1,
	slSpecialAttack2,
	slChaseEnemy,
	slChaseEnemyFailed,
	slSmallFlinch,
	slDie,
	slVictoryDance,
	slBarnacleVictimGrab,
	slBarnacleVictimChomp,
	slError,
	slWalkToScript,
	slRunToScript,
	slWaitScript,
	slFaceScript,
	slCower,
	slTakeCoverFromOrigin,
	slTakeCoverFromBestSound,
	slTakeCoverFromEnemy,
	slFail
};

Schedule_t *CBaseMonster::ScheduleFromName( const char *pName )
{
	return ScheduleInList( pName, m_scheduleList, VARRAYSIZE(m_scheduleList) );
}


Schedule_t *CBaseMonster :: ScheduleInList( const char *pName, Schedule_t **pList, int listCount )
{
	int i;

	if ( !pName )
	{
		ALERT( at_console, "%s set to unnamed schedule!\n", STRING(pev->classname) );
		return NULL;
	}


	for ( i = 0; i < listCount; i++ )
	{
		if ( !pList[i]->pName )
		{
			ALERT( at_console, "Unnamed schedule!\n" );
			continue;
		}
		if ( stricmp( pName, pList[i]->pName ) == 0 )
			return pList[i];
	}
	return NULL;
}

//=========================================================
// GetScheduleOfType - returns a pointer to one of the
// monster's available schedules of the indicated type.
//=========================================================
Schedule_t* CBaseMonster :: GetScheduleOfType ( int Type )
{
//	ALERT ( at_console, "Sched Type:%d\n", Type );
	switch	( Type )
	{
	// This is the schedule for scripted sequences AND scripted AI
	case SCHED_AISCRIPT:
		{
			ASSERT( m_pCine != NULL );
			if ( !m_pCine )
			{
				ALERT( at_aiconsole, "Script failed for %s\n", STRING(pev->classname) );
				CineCleanup();
				return GetScheduleOfType( SCHED_IDLE_STAND );
			}
//			else
//				ALERT( at_aiconsole, "Starting script %s for %s\n", STRING( m_pCine->m_iszPlay ), STRING(pev->classname) );

			switch ( m_pCine->m_fMoveTo )
			{
				case 0:
				case 4:
					return slWaitScript;
				case 1:
					return slWalkToScript;
				case 2:
					return slRunToScript;
				case 5:
					return slFaceScript;
			}
			break;
		}
	case SCHED_IDLE_STAND:
		{
			if ( RANDOM_LONG(0,14) == 0 && FCanActiveIdle() )
			{
				return &slActiveIdle[ 0 ];
			}

			return &slIdleStand[ 0 ];
		}
	case SCHED_IDLE_WALK:
		{
			return &slIdleWalk[ 0 ];
		}
	case SCHED_WAIT_TRIGGER:
		{
			return &slIdleTrigger[ 0 ];
		}
	case SCHED_WAKE_ANGRY:
		{
			return &slWakeAngry[ 0 ];
		}
	case SCHED_ALERT_FACE:
		{
			return &slAlertFace[ 0 ];
		}
	case SCHED_ALERT_STAND:
		{
			return &slAlertStand[ 0 ];
		}
	case SCHED_COMBAT_STAND:
		{
			return &slCombatStand[ 0 ];
		}
	case SCHED_COMBAT_FACE:
		{
			return &slCombatFace[ 0 ];
		}
	case SCHED_CHASE_ENEMY:
		{
			return &slChaseEnemy[ 0 ];
		}
	case SCHED_CHASE_ENEMY_FAILED:
		{
			return &slFail[ 0 ];
		}
	case SCHED_SMALL_FLINCH:
		{
			return &slSmallFlinch[ 0 ];
		}
	case SCHED_ALERT_SMALL_FLINCH:
		{
			return &slAlertSmallFlinch[ 0 ];
		}
	case SCHED_RELOAD:
		{
			return &slReload[ 0 ];
		}
	case SCHED_ARM_WEAPON:
		{
			return &slArmWeapon[ 0 ];
		}
	case SCHED_STANDOFF:
		{
			return &slStandoff[ 0 ];
		}
	case SCHED_RANGE_ATTACK1:
		{
			return &slRangeAttack1[ 0 ];
		}
	case SCHED_RANGE_ATTACK2:
		{
			return &slRangeAttack2[ 0 ];
		}
	case SCHED_MELEE_ATTACK1:
		{
			return &slPrimaryMeleeAttack[ 0 ];
		}
	case SCHED_MELEE_ATTACK2:
		{
			return &slSecondaryMeleeAttack[ 0 ];
		}
	case SCHED_SPECIAL_ATTACK1:
		{
			return &slSpecialAttack1[ 0 ];
		}
	case SCHED_SPECIAL_ATTACK2:
		{
			return &slSpecialAttack2[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_BEST_SOUND:
		{
			return &slTakeCoverFromBestSound[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_ENEMY:
		{
			return &slTakeCoverFromEnemy[ 0 ];
		}
	case SCHED_COWER:
		{
			return &slCower[ 0 ];
		}
	case SCHED_AMBUSH:
		{
			return &slAmbush[ 0 ];
		}
	case SCHED_BARNACLE_VICTIM_GRAB:
		{
			return &slBarnacleVictimGrab[ 0 ];
		}
	case SCHED_BARNACLE_VICTIM_CHOMP:
		{
			return &slBarnacleVictimChomp[ 0 ];
		}
	case SCHED_INVESTIGATE_SOUND:
		{
			return &slInvestigateSound[ 0 ];
		}
	case SCHED_DIE:
		{
			return &slDie[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_ORIGIN:
		{
			return &slTakeCoverFromOrigin[ 0 ];
		}
	case SCHED_VICTORY_DANCE:
		{
			return &slVictoryDance[ 0 ];
		}
	case SCHED_FAIL:
		{
			return slFail;
		}
	default:
		{
			ALERT ( at_console, "GetScheduleOfType()\nNo CASE for Schedule Type %d!\n", Type );

			return &slIdleStand[ 0 ];
			break;
		}
	}

	return NULL;
}
