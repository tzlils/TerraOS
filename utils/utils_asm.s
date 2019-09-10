section .text
global fastcp, halt, pause

fastcp:
    push   esi
    push   edi
    mov    esp, edi
    mov    esp, esi
    mov    esp, ecx
    rep     movsb
    pop    edi
    pop    esi
    ret

halt:
    cli
    hlt

pause:
    hlt
    ret

sys_cli:
    hlt
    ret


sys_sti:
    hlt
    ret