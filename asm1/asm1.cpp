#include <stdio.h>
#include <string.h>

int main() {
    /*printf("input string:\n");
    char str[256];
    fgets(str, 256, stdin);
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

    char input[] = "input string:\n";
    char result[] = "result:\n%s";
    char str_arr[256];
    __asm {
        lea     eax, input
        push    eax
        call    dword ptr printf
        add     esp, 4

        lea     ebx, str_arr
        ; т.к. #define stdin(__acrt_iob_func(0))
        push    0
        call    __acrt_iob_func
        add     esp, 4
        push    eax
        push    256
        push    ebx
        call    fgets               ; scanf не учитывает пробелы, поэтому - fgets
        add     esp, 12

        lea     ecx, str_arr
        push    ecx
        call    strlen
        add     esp, 4

        ; Распределим данные по регистрам:
        ; esi i
        ; edi - j
        ; ebx - str_arr
        ; eax - буферный элемент
        ; edx - n

        mov     edx, eax                ; n = strlen(str)
        sub     edx, 1
        mov     esi, edx                ; i = n - 1
        lea     ebx, str_arr
FOR_I:
        cmp     esi, 0                  ; i >= 0
        jl      FIN
        movsx   eax, [ebx][esi]
        cmp     eax, 32                 ; str[i] == ' '
        jne     NEXT_J
        mov     edi, esi                ; j = i
FOR_J:
        cmp     edi, edx                ; j < n
        jge     NEXT_I

        ; str[j] = str[j + 1]
        mov     eax, [ebx][edi + 1]
        mov     [ebx][edi], eax
        inc     edi                     ; j++
        jmp     FOR_J
NEXT_I:
        sub     edx, 1
NEXT_J:
        dec     esi                     ; i--
        jmp     FOR_I
FIN:
        push    ebx
        lea     eax, result
        push    eax
        call    dword ptr printf
        add     esp, 8
    }
}