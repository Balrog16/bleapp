library bleapp_platform_interface;

import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'method_channel_bleapp.dart';

abstract class BleappPlatform extends PlatformInterface {
  BleappPlatform() : super(token: _token);

  static final Object _token = Object();

  static BleappPlatform _instance = MethodChannelbleapp();

  static BleappPlatform get instance => _instance;

  static set instance(BleappPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  ///
  Future<String> get getPlatformVersion {
    throw UnimplementedError('getPlatformversion() has not been implemented.');
  }

  ///
  Stream<String> get onChargePercentageChanged {
    throw UnimplementedError(
        'get onChargePercentageChanged is not implmeneted yet');
  }
}
