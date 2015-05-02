	ORG 800H
start:	PUSH R1
	PUSH R2
	PUSH R3
	OR R2,R2
	JP NZ,next
	MOV R0,R1
	JP ok
next:	XOR R0,R0
	DIV R2
	MOV R1,R2
	MOV R2,R0
	MOV R3,start
	CALL R3
ok:	POP R3
	POP R2
	POP R1
	RET
END start
