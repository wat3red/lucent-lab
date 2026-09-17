#pragma once

//0x10 bytes (sizeof)
struct _LIST_ENTRY
{
    _LIST_ENTRY* Flink;                                              //0x0
    _LIST_ENTRY* Blink;                                              //0x8
};
//0x10 bytes (sizeof)
struct _UNICODE_STRING
{
    short Length;                                                          //0x0
    short MaximumLength;                                                   //0x2
    wchar_t* Buffer;                                                          //0x8
};

//0xa8 bytes (sizeof)
struct _LDR_DATA_TABLE_ENTRY
{
    _LIST_ENTRY InLoadOrderLinks;                                    //0x0
    _LIST_ENTRY InMemoryOrderLinks;                                  //0x8
    _LIST_ENTRY InInitializationOrderLinks;                          //0x10
    void* DllBase;                                                          //0x18
    void* EntryPoint;                                                       //0x1c
    long SizeOfImage;                                                      //0x20
    _UNICODE_STRING* FullDllName;                                     //0x24
    _UNICODE_STRING* BaseDllName;                                     //0x2c
    union un1;
    short ObsoleteLoadCount;                                               //0x38
    short TlsIndex;                                                        //0x3a
    struct _LIST_ENTRY HashLinks;                                           //0x3c
    long TimeDateStamp;                                                    //0x44
    struct _ACTIVATION_CONTEXT* EntryPointActivationContext;                //0x48
    void* Lock;                                                             //0x4c
    struct _LDR_DDAG_NODE* DdagNode;                                        //0x50
    struct _LIST_ENTRY NodeModuleLink;                                      //0x54
    struct _LDRP_LOAD_CONTEXT* LoadContext;                                 //0x5c
    void* ParentDllBase;                                                    //0x60
    void* SwitchBackContext;                                                //0x64
    void* BaseAddressIndexNode;                         //0x68
    void* MappingInfoIndexNode;                         //0x74
    long OriginalBase;                                                     //0x80
    char LoadTime[2];                                          //0x88
    long BaseNameHashValue;                                                //0x90
    enum _LDR_DLL_LOAD_REASON LoadReason;                                   //0x94
    long ImplicitPathOptions;                                              //0x98
    long ReferenceCount;                                                   //0x9c
    long DependentLoadFlags;                                               //0xa0
    char SigningLevel;                                                     //0xa4
};

//0x58 bytes (sizeof)
struct _PEB_LDR_DATA
{
    long Length;                                                           //0x0
    char Initialized;                                                      //0x4
    void* SsHandle;                                                         //0x8
    _LIST_ENTRY InLoadOrderModuleList;                               //0x10
    _LIST_ENTRY InMemoryOrderModuleList;                             //0x20
    _LIST_ENTRY InInitializationOrderModuleList;                     //0x30
    void* EntryInProgress;                                                  //0x40
    char ShutdownInProgress;                                               //0x48
    void* ShutdownThreadId;                                                 //0x50
};

