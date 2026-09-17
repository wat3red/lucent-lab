// functions.cpp

#include <oxorany/oxorany.h>
#include <xorstr.h>

#include "functions.h"
#include "utils.h"
#include "il2cpp/il2cpp_utils.h"
#include "signatures/signature_resolver.h"

#define RESOLVE_BY_OFFSET(return_type, name, offset, params) \
    name##_t name = nullptr;

#define RESOLVE_BY_NAME_AND_PARAMS(return_type, name, namespace, klass, methodName, params, paramsCount, ...) \
    name##_t name = nullptr;

#define RESOLVE_BY_NAME(return_type, name, namespace, klass, methodName, params) \
    name##_t name = nullptr;

#define RESOLVE_BY_SIGNATURE(return_type, name, params) \
    name##_t name = nullptr;

#include "il2cpp_api.h"
#include "functions_list.h"

#undef RESOLVE_BY_NAME_AND_PARAMS
#undef RESOLVE_BY_NAME
#undef RESOLVE_BY_SIGNATURE
#undef RESOLVE_BY_OFFSET

void InitFunctionPointers(uintptr_t baseAddress, size_t libSize) {
    SignatureResolver resolver(baseAddress, libSize);
    resolver.Init();

#define RESOLVE_BY_OFFSET(return_type, name, offset, params) \
    name = reinterpret_cast<name##_t>(g_LibUnityBase + offset);

#define RESOLVE_BY_NAME_AND_PARAMS(return_type, name, namespace, klass, methodName, params, paramsCount, ...) \
    name = reinterpret_cast<name##_t>(Il2CppUtils::FindMethod(Il2CppUtils::FindKlass(namespace, klass), methodName, paramsCount, name##_paramTypes)->methodPointer);

#define RESOLVE_BY_NAME(return_type, name, namespace, klass, methodName, params) \
    name = reinterpret_cast<name##_t>(Il2CppUtils::FindMethod(Il2CppUtils::FindKlass(namespace, klass), methodName)->methodPointer);

#define RESOLVE_BY_SIGNATURE(return_type, name, params) \
    name = reinterpret_cast<name##_t>(resolver.Get(#name));

#include "il2cpp_api.h"
#include "functions_list.h"

#undef RESOLVE_BY_OFFSET
#undef RESOLVE_BY_NAME_AND_PARAMS
#undef RESOLVE_BY_NAME
#undef RESOLVE_BY_SIGNATURE
}