//
//  Generated file. Do not edit.
//

#include "generated_plugin_registrant.h"

#include <bleapp/bleapp_plugin.h>
#include <bleapp_windows/bleapp_windows_plugin.h>

void RegisterPlugins(flutter::PluginRegistry* registry) {
  BleappPluginRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("BleappPlugin"));
  BleappWindowsPluginRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("BleappWindowsPlugin"));
}