#ifdef WIN32
//0x480 bytes (sizeof)
struct _PEB
{
    char InheritedAddressSpace;                                            //0x0
    char ReadImageFileExecOptions;                                         //0x1
    char BeingDebugged;                                                    //0x2
    union
    {
        char BitField;                                                     //0x3
        struct
        {
            char ImageUsesLargePages : 1;                                    //0x3
            char IsProtectedProcess : 1;                                     //0x3
            char IsImageDynamicallyRelocated : 1;                            //0x3
            char SkipPatchingUser32Forwarders : 1;                           //0x3
            char IsPackagedProcess : 1;                                      //0x3
            char IsAppContainer : 1;                                         //0x3
            char IsProtectedProcessLight : 1;                                //0x3
            char IsLongPathAwareProcess : 1;                                 //0x3
        };
    };
    void* Mutant;                                                           //0x4
    void* ImageBaseAddress;                                                 //0x8
    _PEB_LDR_DATA* Ldr;                                              //0xc
    /*struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                 //0x10
    void* SubSystemData;                                                    //0x14
    void* ProcessHeap;                                                      //0x18
    struct _RTL_CRITICAL_SECTION* FastPebLock;                              //0x1c
    union _SLIST_HEADER* volatile AtlThunkSListPtr;                         //0x20
    void* IFEOKey;                                                          //0x24
    union
    {
        unsigned long CrossProcessFlags;                                            //0x28
        struct
        {
            unsigned long ProcessInJob : 1;                                           //0x28
            unsigned long ProcessInitializing : 1;                                    //0x28
            unsigned long ProcessUsingVEH : 1;                                        //0x28
            unsigned long ProcessUsingVCH : 1;                                        //0x28
            unsigned long ProcessUsingFTH : 1;                                        //0x28
            unsigned long ProcessPreviouslyThrottled : 1;                             //0x28
            unsigned long ProcessCurrentlyThrottled : 1;                              //0x28
            unsigned long ProcessImagesHotPatched : 1;                                //0x28
            unsigned long ReservedBits0 : 24;                                         //0x28
        };
    };
    union
    {
        void* KernelCallbackTable;                                          //0x2c
        void* UserSharedInfoPtr;                                            //0x2c
    };
    unsigned long SystemReserved;                                                   //0x30
    union _SLIST_HEADER* volatile AtlThunkSListPtr32;                       //0x34
    void* ApiSetMap;                                                        //0x38
    unsigned long TlsExpansionCounter;                                              //0x3c
    void* TlsBitmap;                                                        //0x40
    unsigned long TlsBitmapBits[2];                                                 //0x44
    void* ReadOnlySharedMemoryBase;                                         //0x4c
    void* SharedData;                                                       //0x50
    void** ReadOnlyStaticServerData;                                        //0x54
    void* AnsiCodePageData;                                                 //0x58
    void* OemCodePageData;                                                  //0x5c
    void* UnicodeCaseTableData;                                             //0x60
    unsigned long NumberOfProcessors;                                               //0x64
    unsigned long NtGlobalFlag;                                                     //0x68
    union _LARGE_INTEGER CriticalSectionTimeout;                            //0x70
    unsigned long HeapSegmentReserve;                                               //0x78
    unsigned long HeapSegmentCommit;                                                //0x7c
    unsigned long HeapDeCommitTotalFreeThreshold;                                   //0x80
    unsigned long HeapDeCommitFreeBlockThreshold;                                   //0x84
    unsigned long NumberOfHeaps;                                                    //0x88
    unsigned long MaximumNumberOfHeaps;                                             //0x8c
    void** ProcessHeaps;                                                    //0x90
    void* GdiSharedHandleTable;                                             //0x94
    void* ProcessStarterHelper;                                             //0x98
    unsigned long GdiDCAttributeList;                                               //0x9c
    struct _RTL_CRITICAL_SECTION* LoaderLock;                               //0xa0
    unsigned long OSMajorVersion;                                                   //0xa4
    unsigned long OSMinorVersion;                                                   //0xa8
    unsigned short OSBuildNumber;                                                   //0xac
    unsigned short OSCSDVersion;                                                    //0xae
    unsigned long OSPlatformId;                                                     //0xb0
    unsigned long ImageSubsystem;                                                   //0xb4
    unsigned long ImageSubsystemMajorVersion;                                       //0xb8
    unsigned long ImageSubsystemMinorVersion;                                       //0xbc
    unsigned long ActiveProcessAffinityMask;                                        //0xc0
    unsigned long GdiHandleBuffer[34];                                              //0xc4
    void(*PostProcessInitRoutine)();                                       //0x14c
    void* TlsExpansionBitmap;                                               //0x150
    unsigned long TlsExpansionBitmapBits[32];                                       //0x154
    unsigned long SessionId;                                                        //0x1d4
    union _ULARGE_INTEGER AppCompatFlags;                                   //0x1d8
    union _ULARGE_INTEGER AppCompatFlagsUser;                               //0x1e0
    void* pShimData;                                                        //0x1e8
    void* AppCompatInfo;                                                    //0x1ec
    struct _UNICODE_STRING CSDVersion;                                      //0x1f0
    struct _ACTIVATION_CONTEXT_DATA* ActivationContextData;                 //0x1f8
    struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;                //0x1fc
    struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;    //0x200
    struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;                 //0x204
    unsigned long MinimumStackCommit;                                               //0x208
    void* SparePointers[4];                                                 //0x20c
    unsigned long SpareUlongs[5];                                                   //0x21c
    void* WerRegistrationData;                                              //0x230
    void* WerShipAssertPtr;                                                 //0x234
    void* pUnused;                                                          //0x238
    void* pImageHeaderHash;                                                 //0x23c
    union
    {
        unsigned long TracingFlags;                                                 //0x240
        struct
        {
            unsigned long HeapTracingEnabled : 1;                                     //0x240
            unsigned long CritSecTracingEnabled : 1;                                  //0x240
            unsigned long LibLoaderTracingEnabled : 1;                                //0x240
            unsigned long SpareTracingBits : 29;                                      //0x240
        };
    };
    unsigned long long CsrServerReadOnlySharedMemoryBase;                            //0x248
    unsigned long TppWorkerpListLock;                                               //0x250
    struct _LIST_ENTRY TppWorkerpList;                                      //0x254
    void* WaitOnAddressHashTable[128];                                      //0x25c
    void* TelemetryCoverageHeader;                                          //0x45c
    unsigned long CloudFileFlags;                                                   //0x460
    unsigned long CloudFileDiagFlags;                                               //0x464
    char PlaceholderCompatibilityMode;                                      //0x468
    char PlaceholderCompatibilityModeReserved[7];                           //0x469
    struct _LEAP_SECOND_DATA* LeapSecondData;                               //0x470
    union
    {
        unsigned long LeapSecondFlags;                                              //0x474
        struct
        {
            unsigned long SixtySecondEnabled : 1;                                     //0x474
            unsigned long Reserved : 31;                                              //0x474
        };
    };
    unsigned long NtGlobalFlag2;  
    */
};
#else
///0x478
struct _PEB
{
    char InheritedAddressSpace;                                            //0x0
    char ReadImageFileExecOptions;                                         //0x1
    char BeingDebugged;                                                    //0x2
    char union1;
    char Padding0[4];                                                      //0x4
    void* Mutant;                                                           //0x8
    void* ImageBaseAddress;                                                 //0x10
    struct _PEB_LDR_DATA* Ldr;                                              //0x18
    struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                 //0x20
    void* SubSystemData;                                                    //0x28
    void* ProcessHeap;                                                      //0x30
    struct _RTL_CRITICAL_SECTION* FastPebLock;                              //0x38
    union _SLIST_HEADER* volatile AtlThunkSListPtr;                         //0x40
    void* IFEOKey;                                                          //0x48
    char union2;
    char Padding1[4];                                                      //0x54
    union
    {
        void* KernelCallbackTable;                                          //0x58
        void* UserSharedInfoPtr;                                            //0x58
    };
    long SystemReserved;                                                   //0x60
    long AtlThunkSListPtr32;                                               //0x64
    void* ApiSetMap;                                                        //0x68
    long TlsExpansionCounter;                                              //0x70
    char Padding2[4];                                                      //0x74
    struct _RTL_BITMAP* TlsBitmap;                                          //0x78
    long TlsBitmapBits[2];                                                 //0x80
    void* ReadOnlySharedMemoryBase;                                         //0x88
    void* SharedData;                                                       //0x90
    void** ReadOnlyStaticServerData;                                        //0x98
    void* AnsiCodePageData;                                                 //0xa0
    void* OemCodePageData;                                                  //0xa8
    void* UnicodeCaseTableData;                                             //0xb0
    long NumberOfProcessors;                                               //0xb8
    long NtGlobalFlag;                                                     //0xbc
    union CriticalSectionTimeout;                            //0xc0
    long long HeapSegmentReserve;                                           //0xc8
    long long HeapSegmentCommit;                                            //0xd0
    long long HeapDeCommitTotalFreeThreshold;                               //0xd8
    long long HeapDeCommitFreeBlockThreshold;                               //0xe0
    long NumberOfHeaps;                                                    //0xe8
    long MaximumNumberOfHeaps;                                             //0xec
    void** ProcessHeaps;                                                    //0xf0
    void* GdiSharedHandleTable;                                             //0xf8
    void* ProcessStarterHelper;                                             //0x100
    long GdiDCAttributeList;                                               //0x108
    char Padding3[4];                                                      //0x10c
    struct _RTL_CRITICAL_SECTION* LoaderLock;                               //0x110
    long OSMajorVersion;                                                   //0x118
    long OSMinorVersion;                                                   //0x11c
    short OSBuildNumber;                                                   //0x120
    short OSCSDVersion;                                                    //0x122
    long OSPlatformId;                                                     //0x124
    long ImageSubsystem;                                                   //0x128
    long ImageSubsystemMajorVersion;                                       //0x12c
    long ImageSubsystemMinorVersion;                                       //0x130
    char Padding4[4];                                                      //0x134
    long long ActiveProcessAffinityMask;                                    //0x138
    long GdiHandleBuffer[60];                                              //0x140
    void(*PostProcessInitRoutine)();                                       //0x230
    struct _RTL_BITMAP* TlsExpansionBitmap;                                 //0x238
    long TlsExpansionBitmapBits[32];                                       //0x240
    long SessionId;                                                        //0x2c0
    char Padding5[4];                                                      //0x2c4
    union  AppCompatFlags;                                   //0x2c8
    union AppCompatFlagsUser;                               //0x2d0
    void* pShimData;                                                        //0x2d8
    void* AppCompatInfo;                                                    //0x2e0
    struct _CSDVersion;                                      //0x2e8
    struct _ACTIVATION_CONTEXT_DATA* ActivationContextData;                 //0x2f8
    struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;                //0x300
    struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;    //0x308
    struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;                 //0x310
    long long MinimumStackCommit;                                           //0x318
    void* SparePointers[2];                                                 //0x320
    void* PatchLoaderData;                                                  //0x330
    struct _CHPEV2_PROCESS_INFO* ChpeV2ProcessInfo;                         //0x338
    long AppModelFeatureState;                                             //0x340
    long Sparelongs[2];                                                   //0x344
    short ActiveCodePage;                                                  //0x34c
    short OemCodePage;                                                     //0x34e
    short UseCaseMapping;                                                  //0x350
    short UnusedNlsField;                                                  //0x352
    void* WerRegistrationData;                                              //0x358
    void* WerShipAssertPtr;                                                 //0x360
    void* EcCodeBitMap;                                                     //0x368
    void* pImageHeaderHash;                                                 //0x370
    union
    {
        long TracingFlags;                                                 //0x378
        struct
        {
            long HeapTracingEnabled : 1;                                     //0x378
            long CritSecTracingEnabled : 1;                                  //0x378
            long LibLoaderTracingEnabled : 1;                                //0x378
            long SpareTracingBits : 29;                                      //0x378
        };
    };
    char Padding6[4];                                                      //0x37c
    long long CsrServerReadOnlySharedMemoryBase;                            //0x380
    long long TppWorkerpListLock;                                           //0x388
    struct TppWorkerpList;                                      //0x390
    void* WaitOnAddressHashTable[128];                                      //0x3a0
    void* TelemetryCoverageHeader;                                          //0x7a0
    long CloudFileFlags;                                                   //0x7a8
    long CloudFileDiagFlags;                                               //0x7ac
    char PlaceholderCompatibilityMode;                                      //0x7b0
    char PlaceholderCompatibilityModeReserved[7];                           //0x7b1
    struct _LEAP_SECOND_DATA* LeapSecondData;                               //0x7b8
    union
    {
        long LeapSecondFlags;                                              //0x7c0
        struct
        {
            long SixtySecondEnabled : 1;                                     //0x7c0
            long Reserved : 31;                                              //0x7c0
        };
    };
    long NtGlobalFlag2;                                                    //0x7c4
    long long ExtendedFeatureDisableMask;                                   //0x7c8
};
#endif
//0x7d0 bytes (sizeof)

