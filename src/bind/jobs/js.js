// JobSystem
function JobSystem() { throw "cannot construct a JobSystem, no constructor in IDL" }
JobSystem.prototype = Object.create(WrapperObject.prototype);
JobSystem.prototype.constructor = JobSystem;
JobSystem.prototype.__class__ = JobSystem;
JobSystem.__cache__ = {};
Module['JobSystem'] = JobSystem;
JobSystem.prototype["__destroy__"] = JobSystem.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_JobSystem__destroy(self);
};

(function() {
    function setupEnums() {
    }
    if (Module['calledRun']) setupEnums();
    else addOnPreMain(setupEnums);
})();
