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
endl EQU <0dh,0ah>

;Time
startTime1 SYSTEMTIME <>
endTime1 SYSTEMTIME <>
startTime2 SYSTEMTIME <>
endTime2 SYSTEMTIME <>
startTime3 SYSTEMTIME <>
endTime3 SYSTEMTIME <>

;Title Screen
title_prompt BYTE "PRESS ANY KEY", 0
empty_prompt BYTE "             ", 0
title_state BYTE 1d
title_counter WORD 400d
title0_offset BYTE 10d
title_art0 BYTE "         _____      _____      _____    _______       _____    _____    _______ __         __ _______   _____           ", 0
title_art1 BYTE "        |  _  \    /     \    /     \  |___ ___|     |  _  \  |  __ \  |___ ___|\ \       / /|  _____| |  __ \          ", 0
title_art2 BYTE "        | | \  \  /   _   \  /   _   \    | |        | | \  \ | |  \ |    | |    \ \     / / | |       | |  \ \         ", 0
title_art3 BYTE "        | |  | |  |  / \  |  |  / \  |    | |        | |  | | | |__/ |    | |     \ \   / /  | |_____  | |__/ |         ", 0
title_art4 BYTE "        | |  | |  |  | |  |  |  | |  |    | |        | |  | | |  _  /     | |      \ \ / /   |  _____| |  _  /          ", 0
title_art5 BYTE "        | |  | |  |  \_/  |  |  \_/  |    | |        | |  | | | | \ \     | |       \ V /    | |       | | \ \          ", 0
title_art6 BYTE "        | |_/  /  \       /  \       /    | |        | |_/  / | |  \ \  __| |__      \ /     | |_____  | |  \ \         ", 0
title_art7 BYTE "        |_____/    \_____/    \_____/     |_|        |_____/  |_|   \_\|_______|      V      |_______| |_|   \_\        ", 0

;Stage
score_art00 BYTE "|----------------------------------------------------------------------------------------------------------------------|",
                 "|                                                                                                                      |",
                 "|    Lap: -/3     CheckPoint: -/8                 Lap 1: --:--:-----     Lap 2: --:--:-----     Lap 3: --:--:-----     |", 0
score_art01 BYTE "|                                                                                                                      |",
                 "|----------------------------------------------------------------------------------------------------------------------|", 0
stage_art00 BYTE "|         *     _______________________________________________________________________________________      #         |",
                 "|             @/                                                                                       \@              |",
                 "|            @/                                                                                         \@       #     |", 0
stage_art01 BYTE "|    #      @/                                                                                           \@            |",
                 "|          @/      ________________________________________________________________________________       \@           |",
                 "|      *  @/      /                                                                                \       \@          |", 0
stage_art02 BYTE "|        @/      /             *                                           *                #       \       \@     #   |",
                 "|       @/      /   #                     #                     #                                    \       \@        |",
                 "|      @/      /                                                                   *                  \       \@       |", 0
stage_art03 BYTE "|       |      |    #            #              #                                                     /       /@       |",
                 "|    #  |      |                                                              #           *          /       /@        |",
                 "|       |      |           ___________________________________                                      /       /@         |", 0
stage_art04 BYTE "|       |      |         @/                                   \                                    /       /@      #   |",
                 "|       |      |        @/                                     \__________________________________/       /@           |",
                 "|       |      |     # @/                                                                                /@            |", 0
stage_art05 BYTE "|    *  |      |      @/         ________________________                                               /@             |",
                 "|       |      |     @/         /      *                 \                                             /@              |",
                 "|       |      |    @/         /                          \___________________________________________/@               |", 0
stage_art06 BYTE "|   *   |      |   @/         /                      #                                                     #           |",
                 "|       |      |   @|        |                        *                                 #                              |",
                 "|       |      |   @\        \         #                                                                *              |", 0
stage_art07 BYTE "|   #   |======|    @\        \                                 #           #                                    #     |",
                 "|       |      |     @\        \______________________________                                   #                     |",
                 "|       |      |      @\                                      \                            *                           |", 0
stage_art08 BYTE "|  *    |      |       @\                                      \__________________________                  #          |",
                 "|       |      |        @\                                                                \@             *             |",
                 "|       |      |         @\_____________________________                                   \@                          |", 0
stage_art09 BYTE "|       |      |                                        \                                   \@                         |",
                 "|       |      |       #                 #               \____________________________       \@    #               #   |",
                 "|       |      |                                                                      \       \@            *          |", 0
