#include "StdAfx.h"
#include "FlashModule.h"

#include <Env/DynamicRegistry.h>
#include <Env/FlashRegistry.h>
#include <Env/Elements/EnvFlashComponent.h>
#include <Components/Flash/DynamicComponent.h>
#include <Components/Flash/DynamicFunctions.h>
#include <Components/Flash/ElementFunctions.h>
#include <Components/Flash/Signals.h>

#include <CrySchematyc/Compiler/ICompiler.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>

CFlashEnvModule::CFlashEnvModule()
{
	gEnv->pFlashUI->RegisterModule(this, "FlashEnvModule");
}

CFlashEnvModule::~CFlashEnvModule()
{
	// If the flash UI is nullptr, we assume it was shutdown already.
	// In which case we have already unregistered this module.
	if (gEnv->pFlashUI != nullptr)
	{
		gEnv->pFlashUI->UnregisterModule(this);
	}
}

CFlashEnvModule& CFlashEnvModule::GetInstance()
{
	static CFlashEnvModule module;
	return module;
}

void CFlashEnvModule::RegisterSchematyc() const
{
	gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
		stl::make_unique<Schematyc::CEnvPackage>(
			s_packageGUID,
			"FlashComponents",
			"Crytek GmbH",
			"Flash",
			&RegisterFlashElements
		)
	);
}

void CFlashEnvModule::UnregisterSchematyc() const
{
	gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(s_packageGUID);
}

void CFlashEnvModule::SetAllowReload(const bool allow)
{
	m_bAllowReload = allow;
}

void CFlashEnvModule::Shutdown()
{
	Schematyc::CFlashRegistry::GetInstance().Reset();
	gEnv->pFlashUI->UnregisterModule(this);
}

bool CFlashEnvModule::EditorAllowReload() 
{ 
	if (m_bAllowReload && m_numPreviewInstances > 0)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Cannot reload UI while Schematyc Editor is open. Please close Schematyc editors before reloading UI.");
		return false;
	}

	return m_bAllowReload;
}
void CFlashEnvModule::EditorReload()
{
	UnregisterSchematyc();
	Schematyc::CFlashRegistry::GetInstance().Reset();
	Schematyc::CDynamicTypeDescRegistry::GetInstance().Reset();
	RegisterSchematyc();

	gEnv->pSchematyc->GetCompiler().CompileAll();
}

void CFlashEnvModule::AdjustPreviewInstances(const int val)
{
	m_numPreviewInstances += val;
}

void CFlashEnvModule::RegisterFlashElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.RootScope().Register(SCHEMATYC_MAKE_ENV_MODULE(s_packageGUID, "FlashUI"));
	RegisterDynamicComponents(scope);
}

void CFlashEnvModule::RegisterDynamicComponents(Schematyc::CEnvRegistrationScope& packageScope)
{
	const int numElements = gEnv->pFlashUI->GetUIElementCount();
	for (int elemIndex = 0; elemIndex < numElements; elemIndex++)
	{
		IUIElement* pElement = gEnv->pFlashUI->GetUIElement(elemIndex);

		// We skip any elements that were described but not found or incorrectly described for the GFX package
		if (!pElement->IsValid())
		{
#ifndef _RELEASE
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "Could not register UI Element with Schematyc, element was not valid. Element: '%s'", pElement->GetName());
#endif
			continue;
		}

		auto typeName = Schematyc::Utils::CTypeNameWrapper(Schematyc::EnvDynamicComponent::TypeString(pElement->GetName()));
		const CryGUID guid(typeName.ToGUID(Schematyc::EnvDynamicComponent::GUIDHigher));

		string componentName;
		componentName.Format("FlashElement:%s", pElement->GetName());
		
		// Collect reflected data from component definition first
		auto pDesc = Schematyc::CDynDescFactory::GetFactory().GetOrCreateDesc(guid);

		// Override with dynamic properties
		pDesc->SetGUID(guid);
		pDesc->SetName(typeName);
		pDesc->SetLabel(componentName.c_str());
		pDesc->SetEditorCategory("FlashUI");
		pDesc->SetComponentFlags({ IEntityComponent::EFlags::HideFromInspector, IEntityComponent::EFlags::Singleton });
		
		// Create the component and place it under the package scope
		auto pComponent = Schematyc::EnvFlashComponent::MakeShared<CFlashUIComponent>(std::move(typeName), *pDesc, SCHEMATYC_SOURCE_FILE_INFO);
		Schematyc::CEnvRegistrationScope componentScope = packageScope.Register(pComponent);

		// Add the element to the flash registry so we can track listeners
		auto& pComponentRegistry = Schematyc::CFlashRegistry::GetInstance();
		pComponentRegistry.RegisterElement(guid, pElement);
		
		RegisterDynamicFunctions(componentScope, pElement, &pComponent->GetDesc());
		RegisterElementFunctions(componentScope, pElement, &pComponent->GetDesc());
		RegisterElementEvents(componentScope, pElement, guid);
	}
}
