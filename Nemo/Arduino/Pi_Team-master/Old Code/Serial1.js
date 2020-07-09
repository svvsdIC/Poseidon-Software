var serialport = require("serialport")
var SerialPort = serialport.SerialPort;
var sp = new serialport("/dev/ttyUSB0"),{
  BaudRate: 9600,
  parser: new serialport.parsers.Readline("\r\n")
});