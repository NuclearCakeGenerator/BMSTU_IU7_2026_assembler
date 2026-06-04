%include "constants.inc"
%include "../chars.inc"
%include "../linux_sys.inc"

global print_exceeding_degree

section .text
print_exceeding_degree:
    ; the number is in rdi
    bsr rax, rdi
    shl rax, 1
    
    ret
    