stage_art10 BYTE "|       |      |              *                                     #                  \       \@                      |",
                 "|  #    |      |                              #                                 #       \       \@       ______________|",
                 "|       |      |                                     *                                   \       \@     |              |", 0
stage_art11 BYTE "|       |      |       #         #                                     *           *      \       \@    |   |---4      |",
                 "|      @\       \                         _________________________                       /       /@    |   |          |",
                 "|       @\       \        #              /                         \                     /       /@     |   |---3      |", 0
stage_art12 BYTE "|        @\       \               *     /                           \       #           /       /@      |   |          |",
                 "|         @\       \___________________/                             \_________________/       /@       |   |---2      |",
                 "|          @\                                   _____________                                 /@        |   |          |", 0
stage_art13 BYTE "|           @\                                 /             \                               /@    #    |   |---1      |",
                 "|            @\                               /               \                             /@          |   |          |",
                 "|       #     @\_____________________________/                 \___________________________/@           |   |---0      |", 0
stage_art14 BYTE "|                                                       #                                               |              |",
                 "|_______________________________________________________________________________________________________|______________|", 0

lap_counter BYTE 1d
check_counter BYTE 1d

;Input
input_char WORD ?
speed_rate BYTE 5
counter DWORD ?

;Player
player_x BYTE 11
player_y BYTE 28
player_char BYTE '|'
rot_state BYTE 0
speed_state BYTE 0

update_time DWORD 300 ;Game Loop Update

.code
main proc
;---------------------------------INIT--------------------------------
	; Get Max windows width & height
	call GetMaxXY
	mov maxRows, al
	mov maxCols, dl

	call set_speed
	call reset_color

;----------------------------TITLE-SCREEN-----------------------------
	;Draw Title Art
	mov dl, 0
	mov dh, 10
	call Gotoxy
	mov edx, OFFSET title_art0
	call WriteString
	mov edx, OFFSET title_art1
	call WriteString
	mov edx, OFFSET title_art2
	call WriteString
	mov edx, OFFSET title_art3
	call WriteString
	mov edx, OFFSET title_art4
	call WriteString
	mov edx, OFFSET title_art5
	call WriteString
	mov edx, OFFSET title_art6
	call WriteString
	mov edx, OFFSET title_art7
	call WriteString

TITLES:

	;Get any key
	call ReadKey ;Constantly reads input
	mov input_char, ax
	;Start the game
	cmp input_char, 1h
	jne START
	;Draw "PRESS ANY KEY"
	mov dl, 52
	mov dh, 33
	call Gotoxy
	cmp title_state, 0d
	mov edx, OFFSET title_prompt
	jne EMPTY
	mov edx, OFFSET empty_prompt
EMPTY:
	call WriteString
	;Animate "PRESS ANY KEY"
	dec title_counter
	cmp title_counter, 0d
	jne TSTATE
	mov title_counter, 5000d
	cmp title_state, 0d
	jne NSTATE
	mov title_state, 1d
	jmp TSTATE
NSTATE:
	mov title_state, 0d
TSTATE:
	jmp TITLES

START:
	call Clrscr
;------------------------------INIT-GAME------------------------------
	mov dl, 0
	mov dh, 0
	call Gotoxy
	
	;DRAW SCORE BOARD
	mov edx, OFFSET score_art00
	call WriteString
	mov edx, OFFSET score_art01
	call WriteString
	call draw_stage

	INVOKE GetLocalTime, ADDR startTime1

;------------------------------GAME-LOOP------------------------------
;Initial Game loop
Game_Loop:

;jmp DONE
	;DRAW PART OF STAGE IF PLAYER IS OVERLAPPING (DONT NEED TO REDRAW ENTIRE MAP)
	cmp speed_state, 0d
	je DONE
	cmp player_y, 7d
	jg R1
	mov dl, 0
	mov dh, 5d
	call Gotoxy
	mov edx, OFFSET stage_art00
	call WriteString
	mov edx, OFFSET stage_art01
	call WriteString
	jmp DONE 
R1:
	cmp player_y, 10d
	jg R2
	mov dl, 0
	mov dh, 5d
	call Gotoxy
	mov edx, OFFSET stage_art00
	call WriteString
	mov edx, OFFSET stage_art01
	call WriteString
	mov edx, OFFSET stage_art02
	call WriteString
	jmp DONE 
