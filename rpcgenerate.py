#!/usr/bin/python3.6

import subprocess
import json
import sys
import os

TODO = "\t" + "//TODO\n"
OPEN = "{\n"
CLOSE = "}\n"

def setup():
    filename, decls = parse_idl()
    filename = filename.split(".")[0]
    write_header(filename, "proxy")
    write_header(filename, "stub")
    write_proxy_fn(filename, decls)
    write_stub_fn(filename, decls)

def parse_idl():
    IDL_TO_JSON_EXECUTABLE = './idl_to_json'
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
        return filename, decls
    except Exception as e:
        print("Usage: %s <idlfilename>" % sys.argv[0], file=sys.stderr)
        sys.exit()

"""
Writes header text to file, creating the file if
it doesn't exist, overwriting the file if it already
exists.
"""
def write_header(filename, stubOrProxy):
    header = "//"+filename+"."+stubOrProxy+".cpp\n//authors: Jialu Wei, Remmy Chen\n"
    includes = ("#include <string>\n"
                "using namespace std;\n"
                "#include \"" + filename + ".idl\"\n"
                "#include \"rpc" + stubOrProxy + "helper.h\"\n"
                "#include <cstdio>\n"
                "#include <cstring>\n"
                "#include \"c150debug.h\"\n"
                "#include <iostream>\n"
                "#include <vector>\n"
                "using namespace C150NETWORK;\n")
    defines = "#define INT_LEN 4\n#define FLOAT_LEN 4\n"
    structs = "struct Buffer_info " + OPEN + "\tchar *buf;\n\tint buf_len;\n" + "};\n"
    with open(filename+"."+stubOrProxy+".cpp", 'w+') as f:
        f.write('%s\n' % header)
        f.write('%s\n' % includes)
        f.write('%s\n' % defines)
        f.write('%s\n' % structs)
        f.close()

"""
Appends functions to proxy file.
3 types of functions:
    - remote calls 
    - deserializers (handlers)
    - serializers (pack)
"""
"""
retType = sig["return_type"]
serializers = writeSerializers()
f.write('%s\n' % serializers)
deserializers = TODO
f.write('%s\n' % deserializers)
"""
def write_proxy_fn(filename, decls):
    with open(filename+".proxy.cpp", 'a+') as f:
        typeSet = getTypeSet(decls)
        writeSerializersAndDeserializers(typeSet, f, "PROXY")
        for name, sig in decls["functions"].items():
            args = sig["arguments"]
            retType = sig["return_type"]
            writeFnRemoteCalls(name, args, retType, f)
    f.close()

def getTypeSet(decls):
    typeSet = set()
    typeSet.add("int") # always need int serializer and deserializer
    typeSet.add("string") # always need string serializer and deserializer
    for name, sig in decls["functions"].items():
        args = sig["arguments"]
        for arg in args:
            typeSet.add(arg["type"])
        typeSet.add(sig["return_type"])
    for name, sig in decls["types"].items():
        typeOfType = sig["type_of_type"]
        if typeOfType == "struct":
            typeSet.add(name)
            members = sig["members"]
            for member in members:
                typeSet.add(member["type"])
        elif typeOfType == "array":
            typeSet.add(name)
            typeSet.add(sig["member_type"])
    return typeSet


def writeSerializersAndDeserializers(typeSet, f, stubOrProxy):
    serializers = ""
    deserializers = ""
    for typeOfType in typeSet:
        if typeOfType == "int" or typeOfType == "string" or typeOfType == "float" or typeOfType == "void":
            serializer, serializerContract = genBuiltInSerializer(typeOfType)
            serializers += serializer
            f.write('%s\n' % serializerContract)
            deserializer, deserializerContract = genBuiltInDeserializer(typeOfType, stubOrProxy)
            deserializers += deserializer
            f.write('%s\n' % deserializerContract)
        elif typeOfType.startswith("__"):
            print("TODO")
        else:
            serializer = (typeOfType + " " + typeOfType + "_serializer() " + OPEN + 
                                "\t" + typeOfType + " s1;\n" + TODO + CLOSE)
            serializers += serializer
            print("TODO")
    f.write('%s\n' % serializers)
    f.write('%s\n' % deserializers)

def genBuiltInSerializer(name):
    serializer = ""
    serializerContract = ""
    if name == "int":
        serializer = ("void int_serializer(struct Buffer_info *b, int i) " + OPEN +
                        "\tint converted = htonl(i);\n"
                        "\tint new_buf_len;\n"
                        "\tchar *new_buf;\n"
                        "\tnew_buf_len = b->buf_len + INT_LEN;\n"
                        "\tnew_buf = (char*) malloc(new_buf_len);\n"
                        "\tmemcpy(new_buf, b->buf, b->buf_len);\n"
                        "\tmemcpy(new_buf+b->buf_len, (char*)&converted, INT_LEN);\n"
                        "\tfree(b->buf);\n"
                        "\tb->buf = new_buf;\n"
                        "\tb->buf_len = new_buf_len;\n" + CLOSE)
        serializerContract = "void int_serializer(struct Buffer_info *b, int i);\n"
    elif name == "string":
        serializer = ("void string_serializer(struct Buffer_info *b, string s) " + OPEN + 
                        "\tint_serializer(b, s.length());\n"
                        "\tint new_buf_len;\n"
                        "\tchar *new_buf;\n"
                        "\tnew_buf_len = b->buf_len + s.length();\n"
                        "\tnew_buf = (char*) malloc(new_buf_len);\n"
                        "\tmemcpy(new_buf, b->buf, b->buf_len);\n"
                        "\tmemcpy(new_buf+b->buf_len, s.c_str(), s.length());\n"
                        "\tfree(b->buf);\n"
                        "\tb->buf = new_buf;\n"
                        "\tb->buf_len = new_buf_len;\n" + CLOSE)
        serializerContract = "void string_serializer(struct Buffer_info *b, string s);\n"
    elif name == "float":
        serializer = ("void float_serializer(struct Buffer_info *b, float f) " + OPEN +
                        "\tstring s = to_string(f);\n"
                        "\tstring_serializer(b, s);\n" + CLOSE)
        serializerContract = "void float_serializer(struct Buffer_info *b, float f);\n"
    elif name == "void":
        serializer = "" # TODO : HOW TO HANDLE VOID
    return serializer, serializerContract

