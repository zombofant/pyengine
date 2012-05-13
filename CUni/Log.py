try:
    from _cuni_log import *
except ImportError:
    class Severity(object):
        class __metaclass__(type):
            def __getattr__(self, name):
                return name
    
    class server(object):
        @staticmethod
        def log(severity, message):
            pass

        @classmethod
        def getChannel(cls, name):
            return cls

