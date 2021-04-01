// Copyright Epic Games, Inc. All Rights Reserved.

/**

NOTE: Things still needed. Would be nice to have support from Epic. Could possibly be worked around via a plugin view configuration. Haven't thought this through much.

- A way for an extension plugin to require a specific XR blend mode
- A way for an extension plugin to set projection layer properties

NOTE: Nice to haves

- A pop up back in asking if overlays should be attempted or not (currently coded to always run as an overlay if available)
- A more explicit check to see if d3d11 is indeed active, plugin currently relies on implicit behavior by OpenXRHMD
- A name that matches other OpenXR plugin conventsions (prefix OpenXR)

*/

#pragma once

#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleInterface.h"
#include "OpenXRCore.h"

class FEXTXOverlaysModule : public IModuleInterface, public IOpenXRExtensionPlugin
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** IOpenXRExtensionPlugin */
	virtual bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
	virtual const void* OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext) override;

private:
	TSet<FString> AvailableExtensions;
	TUniquePtr<XrSessionCreateInfoOverlayEXTX> OverlaySessionCreateInfo;

	bool IsOverlayEnabled = false;
	bool EnumerateExtensions();
};
