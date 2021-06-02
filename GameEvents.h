#pragma once

#include <IGameEvents.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/IObject.h>

class CGameEvents
{
	CGameEvents() = default;
	CGameEvents(const CGameEvents& other) = delete;

public:
	~CGameEvents();

	static CGameEvents* GetInstance()
	{
		static CGameEvents instance;
		return &instance;
	}

	void SubscribeToEvents(IGameEventListener* pListener, const EGameEventMask& events);
	void UnsubscribeFromEvents(IGameEventListener* pListener, const EGameEventMask& events);

	template <class T>
	void SendEvent(const T& event)
	{
		for (auto& pListener : m_pListeners)
		{
			if (pListener.second.Check(event.m_type))
			{
				pListener.first->OnEvent(event);
			}
		}
	}

	template <class T>
	void SendEvent(IEntity* pEntity, const T& event)
	{
		if (pEntity == nullptr)
		{
			SendEvent(event);
			return;
		}

		for (auto& pListener : m_pListeners)
		{
			if (pListener.second.Check(event.m_type) && pListener.first->GetEntity() == pEntity)
			{
				pListener.first->OnEvent(event);
			}
		}
	}

private:
	template <class T>
	void SendEventInternal(IEntity* pEntity, const T& event)
	{
		assert(pEntity != nullptr);

		if (Schematyc::IObject* pSchematycObject = pEntity->GetSchematycObject())
		{
			// Send signal to this entity (Only ever one listener per entity as per Singleton flag in Subscriber component).
			pSchematycObject->ProcessSignal(event);
		}
	}

private:
	std::map<IGameEventListener*, EGameEventMask> m_pListeners = {};
};