R2:
	cmp player_y, 13d
	jg R3
	mov dl, 0
	mov dh, 8d
	call Gotoxy
	mov edx, OFFSET stage_art01
	call WriteString
	mov edx, OFFSET stage_art02
	call WriteString
	mov edx, OFFSET stage_art03
	call WriteString
	jmp DONE 
R3:
	cmp player_y, 16d
	jg R4
	mov dl, 0
	mov dh, 11d
	call Gotoxy
	mov edx, OFFSET stage_art02
	call WriteString
	mov edx, OFFSET stage_art03
	call WriteString
	mov edx, OFFSET stage_art04
	call WriteString
	jmp DONE 
R4:
	cmp player_y, 19d
	jg R5
	mov dl, 0
	mov dh, 14d
	call Gotoxy
	mov edx, OFFSET stage_art03
	call WriteString
	mov edx, OFFSET stage_art04
	call WriteString
	mov edx, OFFSET stage_art05
	call WriteString
	jmp DONE 
R5:
	cmp player_y, 22d
	jg R6
	mov dl, 0
	mov dh, 17d
	call Gotoxy
	mov edx, OFFSET stage_art04
	call WriteString
	mov edx, OFFSET stage_art05
	call WriteString
	mov edx, OFFSET stage_art06
	call WriteString
	jmp DONE 
R6:
	cmp player_y, 25d
	jg R7
	mov dl, 0
	mov dh, 20d
	call Gotoxy
	mov edx, OFFSET stage_art05
	call WriteString
	mov edx, OFFSET stage_art06
	call WriteString
	mov edx, OFFSET stage_art07
	call WriteString
	jmp DONE 
R7:
	cmp player_y, 28d
	jg RR8
	mov dl, 0
	mov dh, 23d
	call Gotoxy
	mov edx, OFFSET stage_art06
	call WriteString
	mov edx, OFFSET stage_art07
	call WriteString
	mov edx, OFFSET stage_art08
	call WriteString
	jmp DONE 
RR8:
	cmp player_y, 31d
	jg RR9
	mov dl, 0
	mov dh, 26d
	call Gotoxy
	mov edx, OFFSET stage_art07
	call WriteString
	mov edx, OFFSET stage_art08
	call WriteString
	mov edx, OFFSET stage_art09
	call WriteString
	jmp DONE 
RR9:
	cmp player_y, 34d
	jg RR10
	mov dl, 0
	mov dh, 29d
	call Gotoxy
	mov edx, OFFSET stage_art08
	call WriteString
	mov edx, OFFSET stage_art09
	call WriteString
	mov edx, OFFSET stage_art10
	call WriteString
	jmp DONE 
RR10:
	cmp player_y, 37d
	jg RR11
	mov dl, 0
	mov dh, 32d
	call Gotoxy
	mov edx, OFFSET stage_art09
	call WriteString
	mov edx, OFFSET stage_art10
	call WriteString
	mov edx, OFFSET stage_art11
	call WriteString
	jmp DONE 
RR11:
	cmp player_y, 40d
	jg RR12
	mov dl, 0
	mov dh, 35d
	call Gotoxy
	mov edx, OFFSET stage_art10
	call WriteString
	mov edx, OFFSET stage_art11
	call WriteString
	mov edx, OFFSET stage_art12
	call WriteString
	jmp DONE 
RR12:
	cmp player_y, 43d
	jg RR13
	mov dl, 0
	mov dh, 38d
	call Gotoxy
	mov edx, OFFSET stage_art11
	call WriteString
	mov edx, OFFSET stage_art12
	call WriteString
	mov edx, OFFSET stage_art13
	call WriteString
	jmp DONE 
RR13:
	cmp player_y, 46d
	jg RR14
	mov dl, 0
	mov dh, 41d
	call Gotoxy
	mov edx, OFFSET stage_art12
	call WriteString
	mov edx, OFFSET stage_art13
	call WriteString
	mov edx, OFFSET stage_art14
	call WriteString
	jmp DONE 
RR14:
	cmp player_y, 49d
	jg DONE
	mov dl, 0
	mov dh, 44d
	call Gotoxy
	mov edx, OFFSET stage_art13
	call WriteString
	mov edx, OFFSET stage_art14
	call WriteString
DONE:

	;Set Color and Draw Player
	mov dl, player_x
	mov dh, player_y
	call Gotoxy
	mov eax, white + (cyan * 16)
	call SetTextColor
	mov al, player_char
	call WriteChar
	call reset_color

;---------------------------CHECKPOINT------------------------

	;Draw CheckPoint 1
	cmp check_counter, 1d
	jne CC1
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 61d ;x
	mov dh, 6d ;y
	mov ecx, 4d
	mov al, '|'
