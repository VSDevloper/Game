/*	FlashModule

	The Flash UI Module handles registration and de-registration of 
	the dynamic Flash components and associated nodes. It is also 
	capable of receiving certain events from the UI system and 
	restricting the UI system itself from reloading elements that 
	would invalidate the registered components.
*/
#pragma once

#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

class CFlashUIComponent;
class CFlashEnvModule final : public IUIModule
{
	friend class CFlashUIComponent;
	
	static constexpr CryGUID s_packageGUID = "{58CC50B0-EC1A-4EFD-A5D6-9528ED3CCCF4}"_cry_guid;

protected:
	CFlashEnvModule();

public:
	CFlashEnvModule(const CFlashEnvModule& other) = delete;
	virtual ~CFlashEnvModule() override;
	static CFlashEnvModule& GetInstance();

	void RegisterSchematyc() const;
	void UnregisterSchematyc() const;
	void SetAllowReload(const bool allow);

	// IUIModule
	virtual void Shutdown() override;
	virtual bool EditorAllowReload() override;
	virtual void EditorReload() override;
	// ~IUIModule

protected:
	void AdjustPreviewInstances(const int val);

private:
	static void RegisterFlashElements(Schematyc::IEnvRegistrar& registrar);
	static void RegisterDynamicComponents(Schematyc::CEnvRegistrationScope& packageScope);

	bool m_bAllowReload = false;
	int m_numPreviewInstances = 0;
};