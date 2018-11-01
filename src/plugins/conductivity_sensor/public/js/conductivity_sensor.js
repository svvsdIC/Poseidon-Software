(function(window)
{
    'use strict';
    class conductivity_sensor
    {
        constructor( cockpit )
        {
            console.log('conductivity_sensor Plugin running');
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
                name: 'conductivity_sensor.value',
                description: 'The conductivity_sensor value'
            }]
        };

        // This pattern will hook events in the cockpit and pull them all back
        // so that the reference to this instance is available for further processing
        listen()
        {
            var self = this;
            // Listen for response messages from the Node plugin
            this.cockpit.rov.withHistory.on('plugin.conductivity_sensor.value', function( message )
            {
                self.cockpit.emit('plugin.conductivity_sensor.value', message);
            });
        };
    };

    // Add plugin to the window object and add it to the plugins list
    var plugins = namespace('plugins');
    plugins.conductivity_sensor = conductivity_sensor;
    window.Cockpit.plugins.push( plugins.conductivity_sensor );

}(window));
