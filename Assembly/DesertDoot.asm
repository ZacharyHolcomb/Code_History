;Edouard Gruyters and Zachary Holcomb

include Irvine32.inc

.386
.model flat,stdcall
.stack 4096
ExitProcess PROTO, dwExitCode:DWORD

.data
;Misc
maxRows BYTE ?
maxCols BYTE ?

;Gwaphics
bus1 BYTE "________________________________________________________________________________________________________________________",
		  "| _________________________     Departure Time              |                 Arrival Time          _________________  |",
		  "| |    Your Driver is:    |         =======         =================           =======             |  Next Stop:   |  |", 0
bus2 BYTE "| |      DOOT SLAYER      |         |00:00|         |###############|           |00:00|             | Las Vegas, AZ |  |",
		  "| |_______________________|         =======         =================           =======             |_______________|  |",
		  "|______________________________________________________________________________________________________________________|", 0
win  BYTE "|  |                                                                                                                |  |", 0
bus3 BYTE "|__|IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII|__|",
		  "|           ===================                                                                                        |",
	 	  "|           |        |        | ====                                                            _____________          |", 0
bus4 BYTE "|           |        |        | |67|  =========                                                |             |         |",
		  "|           |      =====      | ====  |0001995|             ====================               |_____________|         |",
		  "| _________ |======|   |======|       =========             | |   |   | |  | am|                \___________/          |", 0
bus5 BYTE "| | _---_ | |      =====      |         + + +               |88  90  92 | 94   |                                       |",
		  "| |-     -| |        |        |                             ====================                                       |",
		  "| |e  /  f| |        |        |                                                                                        |", 0
bus6 BYTE "| |_______| ===================                                                                                        |",
		  "|______________________________________________________________________________________________________________________|", 0

horizon BYTE "|  |________________________________________________________________________________________________________________|  |", 0

midstrip BYTE "||", 0
clrmidstrip BYTE "  ", 0

game_over1 BYTE "| YOU'RE  |", 0
game_over2 BYTE "|  FIRED  |", 0
game_over3 BYTE "|~~~~~~~~~|", 0

winner1 BYTE "| YOU'RE  |", 0
winner2 BYTE "|  WINNER |", 0
winner3 BYTE "|~~~~~~~~~|", 0

;Time
sysTime SYSTEMTIME <>
start_time DWORD ?

;Skip to end
konami BYTE 48h, 48h, 50h, 50h, 4Bh, 4Dh, 4Bh, 4Dh
k_count DWORD 0

;Input
input_char WORD ?
counter BYTE 0
timer WORD 0

;Player
player_x BYTE 58
player_y BYTE 18
player_char BYTE '@'
start_point_x BYTE 3
start_point_y BYTE 18
max_busview_x BYTE 112
max_busview_y BYTE 20
Max_X BYTE 120
Max_Y BYTE 50

update_time DWORD 300 ;Game Loop Update

.code
main proc
;---------------------------------INIT--------------------------------
	; Get Max windows width & height
	call GetMaxXY
	mov maxRows, al
	mov maxCols, dl

    ;Get time
	INVOKE GetLocalTime, ADDR sysTime
	call GetMseconds
	mov start_time, eax

	;Draw Bus Hud
	mov dh, 0
	mov dl, 0
	call Gotoxy
	mov edx, OFFSET bus1
	call WriteString
	mov edx, OFFSET bus2
	call WriteString
	mov ecx, 12
A1:	mov edx, OFFSET win
	call WriteString
	loop A1
	mov edx, OFFSET horizon
	call WriteString
	mov ecx, 20
A2:	mov edx, OFFSET win
	call WriteString
	loop A2
	mov edx, OFFSET bus3
	call WriteString
	mov edx, OFFSET bus4
	call WriteString
	mov edx, OFFSET bus5
	call WriteString
	mov edx, OFFSET bus6
	call WriteString

	;Departure Time
	mov dh, 3
	mov dl, 40
	movzx eax, sysTime.wMinute ;Minute
	cmp eax, 10
	jge Not_inc
	inc dl ;Incremnt x pos if single digit
Not_inc:
	call Gotoxy
	call WriteDec

	mov dh, 3
	mov dl, 37
	movzx eax, sysTime.wHour ;Hour
	cmp eax, 10
	jge Not_inc2
	inc dl ;Incremnt x pos if single digit
Not_inc2:
	call Gotoxy
	call WriteDec

	;Arrival Time
	mov dh, 3
	mov dl, 84
	movzx eax, sysTime.wMinute
	cmp eax, 10
	jge Not_inc3
	inc dl ;Incremnt x pos if single digit
Not_inc3:
	call Gotoxy
	call WriteDec

	mov dh, 3
	mov dl, 81
	movzx eax, sysTime.wHour
	add eax, 8d
	cmp eax, 24d ;Roll over if over 24
	jl Time
	sub eax, 24d
Time:
	cmp eax, 10
	jge Not_inc4
	inc dl ;Incremnt x pos if single digit
Not_inc4:
	call Gotoxy
	call WriteDec

