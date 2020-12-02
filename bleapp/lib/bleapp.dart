import 'package:bleapp_platform_interface/bleapp_platform_interface.dart';

class Bleapp {
  factory Bleapp() {
    if (_singleton == null) {
      _singleton = Bleapp._();
    }
    return _singleton;
  }

  Bleapp._();
  static Bleapp _singleton;
  static BleappPlatform get _platform => BleappPlatform.instance;

  Future<String> get getPlatformVersion {
    return _platform.getPlatformVersion;
  }

  /// Is called or invoked as and when the percentage charge changes
  Stream<String> get onChargePercentageChanged {
    print('Hello is it me?');
    return _platform.onChargePercentageChanged;
  }
}
