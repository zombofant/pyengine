#!/usr/bin/python2
# encoding=utf8
# File name: py-universe.py
# This file is part of: pyuni
#
# LICENSE
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and limitations
# under the License.
#
# Alternatively, the contents of this file may be used under the terms
# of the GNU General Public license (the  "GPL License"), in which case
# the provisions of GPL License are applicable instead of those above.
#
# FEEDBACK & QUESTIONS
#
# For feedback and questions about pyuni please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
"""
Nothing yet.
"""

from __future__ import unicode_literals, print_function, division
from our_future import *

# global PyOpenGL flags MUST ONLY be set here.
import OpenGL
OpenGL.ERROR_ON_COPY = True

if __name__ == '__main__':
    import argparse
    import sys
    import CUni.Window
    from CUni.Log import server as log, Severity
    from Client.PythonicUniverse import PythonicUniverse

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p", "--profile",
        dest="profile",
        nargs="?",
        const=True,
        default=False,
        metavar="FILE",
        help="Run in cProfile mode. If FILE is given, the cProfile\
 output is sent to that file. If FILE is omitted and --profile-shell is\
 set, a default of /tmp/pyuniverse-profile is assumed."
    )
    parser.add_argument(
        "-s", "--profile-shell",
        dest="profileShell",
        action="store_true",
        help="In profiling mode, drop into a shell after the\
 application terminated. In that shell, the profiling data will be\
 available in a pstats variable called p."
    )
    parser.add_argument(
        "-f", "--frame-count",
        dest="profileFrames",
        type=int,
        default=0,
        metavar="N",
        help="Render exactly N frames and stop then."
    )
    args = parser.parse_args(sys.argv[1:])
    
    log.log(Severity.Information, "Python initialized")
    app = PythonicUniverse(CUni.Window.display)
    if args.profile:
        import cProfile
        log.log(Severity.Warning, "Running in cProfile mode")
        if args.profileShell and args.profile is True:
            log.log(Severity.Warning, "No output file defined but shell requested. Defaulting to /tmp/pyuniverse-profile")
            args.profile = "/tmp/pyuniverse-profile"
        if args.profileFrames:
            if args.profileFrames < 0:
                raise ValueError("Nice try.")
            app._eventLoop.setFrameCount(args.profileFrames)
        if args.profile is not True:
            log.log(Severity.Information, "cProfile output is going to file: {0}".format(args.profile))
            cProfile.run("app.run()", filename=args.profile)
        else:
            cProfile.run("app.run()")
        if args.profileShell:
            import code
            import pstats
            import readline
            p = pstats.Stats(args.profile)
            def topTimes(n=10):
                p.sort_stats("cum").print_stats(n)
            namespace = {}
            namespace["p"]          = p
            namespace["app"]        = app
            namespace["topTimes"]   = topTimes
            code.InteractiveConsole(namespace).interact("Profiling shell. Stats of the current run are available in the p object. Application state is available in the app object.")
    else:
        app.run()
