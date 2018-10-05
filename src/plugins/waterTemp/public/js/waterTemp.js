(function(window)
{
    'use strict';
    class waterTemp
    {
        constructor( cockpit )
        {
            console.log('waterTemperature Plugin running');
            var self = this;
            self.cockpit = cockpit;

            self.settings = null;

            //Set up actions associated with this plugin
            this.actions =
            {};

            // Setup input handlers
            this.inputDefaults =
            {
                keyboard:
                {}
            };
        };
        getTelemetryDefinitions()
        {
            return [{
                name: 'waterTemp.value',
                description: 'The message sent from the example module in the MCU'
            }]
        };

        // This pattern will hook events in the cockpit and pull them all back
        // so that the reference to this instance is available for further processing
        listen()
        {
            var self = this;
            console.log('Peter: waterTemp listening');
            // Listen for response messages from the Node plugin
            this.cockpit.rov.withHistory.on('plugin.waterTemp.value', function( message )
            {
                //console.log( "New count is : " + message );
                self.cockpit.emit('plugin.waterTemp.value', message);
            });
        };
    };

    // Add plugin to the window object and add it to the plugins list
    var plugins = namespace('plugins');
    plugins.waterTemp = waterTemp;
    window.Cockpit.plugins.push( plugins.waterTemp);

}(window));
