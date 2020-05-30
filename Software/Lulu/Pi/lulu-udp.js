console.log('starting...')

var net = require('net');
var HOST = '192.168.3.38';
var PORT = 45246;
var TTY = "/dev/ttyACM0";

const dgram = require('dgram');
const message = Buffer.from('Some bytes');
const client = dgram.createSocket('udp4');

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const ser = new SerialPort(TTY, { baudRate: 115200 });
const serialInput = ser.pipe(new Readline({ delimiter: '\n' }));

ser.on("open", () => {
  console.log('serial port open');
});

//client.connect(PORT, HOST, function() {
//   console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // Write a message to the socket as soon as the client is connected, the server will receive it as message from t$
serialInput.on('data', data =>{
	console.log(`passing thru to nemo: ${data}`);
	client.send(data, PORT, HOST, (err) =>  {
		if (err){
			console.log(`sending ${data} : received err: ${err}`);
		}
        });
});

client.on('error', function(err) {
        console.log('Youve messed up: ' + err);
});
