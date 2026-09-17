#pragma once

#include <stdio.h>

#include "sdk/functions.h"

extern uintptr_t g_LibUnityBase;

#pragma pack(push, 1)

typedef void (*Il2CppMethodPointer)();

struct MethodInfo;

// struct VirtualInvokeData {
//     Il2CppMethodPointer methodPtr;
//     MethodInfo* method; // const
// };

typedef enum Il2CppTypeEnum
{
    IL2CPP_TYPE_END = 0x00,       /* End of List */
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0a,
    IL2CPP_TYPE_U8 = 0x0b,
    IL2CPP_TYPE_R4 = 0x0c,
    IL2CPP_TYPE_R8 = 0x0d,
    IL2CPP_TYPE_STRING = 0x0e,
    IL2CPP_TYPE_PTR = 0x0f,       /* arg: <type> token */
    IL2CPP_TYPE_BYREF = 0x10,       /* arg: <type> token */
    IL2CPP_TYPE_VALUETYPE = 0x11,       /* arg: <type> token */
    IL2CPP_TYPE_CLASS = 0x12,       /* arg: <type> token */
    IL2CPP_TYPE_VAR = 0x13,       /* Generic parameter in a generic type definition, represented as number (compressed unsigned integer) number */
    IL2CPP_TYPE_ARRAY = 0x14,       /* type, rank, boundsCount, bound1, loCount, lo1 */
    IL2CPP_TYPE_GENERICINST = 0x15,     /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1b,        /* arg: full method signature */
    IL2CPP_TYPE_OBJECT = 0x1c,
    IL2CPP_TYPE_SZARRAY = 0x1d,       /* 0-based one-dim-array */
    IL2CPP_TYPE_MVAR = 0x1e,       /* Generic parameter in a generic method definition, represented as number (compressed unsigned integer)  */
    IL2CPP_TYPE_CMOD_REQD = 0x1f,       /* arg: typedef or typeref token */
    IL2CPP_TYPE_CMOD_OPT = 0x20,       /* optional arg: typedef or typref token */
    IL2CPP_TYPE_INTERNAL = 0x21,       /* CLR internal type */

    IL2CPP_TYPE_MODIFIER = 0x40,       /* Or with the following types */
    IL2CPP_TYPE_SENTINEL = 0x41,       /* Sentinel for varargs method signature */
    IL2CPP_TYPE_PINNED = 0x45,       /* Local var that points to pinned object */

    IL2CPP_TYPE_ENUM = 0x55,        /* an enumeration */
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xff        /* an index into IL2CPP type metadata table */
} Il2CppTypeEnum;

typedef int32_t TypeDefinitionIndex;
typedef int32_t GenericParameterIndex;
struct Il2CppArrayType
{
    const Il2CppType* etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int* sizes;
    int* lobounds;
};

struct Il2CppGenericInst
{
    uint32_t type_argc;
    const Il2CppType** type_argv;
};

struct Il2CppGenericContext
{
    /* The instantiation corresponding to the class generic parameters */
    const Il2CppGenericInst* class_inst;
    /* The instantiation corresponding to the method generic parameters */
    const Il2CppGenericInst* method_inst;
};

struct Il2CppGenericClass
{
    const Il2CppType* type;        /* the generic type definition */
    Il2CppGenericContext context;  /* a context that contains the type instantiation doesn't contain any method instantiation */
    Il2CppClass* cached_class;     /* if present, the Il2CppClass corresponding to the instantiation.  */
};

struct Il2CppType {
    union
    {
        // We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union.
        void* dummy;
        TypeDefinitionIndex __klassIndex; /* for VALUETYPE and CLASS at startup */
        void* typeHandle; /* for VALUETYPE and CLASS at runtime */
        const Il2CppType* type;   /* for PTR and SZARRAY */
        Il2CppArrayType* array; /* for ARRAY */
        //MonoMethodSignature *method;
        GenericParameterIndex __genericParameterIndex; /* for VAR and MVAR at startup */
        void* genericParameterHandle; /* for VAR and MVAR at runtime */
        Il2CppGenericClass* generic_class; /* for GENERICINST */
    } data;
    unsigned int attrs : 16; /* param attributes or field flags */
    Il2CppTypeEnum type : 8;
    unsigned int num_mods : 5;  /* max 64 modifiers follow at the end */
    unsigned int byref : 1;
    unsigned int pinned : 1;  /* valid when included in a local var signature */
    unsigned int valuetype : 1;
};

