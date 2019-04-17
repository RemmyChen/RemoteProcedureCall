# RemoteProcedureCall
A Remote Procedure Call stub and proxy generator.

The rpcgenerate program was written in python and partially relies on the provided idl_to_json program to parse IDL files. 

It may handle function calls involving the following types: void, int, float, string, structs, and arrays.

For each function call, we start by sending the function name, which is a string. We send function arguments immediately after we send the function name. We took advantage of the fact that client and server share the secret of a common .idl file, and designed proxies and stubs in such a way that function calls and returns are done symmetrically. More specifically, in each proxy and stub, we include a serializer and a deserializer for each type mentioned in the JSON file. To be clear with our definitions, we define “Serializer for type X” to be a function that turns a value of type X into char* message, and “deserializer for type X” to be a function that turns a char* message back into the actual value. Our algorithm is that the “outer type” (e.g. float array[3][1]) serializer/deserializer delegates all its TCP I/O tasks to the “inner type” (e.g. float array[1]) serializer/deserializer. Similar relationship applies for struct and its fields. The hierarchy keeps going down until we reach the lowest layer -- the primitive types, where all the actual TCP I/Os are done. Since proxy and stub always do serialization and deserialization of function arguments in the same order, they will always agree on the order of fields and interpret corresponding values in the correct order.

Int:
<br/>Serializer: We send the integer after converting it to network order
<br/>Deserializer: As we know integers are always 4 bytes, we simply read in 4 bytes and convert it back to host order.

String:
<br/>Serializer: We send a 4-byte integer (using Int serializer) which represents the string length, followed by the actual string itself. 
<br/>Deserializer: We first read the string length (using Int deserializer). Then use that information to read in the actual string. 

Float:
<br/>Serializing: We turn the float into string and send the string over using String serializer. 
<br/>Deserializing: We read in the string using String deserializer, and then convert the string back to float using atof().

To compile our code:
<br/>Step 1: “make idl_to_json”
<br/>Step 2: “./rpcgenerate [name].idl”
<br/>Step 3: “make [name]client and “make [name]server”


Now the client and the server can be run on two windows ("./[name]server" on one window and "./[name]client [COMP117 machine name]"; NOTE: need Tufts COMP117 machines on same network to run).

