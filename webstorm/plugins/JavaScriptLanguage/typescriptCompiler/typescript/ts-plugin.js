/**
 * Entry point for the TypeScript plugin
 */
"use strict";
var service_loader_1 = require("./service-loader");
var ts_session_1 = require("./ts-session");
var logger_impl_1 = require("./logger-impl");
var out_path_process_1 = require("./out-path-process");
var TypeScriptLanguagePlugin = (function () {
    function TypeScriptLanguagePlugin(state) {
        var serviceInfo = service_loader_1.getService(state.serverFolderPath);
        var serviceContext = serviceInfo.context;
        var serverFilePath = serviceInfo.serverFilePath;
        var ts_impl = serviceContext.ts;
        var loggerImpl = logger_impl_1.createLoggerFromEnv(ts_impl);
        overrideSysDefaults(ts_impl, serverFilePath);
        var commonDefaultCommandLine = state.hasManualParams && state.commandLineArguments ?
            ts_impl.parseCommandLine(state.commandLineArguments) :
            null;
        var commonDefaultOptions = null;
        if (commonDefaultCommandLine != null) {
            commonDefaultOptions = commonDefaultCommandLine.options;
        }
        if (commonDefaultOptions === null && state.hasManualParams) {
            commonDefaultOptions = {};
        }
        var pathProcessor = null;
        if (state.hasManualParams && state.outPath) {
            pathProcessor = out_path_process_1.getPathProcessor(ts_impl, state);
        }
        var mainFile = null;
        if (state.hasManualParams && state.mainFilePath) {
            mainFile = state.mainFilePath;
        }
        this._session = ts_session_1.getSession(ts_impl, loggerImpl, commonDefaultOptions, pathProcessor, mainFile);
        this.readyMessage = { version: ts_impl.version };
        if (ts_impl.getSupportedCodeFixes) {
            var codes = ts_impl.getSupportedCodeFixes();
            if (codes && codes.length > 0) {
                this.readyMessage.supportedErrorCodes = codes;
            }
        }
    }
    TypeScriptLanguagePlugin.prototype.onMessage = function (p) {
        this._session.onMessage(p);
    };
    return TypeScriptLanguagePlugin;
}());
var TypeScriptLanguagePluginFactory = (function () {
    function TypeScriptLanguagePluginFactory() {
    }
    TypeScriptLanguagePluginFactory.prototype.create = function (state) {
        var typeScriptLanguagePlugin = new TypeScriptLanguagePlugin(state);
        return {
            languagePlugin: typeScriptLanguagePlugin,
            readyMessage: typeScriptLanguagePlugin.readyMessage
        };
    };
    return TypeScriptLanguagePluginFactory;
}());
function overrideSysDefaults(ts_impl, serverFolderPath) {
    var pending = [];
    var canWrite = true;
    function writeMessage(s) {
        if (!canWrite) {
            pending.push(s);
        }
        else {
            canWrite = false;
            process.stdout.write(new Buffer(s, "utf8"), setCanWriteFlagAndWriteMessageIfNecessary);
        }
    }
    function setCanWriteFlagAndWriteMessageIfNecessary() {
        canWrite = true;
        if (pending.length) {
            writeMessage(pending.shift());
        }
    }
    // Override sys.write because fs.writeSync is not reliable on Node 4
    ts_impl.sys.write = function (s) { return writeMessage(s); };
    //ts 2.0 compatibility
    ts_impl.sys.setTimeout = setTimeout;
    ts_impl.sys.clearTimeout = clearTimeout;
    //ts2.0.5 & 2.1
    ts_impl.sys.setImmediate = setImmediate;
    ts_impl.sys.clearImmediate = clearImmediate;
    if (typeof global !== "undefined" && global.gc) {
        ts_impl.sys.gc = function () { return global.gc(); };
    }
    ts_impl.sys.getExecutingFilePath = function () {
        return serverFolderPath;
    };
    var pollingWatchedFileSet = createPollingWatchedFileSet(ts_impl);
    ts_impl.sys.watchFile = function (fileName, callback) {
        var watchedFile = pollingWatchedFileSet.addFile(fileName, callback);
        return {
            close: function () { return pollingWatchedFileSet.removeFile(watchedFile); }
        };
    };
}
//copy ts-server implementation
function createPollingWatchedFileSet(ts_impl, interval, chunkSize) {
    if (interval === void 0) { interval = 2500; }
    if (chunkSize === void 0) { chunkSize = 30; }
    var fs = require("fs");
    var watchedFiles = [];
    var nextFileToCheck = 0;
    var watchTimer;
    function getModifiedTime(fileName) {
        return fs.statSync(fileName).mtime;
    }
    function poll(checkedIndex) {
        var watchedFile = watchedFiles[checkedIndex];
        if (!watchedFile) {
            return;
        }
        fs.stat(watchedFile.fileName, function (err, stats) {
            if (err) {
                watchedFile.callback(watchedFile.fileName);
            }
            else if (watchedFile.mtime.getTime() !== stats.mtime.getTime()) {
                watchedFile.mtime = getModifiedTime(watchedFile.fileName);
                watchedFile.callback(watchedFile.fileName, watchedFile.mtime.getTime() === 0);
            }
        });
    }
    // this implementation uses polling and
    // stat due to inconsistencies of fs.watch
    // and efficiency of stat on modern filesystems
    function startWatchTimer() {
        watchTimer = setInterval(function () {
            var count = 0;
            var nextToCheck = nextFileToCheck;
            var firstCheck = -1;
            while ((count < chunkSize) && (nextToCheck !== firstCheck)) {
                poll(nextToCheck);
                if (firstCheck < 0) {
                    firstCheck = nextToCheck;
                }
                nextToCheck++;
                if (nextToCheck === watchedFiles.length) {
                    nextToCheck = 0;
                }
                count++;
            }
            nextFileToCheck = nextToCheck;
        }, interval);
    }
    function addFile(fileName, callback) {
        var file = {
            fileName: fileName,
            callback: callback,
            mtime: getModifiedTime(fileName)
        };
        watchedFiles.push(file);
        if (watchedFiles.length === 1) {
            startWatchTimer();
        }
        return file;
    }
    function removeFile(file) {
        unorderedRemoveItem(watchedFiles, file);
    }
    function unorderedRemoveItem(array, item) {
        unorderedRemoveFirstItemWhere(array, function (element) { return element === item; });
    }
    function unorderedRemoveFirstItemWhere(array, predicate) {
        for (var i = 0; i < array.length; i++) {
            if (predicate(array[i])) {
                unorderedRemoveItemAt(array, i);
                break;
            }
        }
    }
    function unorderedRemoveItemAt(array, index) {
        // Fill in the "hole" left at `index`.
        array[index] = array[array.length - 1];
        array.pop();
    }
    return {
        getModifiedTime: getModifiedTime,
        poll: poll,
        startWatchTimer: startWatchTimer,
        addFile: addFile,
        removeFile: removeFile
    };
}
var typescriptLanguagePluginFactory = new TypeScriptLanguagePluginFactory();
exports.typescriptLanguagePluginFactory = typescriptLanguagePluginFactory;
