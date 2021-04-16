var net = require('net');
var HOST = '10.0.0.203';
var PORT = 45246;
var client = new net.Socket();

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort('/dev/ttyACM1', { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

port.on("open", () => {
  console.log('serial port open');
});

parser.on('data', data =>{
      console.log("recieved")
      if (client.readyState==='open') {
          client.write(data);
          console.log("sent")
      }
});

connect();

client.on('error', function(err) {
        console.log('Youve messed up: ' + err);
});

client.on('close', function(hadError) {
	connect();
});

function connect() {
    client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
});
}