;------------------------------GAME-LOOP------------------------------
;Initial Game loop
Game_Loop:
	
	;Clear Center Road Strip
	mov cl, 20d
C1:	
	mov dh, player_y
	add dh, cl ;Move y pos down with each iteration
	mov dl, player_x
	call Gotoxy
	mov edx, OFFSET clrmidstrip
	call WriteString
	loop C1

	;Clear Right Lane
	mov cl, 20d
R1:	
	mov dh, player_y
	add dh, cl ;Move y pos down with each iteration
	mov dl, player_x
	add dl, 20d
	add dl, cl ;Move x pos right with each iteration
	cmp dl, 116
	jge J1
	call Gotoxy
	mov al, ' '
	call WriteChar
J1:
	loop R1

	;Clear Left Lane
	mov cl, 20d
L1:	
	mov dh, player_y
	add dh, cl ;Move y pos down with each iteration
	mov dl, player_x
	sub dl, 20d
	sub dl, cl ;Move x pos left with each iteration
	cmp dl, 3
	jle J2
	call Gotoxy
	mov al, ' '
	call WriteChar
J2:
	loop L1

;----------------------------INPUT----------------------------
	;Read Input
	call ReadKey ;Constantly reads input
	mov input_char, ax

	;Move left
	mov ax, input_char
	sub ah, 1Eh
	jne NLEFT
	inc player_x
	jmp NOT_INPUT
NLEFT:

	;Move right
	mov ax, input_char
	sub ah, 20h
	jne NRIGHT
	dec player_x
	jmp NOT_INPUT
NRIGHT:

	;Auto move bus
	cmp counter, 20
	jl NOT_INPUT
	dec player_x
	mov counter, 0
NOT_INPUT:
	inc counter

	;Draw Center
	mov cl, 20d
C2:	
	mov dh, player_y
	add dh, cl ;Move y pos down with each iteration
	mov dl, player_x
	call Gotoxy
	mov edx, OFFSET midstrip
	call WriteString
	loop C2

	;Draw Right
	mov cl, 20d
	mov dh, player_y
	mov dl, player_x
	add dl, 20d
R2:
	inc dh ;Move x pos right with each iteration
	inc dl ;Move y pos down with each iteration
	cmp dl, 116
	jge J3
	call Gotoxy
	mov al, 5Ch
	call WriteChar
	loop R2
J3:

	;Draw Left
	mov cl, 20d
	mov dl, player_x
	mov dh, player_y
	sub dl, 20d
L2:	
	inc dh ;Move x pos left with each iteration
	dec dl ;Move y pos down with each iteration
	cmp dl, 3
	jle J4
	call Gotoxy
	mov al, 2Fh
	call WriteChar
	loop L2
J4:

	;Crash Condition
	cmp player_x, 18
	jl Game_Over
	cmp player_x, 100
	jg Game_Over

	;Win condition
	call Gotoxy
	call GetMseconds
	sub eax, start_time
	cmp eax, 28800000d
	jge winner

	;Konami Code
	mov ecx, k_count
	mov ax, input_char
	sub ax, 1h
	je SKIP_KONAMI
	mov eax, 0
	mov ah, konami[ecx]
	sub ax, input_char
	jne RESET_KONAMI
	inc k_count
	;Check if code is complete
	mov eax, k_count
	cmp eax, SIZEOF konami
	je Winner
	jmp SKIP_KONAMI
RESET_KONAMI:
	mov k_count, 0
SKIP_KONAMI:

	;mov eax, update_time
	;call Delay
	;call clr
	jmp Game_Loop

;---------------------------GAME-OVER--------------------------
Game_Over:
	mov dh, 44
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET game_over1
	call WriteString
	mov dh, 45
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET game_over2
	call WriteString
	mov dh, 46
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET game_over3
	call WriteString

	mov eax, 10000
	call Delay

	jmp Exit_loop

;---------------------------WINNER----------------------------
Winner:
	mov dh, 44
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET winner1
	call WriteString
	mov dh, 45
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET winner2
	call WriteString
	mov dh, 46
	mov dl, 97
	call Gotoxy
	mov edx, OFFSET winner3
	call WriteString

	mov eax, 10000
	call Delay

	jmp Exit_loop

Exit_loop:
	call Clrscr
	invoke ExitProcess,0

main endp

;Clear the screen from min_x, min_y to max_x, max_y
clr proc
	push eax
	push ebx
	push ecx
	push edx
	push edi

	movzx ecx, max_busview_x
L1:
	mov eax, ecx ;stores current outer loop counter 
	movzx ecx, max_busview_y ;sets up inner loop start point
l2:
	mov edx, ecx
	movzx edi, start_point_y
	add edx, edi
	
	mov ebx, eax
	movzx edi, start_point_x
	add ebx, edi
	
	push edx
	mov dh, dl
	mov dl, bl
	call Gotoxy
	pop edx

	push eax
	mov al, ' '
	call WriteChar
	pop eax

	loop L2 ;End of L2

	mov ecx, eax ;restores ecx to the outer loops current position
	loop L1;End of L1
	
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop edi
	ret
clr endp
end main