#include "StdAfx.h"
#include "GamePlugin.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include "FlashModule.h"
#include <Env/GameEnv.h>
#include "GameEvents.h"

CGamePlugin::~CGamePlugin()
{
	// Unregister from system events
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
	
	// Unregister components from this plugin
	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CGamePlugin::GetCID());
	}
}

bool CGamePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	// Register for engine system events, we need to register the Schematyc components at the right time
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGamePlugin");

	return true;
}

void CGamePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{

	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		if (!gEnv->IsEditor())
		{
			CFlashEnvModule::GetInstance().SetAllowReload(false);
		}
	}

	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		if (gEnv->pSchematyc)
		{
			// Register entity components with schematyc by calling all static callbacks registered with the CRY_STATIC_AUTO_REGISTER_FUNCTION macro
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CGamePlugin::GetCID(),
					"EntityComponents",
					"Crytek GmbH",
					"Components",
					[](Schematyc::IEnvRegistrar& registrar)
					{
						Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
					}
				)
			);

			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					Schematyc::g_gameModuleGUID,
					"GameNodes",
					"Crytek GmbH",
					"Game",
					&Schematyc::RegisterGameEnvPackage
				)
			);

			CFlashEnvModule::GetInstance().RegisterSchematyc();
		}
	}
	break;

	case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
	{
		CFlashEnvModule::GetInstance().SetAllowReload(false);
	}
	break;

	case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:
	{
		CFlashEnvModule::GetInstance().SetAllowReload(true);
	}
	break;

	}
}

CRYREGISTER_SINGLETON_CLASS(CGamePlugin)