#ifdef WIN32
struct _LDR_DATA_ITEM
{
    _LIST_ENTRY list;
    char pad_0000[8];
    void* base;                                                          //0x18
    void* entry_point;                                                       //0x1c
    char pad_0018[16]; //0x0018
    wchar_t* mod_name; //0x0024
}; //Size: 0x0028
#else
struct _LDR_DATA_ITEM
{
    _LIST_ENTRY list;
    char pad_0000[16]; //0x0000
    void* base; //0x0020
    void* entry_point; //0x0018
    char pad_0028[32]; //0x0028
    wchar_t* mod_name; //0x0050
}; //Size: 0x0130*/
#endif

//0x40 bytes (sizeof)
struct _IMAGE_DOS_HEADER
{
    short e_magic;                                                         //0x0
    short e_cblp;                                                          //0x2
    short e_cp;                                                            //0x4
    short e_crlc;                                                          //0x6
    short e_cparhdr;                                                       //0x8
    short e_minalloc;                                                      //0xa
    short e_maxalloc;                                                      //0xc
    short e_ss;                                                            //0xe
    short e_sp;                                                            //0x10
    short e_csum;                                                          //0x12
    short e_ip;                                                            //0x14
    short e_cs;                                                            //0x16
    short e_lfarlc;                                                        //0x18
    short e_ovno;                                                          //0x1a
    short e_res[4];                                                        //0x1c
    short e_oemid;                                                         //0x24
    short e_oeminfo;                                                       //0x26
    short e_res2[10];                                                      //0x28
    long e_lfanew;                                                          //0x3c
};

