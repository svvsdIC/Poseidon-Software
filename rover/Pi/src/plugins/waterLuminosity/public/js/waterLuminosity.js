(function(window)
{
    'use strict';
    class waterLuminosity
    {
        constructor( cockpit )
        {
            console.log('waterLuminosity Plugin running');
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
                name: 'waterLuminosity.value',
                description: 'The waterLuminosity value'
            }]
        };

        // This pattern will hook events in the cockpit and pull them all back
        // so that the reference to this instance is available for further processing
        listen()
        {
            var self = this;
            // Listen for response messages from the Node plugin
            this.cockpit.rov.withHistory.on('plugin.waterLuminosity.value', function( message )
            {
                self.cockpit.emit('plugin.waterLuminosity.value', message);
            });
        };
    };

    // Add plugin to the window object and add it to the plugins list
    var plugins = namespace('plugins');
    plugins.waterLuminosity = waterLuminosity;
    window.Cockpit.plugins.push( plugins.waterLuminosity);

}(window));
