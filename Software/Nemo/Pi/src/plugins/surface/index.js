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
                        var message = data.toString();
			
			//Turn message into a dictionary
			var parts = message.split(';');
		    	var status = {};
    			for (var i = 0; i < parts.length; i++)
    			{
      				var subParts = parts[i].split(':');
          			status[subParts[0]] = subParts[1];
    			}

                        // Emit the message on the global messaging bus
			console.log("SOCKET RECIEVED: " + message);
                        self.globalBus.emit( 'mcu.status', status);
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
