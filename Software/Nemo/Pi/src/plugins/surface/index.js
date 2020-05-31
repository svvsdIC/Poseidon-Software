(function()
{
    const Listener = require( 'Listener' );

    class surface
    {
        constructor(name, deps)
        {
	
	    deps.logger.debug('surface plugin loaded!');

            this.globalBus  = deps.globalEventLoop;   // This is the server-side messaging bus. The MCU sends messages to server plugins over this
            //this.cockpitBus = deps.cockpit;           // This is the server<->client messaging bus. This is how the server talks to the browser

	    var PORT = 45246;

            var self = this;

            // Pre-define all of the event listeners here. We defer enabling them until later.
            // Look at src/libs/Listener.js to see how these work.
            this.listeners = {} //There are none.

            // Create the socket server
            require('net').createServer(function(socket) {
                // When data is recieved...
            	socket.on( 'data', function(data){
                    // Turn data into a string
                    var message = data.toString().trim();
//                    if(message.endsWith(';')){
                          message = message.slice(0,-1);
//                    }
                    console.log("b0be-SOCKET RECIEVED: " + message);

                    if(message.includes('(')){
                        // if command for the Nemo Arduino  e.g. 'claser(255);'
                        self.globalBus.emit('mcu.SendCommand', message);
                    } else {
                        // is a status for the Nemo Pi;

                        //Turn message into a dictionary
                        var parts = message.split(';');
                            var status = {};
                            for (var i = 0; i < parts.length; i++)
                            {
                                var subParts = parts[i].split(':');
                                status[subParts[0]] = subParts[1];
                            }

                        // Emit the message on the global messaging bus
                        self.globalBus.emit( 'mcu.status', status);
                    }    
		        });

                // If error occurs...log it	
                socket.on('error', function(err){
                            console.log(err);
                });
            }).listen(PORT);
        }
    }

    module.exports = function(name, deps)
    {
        return new surface(name, deps);
    };
}());
