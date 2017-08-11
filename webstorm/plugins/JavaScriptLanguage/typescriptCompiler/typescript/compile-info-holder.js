"use strict";
var crypto = require('crypto');
var HolderContainer = (function () {
    function HolderContainer() {
        this.value = {};
    }
    HolderContainer.prototype.reset = function () {
        this.value = {};
    };
    return HolderContainer;
}());
exports.HolderContainer = HolderContainer;
;
/**
 * Emulating incremental compilation.
 * If file content wasn't changes we don't need recompile the file
 */
var CompileInfoHolder = (function () {
    function CompileInfoHolder(ts_impl) {
        this._lastCompilerResult = {};
        this.ts_impl = ts_impl;
    }
    CompileInfoHolder.prototype.checkUpdateAndAddToCache = function (file) {
        if (file) {
            var fileName = this.ts_impl.normalizePath(file.fileName);
            var newHash = calcHash(file.text);
            var oldHash = this._lastCompilerResult[fileName];
            if (oldHash != null && oldHash == newHash) {
                return false;
            }
            this._lastCompilerResult[fileName] = newHash;
            return true;
        }
        return false;
    };
    CompileInfoHolder.prototype.resetForFile = function (fileName) {
        if (this._lastCompilerResult[fileName]) {
            this._lastCompilerResult[fileName] = null;
        }
    };
    CompileInfoHolder.prototype.reset = function () {
        this._lastCompilerResult = {};
    };
    return CompileInfoHolder;
}());
exports.CompileInfoHolder = CompileInfoHolder;
function calcHash(content) {
    return crypto.createHash('md5').update(content).digest("hex");
}