CCL1:
	call Gotoxy
	call WriteChar
	inc dh
	loop CCL1
	;Check if player inside checkpoint
	cmp player_x, 60d
	jl CC1
	cmp player_x, 61d
	jg CC1
	cmp player_y, 6d
	jl CC1
	cmp player_y, 10d
	jge CC1
	inc check_counter
	call draw_stage
	jmp DONE2
CC1:

	;Draw CheckPoint 2
	cmp check_counter, 2d
	jne CC2
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 103d ;x
	mov dh, 13d ;y
	mov ecx, 7d
	mov al, '-'
CCL2:
	call Gotoxy
	call WriteChar
	inc dl
	loop CCL2
	;Check if player inside checkpoint
	cmp player_y, 13d
	jne CC2
	cmp player_x, 103d
	jl CC2
	cmp player_x, 110d
	jge CC2
	inc check_counter
	call draw_stage
	jmp DONE2
CC2:

	;Draw CheckPoint 3
	cmp check_counter, 3d
	jne CC3
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 65d ;x
	mov dh, 19d ;y
	mov ecx, 4d
	mov al, '|'
CCL3:
	call Gotoxy
	call WriteChar
	inc dh
	loop CCL3
	;Check if player inside checkpoint
	cmp player_x, 64d
	jl CC3
	cmp player_x, 65d
	jg CC3
	cmp player_y, 19d
	jl CC3
	cmp player_y, 23d
	jge CC3
	inc check_counter
	call draw_stage
	jmp DONE2
CC3:

	;Draw CheckPoint 4
	cmp check_counter, 4d
	jne CC4
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 21d ;x
	mov dh, 24d ;y
	mov ecx, 8d
	mov al, '-'
CCL4:
	call Gotoxy
	call WriteChar
	inc dl
	loop CCL4
	;Check if player inside checkpoint
	cmp player_y, 24d
	jne CC4
	cmp player_x, 21d
	jl CC4
	cmp player_x, 29d
	jge CC4
	inc check_counter
	call draw_stage
	jmp DONE2
CC4:

	;Draw CheckPoint 5
	cmp check_counter, 5d
	jne CC5
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 65d ;x
	mov dh, 30d ;y
	mov ecx, 4d
	mov al, '|'
CCL5:
	call Gotoxy
	call WriteChar
	inc dh
	loop CCL5
	;Check if player inside checkpoint
	cmp player_x, 64d
	jl CC5
	cmp player_x, 65d
	jg CC5
	cmp player_y, 30d
	jl CC5
	cmp player_y, 34d
	jge CC5
	inc check_counter
	call draw_stage
	jmp DONE2
CC5:

	;Draw CheckPoint 6
	cmp check_counter, 6d
	jne CC6
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 91d ;x
	mov dh, 38d ;y
	mov ecx, 7d
	mov al, '-'
CCL6:
	call Gotoxy
	call WriteChar
	inc dl
	loop CCL6
	;Check if player inside checkpoint
	cmp player_y, 38d
	jne CC6
	cmp player_x, 91d
	jl CC6
	cmp player_x, 98d
	jge CC6
	inc check_counter
	call draw_stage
	jmp DONE2
CC6:

	;Draw CheckPoint 7
	cmp check_counter, 7d
	jne CC7
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 54d ;x
	mov dh, 40d ;y
	mov ecx, 4d
	mov al, '|'
CCL7:
	call Gotoxy
	call WriteChar
	inc dh
	loop CCL7
	;Check if player inside checkpoint
	cmp player_x, 54d
	jl CC7
	cmp player_x, 55d
	jg CC7
	cmp player_y, 40d
	jl CC7
	cmp player_y, 44d
	jge CC7
	inc check_counter
	call draw_stage
	jmp DONE2
CC7:

	;Draw CheckPoint 8
	cmp check_counter, 8d
	jne CC8
	;Draw checkpoint
	mov eax, white + (red * 16) ;Set color
	call SetTextColor
	mov dl, 9d ;x
	mov dh, 27d ;y
	mov ecx, 6d
	mov al, '-'
CCL8:
	call Gotoxy
	call WriteChar
	inc dl
	loop CCL8
	;Check if player inside checkpoint
	cmp player_y, 27d
	jne CC8
	cmp player_x, 9d
	jl CC8
	cmp player_x, 15d
	jge CC8
	mov check_counter, 1d
	;Update Time
	cmp lap_counter, 1d
	jne B1
	INVOKE GetLocalTime, ADDR endTime1
	INVOKE GetLocalTime, ADDR startTime2
