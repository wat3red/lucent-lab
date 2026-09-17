IFNDEF RAX
.model flat,C
ENDIF
.CODE
get_peb PROC
IFDEF RAX
	mov rax, qword ptr GS:[60h]
ELSE
	assume fs:nothing
	mov eax, dword ptr FS:[30h]
	assume fs:error
ENDIF
	ret
get_peb ENDP
END