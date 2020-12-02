#include "include/bleapp_windows/bleapp_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.System.Power.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/event_channel.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace {

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Power;
using Ins = winrt::Windows::Foundation::IInspectable;

using flutter::EncodableMap;
using flutter::EncodableValue;

class BleappWindowsPlugin : public flutter::Plugin, 
                              public flutter::StreamHandler<EncodableValue> {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  BleappWindowsPlugin();

  virtual ~BleappWindowsPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
	  
  std::unique_ptr<flutter::StreamHandlerError<>>
  OnListenInternal(const EncodableValue *arguments,
                   std::unique_ptr<flutter::EventSink<>> &&events) override;

  std::unique_ptr<flutter::StreamHandlerError<>>
  OnCancelInternal(const EncodableValue *arguments) override;
  
  std::unique_ptr<flutter::EventSink<EncodableValue>> scan_result_sink_;
   
  bool bCreated = false;
  winrt::event_token remChargePercentChangedToken;
  void RemainingChargeChanged(winrt::Windows::Foundation::IInspectable const &,
                              winrt::Windows::Foundation::IInspectable); 
   
   
};

// static
void BleappWindowsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "bleapp/method",
          &flutter::StandardMethodCodec::GetInstance());
		  
 auto event_scan_result =
      std::make_unique<flutter::EventChannel<EncodableValue>>(
          registrar->messenger(), "bleapp/event",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<BleappWindowsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });
	  
  auto handler = std::make_unique<flutter::StreamHandlerFunctions<>>(
      [plugin_pointer =
           plugin.get()](const EncodableValue *arguments,
                         std::unique_ptr<flutter::EventSink<>> &&events)
          -> std::unique_ptr<flutter::StreamHandlerError<>> {
        return plugin_pointer->OnListen(arguments, std::move(events));
      },
      [plugin_pointer = plugin.get()](const EncodableValue *arguments)
          -> std::unique_ptr<flutter::StreamHandlerError<>> {
        return plugin_pointer->OnCancel(arguments);
      });
  event_scan_result->SetStreamHandler(std::move(handler));

  registrar->AddPlugin(std::move(plugin));
}

BleappWindowsPlugin::BleappWindowsPlugin() {}

BleappWindowsPlugin::~BleappWindowsPlugin() {}

void BleappWindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
	
	if (!bCreated) {

      event_token evtBatterChargeChange =
          PowerManager::RemainingChargePercentChanged(
              {this, &BleappWindowsPlugin::RemainingChargeChanged});
      bCreated = true;
    }
	
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

void BleappWindowsPlugin::RemainingChargeChanged(
    winrt::Windows::Foundation::IInspectable const &obj,
    winrt::Windows::Foundation::IInspectable args) {
  /// Obtain the remaining charge changed information
  /// Add scan result sink
  int remCharge = PowerManager::RemainingChargePercent();
  OutputDebugString(L"Has the battery level changed\n");
  if (scan_result_sink_) {
    scan_result_sink_->Success(std::to_string(remCharge));
  }
}

std::unique_ptr<flutter::StreamHandlerError<>>
BleappWindowsPlugin::OnListenInternal(
    const EncodableValue *arguments,
    std::unique_ptr<flutter::EventSink<>> &&events) {

  scan_result_sink_ = std::move(events);
  return nullptr;
}

std::unique_ptr<flutter::StreamHandlerError<>>
BleappWindowsPlugin::OnCancelInternal(const EncodableValue *arguments) {
  scan_result_sink_ = nullptr;
  return nullptr;
}

}  // namespace

void BleappWindowsPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  BleappWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