//0x14 bytes (sizeof)
struct _IMAGE_FILE_HEADER
{
    short Machine;                                                         //0x0
    short NumberOfSections;                                                //0x2
    long TimeDateStamp;                                                    //0x4
    long PointerToSymbolTable;                                             //0x8
    long NumberOfSymbols;                                                  //0xc
    short SizeOfOptionalHeader;                                            //0x10
    short Characteristics;                                                 //0x12
};

//0x8 bytes (sizeof)
struct _IMAGE_DATA_DIRECTORY
{
    long VirtualAddress;                                                   //0x0
    long Size;                                                             //0x4
};

//0xf0 bytes (sizeof)
struct _IMAGE_OPTIONAL_HEADER64
{
    short Magic;                                                           //0x0
    char MajorLinkerVersion;                                               //0x2
    char MinorLinkerVersion;                                               //0x3
    long SizeOfCode;                                                       //0x4
    long SizeOfInitializedData;                                            //0x8
    long SizeOfUninitializedData;                                          //0xc
    long AddressOfEntryPoint;                                              //0x10
    long BaseOfCode;                                                       //0x14
    long long ImageBase;                                                    //0x18
    long SectionAlignment;                                                 //0x20
    long FileAlignment;                                                    //0x24
    short MajorOperatingSystemVersion;                                     //0x28
    short MinorOperatingSystemVersion;                                     //0x2a
    short MajorImageVersion;                                               //0x2c
    short MinorImageVersion;                                               //0x2e
    short MajorSubsystemVersion;                                           //0x30
    short MinorSubsystemVersion;                                           //0x32
    long Win32VersionValue;                                                //0x34
    long SizeOfImage;                                                      //0x38
    long SizeOfHeaders;                                                    //0x3c
    long CheckSum;                                                         //0x40
    short Subsystem;                                                       //0x44
    short DllCharacteristics;                                              //0x46
    long long SizeOfStackReserve;                                           //0x48
    long long SizeOfStackCommit;                                            //0x50
    long long SizeOfHeapReserve;                                            //0x58
    long long SizeOfHeapCommit;                                             //0x60
    long LoaderFlags;                                                      //0x68
    long NumberOfRvaAndSizes;                                              //0x6c
    _IMAGE_DATA_DIRECTORY DataDirectory[16];                         //0x70
};
//0xe0 bytes (sizeof)
struct _IMAGE_OPTIONAL_HEADER
{
    unsigned short Magic;                                                           //0x0
    unsigned char MajorLinkerVersion;                                               //0x2
    unsigned char MinorLinkerVersion;                                               //0x3
    unsigned long SizeOfCode;                                                       //0x4
    unsigned long SizeOfInitializedData;                                            //0x8
    unsigned long SizeOfUninitializedData;                                          //0xc
    unsigned long AddressOfEntryPoint;                                              //0x10
    unsigned long BaseOfCode;                                                       //0x14
    unsigned long BaseOfData;                                                       //0x18
    unsigned long ImageBase;                                                        //0x1c
    unsigned long SectionAlignment;                                                 //0x20
    unsigned long FileAlignment;                                                    //0x24
    unsigned short MajorOperatingSystemVersion;                                     //0x28
    unsigned short MinorOperatingSystemVersion;                                     //0x2a
    unsigned short MajorImageVersion;                                               //0x2c
    unsigned short MinorImageVersion;                                               //0x2e
    unsigned short MajorSubsystemVersion;                                           //0x30
    unsigned short MinorSubsystemVersion;                                           //0x32
    unsigned long Win32VersionValue;                                                //0x34
    unsigned long SizeOfImage;                                                      //0x38
    unsigned long SizeOfHeaders;                                                    //0x3c
    unsigned long CheckSum;                                                         //0x40
    unsigned short Subsystem;                                                       //0x44
    unsigned short DllCharacteristics;                                              //0x46
    unsigned long SizeOfStackReserve;                                               //0x48
    unsigned long SizeOfStackCommit;                                                //0x4c
    unsigned long SizeOfHeapReserve;                                                //0x50
    unsigned long SizeOfHeapCommit;                                                 //0x54
    unsigned long LoaderFlags;                                                      //0x58
    unsigned long NumberOfRvaAndSizes;                                              //0x5c
    _IMAGE_DATA_DIRECTORY DataDirectory[16];                         //0x60
}; 

struct _IMAGE_NT_HEADERS
{
    long Signature;                                                        //0x0
    struct _IMAGE_FILE_HEADER FileHeader;    //0x4
#ifdef WIN32
    struct _IMAGE_OPTIONAL_HEADER OptionalHeader;
#else
    struct _IMAGE_OPTIONAL_HEADER64 OptionalHeader;//0x18
#endif              
};

struct _IMAGE_EXPORT_DIRECTORY {
    int Characteristics;
    int TimeDateStamp;
    short MajorVersion;
    short MinorVersion;
    int Name;
    int Base;
    int NumberOfFunctions;
    int NumberOfNames;
    int AddressOfFunctions;
    int AddressOfNames;
    int AddressOfNameOrdinals;
};