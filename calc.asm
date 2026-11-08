[org 0x7c00]
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    in al, 0x61 
    and al, 0xFC 
    out 0x61, al

    mov si, msg_prompt1
    call print

    call read_digit
    mov bl, al          ; bl = premier nombre
    add al, '0'
    mov ah, 0x0e
    int 0x10            ; écho du chiffre lu

    mov si, msg_prompt2
    call print

    call read_operator
    mov cl, al          ; cl = l'opérateur ('+' ou '-')
    mov ah, 0x0e
    int 0x10

    mov si, msg_prompt3
    call print

    call read_digit
    mov bh, al          ; bh = second nombre
    add al, '0'
    mov ah, 0x0e
    int 0x10

    cmp cl, '+'
    je do_add
    cmp cl, '-'
    je do_sub
    cmp cl, '*'
    je do_mul
    jmp start           ; opérateur inconnu (ne devrait jamais arriver ici)

do_add:
    mov al, bl
    add al, bh
    mov dh, 0
    jmp show_result

do_sub:
    mov al, bl
    sub al, bh
    mov dh, 0
    jns show_result     ; si résultat >= 0, rien à faire 
    neg al              ; sinon on prend la valeur absolue
    mov dh, 1 
    jmp show_result

do_mul:
    mov al, bl
    mul bh
    mov dh, 0 
    jmp show_result

show_result:
    xor ah, ah
    mov dl, 10
    div dl              ; al = dizaines, ah = unités (division non signée)

    push ax
    mov si, msg_result
    call print
    pop ax

    cmp dh, 0
    je no_sign
    push ax
    mov al, '-'
    mov ah, 0x0e 
    int 0x10 
    pop ax 

no_sign:
    cmp al, 0 
    je skip_tens
    push ax
    add al, '0'
    mov ah, 0x0e 
    int 0x10
    pop ax

skip_tens:
    mov al, ah
    add al, '0'
    mov ah, 0x0e
    int 0x10

    mov ah, 0x00
    int 0x16            ; attend une touche avant de recommencer

    jmp start

; ---------- lecture d'un chiffre via scancode (indépendant du layout) ----------
read_digit:
    mov ah, 0x00
    int 0x16
    cmp ah, 0x0B
    je .d0
    cmp ah, 0x02
    je .d1
    cmp ah, 0x03
    je .d2
    cmp ah, 0x04
    je .d3
    cmp ah, 0x05
    je .d4
    cmp ah, 0x06
    je .d5
    cmp ah, 0x07
    je .d6
    cmp ah, 0x08
    je .d7
    cmp ah, 0x09
    je .d8
    cmp ah, 0x0A
    je .d9
    jmp read_digit      ; touche invalide -> on relit, silencieusement
.d0: mov al, 0
     ret
.d1: mov al, 1
     ret
.d2: mov al, 2
     ret
.d3: mov al, 3
     ret
.d4: mov al, 4
     ret
.d5: mov al, 5
     ret
.d6: mov al, 6
     ret
.d7: mov al, 7
     ret
.d8: mov al, 8
     ret
.d9: mov al, 9
     ret

; ---------- lecture de l'opérateur : touche "A" (0x10) = +, touche "S" (0x1F) = -, touche "M" (0x32) = * ----------
read_operator:
    mov ah, 0x00
    int 0x16
    cmp ah, 0x10
    je .plus
    cmp ah, 0x1F
    je .minus
    cmp ah, 0x32
    je .mul
    jmp read_operator
.plus:  mov al, '+'
        ret
.minus: mov al, '-'
        ret
.mul: mov al, '*'
        ret

print:
    lodsb
    or al, al
    jz print_done
    mov ah, 0x0e
    int 0x10
    jmp print
print_done:
    ret

msg_prompt1 db 13, 10, "Premier chiffre (rangee du haut, 0-9): ", 0
msg_prompt2 db 13, 10, "Operateur (touche pos.A=+, pos.S=-, pos.M=*): ", 0
msg_prompt3 db 13, 10, "Second chiffre: ", 0
msg_result  db 13, 10, "Resultat: ", 0

times 510-($-$$) db 0
dw 0xaa55
