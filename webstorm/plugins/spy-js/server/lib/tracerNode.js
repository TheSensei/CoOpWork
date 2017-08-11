/*
 * spy-js - v2.0.0
 * http://spy-js.com
 * Copyright (c) 2013-2016 JetBrains - All Rights Reserved.
 *
 * This source code file is a part of spy-js plugin.
 */
var modulePrototype=require("module").Module.prototype,EventEmitter=require("events").EventEmitter,instrumentor=require("./instrument"),evaluator=require("./evaluator"),_=_spy_js._,Tracer=function(session,streamId,opts){var self=this;opts=opts||{},self._core=session,self._streamId=streamId,self._originalUndefined=void 0,self._originalObjectToString=Object.prototype.toString,self._originalFunctionToString=Function.prototype.toString,self._originalDateToString=Date.prototype.toString,self._originalArrayConcat=Array.prototype.concat,self._originalGetOwnPropertyNames=Object.getOwnPropertyNames,self._originalError=Error,self._originalGetOwnPropertyDescriptor=Object.getOwnPropertyDescriptor,self._originalHasOwnProperty=Object.prototype.hasOwnProperty,self._originalProcessHrtime=process.hrtime,self._originalProcessExit=process.exit,self._originalSetTimeout=global.setTimeout,self._originalSetInterval=global.setInterval,self._originalSetImmediate=global.setImmediate,self._eventContext=1,self._isFromSetTimeout=!1,self._isFromSetInterval=!1,self._isFromSetImmediate=!1,self._queuePerEventLimit=opts.queuePerEventLimit-2||1e6,self._functionFrequencyPerEventLimit=opts.functionFrequencyPerEventLimit-2||1e5,self._sendInterval=opts.sendInterval||100,self._evaluatedPropertyNumberLimit=opts.evaluatedPropertyNumberLimit,self._evaluatedStrLengthLimit=opts.evaluatedStrLengthLimit,self._eventChunkSize=500,self._eventQueueTotalLimit=1e4,self._tracer={},self._tracer.__pt=_.bind(self._timestamp,self),self._tracer.__pargs=_.bind(self._arguments,self),self._tracer.__pi=_.bind(self._instruction,self),self._tracer.__pr=_.bind(self._return,self),self._tracer.__pru=_.bind(self._returnUndefined,self),self._tracer.__pf=_.bind(self._function,self),self._tracer.__pp=_.bind(self._program,self),self._tracer.__pfe=_.bind(self._functionEnd,self),self._tracer.__ppe=_.bind(self._programEnd,self),self._tracer.__pe=_.bind(self._exception,self),self._tracer.__pv=_.bind(self._evaluator,self),self._tracer.__pby=_.bind(self._beforeYieldInstruction,self),self._tracer.__pay=_.bind(self._afterYieldInstruction,self),self._tracer.__pig=_.bind(self._generatorInstruction,self),self._tracer.__prg=_.bind(self._generatorReturn,self),self._tracer.__prgu=_.bind(self._generatorReturnUndefined,self),self._tracer.__pfg=_.bind(self._generatorFunction,self),self._tracer.__pfge=_.bind(self._generatorFunctionEnd,self),self._tracer.__peg=_.bind(self._generatorException,self),self._stack=[],self._stackFrameId=1,self._initializeNormal(),self._currentEventQueue=[],self._eventsQueue=[],self._generatorsCache={},self._limits={},self._mutedFiles={},self._effectiveSettings={},self._core.on("event.config.change",function(a){self._applyTracerSettings(a.filter)}),self._core.on("event.file.cache",function(a){self._applyTracerSettings({limits:!0,_config:[a],files:self._effectiveSettings.files,_file:[a.url],events:self._effectiveSettings.events})}),self._tracingCompile=!0,self._core.on("internal.event.stopTracerCompiler",function(){self._tracingCompile=!1}),self._core.on("internal.event.startTracerCompiler",function(){self._tracingCompile=!0}),self._dataInterval=self._originalSetInterval(function(){self._executionContextChanged();var a=Math.min(self._eventsQueue.length,self._eventChunkSize),b=self._eventsQueue.splice(0,a);b.length&&(self.emit("data",{streamId:self._streamId,entries:b}),self._eventsQueue.length>self._eventQueueTotalLimit&&(self._eventsQueue.length=0,self._eventsQueue.push({truncated:!0}))),self._initializeNormal(),self._core.isActive()||(self._eventsQueue.length=0,self._initializeNoOp(),clearInterval(self._dataInterval))},self._sendInterval);var funcNameRegex=/function (.{1,})\(/,instrumentEmit=function(){var a=require("events").EventEmitter.prototype.emit;require("events").EventEmitter.prototype.emit=function(b){if(this==self||this==self._core)return a.apply(this,arguments);var c=self._event,d=!self._insideTraceableStack;if(d){var e=funcNameRegex.exec(this.constructor.toString()),f=e&&e.length>1?e[1]+".":"";self._event={type:f+b}}try{return a.apply(this,arguments)}finally{d&&(self._event=c)}}};instrumentEmit(),self._forceNormalRequire(instrumentEmit),global.setTimeout=function(){var callback=arguments[0],args=[],len=arguments.length;if(len>2)for(var i=2;i<len;i++)args.push(arguments[i]);return arguments[0]=function(){self._isFromSetTimeout=!0,self._executionContextChanged(),callback instanceof Function?Function.prototype.apply.call(callback,null,args):eval(callback),self._isFromSetTimeout=!1},Function.prototype.apply.call(self._originalSetTimeout,null,arguments)},global.setInterval=function(){var callback=arguments[0],args=[],len=arguments.length;if(len>2)for(var i=2;i<len;i++)args.push(arguments[i]);return arguments[0]=function(){self._isFromSetInterval=!0,self._executionContextChanged(),callback instanceof Function?Function.prototype.apply.call(callback,null,args):eval(callback),self._isFromSetInterval=!1},Function.prototype.apply.call(self._originalSetInterval,null,arguments)},global.setImmediate=function(){var callback=arguments[0],args=[],len=arguments.length;if(len>1)for(var i=1;i<len;i++)args.push(arguments[i]);return arguments[0]=function(){self._isFromSetImmediate=!0,self._executionContextChanged(),callback instanceof Function?Function.prototype.apply.call(callback,null,args):eval(callback),self._isFromSetImmediate=!1},Function.prototype.apply.call(self._originalSetImmediate,null,arguments)};var originalProcessExit=process.exit;process.exit=function(){self._core.info("----------- Traced process "+process.pid+" invoked process.exit -----------","blue"),opts.childProcess?self._originalSetTimeout(function(){originalProcessExit.apply(process,arguments)},2*self._sendInterval):self._core.info("Stop the process from spy-js tool window once you have completed working with the trace","blue")};var trackedNativeModules={fs:["rename","ftruncate","truncate","chown","fchown","lchown","chmod","fchmod","lchmod","stat","lstat","fstat","link","symlink","readlink","realpath","unlink","rmdir","mkdir","readdir","close","open","utimes","futimes","fsync","write","read","readFile","writeFile","appendFile","exists"],util:["pump"],process:{value:process,functions:["nextTick","_nextDomainTick","_tickCallback","_tickDomainCallback","_tickFromSpinner"]}};self._forceNormalRequire(function(){Object.keys(trackedNativeModules).forEach(function(a){var b=trackedNativeModules[a],c=b.value||require(a),d=b.value?b.functions:b;d.forEach(function(b){var d=c[b];c[b]=function(){var c=arguments.length;if(c){var e=arguments[c-1];_.isFunction(e)&&(arguments[c-1]=function(){var c=self._event;self._event={type:a+"."+b};try{return e.apply(this,arguments)}finally{self._event=c}})}return d.apply(this,arguments)}})})}),self._forceNormalRequire(function(){self.IncomingMessage=require("http").IncomingMessage}),self._originalCompile=modulePrototype._compile,modulePrototype._compile=self._compile()};Tracer.prototype=_.extend(Object.create(EventEmitter.prototype),{dispose:function(){var a=this;global.clearInterval(a._dataInterval),global.setTimeout=a._originalSetTimeout,global.setInterval=a._originalSetInterval,global.setImmediate=a._originalSetImmediate,process.exit=a._originalProcessExit,modulePrototype._compile=a._originalCompile},start:function(){var a=this;a._event=null,a._tracingCompile=!0},_compile:function(){var a=this;return function(b,c){var d=a._timestamp(),e=b;try{if(a._tracingCompile){var f=a._core.config(c);if(f.instrument&&!a._mutedFile(c)){var g=a._newFileId(),h=instrumentor.run(b.replace(/^\#\!.*/,""),{file:c,fileId:g,prettify:!!f.instrument.prettify,compact:!0,allowReturnOutsideFunction:!0,insertExpressionEvaluators:a._core.realtimeEvaluation()}),i={sourceUrl:c,instrumentedCode:h.code,originalCode:h.originalCode,mtime:(new Date).getTime(),instrumentationMap:h.map,fileId:g,compact:!0};a._core.cache(i),e=h.code}}}catch(b){b&&delete b.stack,a._core.error({message:"Error while instrumenting '"+c+"'\n",err:b})}finally{a._tracingCompile&&a._stackLevel.tracingTime&&a._timeAdd(a._stackLevel.tracingTime,a._timeDiff(d)),a._originalCompile.call(this,e,c)}}},evaluate:function(a,b){var c=this,d={properties:[]};try{c._externalEvent={type:"external.evaluation"};var e=c._core.fileIds(a.file)[0];if(e&&c._core.functionsInFile(e)===a.functionsInFile){var f=global["__pv"+e+"_"+a.functionId];f&&_.isFunction(f)?(evaluator.evaluate({evaluator:f,request:a,result:d,evaluatedPropertyNumberLimit:c._evaluatedPropertyNumberLimit,evaluatedStrLengthLimit:c._evaluatedStrLengthLimit,original:{Undefined:c._originalUndefined,ObjectToString:c._originalObjectToString,DateToString:c._originalDateToString,FunctionToString:c._originalFunctionToString,GetOwnPropertyNames:c._originalGetOwnPropertyNames}}),d.error&&c._core.info(d.error)):d.functionNotFound=!0}else d.fileNotFound=!0}finally{delete c._externalEvent,b(d)}},_newFileId:function(){return Math.random().toString(36).substr(2,5)},_initializeNormal:function(){var a=this;a._initializedNormally||(a._initializedNormally=!0,a._stack.length=0,a._stackLevel={},a._stackFirstMessageAdded=!1,a._lastEvent=null,a._eventContextType=0,a._functionFrequency={},a._insideTraceableStack=!1,global.__pt=a._tracer.__pt,global.__pargs=a._tracer.__pargs,global.__pi=a._tracer.__pi,global.__pr=a._tracer.__pr,global.__pru=a._tracer.__pru,global.__pf=a._tracer.__pf,global.__pp=a._tracer.__pp,global.__pfe=a._tracer.__pfe,global.__ppe=a._tracer.__ppe,global.__pe=a._tracer.__pe,global.__pv=a._tracer.__pv,global.__pby=a._tracer.__pby,global.__payi=a._tracer.__pay,global.__pay=function(){return global.__payi.apply(null,arguments)},global.__pig=a._tracer.__pig,global.__prg=a._tracer.__prg,global.__prgu=a._tracer.__prgu,global.__pfg=a._tracer.__pfg,global.__pfge=a._tracer.__pfge,global.__peg=a._tracer.__peg)},_forceNormalRequire:function(a){global._forceNormalRequire=!0,a(),global._forceNormalRequire=!1},_initializeNoOp:function(){this._initializedNormally=!1;var a=function(){};global.__pt=a,global.__pi=a,global.__pr=function(a,b,c){return c},global.__pru=a,global.__pf=a,global.__pfe=a,global.__pp=a,global.__ppe=a,global.__pe=a,global.__pv=this._tracer.__pv,global.__pargs=this._tracer.__pargs,global.__pby=global.__payi=function(a,b){return b},global.__pig=a,global.__prg=function(a,b,c,d,e){return e},global.__prgu=a,global.__pfg=a,global.__pfge=a,global.__peg=a},_initializeOnlyStackTracking:function(){var a=this;a._initializeNoOp();var b=[];b.push({id:a._stackFrameId++});var c=function(c){var d=a._generatorsCache[c];!d.stack.length||b.length&&b[b.length-1].id===d.stack.current||(d.stack.current=d.stack.pop(),b.push({id:d.stack.current}))};global.__pf=global.__pp=function(){b.push({})},global.__pfe=global.__ppe=function(c,d,e,f,g){b.pop(),a._setupEvaluator(e,f,g),b.length||a._initializeNormal()},global.__pfg=function(c,d,e){a._cacheGeneratorFunctionData(c,d,e),b.push({id:a._stackFrameId++})},global.__pfge=function(d,e,f,g,h,i,j){c(d),b.pop(),a._setupEvaluator(h,i,j),b.length||a._initializeNormal()},global.__pby=function(d,e){c(d);var f=a._generatorsCache[d],g=b[b.length-1].id;return f.stack.push(g),f.stack.current=g,b.pop(),b.length||a._initializeNormal(),e},global.__payi=function(c,d){var e=a._generatorsCache[c];return e.stack.current=e.stack.pop(),b.push({id:e.stack.current}),d}},_mutedFile:function(a){for(var b=this,c=b._effectiveSettings.files||[],d=0,e=c.length;d<e;d++)if(b._matcher(c[d])(a))return!0;return!1},_applyTracerSettings:function(a){var b,c,d=this;if(a.limits){var e=a._config||d._core.allConfigs();for(b=0,c=e.length;b<c;b++){var f=e[b];f.instrument&&f.instrument.objectDump&&_.each(d._core.fileIds(f.url),function(a){d._limits[a]={lvl:f.instrument.objectDump.depth,prop:f.instrument.objectDump.propertyNumber,arr:f.instrument.objectDump.arrayLength,str:f.instrument.objectDump.stringLength}})}}if(a.files){d._effectiveSettings.files=a.files;var g=a._file||d._core.allFileUrls();for(b=0,c=g.length;b<c;b++){var h=g[b];_.each(d._core.fileIds(h),function(a){d._mutedFiles[a]=d._mutedFile(h)})}}if(a.events){var i=[];for(b=0,c=a.events.length;b<c;b++)i.push(d._matcher(a.events[b]));d._eventFilter=function(a){for(var b=0,c=i.length;b<c;b++)if(i[b](a))return!0;return!1},d._effectiveSettings.events=a.events}},_matcher:function(a){var b=!1;"!"==a.charAt(0)&&(a=a.substring(1),b=!0);var c=new RegExp(a,"i");return b?function(a){return!c.test(a)}:function(a){return c.test(a)}},_executionContextChanged:function(){var a=this;a._eventContext++,a._currentEventQueue&&a._currentEventQueue.length&&a._eventsQueue.push(a._currentEventQueue),a._currentEventQueue=[]},_arguments:function(a,b){return this._originalArrayConcat.call(a,[b])},_timestamp:function(){return this._originalProcessHrtime()},_timeDiff:function(a){return this._originalProcessHrtime(a||this._originalProcessHrtime())},_timeAdd:function(a,b){return a[0]+=b[0],a[1]+=b[1],a},_timeSubtract:function(a,b){return a[0]-=b[0],a[1]-=b[1],a},_instruction:function(a,b){var c=this;return c._stackLevelMuted||c._stackLevelExecuted[a]?void c._timeAdd(c._stackLevel.tracingTime,c._timeDiff(b)):(c._stackLevelExecuted[a]=!0,c._currentEventQueue.push({callData:a}),void c._timeAdd(c._stackLevel.tracingTime,c._timeDiff(b)))},_simpleInstruction:function(a,b){var c=this;c._currentEventQueue.push({callData:a,objDump:b})},_instructionWithTime:function(a,b,c){var d=this;d._currentEventQueue.push({callData:a,time:c,objDump:b})},_stackStartInstruction:function(a,b,c){var d=this;d._currentEventQueue.push({callData:a,pageEvent:d._lastEvent,eventContextType:d._eventContextType,time:c,eventContext:d._eventContext,objDump:b})},_context:function(){var a=this,b=a._externalEvent||a._event;if(b?a._lastEvent&&a._lastEvent==b||(a._lastEvent=b,a._executionContextChanged()):a._lastEvent&&(delete a._lastEvent,a._executionContextChanged()),a._eventContextType=a._isFromSetTimeout?1:a._isFromSetInterval?2:a._isFromSetImmediate?3:0,a._eventFilter){var c=a._isFromSetTimeout?"timeout":a._isFromSetInterval?"interval":a._isFromSetImmediate?"immediate":a._lastEvent?a._lastEvent.type:"external";if(a._eventFilter(c))return!1}return!0},_stackLevelStart:function(a){var b=this;if(!b._stack.length){if(!b._context())return b._initializeOnlyStackTracking(),!1;b._stackFirstMessageAdded=!1,b._insideTraceableStack=!0}return b._stackLevelMuted=b._mutedFiles[a],b._stackLevelExecuted={},b._stackLevel={stackLevelMuted:b._stackLevelMuted,tracingTime:[0,0],executed:b._stackLevelExecuted},b._stack.push(b._stackLevel),!b._stackLevelMuted},_stackLevelEnd:function(){var a=this,b=a._stackLevelMuted,c=a._stackLevel;return a._stack.pop(),a._stack.length?(a._stackLevel=a._stack[a._stack.length-1],a._stackLevelMuted=a._stackLevel.stackLevelMuted,a._stackLevelExecuted=a._stackLevel.executed,a._timeAdd(a._stackLevel.tracingTime,c.tracingTime)):a._insideTraceableStack=!1,!b},_return:function(a,b,c,d){var e=this;if(e._stackLevelMuted)return c;var f=e._limits[a];return e._simpleInstruction(b,f?e._prepareObj(c,f,0):e._originalUndefined),e._timeAdd(e._stackLevel.tracingTime,e._timeDiff(d)),c},_returnUndefined:function(a,b,c){var d=this;d._return(a,b,d._originalUndefined,c)},_function:function(a,b,c,d){var e=this;if(!e._stackLevelStart(a))return!1;var f,g=e._limits[a];if(g){f=[],c=c||[];for(var h=0,i=c.length;h<i;h++)f.push(e._prepareObj(c[h],g,0))}else f=e._originalUndefined;return e._stackFirstMessageAdded?e._instructionWithTime(b,f,d):(e._stackStartInstruction(b,f,d),e._stackFirstMessageAdded=!0),e._timeAdd(e._stackLevel.tracingTime,e._timeDiff(d)),!0},_program:function(a,b,c){var d=this;d._stackLevelStart(a)&&(d._stackFirstMessageAdded?d._instructionWithTime(b,d._originalUndefined,c):(d._stackStartInstruction(b,d._originalUndefined,c),d._stackFirstMessageAdded=!0),d._timeAdd(d._stackLevel.tracingTime,d._timeDiff(c)))},_exception:function(a,b,c,d){var e=this;if(!e._stackLevelMuted){var f=e._limits[a],g=f?e._prepareObj(c,f,0):{};c&&g&&e._addStandardErrorProperties(c,g),e._simpleInstruction(b,g),e._timeAdd(e._stackLevel.tracingTime,e._timeDiff(d))}},_evaluator:function(a){return a},_functionEnd:function(a,b,c,d,e){var f=this,g=f._timeSubtract(b,f._stackLevel.tracingTime);if(f._setupEvaluator(c,d,e),f._stackLevelEnd()){f._instructionWithTime(a,f._originalUndefined,g);var h=f._functionFrequency[a];f._currentEventQueue.length>f._queuePerEventLimit||h>f._functionFrequencyPerEventLimit?f._initializeNoOp():f._functionFrequency[a]=h?++h:1}},_programEnd:function(a,b,c,d,e){var f=this,g=f._timeSubtract(b,f._stackLevel.tracingTime);f._setupEvaluator(c,d,e),f._stackLevelEnd()&&f._instructionWithTime(a,f._originalUndefined,g)},_setupEvaluator:function(a,b,c){b&&!global[b]&&(global[b]=function(b){return c.call(a,b)})},_beforeYieldInstruction:function(a,b,c,d,e){e=this._startGeneratorSourcedStackLevelIfRequired(a,c,e);var f=this._generatorsCache[a];return f.stack.push(this._stackLevel.id),f.stack.current=this._stackLevel.id,this._functionEnd(d,e),b},_afterYieldInstruction:function(a,b,c,d){var e=this._generatorsCache[a];return e.stack.current=e.stack.pop(),this._startGeneratorSourcedStackLevel(e.fileId,e.data,c,d,e.stack.current),b},_generatorFunction:function(a,b,c,d,e){this._cacheGeneratorFunctionData(a,b,c),this._startGeneratorSourcedStackLevel(b,c,d,e)},_generatorInstruction:function(a,b,c,d){this._instruction(b,this._startGeneratorSourcedStackLevelIfRequired(a,c,d))},_generatorReturn:function(a,b,c,d,e,f){return this._return(b,c,e,this._startGeneratorSourcedStackLevelIfRequired(a,d,f))},_generatorReturnUndefined:function(a,b,c,d,e){this._returnUndefined(b,c,this._startGeneratorSourcedStackLevelIfRequired(a,d,e))},_generatorFunctionEnd:function(a,b,c,d,e,f,g){return this._functionEnd(b,this._startGeneratorSourcedStackLevelIfRequired(a,c,d),e,f,g)},_generatorException:function(a,b,c,d,e,f){this._exception(b,c,e,this._startGeneratorSourcedStackLevelIfRequired(a,d,f))},_cacheGeneratorFunctionData:function(a,b,c){this._generatorsCache[a]=this._generatorsCache[a]||{fileId:b,data:c,stack:[]}},_startGeneratorSourcedStackLevelIfRequired:function(a,b,c){var d=this._generatorsCache[a];return!d.stack.length||this._insideTraceableStack&&this._stackLevel.id===d.stack.current?c:(d.stack.current=d.stack.pop(),this._startGeneratorSourcedStackLevel(d.fileId,d.data,b,c,d.stack.current),this._timestamp())},_startGeneratorSourcedStackLevel:function(a,b,c,d,e){this._function(a,b,c,d)&&(this._stackLevel.id=e||this._stackFrameId++)},_prepareIncomingMessage:function(a,b,c){return{method:this._prepareObj(a.method,b,c+1),url:this._prepareObj(a.url,b,c+1),statusCode:this._prepareObj(a.statusCode,b,c+1),headers:this._prepareObj(a.headers,b,c),P:""}},_addStandardErrorProperties:function(a,b){this._canAccessProperty(a,"name")&&"string"==typeof a.name&&(b.name="$"+a.name),this._canAccessProperty(a,"message")&&"string"==typeof a.message&&(b.message="$"+a.message)},_prepareObj:function(a,b,c){var d,e=this;switch(typeof a){case"object":if(null===a)d=a;else if(a===global)d="KG";else if(a===process)d="KP";else if(a===console)d="KC";else if("[object Array]"===e._originalObjectToString.call(a)){d="EA";var f=a.length;if(f)if(c>=b.lvl)d="LA";else{d=[];for(var g=0;g<f;g++){if(g>=b.arr){d[g]="A"+f;break}d[g]=e._prepareObj(a[g],b,c+1)}}}else if("[object Date]"===e._originalObjectToString.call(a))d="D"+e._originalDateToString.apply(a);else if(c>=b.lvl)d="LO";else{d={};try{var h=0,i=!1;if(a instanceof e.IncomingMessage)d=e._prepareIncomingMessage(a,b,c);else{for(var j in a)if(e._canAccessProperty(a,j)){if(h>=b.prop){d.P="";break}d[j]=e._prepareObj(a[j],b,c+1),h++}else i=!0;!h&&i&&(d="LO"),a instanceof e._originalError&&e._addStandardErrorProperties(a,d)}}catch(a){d="E"+(a&&e._canAccessProperty(a,"message")&&a.message)}}break;case"string":var k=a.length;d="$"+(k>b.str?k+"$"+a.slice(0,b.str)+"$$":a);break;case"function":d="F"+("string"==typeof a.name?a.name:"");break;case"undefined":d="U";break;case"number":d=isNaN(a)?"NN":a===Number.NEGATIVE_INFINITY?"IN":a===Number.POSITIVE_INFINITY?"IP":a;break;default:d=a}return d},_canAccessProperty:function(a,b){var c=this;try{if(!c._originalHasOwnProperty.call(a,b))return!1;var d=c._originalGetOwnPropertyDescriptor(a,b);return d&&!d.get}catch(a){return!1}}}),module.exports=Tracer;