B1:
	cmp lap_counter, 2d
	jne B2
	INVOKE GetLocalTime, ADDR endTime2
	INVOKE GetLocalTime, ADDR startTime3
B2:
	cmp lap_counter, 3d
	jne B3
	INVOKE GetLocalTime, ADDR endTime3
B3:
	inc lap_counter
	call draw_stage
	jmp DONE2
CC8:

DONE2:

	

;----------------------------INPUT----------------------------	
	;Read Input
	call ReadKey ;Constantly reads input
	mov input_char, ax

	;Move left
	mov ax, input_char
	sub ah, 1Eh
	jne NLEFT
	cmp rot_state, 0d
	;Below 0
	jne SKIP1
	mov rot_state, 7d
	jmp NLEFT
SKIP1:
	dec rot_state
NLEFT:

	;Move right
	mov ax, input_char
	sub ah, 20h
	jne NRIGHT
	inc rot_state
	cmp rot_state, 8d ;Below 360
	jl NRIGHT
	mov rot_state, 0d
NRIGHT:

	;Accelerate
	mov ax, input_char
	sub ah, 11h
	jne NFORWARD
	cmp speed_state, 4d
	je NFORWARD
	inc speed_state
	call reset_color
	call reset_graphic
	call set_speed
NFORWARD:

	;Decelerate
	mov ax, input_char
	sub ah, 1Fh
	jne NBACKWARDS
	cmp speed_state, 0d
	je NBACKWARDS
	dec speed_state
	call reset_color
	call reset_graphic
	call set_speed
NBACKWARDS:

	;Draw Current speed gear
	mov dl, 108
	mov dh, 46
	movzx cx, speed_state
L2:
	sub dh, 2
	loop L2
	call Gotoxy
	call reset_color
	mov al, '@'
	call WriteChar
	
	;Draw Lap counter
	mov dl, 10
	mov dh, 2
	call Gotoxy
	movzx eax, lap_counter
	call WriteDec

	;Draw Checkpoint counter
	mov dl, 30
	mov dh, 2
	call Gotoxy
	movzx eax, check_counter
	call WriteDec

	;Lap 1 Time
	cmp lap_counter, 2d
	jl A1
	mov dl, 57
	mov dh, 2
	call Gotoxy
	movzx eax, endTime1.wMinute
	sub ax, startTime1.WMinute
	call WriteDec
	add dl, 3
	call Gotoxy
	movzx eax, endTime1.wSecond
	sub ax, startTime1.wSecond
	call WriteDec
	add dl, 4
	call Gotoxy
	movzx eax, endTime1.wMilliseconds
	sub ax, startTime1.wMilliseconds
	call WriteDec
A1:

	;Lap 2 Time
	cmp lap_counter, 3d
	jl A2
	mov dl, 80
	mov dh, 2
	call Gotoxy
	movzx eax, endTime2.wMinute
	sub ax, startTime2.WMinute
	call WriteDec
	add dl, 3
	call Gotoxy
	movzx eax, endTime2.wSecond
	sub ax, startTime2.wSecond
	call WriteDec
	add dl, 4
	call Gotoxy
	movzx eax, endTime2.wMilliseconds
	sub ax, startTime2.wMilliseconds
	call WriteDec
A2:

	;Lap 2 Time
	cmp lap_counter, 4d
	jl A3
	mov dl, 103
	mov dh, 2
	call Gotoxy
	movzx eax, endTime3.wMinute
	sub ax, startTime3.WMinute
	call WriteDec
	add dl, 3
	call Gotoxy
	movzx eax, endTime3.wSecond
	sub ax, startTime3.wSecond
	call WriteDec
	add dl, 4
	call Gotoxy
	movzx eax, endTime3.wMilliseconds
	sub ax, startTime3.wMilliseconds
	call WriteDec
A3:

	;Update counter
	cmp speed_state, 0d ;Dont update if lowest gear
	je NMOVE
	cmp counter, 0d ;Move and reset counter
	jne NMOVE
	call set_speed

	;Move in rotation direction
	movzx eax, rot_state
	;N state
	cmp eax, 0d
	jne S0
	dec player_y
S0:
	;NE state
	cmp eax, 1d
	jne S1
	dec player_y
	inc player_x
S1:
	;E state
	cmp eax, 2d
	jne S2
	add player_x, 2
