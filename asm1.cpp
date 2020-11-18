#include "stdio.h"
#include "string.h"

int main() {
    char input[] = "input string:\n";
    char result[] = "result:\n%s";
    char str_arr[256];

    __asm {
        lea     eax, input
        push    eax
        call    printf
        add     esp, 4

        lea     ebx, str_arr
        push    256
        push    ebx
        call    gets_s          ; scanf не учитывает пробелы, поэтому - gets_s
        add     esp, 8

        lea     ecx, str_arr
        push    ecx
        call    strlen
        add     esp, 4

        ; Распределим данные по регистрам :
        ; esi i
        ; edi - j
        ; ebx - str_arr
        ; al и eax - буферные элементы
        ; edx - n

        mov     edx, eax                ; n = strlen(str)
        mov     esi, edx                ; i = n - 1
        lea     ebx, str_arr
FOR_I:
        cmp     esi, 0                  ; i >= 0
        jl      FIN
        mov     al, [ebx][esi]
        cmp     al, ' '                 ; str[i] == ' '
        jne     NEXT_J
        mov     edi, esi                ; j = i
FOR_J:
        cmp     edi, edx                ; j < n
        jge     IF

        mov     al, [ebx][edi + 1]
        mov     [ebx][edi], al
        inc     edi                     ; j++
        jmp     FOR_J
IF:
        dec     edx                     ; n--
NEXT_J:
        dec     esi                     ; i--
        jmp     FOR_I
FIN:
        push    ebx
        lea     eax, result
        push    eax
        call    printf
        add     esp, 8
    }

    /* Эквивалентный код на си:
    printf("input string:\n");
    char str[256];
    gets_s(str, 256);
    int n = strlen(str);
    for (int i = n - 1; i >= 0; i--) {
        if (str[i] == ' ') {
            for (int j = i; j < n; j++) {
                str[j] = str[j + 1];
            }
            n--;
        }
    }
    printf("result:\n%s", str);*/
}