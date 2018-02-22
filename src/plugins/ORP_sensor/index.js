(function()
{
    const Listener = require( 'Listener' );

    class ORP_sensor
    {
        constructor(name, deps)
        {
		var os = require('os');
		var addresses = [];
		var interfaces = os.networkInterfaces();
	
			    deps.logger.debug('ORP_sensor plugin loaded!');

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
                    if( 'ORP' in data )
                    {
                        // Get the message that the MCU sent to u
                        var message = data.ORP;
                        // Re-emit the message on the cockpit messaging bus (talks to the browser)
			var util = require('util');
	                self.cockpitBus.emit( 'plugin.ORP_sensor.value', message );
                    }
                })
            }
        }

        // This is automatically called when cockpit loads all of the plugins, and when a plugin is enabled
        start()
        {
          // Enable the listeners!
          this.listeners.mcuStatus.enable();
          console.log("ORP_sensor started!!");
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
        return new ORP_sensor(name, deps);
    };
}());
