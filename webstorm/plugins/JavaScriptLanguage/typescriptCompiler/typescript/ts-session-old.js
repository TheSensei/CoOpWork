"use strict";
var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var ts15impl_1 = require("./ts15impl");
var util_1 = require("./util");
var logger_impl_1 = require("./logger-impl");
var ts_project_service_old_1 = require("./ts-project-service-old");
function getSessionOld(TypeScriptSession, TypeScriptProjectService, TypeScriptCommandNames, logger, host, ts_impl, commonDefaultOptions, mainFile, projectEmittedWithAllFiles, getFileWrite, fixNameWithProcessor, ensureDirectoriesExist) {
    var projectErrors = new util_1.DiagnosticsContainer();
    var isVersionTypeScript15 = util_1.isTypeScript15(ts_impl);
    var isVersionTypeScript16 = util_1.isTypeScript16(ts_impl);
    var isVersionTypeScript17 = util_1.isTypeScript17(ts_impl);
    if (isVersionTypeScript15) {
        ts15impl_1.setGetFileNames(ts_impl.server.Project);
    }
    ts_project_service_old_1.extendProjectService(TypeScriptProjectService, ts_impl, host, projectEmittedWithAllFiles, projectErrors, isVersionTypeScript15, isVersionTypeScript16, isVersionTypeScript17, commonDefaultOptions);
    var IDESession = (function (_super) {
        __extends(IDESession, _super);
        function IDESession(byteLength, hrtime, logger) {
            if (isVersionTypeScript15) {
                _super.call(this, host, logger, undefined, undefined);
            }
            else {
                _super.call(this, host, byteLength, hrtime, logger);
            }
        }
        IDESession.prototype.afterCompileProcess = function (project, requestedFile, wasOpened) {
        };
        IDESession.prototype.getProjectName = function (project) {
            return project.projectFilename;
        };
        IDESession.prototype.getProjectConfigPathEx = function (project) {
            return project.projectFilename;
        };
        IDESession.prototype.positionToLineOffset = function (project, fileName, position) {
            return project.compilerService.host.positionToLineOffset(fileName, position);
        };
        IDESession.prototype.containsFileEx = function (project, file, reqOpen) {
            return project.getSourceFileFromName(file, reqOpen) != null;
        };
        IDESession.prototype.lineOffsetToPosition = function (project, fileName, line, offset) {
            return project.compilerService.host.lineOffsetToPosition(fileName, line, offset);
        };
        IDESession.prototype.getLanguageService = function (project) {
            return project.compilerService.languageService;
        };
        IDESession.prototype.beforeFirstMessage = function () {
        };
        IDESession.prototype.onMessage = function (message) {
            if (isVersionTypeScript15) {
                ts15impl_1.onMessage15(this, message);
                return;
            }
            _super.prototype.onMessage.call(this, message);
        };
        IDESession.prototype.executeCommand = function (request) {
            var startTime = Date.now();
            var command = request.command;
            try {
                if (TypeScriptCommandNames.Open == command) {
                    //use own implementation
                    var openArgs = request.arguments;
                    this.openClientFileEx(openArgs);
                    return util_1.doneRequest;
                }
                else if (TypeScriptCommandNames.ReloadProjects == command) {
                    projectEmittedWithAllFiles.reset();
                    if (isVersionTypeScript15)
                        return ts15impl_1.reload15(this, ts_impl);
                    var requestWithConfigArgs = request.arguments;
                    if (requestWithConfigArgs.projectFileName) {
                        var configFileName = ts_impl.normalizePath(requestWithConfigArgs.projectFileName);
                        var project = this.projectService.findConfiguredProjectByConfigFile(configFileName);
                        if (project != null) {
                            this.projectService.updateConfiguredProject(project);
                        }
                    }
                    this.refreshStructureEx();
                    return util_1.doneRequest;
                }
                else if (TypeScriptCommandNames.IDEChangeFiles == command) {
                    var updateFilesArgs = request.arguments;
                    return this.updateFilesEx(updateFilesArgs);
                }
                else if (TypeScriptCommandNames.IDECompile == command) {
                    var fileArgs = request.arguments;
                    return this.compileFileEx(fileArgs);
                }
                else if (TypeScriptCommandNames.Close == command) {
                    if (isVersionTypeScript15) {
                        ts15impl_1.close15(this, request);
                        return util_1.doneRequest;
                    }
                    _super.prototype.executeCommand.call(this, request);
                    return util_1.doneRequest;
                }
                else if (TypeScriptCommandNames.IDEGetErrors == command) {
                    var args = request.arguments;
                    return { response: { infos: this.getDiagnosticsEx(args.files) }, responseRequired: true };
                }
                else if (TypeScriptCommandNames.IDEGetMainFileErrors == command) {
                    var args = request.arguments;
                    return { response: { infos: this.getMainFileDiagnosticsForFileEx(args.file) }, responseRequired: true };
                }
                else if (TypeScriptCommandNames.IDEGetProjectErrors == command) {
                    var args = request.arguments;
                    return { response: { infos: this.getProjectDiagnosticsForFileEx(args.file) }, responseRequired: true };
                }
                else if (TypeScriptCommandNames.IDECompletions == command) {
                    if (isVersionTypeScript15)
                        return { response: [], responseRequired: true };
                    return this.getCompletionEx(request);
                }
                return _super.prototype.executeCommand.call(this, request);
            }
            finally {
                var processingTime = Date.now() - startTime;
                logger_impl_1.serverLogger("Message " + request.seq + " '" + command + "' server time, mills: " + processingTime, true);
            }
        };
        IDESession.prototype.closeClientFileEx = function (normalizedFileName) {
            this.projectService.closeClientFile(normalizedFileName);
        };
        IDESession.prototype.refreshStructureEx = function () {
            this.projectService.updateProjectStructure();
        };
        IDESession.prototype.openClientFileEx = function (openArgs) {
            var fileName = openArgs.file;
            var fileContent = openArgs.fileContent;
            var configFile = openArgs.projectFileName;
            var file = ts_impl.normalizePath(fileName);
            return this.openFileEx(file, fileContent, configFile);
        };
        IDESession.prototype.openFileEx = function (fileName, fileContent, configFileName) {
            var projectService = this.projectService;
            if (configFileName) {
                projectService.openOrUpdateConfiguredProjectForFile(ts_impl.normalizePath(configFileName));
            }
            else {
                if (isVersionTypeScript15)
                    return ts15impl_1.openClientFileConfig15(projectService, fileName, fileContent, ts_impl);
                projectService.openOrUpdateConfiguredProjectForFile(fileName);
            }
            var info = projectService.openFile(fileName, /*openedByClient*/ true, fileContent);
            projectService.addOpenFile(info);
            return info;
        };
        IDESession.prototype.changeFileEx = function (fileName, content, tsconfig) {
            var file = ts_impl.normalizePath(fileName);
            var project = this.projectService.getProjectForFile(file);
            if (project) {
                var compilerService = project.compilerService;
                var scriptInfo = compilerService.host.getScriptInfo(file);
                if (scriptInfo != null) {
                    scriptInfo.svc.reload(content);
                    if (logger_impl_1.isLogEnabled) {
                        logger_impl_1.serverLogger("Update file reload content from text " + file);
                    }
                }
                else {
                    if (logger_impl_1.isLogEnabled) {
                        logger_impl_1.serverLogger("Update file scriptInfo is null " + file);
                    }
                }
            }
            else {
                if (logger_impl_1.isLogEnabled) {
                    logger_impl_1.serverLogger("Update file cannot find project for " + file);
                }
                this.openClientFileEx({
                    file: fileName,
                    fileContent: content,
                    projectFileName: tsconfig
                });
            }
        };
        IDESession.prototype.getProjectForCompileRequest = function (req, normalizedRequestedFile) {
            var project = null;
            if (req.file) {
                project = this.projectService.getProjectForFile(normalizedRequestedFile);
                if (project) {
                    return { project: project, wasOpened: false };
                }
                //by some reason scriptInfo can exist but defaultProject == null
                //lets try to detect project by opened files (if they are exist)
                var scriptInfo = this.projectService.filenameToScriptInfo[normalizedRequestedFile];
                if (scriptInfo) {
                    //lets try to file parent project
                    project = getProjectForPath(scriptInfo, normalizedRequestedFile, this.projectService);
                    if (project) {
                        return { project: project, wasOpened: false };
                    }
                }
                this.projectService.openOrUpdateConfiguredProjectForFile(normalizedRequestedFile);
                return { project: this.projectService.getProjectForFile(normalizedRequestedFile), wasOpened: false };
            }
            else {
                if (isVersionTypeScript15)
                    return {
                        project: ts15impl_1.openProjectByConfig(this.projectService, normalizedRequestedFile, ts_impl),
                        wasOpened: false
                    };
                project = this.projectService.findConfiguredProjectByConfigFile(normalizedRequestedFile);
                if (!project) {
                    var configResult = this.projectService.openConfigFile(normalizedRequestedFile);
                    if (configResult && configResult.project) {
                        return { project: configResult.project, wasOpened: false };
                    }
                }
            }
            return { project: project, wasOpened: false };
        };
        IDESession.prototype.getProjectForFileEx = function (fileName, projectFile) {
            fileName = ts_impl.normalizePath(fileName);
            if (!projectFile) {
                return this.projectService.getProjectForFile(fileName);
            }
            projectFile = ts_impl.normalizePath(projectFile);
            return this.projectService.findConfiguredProjectByConfigFile(projectFile);
        };
        IDESession.prototype.needRecompile = function (project) {
            return !project || !project.projectOptions || project.projectOptions.compileOnSave || project.projectOptions.compileOnSave === undefined;
        };
        IDESession.prototype.setNewLine = function (project, options) {
            project.compilerService.host.getNewLine = function () {
                return ts_impl.getNewLineCharacter(options ? options : {});
            };
        };
        IDESession.prototype.getCompileOptionsEx = function (project) {
            return project.projectOptions ? project.projectOptions.compilerOptions : null;
        };
        IDESession.prototype.appendProjectErrors = function (result, processedProjects, empty) {
            var values = projectErrors.value;
            for (var projectName in values) {
                if (values.hasOwnProperty(projectName)) {
                    var processedProjectErrors = values[projectName];
                    if (processedProjectErrors && processedProjectErrors.length > 0) {
                        if (empty || processedProjects[projectName]) {
                            var diagnosticBody = {
                                file: projectName,
                                diagnostics: processedProjectErrors
                            };
                            result = [diagnosticBody].concat(result);
                        }
                    }
                }
            }
            return result;
        };
        return IDESession;
    }(TypeScriptSession));
    function getProjectForPath(scriptInfo, path, projectService) {
        var searchPath = ts_impl.getDirectoryPath(path);
        if (searchPath) {
            var configFileName = projectService.findConfigFile(searchPath);
            if (configFileName) {
                var project = projectService.findConfiguredProjectByConfigFile(configFileName);
                if (project && project.getSourceFile(scriptInfo)) {
                    return project;
                }
            }
        }
        return null;
    }
    var ideSession = new IDESession(Buffer.byteLength, process.hrtime, logger);
    return ideSession;
}
exports.getSessionOld = getSessionOld;