def genBuiltInDeserializer(name, stubOrProxy):
    deserializer = ""
    deserializerContract = ""
    if name == "int":
        deserializer = ("int int_deserializer() " + OPEN +
                        "\tchar buf[INT_LEN];\n"
                        "\tint res;\n"
                        "\tint readlen = RPC" + stubOrProxy + "SOCKET->read(buf, INT_LEN);\n"
                        "\tif (readlen == 0) return 0;\n"
                        "\tmemcpy(&res, buf, INT_LEN);\n"
                        "\treturn ntohl(res);\n" + CLOSE)
        deserializerContract = "int int_deserializer();\n"
    elif name == "string":
        deserializer = ("string string_deserializer() " + OPEN + 
                        "\tint str_len = int_deserializer();\n"
                        "\tchar buf[str_len];\n"
                        "\tint readlen = RPC" + stubOrProxy + "SOCKET->read(buf, str_len);\n"
                        "\tif (readlen == 0) return \"\";\n"
                        "\tstring res(buf, str_len);\n"
                        "\treturn res;\n" + CLOSE)
        deserializerContract = "string string_deserializer();\n"
    elif name == "float":
        deserializer = ("float float_deserializer() " + OPEN +
                        "\tstring s = string_deserializer();\n"
                        "\treturn atof(s.c_str());\n" + CLOSE)
        deserializerContract = "float float_deserializer();\n"
    elif name == "void":
        deserializer = "" # TODO : HOW TO HANDLE VOID
    return deserializer, deserializerContract

def writeFnRemoteCalls(name, args, retType, f):
    argstring = ', '.join([a["type"] + ' ' + a["name"] for a in args])
    serializeargs = ''.join([a["type"] + "_serializer(&b, " + a["name"] + ");\n\t" for a in args])
    fnbody = ("\tstruct Buffer_info b;\n"
                "\tb.buf = (char*) malloc(1);\n"
                "\tb.buf_len = 0;\n"
                "\tstring_serializer(&b, \"" + name + "\");\n"
                "\t" + serializeargs +
                "RPCPROXYSOCKET->write(b.buf, b.buf_len);\n"
                "\tfree(b.buf);\n"
                "\t" + retType + " res = " + retType + "_deserializer();\n"
                "\treturn res;\n")
    fnremotecall = retType + " " + name + "(" + argstring + ") " + OPEN + fnbody + CLOSE
    f.write('%s\n' % fnremotecall)


"""
Appends functions to stub file.
4 types of functions:
    - local calls 
    - deserializers (handlers)
    - serializers (pack)
    - dispatcher (invokes local function)
"""
def write_stub_fn(filename, decls):
    with open(filename+".stub.cpp", 'a+') as f:
        typeSet = getTypeSet(decls)
        writeSerializersAndDeserializers(typeSet, f, "STUB")
        dispatcherDeserializers = ""
        for name, sig in decls["functions"].items():
            args = sig["arguments"]
            retType = sig["return_type"]
            writeFnLocalCalls(name, args, retType, f)
            argnamestring = ', '.join([a["name"] for a in args])
            argDeserializers = ""
            for arg in args:
                argDeserializers += ("\t\t\t" + arg["type"] + " " + arg["name"] + " = " + arg["type"] + "_deserializer();\n")
            argDeserializers += "\t\t\t__" + name + "(" + argnamestring + ");\n"
            dispatcherDeserializers += ("\t\telse if (func_name.compare(\"" + name + "\")==0) " + OPEN + argDeserializers + "\t\t" + CLOSE)
        badfn = "void __badFunction(const char *functionName) " + OPEN + "\tchar doneBuffer[5] = \"BAD\";\n\tRPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);\n" + CLOSE
        f.write('%s\n' % badfn)
        dispatcherbody = ("\tif (!RPCSTUBSOCKET->eof()) " + OPEN +
                            "\t\tstring func_name = string_deserializer();\n"
                            "\t\tif (func_name.compare(\"\")==0) return;\n" + 
                            dispatcherDeserializers + 
                            "\t\telse " + OPEN + "\t\t\tprintf(\"BAD FUNCTION\");\n\t\t"  + CLOSE +
                            "\t" + CLOSE)
        dispatcher = "void dispatchFunction() " + OPEN + dispatcherbody + CLOSE
        f.write('%s\n' % dispatcher)
    f.close()

def writeFnLocalCalls(name, args, retType, f):
    argstring = ', '.join([a["type"] + ' ' + a["name"] for a in args])
    argnamestring = ', '.join([a["name"] for a in args])
    fncall = "\t" + retType + " res = " + name + "(" + argnamestring + ");\n"
    fnbody = ("\tstruct Buffer_info b;\n"
                "\tb.buf = (char*) malloc(1);\n"
                "\tb.buf_len = 0;\n"
                "\t" + retType + "_serializer(&b, res);\n"
                "\tRPCSTUBSOCKET->write(b.buf, b.buf_len);\n")
    fnlocalcall = "void __" + name + "(" + argstring + ") " + OPEN + fncall + fnbody + CLOSE
    f.write('%s\n' % fnlocalcall)

if __name__ == "__main__":
    setup()
    



