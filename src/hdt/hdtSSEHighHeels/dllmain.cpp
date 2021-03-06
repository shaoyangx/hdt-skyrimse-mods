// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include "PapyrusProxy.h"
#include "HighheelManager.h"
#include <skse64/skse64/PluginAPI.h>
#include <skse64/skse64_common/skse_version.h>

#include "../hdtSSEUtils/LogUtils.h"
#include "../hdtSSEUtils/FrameworkUtils.h"
#include "../hdtSSEFramework/IEventListener.h"
#include "../hdtSSEFramework/HookEngine.h"

namespace hdt
{
	static LogToFile s_Log("Data/SKSE/Plugins/hdtSSEHighHeels.log");

	class HHEventListener : public IEventListener<FrameEvent>
	{
	public:
		virtual void onEvent(const FrameEvent& e) override
		{
			if (e.frameEnd) HighheelManager::instance()->update();
		}
	};
	static HHEventListener g_listener;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		hdt::getFramework()->getFrameEventDispatcher()->removeListener(&hdt::g_listener);
        break;
    }
    return TRUE;
}


extern "C"
{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "hdtSSEHighHeels";
		info->version = 1;

		if (skse->isEditor)
		{
			return false;
		}

		if (skse->runtimeVersion != CURRENT_RELEASE_RUNTIME)
			return false;

		if (!hdt::getFramework())
		{
			hdt::LogError("hdtSSEFramework failed to launch");
			return false;
		}

		if (!hdt::getFramework()->isSupportedSkyrimVersion(skse->runtimeVersion))
		{
			hdt::LogError("hdtSSEFramework doesn't support current skyrim version");
			return false;
		}

		if (!hdt::checkFrameworkVersion(1, 0))
			return false;

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{
		auto ipapyrus = reinterpret_cast<SKSEPapyrusInterface*>(skse->QueryInterface(kInterface_Papyrus));
		ipapyrus->Register(hdt::registerPapyrus);
		hdt::getFramework()->getFrameEventDispatcher()->addListener(&hdt::g_listener);
		return true;
	}
}
