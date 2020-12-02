import 'package:flutter/services.dart';
import 'package:bleapp_platform_interface/bleapp_platform_interface.dart';

class MethodChannelbleapp extends BleappPlatform {
  static const MethodChannel _method = const MethodChannel('bleapp/method');
  static const EventChannel _eventCh = const EventChannel('bleapp/event');

  MethodChannelbleapp();

  Stream<String> _onBatteryChargeChanged;

  Future<String> get getPlatformVersion => _method
      .invokeMethod<String>('getPlatformVersion')
      .then<String>((dynamic result) => result);

  Stream<String> get onChargePercentageChanged {
    print('Yes that is me...');
    if (_onBatteryChargeChanged == null) {
      print('Is that right?');
      _onBatteryChargeChanged =
          _eventCh.receiveBroadcastStream().map((dynamic event) {
        return event;
      });
    }
    return _onBatteryChargeChanged;
  }
}
