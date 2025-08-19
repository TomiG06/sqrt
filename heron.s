.intel_syntax noprefix

.section .data
half: .float 0.5

.section .text
.global heron
.global hwsqrt

.type heron, @function
.type hwsqrt, @function

heron:
    # prologue
    push    rbp
    mov     rbp, rsp

    push rcx

    xor rcx, rcx

    xorps     xmm1, xmm1      # guess -> xmm1
    movss     xmm2, [half]    # 0.5   -> xmm2
    xorps     xmm3, xmm3      # tmp   -> xmm3

    # guess = n / 2

    movss  xmm1, xmm0
    mulss   xmm1, xmm2

.loop_start:
    inc rcx

    # compute tmp = n / guess
    movss xmm3, xmm0
    divss xmm3, xmm1

    # guess_(i+1) = 0.5 * (guess_(i) + tmp)
    addss xmm1, xmm3
    mulss xmm1, xmm2

    # computing an error introduces overhead
    # and Heron's method actually converges fast enough
    cmp ecx, 12
    jl .loop_start

    # move final guess to result
    movss xmm0, xmm1

    # epilogue
    pop rcx
    mov     rsp, rbp
    pop     rbp
    ret

hwsqrt:
    # turns out modern CPUs implement sqrt by themselves
    # no frame needed
    sqrtss xmm0, xmm0
    ret

