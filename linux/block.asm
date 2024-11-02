.386
.model flat
.data
add_constant label word
	dd	1116352408
	dd	1899447441
	dd	-1245643825
	dd	-373957723
	dd	961987163
	dd	1508970993
	dd	-1841331548
	dd	-1424204075
	dd	-670586216
	dd	310598401
	dd	607225278
	dd	1426881987
	dd	1925078388
	dd	-2132889090
	dd	-1680079193
	dd	-1046744716
	dd	-459576895
	dd	-272742522
	dd	264347078
	dd	604807628
	dd	770255983
	dd	1249150122
	dd	1555081692
	dd	1996064986
	dd	-1740746414
	dd	-1473132947
	dd	-1341970488
	dd	-1084653625
	dd	-958395405
	dd	-710438585
	dd	113926993
	dd	338241895
	dd	666307205
	dd	773529912
	dd	1294757372
	dd	1396182291
	dd	1695183700
	dd	1986661051
	dd	-2117940946
	dd	-1838011259
	dd	-1564481375
	dd	-1474664885
	dd	-1035236496
	dd	-949202525
	dd	-778901479
	dd	-694614492
	dd	-200395387
	dd	275423344
	dd	430227734
	dd	506948616
	dd	659060556
	dd	883997877
	dd	958139571
	dd	1322822218
	dd	1537002063
	dd	1747873779
	dd	1955562222
	dd	2024104815
	dd	-2067236844
	dd	-1933114872
	dd	-1866530822
	dd	-1538233109
	dd	-1090935817
	dd	-965641998

	.code

fill proc
	push esi
	mov esi, eax
@@:
	cmp esi, edx
	je @F
	mov al, [ebx]
	mov [esi], al
	inc esi
	jmp @B
@@:
	pop esi
	ret
fill endp

hash256_block proc
	push ebx
	push esi
	push edi
	push ebp
	mov ebp, esp
	sub esp, 14ch
	xor eax, eax
	mov [ebp-4ch], ax
	mov [ebp-4ah], ax
	lea ebx, [ebp-4ch]
	lea edx, [ebp-4ch]
	lea eax, [ebp-14ch]
	call fill
	xor edi, edi
@L108:
	mov esi, edi
	shl si, 1
	shl si, 1
	mov ebx, [ebp+18h]
	add ebx, esi
	mov ah, [ebx]
	mov al, [ebx+1]
	xor dh, dh
	mov word ptr [ebp-40h], 0
	mov [ebp-3ah], ax
	mov al, [ebx+2]
	xor ah, ah
	xor dl, dl
	mov ecx, 8
@@:
	shl ax, 1
	rcl dx, 1
	loop @B
	mov cx, [ebp-40h]
	or cx, ax
	or dx, [ebp-3ah]
	mov al, [ebx+3]
	xor ah, ah
	or ax, cx
	mov [ebp+esi-14ch], ax
	mov [ebp+esi-14ah], dx
	inc edi
	cmp edi, 10h
	jb @L108
	mov word ptr [ebp-10h], 10h
@L110:
	movzx esi, word ptr [ebp-10h]
	shl si, 1
	shl si, 1
	mov ax, [ebp+esi-154h]
	mov [ebp-14h], ax
	mov ax, [ebp+esi-152h]
	mov [ebp-6], ax
	mov ax, [ebp-14h]
	mov dx, [ebp-6]
	mov ecx, 0fh
@@:
	shl ax, 1
	rcl dx, 1
	loop @B
	mov bx, [ebp-6]
	shr bx, 1
	or bx, ax
	mov [ebp-40h], bx
	mov ax, [ebp-14h]
	mov di, [ebp-6]
	mov ecx, 0dh
@@:
	shl ax, 1
	rcl di, 1
	loop @B
	mov cl, 3
	mov bx, [ebp-6]
	shr bx, cl
	or ax, bx
	mov bx, [ebp-40h]
	xor bx, ax
	xor di, dx
	mov ax, [ebp-14h]
	mov dx, [ebp-6]
	mov ecx, 0ah
@@:
	shr dx, 1
	rcr ax, 1
	loop @B
	xor ax, bx
	xor dx, di
	mov bx, [ebp+esi-168h]
	add bx, ax
	mov [ebp-38h], bx
	mov ax, [ebp+esi-166h]
	adc ax, dx
	mov [ebp-30h], ax
	mov bx, [ebp+esi-188h]
	mov di, [ebp+esi-186h]
	mov cl, 9
	mov ax, bx
	shl ax, cl
	mov [ebp-3ah], ax
	mov ax, bx
	mov dx, di
	mov ecx, 7
@@:
	shr dx, 1
	rcr ax, 1
	loop @B
	mov [ebp-3eh], ax
	mov ax, [ebp-3ah]
	or ax, dx
	mov [ebp-3ah], ax
	mov ax, bx
	mov dx, di
	mov ecx, 0eh
