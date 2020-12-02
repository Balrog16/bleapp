import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:bleapp/bleapp.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  String _percentCharge = 'Cant Read';
  Bleapp _bleapp = Bleapp();

  ///Declare subscription here
  StreamSubscription<String> _subscribeBatteryPercentage;

  @override
  void initState() {
    super.initState();
    _subscribeBatteryPercentage =
        _bleapp.onChargePercentageChanged.listen((String strCharge) {
      setState(() {
        print("Has the battery level changed?");
        _percentCharge = strCharge;
      });
    });
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    try {
      platformVersion = await _bleapp.getPlatformVersion;
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text(
              'Running on: $_platformVersion with $_percentCharge Battery\n'),
        ),
      ),
    );
  }

  @override
  void dispose() {
    super.dispose();
    if (_subscribeBatteryPercentage != null) {
      _subscribeBatteryPercentage.cancel();
    }
  }
}
