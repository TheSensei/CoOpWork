"use strict";
var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var util_1 = require("./util");
var logger_impl_1 = require("./logger-impl");
var ts_project_service_new_1 = require("./ts-project-service-new");
function getSessionNew(TypeScriptSession, TypeScriptProjectService, TypeScriptCommandNames, logger, host, ts_impl, commonDefaultOptions, mainFile, projectEmittedWithAllFiles, getFileWrite, fixNameWithProcessor, ensureDirectoriesExist) {
    ts_project_service_new_1.extendProjectServiceNew(TypeScriptProjectService, ts_impl, host, projectEmittedWithAllFiles);
    var IDESession = (function (_super) {
        __extends(IDESession, _super);
        function IDESession() {
            _super.apply(this, arguments);
        }
        IDESession.prototype.appendProjectErrors = function (result, processedProjects, empty) {
            var _this = this;
            if (!processedProjects)
                return result;
            var _loop_1 = function(projectName) {
                if (processedProjects.hasOwnProperty(projectName)) {
                    var processedProject_1 = processedProjects[projectName];
                    var errors = processedProject_1.getProjectErrors();
                    if (errors && errors.length > 0) {
                        var items = errors.map(function (el) { return _this.formatDiagnostic(projectName, processedProject_1, el); });
                        result = result.concat({
                            file: projectName,
                            diagnostics: items
                        });
                    }
                }
            };
            for (var projectName in processedProjects) {
                _loop_1(projectName);
            }
            return result;
        };
        IDESession.prototype.beforeFirstMessage = function () {
            if (commonDefaultOptions != null) {
                if (commonDefaultOptions && commonDefaultOptions.compileOnSave == null) {
                    commonDefaultOptions.compileOnSave = true;
                }
                this.getProjectService().setCompilerOptionsForInferredProjects(commonDefaultOptions);
            }
        };
        IDESession.prototype.setNewLine = function (project, options) {
            //todo lsHost is a private field
            var host = project.lsHost;
            if (!host) {
                logger_impl_1.serverLogger("API was changed Project#lsHost is not found", true);
                return;
            }
            if (host && ts_impl.getNewLineCharacter) {
                host.getNewLine = function () {
                    return ts_impl.getNewLineCharacter(options ? options : {});
                };
            }
        };
        IDESession.prototype.getCompileOptionsEx = function (project) {
            return project.getCompilerOptions();
        };
        IDESession.prototype.needRecompile = function (project) {
            if (!project)
                return true;
            var compilerOptions = project.getCompilerOptions();
            if (compilerOptions && compilerOptions.___processed_marker) {
                return project.compileOnSaveEnabled;
            }
            return true;
        };
        IDESession.prototype.afterCompileProcess = function (project, requestedFile, wasOpened) {
            if (project) {
                var projectService = this.getProjectService();
                var externalProjects = projectService.externalProjects;
                for (var _i = 0, externalProjects_1 = externalProjects; _i < externalProjects_1.length; _i++) {
                    var project_1 = externalProjects_1[_i];
                    //close old projects
                    var projectName = this.getProjectName(project_1);
                    logger_impl_1.serverLogger("Close external project " + projectName, true);
                    projectService.closeExternalProject(projectName);
                }
                if (wasOpened) {
                    logger_impl_1.serverLogger("Close the opened file", true);
                    projectService.closeClientFile(requestedFile);
                }
            }
        };
        IDESession.prototype.isExternalProject = function (project) {
            var projectKind = project.projectKind;
            return projectKind && projectKind == ts_impl.server.ProjectKind.External;
        };
        IDESession.prototype.getProjectForCompileRequest = function (req, normalizedRequestedFile) {
            if (req.file) {
                var projectService = this.getProjectService();
                var project = void 0;
                try {
                    var project_2 = projectService.getDefaultProjectForFile(normalizedRequestedFile, true);
                }
                catch (e) {
                }
                if (project) {
                    return { project: project, wasOpened: false };
                }
                project = this.getFromExistingProject(normalizedRequestedFile);
                if (project) {
                    return { project: project, wasOpened: false };
                }
                var openClientFile = projectService.openClientFileWithNormalizedPath(normalizedRequestedFile);
                project = this.getFromExistingProject(normalizedRequestedFile);
                if (project && openClientFile && openClientFile.configFileName) {
                    projectService.openExternalProject({
                        projectFileName: openClientFile.configFileName,
                        rootFiles: [{ fileName: openClientFile.configFileName }],
                        options: {}
                    });
                    projectService.closeClientFile(normalizedRequestedFile);
                    logger_impl_1.serverLogger("Project was created for compiling", true);
                    //keep project
                    return { project: project, wasOpened: false };
                }
                else {
                    logger_impl_1.serverLogger("File was opened for compiling", true);
                    return { project: project, wasOpened: true };
                }
            }
            else if (req.projectFileName) {
                var projectService = this.getProjectService();
                var configProject = projectService.findProject(normalizedRequestedFile);
                if (configProject) {
                    return { project: configProject, wasOpened: false };
                }
                logger_impl_1.serverLogger("Open external project", true);
                projectService.openExternalProject({
                    projectFileName: normalizedRequestedFile,
                    rootFiles: [{ fileName: normalizedRequestedFile }],
                    options: {}
                });
                return { project: projectService.findProject(normalizedRequestedFile), wasOpened: false };
            }
            return { project: null, wasOpened: false };
        };
        IDESession.prototype.positionToLineOffset = function (project, fileName, position) {
            //todo review performance
            var scriptInfo = this.getProjectService().getScriptInfo(fileName);
            if (!scriptInfo) {
                logger_impl_1.serverLogger("ERROR! Cannot find script info for file " + fileName, true);
                return undefined;
            }
            return scriptInfo.positionToLineOffset(position);
        };
        IDESession.prototype.containsFileEx = function (project, file, reqOpen) {
            return project.containsFile(file, reqOpen);
        };
        IDESession.prototype.getProjectName = function (project) {
            return project.getProjectName();
        };
        IDESession.prototype.getProjectConfigPathEx = function (project) {
            if (this.isExternalProject(project)) {
                return this.getProjectName(project);
            }
            //ts2.2
            if (project.getConfigFilePath) {
                return project.getConfigFilePath();
            }
            var configFileName = project.configFileName;
            return configFileName;
        };
        IDESession.prototype.executeCommand = function (request) {
            var startTime = this.getTime();
            var command = request.command;
            try {
                if (TypeScriptCommandNames.Open == command || TypeScriptCommandNames.Close == command) {
                    _super.prototype.executeCommand.call(this, request);
                    //open | close command doesn't send answer so we have to override
                    return util_1.doneRequest;
                }
                else if (TypeScriptCommandNames.ReloadProjects == command) {
                    projectEmittedWithAllFiles.reset();
                    _super.prototype.executeCommand.call(this, request);
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
                else if (TypeScriptCommandNames.IDECompletions == command) {
                    return this.getCompletionEx(request);
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
                    var projectDiagnosticsForFileEx = this.getProjectDiagnosticsForFileEx(args.file);
                    return { response: { infos: projectDiagnosticsForFileEx }, responseRequired: true };
                }
                return _super.prototype.executeCommand.call(this, request);
            }
            finally {
                var processingTime = Date.now() - startTime;
                logger_impl_1.serverLogger("Message " + request.seq + " '" + command + "' server time, mills: " + processingTime, true);
            }
        };
        IDESession.prototype.getProjectForFileEx = function (fileName, projectFile) {
            if (!projectFile) {
                fileName = ts_impl.normalizePath(fileName);
                return this.getProjectService().getDefaultProjectForFile(fileName, true);
            }
            return this.getProjectService().findProject(projectFile);
        };
        IDESession.prototype.closeClientFileEx = function (normalizedFileName) {
            var scriptInfoForNormalizedPath = this.getProjectService().getScriptInfoForNormalizedPath(normalizedFileName);
            if (!scriptInfoForNormalizedPath || !scriptInfoForNormalizedPath.isOpen) {
                return;
            }
            this.projectService.closeClientFile(normalizedFileName);
        };
        IDESession.prototype.configFileDiagnosticEvent = function (triggerFile, configFile, errors) {
        };
        IDESession.prototype.refreshStructureEx = function () {
            this.getProjectService().refreshInferredProjects();
        };
        IDESession.prototype.changeFileEx = function (fileName, content, tsconfig) {
            fileName = ts_impl.normalizePath(fileName);
            var info = this.getProjectService().getScriptInfoForNormalizedPath(fileName);
            if (info && info.isOpen) {
                this.getProjectService().getOrCreateScriptInfo(fileName, true, content);
            }
            else {
                this.getProjectService().openClientFileWithNormalizedPath(fileName, content);
            }
        };
        IDESession.prototype.getLanguageService = function (project) {
            return project.getLanguageService();
        };
        IDESession.prototype.event = function (info, eventName) {
            if (logger_impl_1.isLogEnabled) {
                logger_impl_1.serverLogger("Event " + eventName);
            }
        };
        IDESession.prototype.lineOffsetToPosition = function (project, fileName, line, offset) {
            //todo review performance
            var scriptInfo = this.getProjectService().getScriptInfo(fileName);
            if (!scriptInfo) {
                logger_impl_1.serverLogger("ERROR! Cannot find script info for file " + fileName, true);
                return undefined;
            }
            return scriptInfo.lineOffsetToPosition(line, offset);
        };
        /**
         * todo change d.ts files & replace any by ts.server.ProjectService
         */
        IDESession.prototype.getProjectService = function () {
            return this.projectService;
        };
        IDESession.prototype.getFromExistingProject = function (normalizedRequestedFile) {
            var projectService = this.getProjectService();
            {
                var inferredProjects = projectService.inferredProjects;
                for (var _i = 0, inferredProjects_1 = inferredProjects; _i < inferredProjects_1.length; _i++) {
                    var project = inferredProjects_1[_i];
                    if (this.containsFileEx(project, normalizedRequestedFile, false)) {
                        return project;
                    }
                }
            }
            {
                var configuredProjects = projectService.configuredProjects;
                for (var _a = 0, configuredProjects_1 = configuredProjects; _a < configuredProjects_1.length; _a++) {
                    var project = configuredProjects_1[_a];
                    if (this.containsFileEx(project, normalizedRequestedFile, false)) {
                        return project;
                    }
                }
            }
            return null;
        };
        return IDESession;
    }(TypeScriptSession));
    var cancellationToken;
    try {
        var factory = require("./cancellationToken");
        cancellationToken = factory(host.args);
    }
    catch (e) {
        cancellationToken = {
            isCancellationRequested: function () { return false; }
        };
    }
    var nullTypingsInstaller = {
        enqueueInstallTypingsRequest: function () {
        },
        attach: function (projectService) {
        },
        onProjectClosed: function (p) {
        },
        globalTypingsCacheLocation: undefined
    };
    //todo remove after replacing typing
    var IDESessionImpl = IDESession;
    var session = new IDESessionImpl(host, cancellationToken, false, nullTypingsInstaller, Buffer.byteLength, process.hrtime, logger, true);
    return session;
}
exports.getSessionNew = getSessionNew;
