#pragma once

#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/ResourceTypes.h>

enum class EGameEvent
{
	None = 0,

	//! Triggered when an entity should be killed (Health component has reached 0 or forced kill)
	//! Sent to the specific entity that should handle it's death
	KillEntity = BIT(0),

	//! Triggered when an entity has been killed (Health component was destroyed)
	//! Sent to all subscribed entities
	EntityKilled = BIT(1),

	//! Triggered when an entity leaves the arena trigger bounds
	//! Sent to the specific entity that left the arena bounds
	LeftArena = BIT(2),

	//! Used to signal the player spawn point to spawn the player entity
	//! Sent to all subscribed entities
	SpawnPlayer = BIT(3),

	//! Used to signal that any weapons owned by the entity that recieves this event should not fire
	//! Sent to the specific entity that should stop firing
	DisableWeapons = BIT(4),

	//! Used to signal that any weapons owned by the entity that recieves this event should start firing
	//! Sent to the specific entity that should start firing
	EnableWeapons = BIT(5),

	//! Used to signal that the gameplay simulation should start/resume
	//! Sent to all subscribed entities
	StartSimulation = BIT(6),

	//! Used to signal that the gameplay simulation should stop/pause (Paused Game etc)
	//! Sent to all subscribed entities
	StopSimulation = BIT(7),

	//! Used to signal that the gameplay elements should be reset
	//! Sent to all subscribed entities
	ResetGame = BIT(8),

	//! Used to send custom events by the user using strings
	//! Not handled by the game system, user controlled
	CustomEvent = BIT(30)
};

typedef CEnumFlags<EGameEvent> EGameEventMask;

struct SGameEventSignalBase
{
	SGameEventSignalBase() : 
		m_type(EGameEvent::None) 
	{};

	SGameEventSignalBase(EGameEvent type) : m_type(type)
	{};

	const EGameEvent m_type;
};

struct SSpawnPlayerSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::SpawnPlayer;

	SSpawnPlayerSignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SKillEntitySignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::KillEntity;

	SKillEntitySignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SEntityKilledSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::EntityKilled;

	SEntityKilledSignal() : 
		SGameEventSignalBase(type),
		m_entity(Schematyc::ExplicitEntityId::Invalid),
		m_classname("")
	{};

	SEntityKilledSignal(const Schematyc::ExplicitEntityId& entity, const Schematyc::EntityClassName& classname) :
		SGameEventSignalBase(type),
		m_entity(entity),
		m_classname(classname)
	{};

	Schematyc::ExplicitEntityId m_entity;
	Schematyc::EntityClassName m_classname;
};

struct SLeftArenaSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::LeftArena;

	SLeftArenaSignal() : 
		SGameEventSignalBase(type), 
		m_edgeNormal(Vec3(ZERO))
	{};

	SLeftArenaSignal(const Vec3& edgeNormal) : 
		SGameEventSignalBase(type), 
		m_edgeNormal(edgeNormal)
	{};

	Vec3 m_edgeNormal;
};

struct SDisableWeaponsSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::DisableWeapons;

	SDisableWeaponsSignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SEnableWeaponsSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::EnableWeapons;

	SEnableWeaponsSignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SStartSimulationSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::StartSimulation;

	SStartSimulationSignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SStopSimulationSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::StopSimulation;

	SStopSimulationSignal() : 
		SGameEventSignalBase(type)
	{};
};

struct SResetGameSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::ResetGame;

	SResetGameSignal() :
		SGameEventSignalBase(type)
	{};
};

struct SCustomEventSignal : public SGameEventSignalBase
{
	static constexpr EGameEvent type = EGameEvent::CustomEvent;

	SCustomEventSignal() :
		SGameEventSignalBase(type)
	{};

	SCustomEventSignal(const Schematyc::CSharedString& data) :
		SGameEventSignalBase(type),
		m_data(data)
	{};

	Schematyc::CSharedString m_data;
};

