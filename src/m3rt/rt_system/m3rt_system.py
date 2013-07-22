# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _m3rt_system
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


class PySwigIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PySwigIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PySwigIterator, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _m3rt_system.delete_PySwigIterator
    __del__ = lambda self : None;
    def value(*args): return _m3rt_system.PySwigIterator_value(*args)
    def incr(*args): return _m3rt_system.PySwigIterator_incr(*args)
    def decr(*args): return _m3rt_system.PySwigIterator_decr(*args)
    def distance(*args): return _m3rt_system.PySwigIterator_distance(*args)
    def equal(*args): return _m3rt_system.PySwigIterator_equal(*args)
    def copy(*args): return _m3rt_system.PySwigIterator_copy(*args)
    def next(*args): return _m3rt_system.PySwigIterator_next(*args)
    def previous(*args): return _m3rt_system.PySwigIterator_previous(*args)
    def advance(*args): return _m3rt_system.PySwigIterator_advance(*args)
    def __eq__(*args): return _m3rt_system.PySwigIterator___eq__(*args)
    def __ne__(*args): return _m3rt_system.PySwigIterator___ne__(*args)
    def __iadd__(*args): return _m3rt_system.PySwigIterator___iadd__(*args)
    def __isub__(*args): return _m3rt_system.PySwigIterator___isub__(*args)
    def __add__(*args): return _m3rt_system.PySwigIterator___add__(*args)
    def __sub__(*args): return _m3rt_system.PySwigIterator___sub__(*args)
    def __iter__(self): return self
_m3rt_system.PySwigIterator_swigregister(PySwigIterator)

class M3RtService(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, M3RtService, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, M3RtService, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _m3rt_system.new_M3RtService(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _m3rt_system.delete_M3RtService
    __del__ = lambda self : None;
    def Startup(*args): return _m3rt_system.M3RtService_Startup(*args)
    def Shutdown(*args): return _m3rt_system.M3RtService_Shutdown(*args)
    def AttachRtSystem(*args): return _m3rt_system.M3RtService_AttachRtSystem(*args)
    def RemoveRtSystem(*args): return _m3rt_system.M3RtService_RemoveRtSystem(*args)
    def IsRtSystemOperational(*args): return _m3rt_system.M3RtService_IsRtSystemOperational(*args)
    def SetComponentStateSafeOp(*args): return _m3rt_system.M3RtService_SetComponentStateSafeOp(*args)
    def SetComponentStateOp(*args): return _m3rt_system.M3RtService_SetComponentStateOp(*args)
    def AttachDataService(*args): return _m3rt_system.M3RtService_AttachDataService(*args)
    def AttachRosService(*args): return _m3rt_system.M3RtService_AttachRosService(*args)
    def RemoveRosService(*args): return _m3rt_system.M3RtService_RemoveRosService(*args)
    def RemoveDataService(*args): return _m3rt_system.M3RtService_RemoveDataService(*args)
    def AttachLogService(*args): return _m3rt_system.M3RtService_AttachLogService(*args)
    def AddLogComponent(*args): return _m3rt_system.M3RtService_AddLogComponent(*args)
    def RemoveLogService(*args): return _m3rt_system.M3RtService_RemoveLogService(*args)
    def IsDataServiceRunning(*args): return _m3rt_system.M3RtService_IsDataServiceRunning(*args)
    def IsLogServiceRunning(*args): return _m3rt_system.M3RtService_IsLogServiceRunning(*args)
    def IsRosServiceRunning(*args): return _m3rt_system.M3RtService_IsRosServiceRunning(*args)
    def IsRtSystemRunning(*args): return _m3rt_system.M3RtService_IsRtSystemRunning(*args)
    def GetNumComponents(*args): return _m3rt_system.M3RtService_GetNumComponents(*args)
    def GetComponentName(*args): return _m3rt_system.M3RtService_GetComponentName(*args)
    def GetComponentType(*args): return _m3rt_system.M3RtService_GetComponentType(*args)
    def GetComponentState(*args): return _m3rt_system.M3RtService_GetComponentState(*args)
    def GetComponentIdx(*args): return _m3rt_system.M3RtService_GetComponentIdx(*args)
    def PrettyPrintComponent(*args): return _m3rt_system.M3RtService_PrettyPrintComponent(*args)
    def PrettyPrintRtSystem(*args): return _m3rt_system.M3RtService_PrettyPrintRtSystem(*args)
    def ClientSubscribeStatus(*args): return _m3rt_system.M3RtService_ClientSubscribeStatus(*args)
    def IsDataServiceError(*args): return _m3rt_system.M3RtService_IsDataServiceError(*args)
_m3rt_system.M3RtService_swigregister(M3RtService)