@@:
	shl ax, 1
	rcl dx, 1
	loop @B
	mov cx, di
	shr cx, 1
	shr cx, 1
	or ax, cx
	mov cx, [ebp-3eh]
	xor cx, ax
	mov [ebp-32h], cx
	mov ax, [ebp-3ah]
	xor ax, dx
	mov [ebp-2ch], ax
	mov ax, bx
	mov dx, di
	mov ecx, 3
@@:
	shr dx, 1
	rcr ax, 1
	loop @B
	xor ax, [ebp-32h]
	xor dx, [ebp-2ch]
	add ax, [ebp-38h]
	adc dx, [ebp-30h]
	add ax, [ebp+esi-18ch]
	adc dx, [ebp+esi-18ah]
	mov [ebp+esi-14ch], ax
	mov [ebp+esi-14ah], dx
	inc word ptr [ebp-10h]
	cmp word ptr [ebp-10h], 40h
	jae @F
	jmp @L110
@@:
	mov ebx, [ebp+14h]
	mov bx, [ebx]
	mov esi, [ebp+14h]
	mov ax, [esi+2]
	mov [ebp-2], ax
	mov ax, [esi+4]
	mov [ebp-0ah], ax
	mov ax, [esi+6]
	mov [ebp-0eh], ax
	mov ax, [esi+8]
	mov [ebp-8], ax
	mov ax, [esi+0ah]
	mov [ebp-0ch], ax
	mov ax, [esi+0ch]
	mov [ebp-24h], ax
	mov ax, [esi+0eh]
	mov [ebp-22h], ax
	mov si, [esi+10h]
	mov edi, [ebp+14h]
	mov ax, [edi+12h]
	mov [ebp-4], ax
	mov ax, [edi+14h]
	mov [ebp-1ch], ax
	mov ax, [edi+16h]
	mov [ebp-16h], ax
	mov ax, [edi+18h]
	mov [ebp-1ah], ax
	mov ax, [edi+1ah]
	mov [ebp-18h], ax
	mov dx, [edi+1ch]
	mov ax, [edi+1eh]
	mov word ptr [ebp-12h], 0
@L118:
	movzx ecx, word ptr [ebp-12h]
	mov cl, 0ah
	mov di, si
	shl di, cl
	mov [ebp-48h], si
	mov cx, [ebp-4]
	mov [ebp-46h], cx
	mov ecx, 6
@@:
	shr word ptr [ebp-46h], 1
	rcr word ptr [ebp-48h], 1
	loop @B
	mov cx, [ebp-48h]
	mov [ebp-3eh], cx
	mov cx, [ebp-46h]
	or cx, di
	mov [ebp-3ah], cx
	mov cl, 5
	mov di, si
	shl di, cl
	mov [ebp-48h], si
	mov cx, [ebp-4]
	mov [ebp-46h], cx
	mov ecx, 0bh
@@:
	shr word ptr [ebp-46h], 1
	rcr word ptr [ebp-48h], 1
	loop @B
	mov cx, [ebp-48h]
	mov [ebp-40h], cx
	mov cx, [ebp-46h]
	or cx, di
	mov di, [ebp-3eh]
	xor di, [ebp-40h]
	mov [ebp-36h], di
	mov di, [ebp-3ah]
	xor di, cx
	mov [ebp-2ah], di
	mov [ebp-48h], si
	mov cx, [ebp-4]
	mov [ebp-46h], cx
	mov ecx, 7
@@:
	shl word ptr [ebp-48h], 1
	rcl word ptr [ebp-46h], 1
	loop @B
	mov cl, 9
	mov di, [ebp-4]
	shr di, cl
	mov cx, di
	or cx, [ebp-48h]
	mov di, [ebp-36h]
	xor di, cx
	mov cx, [ebp-2ah]
	xor cx, [ebp-46h]
	add di, dx
	mov [ebp-40h], di
	mov dx, cx
	adc dx, ax
	mov ax, si
	not ax
	mov cx, [ebp-4]
	not cx
	mov di, [ebp-1ah]
	and di, ax
	mov ax, [ebp-18h]
	and ax, cx
	mov [ebp-3eh], ax
	mov cx, [ebp-1ch]
	and cx, si
	mov ax, [ebp-4]
	and ax, [ebp-16h]
	xor cx, di
	xor ax, [ebp-3eh]
	add cx, [ebp-40h]
	adc ax, dx
	movzx edi, word ptr [ebp-12h]
	shl di, 1
	shl di, 1
	mov dx, [edi+offset add_constant]
	add dx, cx
	adc ax, [edi+offset add_constant+2]
	mov cx, [ebp+edi-14ch]
	add cx, dx
	mov [ebp-20h], cx
	mov dx, [ebp+edi-14ah]
	adc dx, ax
	mov [ebp-1eh], dx
	mov cl, 0eh
	mov ax, bx
	shl ax, cl
	mov [ebp-3ah], ax
	mov ax, bx
	mov dx, [ebp-2]
	shr dx, 1
	rcr ax, 1
	shr dx, 1
	rcr ax, 1
	mov [ebp-40h], ax
	mov ax, [ebp-3ah]
	or ax, dx
	mov [ebp-3eh], ax
	mov cl, 3
	mov di, bx
	shl di, cl
	mov ax, bx
	mov dx, [ebp-2]
	mov ecx, 0dh