struct Il2CppClass;

struct Il2CppObject {
    Il2CppClass* klass;
    void* monitor;
};

union Il2CppRGCTXData {
    void* rgctxDataDummy;
    const MethodInfo* method;
    Il2CppType* type;
    Il2CppClass* klass;
};

struct Il2CppRuntimeInterfaceOffsetPair {
    Il2CppClass* interfaceType;
    int32_t offset;
};

struct Il2CppClass_1 {
    void* image;                        // Pointer to the Il2CppImage this class belongs to. An image corresponds to an assembly.
    void* gc_desc;                      // Pointer to the garbage collector descriptor for this class. Used by the GC to understand memory layout.
    const char* name;                   // The name of the class.
    const char* namespaze;              // The namespace of the class.
    Il2CppType byval_arg;               // The Il2CppType representing this class when passed by value.
    Il2CppType this_arg;                // The Il2CppType representing this class when used as 'this' in a method.
    Il2CppClass* element_class;         // For array types, this points to the element class. For generic types, it might point to the generic definition.
    Il2CppClass* castClass;             // The class to which this class can be cast. For interfaces, this is the interface itself.
    Il2CppClass* declaringType;         // For nested types, this points to the enclosing type.
    Il2CppClass* parent;                // The parent class in the inheritance hierarchy.
    void* generic_class;                // For generic instances, this points to the Il2CppGenericClass representing the instantiation.
    void* typeMetadataHandle;           // Handle to the type metadata in the IL2CPP metadata blob.
    void* interopData;                  // Data used for interop scenarios (e.g., P/Invoke).
    Il2CppClass* klass;                 // Self-reference to the Il2CppClass itself (can be used for consistency checks or specific internal logic).
    void* fields;                       // Pointer to an array of Il2CppFieldInfo structures, describing the fields of the class.
    void* events;                       // Pointer to an array of Il2CppEventInfo structures, describing the events of the class.
    void* properties;                   // Pointer to an array of Il2CppPropertyInfo structures, describing the properties of the class.
    void* methods;                      // Pointer to an array of Il2CppMethodInfo structures, describing the methods of the class.
    Il2CppClass** nestedTypes;          // Array of pointers to Il2CppClass structures for nested types within this class.
    Il2CppClass** implementedInterfaces; // Array of pointers to Il2CppClass structures for interfaces implemented by this class.
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // Array of Il2CppRuntimeInterfaceOffsetPair structures, mapping interfaces to their vtable offsets.
};

