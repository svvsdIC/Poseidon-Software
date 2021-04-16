	    var PORT = 45246;

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
                       //self.globalBus.emit( 'mcu.status', status);
                });
		
		// If error occurs...log it	
		socket.on('error', function(err){
                	console.log(err);
       		});
		}).listen(PORT);
