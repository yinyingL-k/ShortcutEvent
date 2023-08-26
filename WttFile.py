import ctypes as C
import os

callbackDatas = {}
class CallbackData:
    def __init__(self, fileName):
        self.size = os.path.getsize(fileName)
        self.f = open(fileName, 'rb')
        return
    
class WttCallback(C.Structure):  
	 _fields_ = (
	 			('cb', C.CFUNCTYPE(C.c_int32,C.c_uint32, C.c_void_p, C.c_void_p)),
	 		  )

def dll_callback_func(bufferSize, buffer, callbackData):
    taskID = C.cast(callbackData, C.POINTER(C.c_int32))[0]
    data = callbackDatas[taskID]
    if data.size <= 0:
        return data.size
    len = bufferSize
    if data.size < bufferSize:
        len = data.size
    data.size = data.size - len
    a = data.f.read(len)
    C.memmove(buffer, a, len)
    return len

dll = C.cdll.LoadLibrary('F:/coding/Project1/x64/Release/WttFile.dll')
callbackFunc= C.CFUNCTYPE(C.c_int32,C.c_uint32, C.c_void_p, C.c_void_p)(dll_callback_func)
data = CallbackData('F:/IMG_20230221_231631.jpg')
taskID = C.c_int32(1)
callbackDatas[taskID.value] = data
handle = WttCallback()
handle.cb = callbackFunc
dll.WriteToFile(b'F:/123.jpg', C.pointer(handle), C.pointer(taskID))
data.f.close()
