#!/usr/bin/python3

import os
import sys

import grpc_tools
os.chdir(os.path.dirname(sys.argv[0]))


grpc_tools.main([sys.argv[0], "--python_out=../server",
                 "--grpc_python_out=../server", "-I.", "./game.proto"])
grpc_tools.main([sys.argv[0], "--python_out=../client",
                 "--grpc_python_out=../client", "-I.", "./game.proto"])
grpc_tools.main([sys.argv[0], "--python_out=../simple search",
                 "--grpc_python_out=../simple search", "-I.", "./game.proto"])
