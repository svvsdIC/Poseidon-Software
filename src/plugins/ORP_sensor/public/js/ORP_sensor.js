(function(window)
{
    'use strict';
    class ORP_sensor
    {
        constructor( cockpit )
        {
            console.log('ORP_sensor Plugin running');
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
                name: 'ORP_sensor.value',
                description: 'The ORP_sensor value'
            }]
        };

        // This pattern will hook events in the cockpit and pull them all back
        // so that the reference to this instance is available for further processing
        listen()
        {
            var self = this;

            // Listen for response messages from the Node plugin
            this.cockpit.rov.withHistory.on('plugin.ORP_sensor.value', function( message )
            {
                self.cockpit.emit('plugin.ORP_sensor.value', message);
            });
        };
    };

    // Add plugin to the window object and add it to the plugins list
    var plugins = namespace('plugins');
    plugins.ORP_sensor = ORP_sensor;
    window.Cockpit.plugins.push( plugins.ORP_sensor );

}(window));
