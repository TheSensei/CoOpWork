"use strict";
function initCommandNames(TypeScriptCommandNames) {
    TypeScriptCommandNames.IDEChangeFiles = "ideChangeFiles";
    TypeScriptCommandNames.IDECompile = "ideCompile";
    TypeScriptCommandNames.IDEGetErrors = "ideGetErr";
    TypeScriptCommandNames.IDEGetAllErrors = "ideGetAllErr";
    TypeScriptCommandNames.IDECompletions = "ideCompletions";
    TypeScriptCommandNames.IDEGetMainFileErrors = "ideGetMainFileErr";
    TypeScriptCommandNames.IDEGetProjectErrors = "ideGetProjectErr";
    if (TypeScriptCommandNames.ReloadProjects == undefined) {
        TypeScriptCommandNames.ReloadProjects = "reloadProjects";
    }
}
exports.initCommandNames = initCommandNames;
exports.DETAILED_COMPLETION_COUNT = 30;
exports.DETAILED_MAX_TIME = 150;
function isTypeScript15(ts_impl) {
    return checkVersion(ts_impl, "1.5");
}
exports.isTypeScript15 = isTypeScript15;
function isTypeScript16(ts_impl) {
    return checkVersion(ts_impl, "1.6");
}
exports.isTypeScript16 = isTypeScript16;
function isTypeScript17(ts_impl) {
    return checkVersion(ts_impl, "1.7");
}
exports.isTypeScript17 = isTypeScript17;
function checkVersion(ts_impl, versionText) {
    return ts_impl.version && (ts_impl.version == versionText || ts_impl.version.indexOf(versionText) == 0);
}
/**
 * Default tsserver implementation doesn't return response in most cases ("open", "close", etc.)
 * we want to override the behaviour and send empty-response holder
 */
exports.doneRequest = {
    responseRequired: true,
    response: "done"
};
var DiagnosticsContainer = (function () {
    function DiagnosticsContainer() {
        this.value = {};
    }
    DiagnosticsContainer.prototype.reset = function () {
        this.value = {};
    };
    return DiagnosticsContainer;
}());
exports.DiagnosticsContainer = DiagnosticsContainer;
function extendEx(ObjectToExtend, name, func) {
    var proto = ObjectToExtend.prototype;
    var oldFunction = proto[name];
    proto[name] = function () {
        return func.apply(this, [oldFunction, arguments]);
    };
}
exports.extendEx = extendEx;
function isFunctionKind(kind) {
    return kind == "method" ||
        kind == "local function" ||
        kind == "function" ||
        kind == "call" ||
        kind == "construct";
}
exports.isFunctionKind = isFunctionKind;