static void ReflectType(Schematyc::CTypeDesc<EGameEvent>& desc)
{
	desc.SetGUID("{AF695110-1D50-4D61-A0A5-791C2CB7D2DC}"_cry_guid);
	desc.SetLabel("Game Event");
	desc.SetDescription("A game event that can be dispatched or recieved by components");
	desc.AddConstant(EGameEvent::KillEntity, "KillEntity", "Kill Entity");
	desc.AddConstant(EGameEvent::EntityKilled, "EntityKilled", "Entity Killed");
	desc.AddConstant(EGameEvent::LeftArena, "LeftArena", "Left Arena");
	desc.AddConstant(EGameEvent::SpawnPlayer, "SpawnPlayer", "Spawn Player");
	desc.AddConstant(EGameEvent::EnableWeapons, "EnableWeapons", "Enable Weapons");
	desc.AddConstant(EGameEvent::DisableWeapons, "DisableWeapons", "Disable Weapons");
	desc.AddConstant(EGameEvent::StartSimulation, "StartSimulation", "Start Simulation");
	desc.AddConstant(EGameEvent::StopSimulation, "StopSimulation", "Stop Simulation");
	desc.AddConstant(EGameEvent::ResetGame, "ResetGame", "Reset Game");
	desc.AddConstant(EGameEvent::CustomEvent, "CustomEvent", "Custom Event");
}

static void ReflectType(Schematyc::CTypeDesc<SSpawnPlayerSignal>& desc)
{
	desc.SetGUID("{1EE837C0-24BA-400C-A27F-1DD186F84927}"_cry_guid);
	desc.SetLabel("OnSpawnPlayer");
}
static void ReflectType(Schematyc::CTypeDesc<SKillEntitySignal>& desc)
{
	desc.SetGUID("{94D34E00-5D97-457C-A8E6-0DD1C96A43A2}"_cry_guid);
	desc.SetLabel("OnKillEntity");
}
static void ReflectType(Schematyc::CTypeDesc<SEntityKilledSignal>& desc)
{
	desc.SetGUID("{AC823D62-9BC7-4718-AA4F-3DC3F0D8D439}"_cry_guid);
	desc.SetLabel("OnEntityKilled");
	desc.AddMember(&SEntityKilledSignal::m_entity,  'eid', "EntityId", "Entity Id", "The unique identifier for the entity that was killed", Schematyc::ExplicitEntityId::Invalid);
	desc.AddMember(&SEntityKilledSignal::m_classname, 'ecn', "EntityClassname", "Entity Classname", "The name of the class this entity belongs to", "");
}
static void ReflectType(Schematyc::CTypeDesc<SLeftArenaSignal>& desc)
{
	desc.SetGUID("{FDB5B99C-FD56-495C-B9E4-F7AA1CBF6A65}"_cry_guid);
	desc.SetLabel("OnLeftArena");
	desc.AddMember(&SLeftArenaSignal::m_edgeNormal, 'enml', "EdgeNormal", "Edge Normal", "Exit Edge Normal (Useful for calculating reflection vector", Vec3(ZERO));
}
static void ReflectType(Schematyc::CTypeDesc<SDisableWeaponsSignal>& desc)
{
	desc.SetGUID("{7F268A54-B294-4E6B-91A6-B3BD0BFA4A93}"_cry_guid);
	desc.SetLabel("OnDisableWeapons");
}
static void ReflectType(Schematyc::CTypeDesc<SEnableWeaponsSignal>& desc)
{
	desc.SetGUID("{6018882A-C66C-42C4-8C39-84F0E1375DDD}"_cry_guid);
	desc.SetLabel("OnEnableWeapons");
}
static void ReflectType(Schematyc::CTypeDesc<SStartSimulationSignal>& desc)
{
	desc.SetGUID("{0AACE79E-26E0-4846-BDA1-E1C30E66C469}"_cry_guid);
	desc.SetLabel("OnStartSimulation");
}
static void ReflectType(Schematyc::CTypeDesc<SStopSimulationSignal>& desc)
{
	desc.SetGUID("{6FECBA1C-B9FC-4F61-8CF0-AF6A4179475A}"_cry_guid);
	desc.SetLabel("OnStopSimulation");
}
static void ReflectType(Schematyc::CTypeDesc<SResetGameSignal>& desc)
{
	desc.SetGUID("{E467F3BD-C5AC-4946-823A-F3A972E358DC}"_cry_guid);
	desc.SetLabel("OnResetGame");
}
static void ReflectType(Schematyc::CTypeDesc<SCustomEventSignal>& desc)
{
	desc.SetGUID("{90089387-1786-4932-AAA0-55D6ABC7937E}"_cry_guid);
	desc.SetLabel("OnCustomEvent");
	desc.AddMember(&SCustomEventSignal::m_data, 'data', "Data", "Data", "The custom data/signal name for this custom event", "");
}

class IGameEventListener
{
public:
	virtual IEntity* GetEntity() const = 0;
	virtual void OnEvent(const SGameEventSignalBase& event) {};
};