S2:
	;ES state
	cmp eax, 3d
	jne S3
	inc player_y
	inc player_x
S3:
	;S state
	cmp eax, 4d
	jne S4
	inc player_y
S4:
	;SW state
	cmp eax, 5d
	jne S5
	inc player_y
	dec player_x
S5:
	;W state
	cmp eax, 6d
	jne S6
	sub player_x, 2
S6:
	;WN state
	cmp eax, 7d
	jne S7
	dec player_y
	dec player_x
S7:
NMOVE:
	
	cmp speed_state, 0d
	je NINC
	dec counter
NINC:
	;Draw Car with rotation
	mov dl, player_x
	mov dh, player_y
	call Gotoxy
	mov player_char, '|' ;Default Symbol
	
	cmp rot_state, 0d
	jg NNN
	mov player_char, '|'
	jmp END1
NNN:
	cmp rot_state, 1d
	jg NNE
	mov player_char, '/'
	jmp END1
NNE:
	cmp rot_state, 2d
	jg NEE
	mov player_char, '-'
	jmp END1
NEE:
	cmp rot_state, 3d
	jg NES
	mov player_char, 5Ch
	jmp END1
NES:
	cmp rot_state, 4d
	jg NSS
	mov player_char, '|'
	jmp END1
NSS:
	cmp rot_state, 5d
	jg NWS
	mov player_char, '/'
	jmp END1
NWS:
	cmp rot_state, 6d
	jg NWW
	mov player_char, '-'
	jmp END1
NWW:
	cmp rot_state, 7d
	jg END1
	mov player_char, 5Ch
	jmp END1
END1:

	;OUT OF BOUNDS
	cmp player_y, 6d
	jge Y_LOWER_BOUND
	mov player_y, 6d
Y_LOWER_BOUND:
	cmp player_y, 47d
	jl Y_UPPER_BOUND
	mov player_y, 47d
Y_UPPER_BOUND:
	cmp player_x, 1d
	jge X_LOWER_BOUND
	mov player_x, 1d
X_LOWER_BOUND:
	cmp player_x, 118d
	jl X_UPPER_BOUND
	mov player_x, 118d
X_UPPER_BOUND:
	
	;Set Color and Draw Player
	mov eax, white + (cyan * 16)
	call SetTextColor
	mov al, player_char
	call WriteChar
	call reset_color

	cmp lap_counter, 3d
	jg GAMEOVER

	jmp Game_Loop

;---------------------GAME-OVER---------------------
GAMEOVER:
	mov eax, 10000
	call Delay
	call Clrscr
	invoke ExitProcess,0
main endp

;Reset graphics for gear box
reset_graphic proc
	mov dl, 108
	mov dh, 38
	mov ecx, 5
	mov al, '|'
L1:
	call Gotoxy
	call WriteChar
	add dh, 2
	loop L1
	ret
reset_graphic endp

;Set speed for set gear speed
set_speed proc
	cmp speed_state, 1d
	jne SS1
	mov counter, 800d
SS1:
	cmp speed_state, 2d
	jne SS2
	mov counter, 400d
SS2:
	cmp speed_state, 3d
	jne SS3
	mov counter, 200d
SS3:
	cmp speed_state, 4d
	jne SS4
	mov counter, 100d
SS4:
	ret
set_speed endp

;Reset Color to default
reset_color proc
	mov eax, white + (black * 16)
	call SetTextColor
	ret
reset_color endp

;Draw entire stage, not score board
draw_stage proc
	call reset_color
	mov dl, 0
	mov dh, 5
	call Gotoxy
	
	mov edx, OFFSET stage_art00
	call WriteString
	mov edx, OFFSET stage_art01
	call WriteString
	mov edx, OFFSET stage_art02
	call WriteString
	mov edx, OFFSET stage_art03
	call WriteString
	mov edx, OFFSET stage_art04
	call WriteString
	mov edx, OFFSET stage_art05
	call WriteString
	mov edx, OFFSET stage_art06
	call WriteString
	mov edx, OFFSET stage_art07
	call WriteString
	mov edx, OFFSET stage_art08
	call WriteString
	mov edx, OFFSET stage_art09
	call WriteString
	mov edx, OFFSET stage_art10
	call WriteString
	mov edx, OFFSET stage_art11
	call WriteString
	mov edx, OFFSET stage_art12
	call WriteString
	mov edx, OFFSET stage_art13
	call WriteString
	mov edx, OFFSET stage_art14
	call WriteString
	ret
draw_stage endp

end main