@@:
	shr dx, 1
	rcr ax, 1
	loop @B
	or dx, di
	mov cx, [ebp-40h]
	xor cx, ax
	mov [ebp-34h], cx
	mov ax, [ebp-3eh]
	xor ax, dx
	mov [ebp-2eh], ax
	mov ax, bx
	mov dx, [ebp-2]
	mov ecx, 0ah
@@:
	shl ax, 1
	rcl dx, 1
	loop @B
	mov cl, 6
	mov di, [ebp-2]
	shr di, cl
	or ax, di
	mov cx, [ebp-34h]
	xor cx, ax
	mov [ebp-44h], cx
	mov ax, [ebp-2eh]
	xor ax, dx
	mov [ebp-3ch], ax
	mov dx, [ebp-0ah]
	and dx, bx
	mov ax, [ebp-2]
	and ax, [ebp-0eh]
	mov [ebp-3eh], ax
	mov ax, [ebp-8]
	and ax, bx
	mov cx, [ebp-2]
	and cx, [ebp-0ch]
	xor dx, ax
	mov ax, [ebp-3eh]
	xor ax, cx
	mov [ebp-3eh], ax
	mov ax, [ebp-0ah]
	and ax, [ebp-8]
	mov cx, [ebp-0eh]
	and cx, [ebp-0ch]
	xor ax, dx
	xor cx, [ebp-3eh]
	mov di, [ebp-44h]
	add di, ax
	mov ax, [ebp-3ch]
	adc ax, cx
	mov [ebp-42h], ax
	mov dx, [ebp-1ah]
	mov ax, [ebp-18h]
	mov cx, [ebp-1ch]
	mov [ebp-1ah], cx
	mov cx, [ebp-16h]
	mov [ebp-18h], cx
	mov [ebp-1ch], si
	mov cx, [ebp-4]
	mov [ebp-16h], cx
	mov cx, [ebp-24h]
	add cx, [ebp-20h]
	mov [ebp-28h], cx
	mov cx, [ebp-22h]
	adc cx, [ebp-1eh]
	mov [ebp-26h], cx
	mov si, [ebp-28h]
	mov [ebp-4], cx
	mov cx, [ebp-8]
	mov [ebp-24h], cx
	mov cx, [ebp-0ch]
	mov [ebp-22h], cx
	mov cx, [ebp-0ah]
	mov [ebp-8], cx
	mov cx, [ebp-0eh]
	mov [ebp-0ch], cx
	mov [ebp-0ah], bx
	mov bx, [ebp-2]
	mov [ebp-0eh], bx
	add di, [ebp-20h]
	mov cx, [ebp-1eh]
	adc cx, [ebp-42h]
	mov bx, di
	mov [ebp-2], cx
	inc word ptr [ebp-12h]
	cmp word ptr [ebp-12h], 40h
	jae @F
	jmp @L118
@@:
	mov ebx, [ebp+14h]
	add [ebx], di
	adc [ebx+2], cx
	mov bx, [ebp-0ah]
	mov esi, [ebp+14h]
	add [esi+4], bx
	mov bx, [ebp-0eh]
	adc [esi+6], bx
	mov bx, [ebp-8]
	add [esi+8], bx
	mov bx, [ebp-0ch]
	adc [esi+0ah], bx
	mov bx, [ebp-24h]
	add [esi+0ch], bx
	mov bx, [ebp-22h]
	adc [esi+0eh], bx
	mov bx, [ebp-28h]
	add [esi+10h], bx
	mov bx, [ebp-26h]
	adc [esi+12h], bx
	mov bx, [ebp-1ch]
	add [esi+14h], bx
	mov bx, [ebp-16h]
	adc [esi+16h], bx
	mov bx, [ebp-1ah]
	add [esi+18h], bx
	mov bx, [ebp-18h]
	adc [esi+1ah], bx
	add [esi+1ch], dx
	adc [esi+1eh], ax
	xor eax, eax
@@:
	inc ax
	cmp ax, 8
	jb @B
	leave
	pop edi
	pop esi
	pop ebx
	ret
hash256_block endp

END
