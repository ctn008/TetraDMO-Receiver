import ctypes

def stream_initialize(logfilepath):
    
    arg = ctypes.c_char_p(logfilepath.encode('utf-8'))
    result = arg

    if result:
        print("find_shim_error", result)

stream_initialize('stream_log.txt')
