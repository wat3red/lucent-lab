RESOLVE_BY_SIGNATURE(Il2CppString*, il2cpp_string_new, (const char* string))
RESOLVE_BY_SIGNATURE(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage* image, const char* namespaze, const char* name))
RESOLVE_BY_SIGNATURE(MethodInfo*, il2cpp_class_get_methods, (Il2CppClass* klass, void** iter))
RESOLVE_BY_SIGNATURE(Il2CppAssembly*, il2cpp_domain_assembly_open, (const char* name))
RESOLVE_BY_SIGNATURE(Il2CppDomain*, il2cpp_domain_get, ())
RESOLVE_BY_SIGNATURE(Il2CppThread*, il2cpp_thread_attach, (Il2CppDomain* domain))
RESOLVE_BY_SIGNATURE(Il2CppObject*, il2cpp_object_new, (Il2CppClass* klass))
// found it via oxorany("UnityEngine.Animator::StringToHash(System.String)") xrefs
RESOLVE_BY_SIGNATURE(void*, il2cpp_resolve_icall, (const char* sig))
RESOLVE_BY_SIGNATURE(uint32_t, il2cpp_gchandle_new, (Il2CppObject* obj, bool pinned))
RESOLVE_BY_SIGNATURE(Il2CppArray<>*, il2cpp_array_new, (Il2CppClass* elementTypeInfo, uintptr_t length))
RESOLVE_BY_SIGNATURE(Il2CppClass*, il2cpp_class_from_type, (const Il2CppType* type));
RESOLVE_BY_SIGNATURE(Il2CppClass*, il2cpp_runtime_invoke, (MethodInfo* method, void* obj, Il2CppObject** params, int paramCount, Il2CppException** exc));
RESOLVE_BY_SIGNATURE(FieldInfo*, il2cpp_class_get_field_from_name, (Il2CppClass* klass, const char* name));