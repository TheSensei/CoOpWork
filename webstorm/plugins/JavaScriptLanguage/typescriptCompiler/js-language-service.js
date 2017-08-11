var pluginFactories = {
    typescript: function () {
        var tsFactory = require("./typescript/ts-plugin");
        return tsFactory.typescriptLanguagePluginFactory;
    }
};
var initializedPlugin = null;
function parseParams() {
    var result = {
        sessionId: null,
        restArgs: null,
        pluginName: null
    };
    var args = process.argv.slice(2);
    var counter = 0;
    var paramNameToPropertyName = {};
    paramNameToPropertyName["-id="] = 'sessionId';
    paramNameToPropertyName["-pluginName="] = 'pluginName';
    args.forEach(function (value, index, arr) {
        function isName(name) {
            return value.indexOf(name) === 0;
        }
        function getValue() {
            return value.split('=')[1];
        }
        Object.keys(paramNameToPropertyName).forEach(function (val) {
            if (isName(val)) {
                result[paramNameToPropertyName[val]] = getValue();
                counter++;
            }
        });
    });
    result.restArgs = args.slice(counter);
    return result;
}
function initAndStartListening(params) {
    var readline = require("readline");
    var rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        terminal: false
    });
    //I am not sure that we need it
    process.stdin.setEncoding('utf8');
    var expectedState = true;
    rl.on("line", function (input) {
        var message = input.trim();
        if (expectedState) {
            var state = JSON.parse(message);
            if (state && state.pluginName) {
                var pluginName = state.pluginName;
                if (initializedPlugin == null) {
                    var pluginFactory = pluginFactories[pluginName]();
                    if (pluginFactory != null) {
                        var result = {};
                        try {
                            var _a = pluginFactory.create(state), languagePlugin = _a.languagePlugin, readyMessage = _a.readyMessage;
                            initializedPlugin = languagePlugin;
                            result.success = true;
                            result.message = readyMessage;
                            sendJson(JSON.stringify(result));
                        }
                        catch (e) {
                            //initialization error
                            //ok, lets kill the process
                            result.success = false;
                            var err = e.message || e.messageText;
                            result.error = "Initialization error (" + pluginName + "). " + err;
                            result.stack = e.stack;
                            sendJson(JSON.stringify(result));
                        }
                    }
                }
                expectedState = false;
            }
        }
        else {
            if (initializedPlugin != null) {
                try {
                    initializedPlugin.onMessage(message);
                }
                catch (e) {
                    console.error(e.message + " " + e.stack);
                }
            }
        }
    });
    rl.on("close", function () {
        exitProcess();
    });
    sendCommand("ready");
    setInterval(function () {
        console.error('Process heartbeat: "alive"');
    }, 60000);
    function sendCommand(command) {
        process.stdout.write(params.sessionId + ' ' + command + '\n');
    }
    function sendJson(json) {
        process.stdout.write(json + '\n');
    }
}
function exitProcess() {
    process.exit(0);
}
initAndStartListening(parseParams());