struct Il2CppClass_2 {
    Il2CppClass** typeHierarchy;        // Array of pointers to Il2CppClass structures representing the class's inheritance hierarchy, from base to current.
    void* unity_user_data;              // Reserved for Unity's internal use.
    uint32_t initializationExceptionGCHandle; // Handle to a managed exception if class initialization (cctor) failed.
    uint32_t cctor_started;             // Flag indicating if the static constructor (cctor) has started.
    uint32_t cctor_finished;            // Flag indicating if the static constructor (cctor) has finished.
    size_t cctor_thread;                // The thread ID of the thread that is currently running or ran the static constructor.
    void* genericContainerHandle;       // Handle to the generic container definition for generic types.
    uint32_t instance_size;             // The size of an instance of this class (including inherited fields).
    uint32_t actualSize;                // The actual size of the type, including potential alignment padding.
    uint32_t element_size;              // For array types, the size of a single element.
    int32_t native_size;                // The size of the type when marshaled to native code.
    uint32_t static_fields_size;        // The total size of all static fields belonging to this class.
    uint32_t thread_static_fields_size; // The total size of all thread-static fields belonging to this class.
    int32_t thread_static_fields_offset; // The offset of thread-static fields within the thread-static data segment.
    uint32_t flags;                     // Flags describing various attributes of the class (e.g., abstract, sealed, public).
    uint32_t token;                     // The metadata token for this class in the IL2CPP metadata.
    uint16_t method_count;              // The number of methods defined in this class.
    uint16_t property_count;            // The number of properties defined in this class.
    uint16_t field_count;               // The number of fields defined in this class.
    uint16_t event_count;               // The number of events defined in this class.
    uint16_t nested_type_count;         // The number of nested types within this class.
    uint16_t vtable_count;              // The number of entries in the vtable for this class.
    uint16_t interfaces_count;          // The number of interfaces directly implemented by this class.
    uint16_t interface_offsets_count;   // The number of interface offset entries.
    uint8_t typeHierarchyDepth;         // The depth of this class in the type hierarchy (0 for System.Object).
    uint8_t genericRecursionDepth;      // The current depth of generic recursion for this type.
    uint8_t rank;                       // For array types, the number of dimensions.
    uint8_t minimumAlignment;           // The minimum alignment required for instances of this class.
    uint8_t naturalAligment;            // The natural alignment of the class fields.
    uint8_t packingSize;                // The packing size for fields within the class.
    uint8_t bitflags1;                  // Additional bit flags, used for various internal states or properties.
    uint8_t bitflags2;                  // More additional bit flags.
};

struct Il2CppClass {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    void* padding;  // what the heeeell
    VirtualInvokeData vtable[255];
};

typedef uintptr_t il2cpp_array_size_t;
typedef int32_t il2cpp_array_lower_bound_t;
struct Il2CppArrayBounds {
    il2cpp_array_size_t length;
    il2cpp_array_lower_bound_t lower_bound;
};

struct Il2CppException : Il2CppObject
{
    Il2CppString* className;
    Il2CppString* message;
    Il2CppObject* _data;
    Il2CppException* inner_ex;
    Il2CppString* _helpURL;
    Il2CppArray<>* trace_ips;
    Il2CppString* stack_trace;
    Il2CppString* remote_stack_trace;
    int remote_stack_index;
    Il2CppObject* _dynamicMethods;
    int32_t hresult;
    Il2CppString* source;
    Il2CppObject* safeSerializationManager;
    Il2CppArray<>* captured_traces;
    Il2CppArray<>* native_trace_ips;
    int32_t caught_in_unmanaged;
};

typedef void (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo*, void*, void**, void*);
struct MethodInfo {
    // Il2CppMethodPointer methodPointer;
    void* methodPointer;
    Il2CppMethodPointer virtualMethodPointer;
    InvokerMethod invoker_method;
    const char* name;
    Il2CppClass* klass;
    const Il2CppType* return_type;
    const Il2CppType** parameters;
    union {
        const Il2CppRGCTXData* rgctx_data;
        const void* methodMetadataHandle;
    };
    union {
        const void* genericMethod;
        const void* genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t bitflags;
};

// struct ICall
// {
//     const char* dll_name;
//     size_t dll_name_len;
//     const char* function_name;
//     size_t function_name_len;
//     size_t flags = 0x200000000;
//     // 8589934592

//     [[maybe_unused]]
//     ICall(const char* dll, const char* function)
//     {
//         dll_name = dll;
//         function_name = function;

//         dll_name_len = strlen(dll);
//         function_name_len = strlen(function);
//     }

//     void* ResolveICall()
//     {
//         return ((void* (*)(void*)) (g_LibUnityBase + 0x20EFA98))(this);
//     }

//     void* ResolveICallUnity()
//     {
//         return ((void* (*)(const char*)) (g_LibUnityBase + 0x20EF46C))(this->function_name);
//     }
// };

struct FieldInfo
{
    const char* name;
    const Il2CppType* type;
    Il2CppClass *parent;
    int32_t offset; // If offset is -1, then it's thread static
    uint32_t token;
};

#pragma pack(pop)