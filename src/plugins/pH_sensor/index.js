(function()
{
    const Listener = require( 'Listener' );

    class pH_sensor
    {
        constructor(name, deps)
        {
		var os = require('os');
		var addresses = [];
		var interfaces = os.networkInterfaces();
	
			    deps.logger.debug('pH_sensor plugin loaded!');

            this.globalBus  = deps.globalEventLoop;   // This is the server-side messaging bus. The MCU sends messages to server plugins over this
            this.cockpitBus = deps.cockpit;           // This is the server<->client messaging bus. This is how the server talks to the browser

            var self = this;

            // Pre-define all of the event listeners here. We defer enabling them until later.
            // Look at src/libs/Listener.js to see how these work.
            this.listeners =
            {
                // Listener for MCU status messages
                mcuStatus: new Listener( self.globalBus, 'mcu.status', false, function( data )
                {
                    // Check for the example field name in the MCU's status update
                    if( 'pH' in data )
                    {
                        // Get the message that the MCU sent to us
                        var message = data.pH;
                        // Re-emit the message on the cockpit messaging bus (talks to the browser)
			var util = require('util');
			console.log("b0be: pH: " + util.inspect(data));
			console.log("b0be: pH message : " + util.inspect(message));
	                self.cockpitBus.emit( 'plugin.pH_sensor.value', message );
                    }
                })
            }
        }

        // This is automatically called when cockpit loads all of the plugins, and when a plugin is enabled
        start()
        {
          // Enable the listeners!
          this.listeners.mcuStatus.enable();
          console.log("pH_sensor started!!");
        }

        // This is called when the plugin is disabled
        stop()
        {
          // Disable listeners
          this.listeners.mcuStatus.disable();
        }
    }

    module.exports = function(name, deps)
    {
        return new pH_sensor(name, deps);
    };
}());
