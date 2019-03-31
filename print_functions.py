#!/usr/bin/python3.6

import subprocess
import json
import sys
import os

IDL_TO_JSON_EXECUTABLE = './idl_to_json'

def print_functions():
    try:
        # Make sure invoked properly
        if len(sys.argv) != 2:
            print("Wrong number of arguments", file=sys.stderr)
            raise Exception

        # Make sure IDL file exists and is readable
        filename = sys.argv[1]
        if (not os.path.isfile(filename)):
            print("Path %s does not designate a file" % filename, file=sys.stderr)
            raise Exception

        if (not os.access(filename, os.R_OK)):
            print("File %s is not readable" % filename, file=sys.stderr)
            raise Exception 

        # Make sure idl_to_json exists and is executable
        if (not os.path.isfile(IDL_TO_JSON_EXECUTABLE)):
            print("Path %s does not designate a file...run \"make\" to create it" % 
                IDL_TO_JSON_EXECUTABLE, file=sys.stderr)
            raise Exception
        if (not os.access(IDL_TO_JSON_EXECUTABLE, os.X_OK)):
            print("File %s exists but is not executable" % 
                IDL_TO_JSON_EXECUTABLE, file=sys.stderr)
            raise Exception

        # Parse declarations into a Python dictionary
        decls = json.loads(subprocess.check_output([IDL_TO_JSON_EXECUTABLE, filename]).decode('utf-8'))

        # Loop printing each function signature
        for  name, sig in decls["functions"].items():

            # Python Array of all args (each is a hash with keys "name" and "type")
            args = sig["arguments"]

            # Make a string of form:  "type1 arg1, type2 arg2" for use in function sig
            argstring = ', '.join([a["type"] + ' ' + a["name"] for a in args])

            # print the function signature
            print("%s %s(%s)" % (sig["return_type"], name, argstring))

    except Exception as e:
        print("Usage: %s <idlfilename>" % sys.argv[0], file=sys.stderr)
        print(e)

if __name__ == "__main__":
    print_functions()
