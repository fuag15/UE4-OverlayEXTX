// Copyright Epic Games, Inc. All Rights Reserved.

#include "EXTXOverlays.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "InputCoreTypes.h"

#define LOCTEXT_NAMESPACE "FEXTXOverlaysModule"

void FEXTXOverlaysModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterOpenXRExtensionModularFeature();
}

void FEXTXOverlaysModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

bool FEXTXOverlaysModule::EnumerateExtensions()
{
	uint32_t ExtensionsCount = 0;
	if (XR_FAILED(xrEnumerateInstanceExtensionProperties(nullptr, 0, &ExtensionsCount, nullptr)))
	{
		// If it fails this early that means there's no runtime installed
		return false;
	}

	TArray<XrExtensionProperties> Properties;
	Properties.SetNum(ExtensionsCount);
	for (auto& Prop : Properties)
	{
		Prop = XrExtensionProperties{ XR_TYPE_EXTENSION_PROPERTIES };
	}

	if (XR_ENSURE(xrEnumerateInstanceExtensionProperties(nullptr, ExtensionsCount, &ExtensionsCount, Properties.GetData())))
	{
		for (const XrExtensionProperties& Prop : Properties)
		{
			AvailableExtensions.Add(Prop.extensionName);
		}
		return true;
	}
	return false;
}

// if this returns false the whole extension is ignored
bool FEXTXOverlaysModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	// Grab info about extensions available
	bool EnumerateExtensionsSucceeded = EnumerateExtensions();
	if (!EnumerateExtensionsSucceeded)
	{
		return false;
	}

	// Plugin only supports d3d11
	FString RHIString = FApp::GetGraphicsRHI();
	if (RHIString.IsEmpty() || RHIString != TEXT("DirectX 11"))
	{
		return false;
	}

	// ignore in editor
	if (GIsEditor)
	{
		return false;
	}

	if (AvailableExtensions.Contains(XR_EXTX_OVERLAY_EXTENSION_NAME))
	{
		OutExtensions.Add(XR_EXTX_OVERLAY_EXTENSION_NAME);
		return true;
	}

	return false;
}

const void* FEXTXOverlaysModule::OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext)
{
	OverlaySessionCreateInfo = MakeUnique<XrSessionCreateInfoOverlayEXTX>();
	OverlaySessionCreateInfo->type = XR_TYPE_SESSION_CREATE_INFO_OVERLAY_EXTX;
	OverlaySessionCreateInfo->next = InNext;
	OverlaySessionCreateInfo->createFlags = 0;
	OverlaySessionCreateInfo->sessionLayersPlacement = 0;

	return OverlaySessionCreateInfo.Get();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEXTXOverlaysModule, EXTXOverlays)