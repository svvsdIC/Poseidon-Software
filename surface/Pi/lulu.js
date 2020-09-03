var net = require('net');
var HOST = '192.168.1.8';
var PORT = 45246;
var client = new net.Socket();

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort('/dev/ttyACM0', { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

port.on("open", () => {
  console.log('serial port open');
});

client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // Write a message to the socket as soon as the client is connected, the server will receive it as message from t$
    parser.on('data', data =>{
       console.log("passing thru: " + data);
       client.write(data);
    });
});

client.on('error', function(err) {
        console.log('Youve messed up: ' + err);
});

