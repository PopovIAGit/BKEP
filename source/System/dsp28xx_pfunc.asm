
	.global		_PFUNC_blkRead
	.global		_PFUNC_blkWrite
	.global		_PFUNC_strRead
	.global		_PFUNC_strWrite
	.global		_PFUNC_wordRead
	.global		_PFUNC_wordWrite

	.sect		".text"

;=================================================================
_PFUNC_blkRead:
	MOVL		XAR7,XAR4

	DEC			@AL
	RPT			@AL
	||PREAD		*XAR5++,*XAR7

	LRETR

;=================================================================
_PFUNC_blkWrite:
	MOVL		XAR7,XAR4

	DEC			@AL
	RPT			@AL
	||PWRITE	*XAR7,*XAR5++

	LRETR

;=================================================================
_PFUNC_strRead:
	MOVL		XAR7,XAR4

L1:
	PREAD		*XAR5,*XAR7
	ADDB		XAR7,#1
	MOV       	AL,*XAR5++
	BF			L1,NEQ

	LRETR

;=================================================================
_PFUNC_strWrite:
	MOVL		XAR7,XAR4

L2:
	PWRITE		*XAR7,*XAR5
	ADDB		XAR7,#1
	MOV       	AL,*XAR5++
	BF			L2,NEQ

	LRETR

;=================================================================
_PFUNC_wordRead:
	MOVL		XAR7,XAR4

	PREAD		AL,*XAR7

	SPM			#0
	LRETR

;=================================================================
_PFUNC_wordWrite:
	MOVL		XAR7,XAR4

	PWRITE		*XAR7,AL

	LRETR

;=================================================================
	.end
