/****************************************************************************

    ���� intel.h

    ����������� ���������� �������� ������ ����������� ����������� IA32.

    ������ ���� �������������               �������� 2007

****************************************************************************/

#ifndef	_INTEL_H_
#define _INTEL_H_

typedef unsigned long   ULONG;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;

//*****************************************************************************
// ��������


#pragma pack(1)
typedef struct _Selector{			// ��������
    USHORT	RPL:2;
    USHORT	Tl:1;
    USHORT	Index:13;
} Selector, *PSelector;
#pragma pack()

#define TL_GLOBAL	0
#define TL_LOCAL	1



#pragma pack(1)
typedef struct _DescriptorCode {		// ���������� ����

    USHORT	LimitLow;				// ������ ����� ������ (0-15)
    USHORT	BaseLow;				// ������ ����� ���� (0-15)
    UCHAR	BaseMedium;				// ������� ����� ���� (16-23)
    union _ARB_CODE {				// ���� ���������� ��������
        
        UCHAR	SecurityByte;		// ���������� ����� �� ���� �����
                                    // ��������� �� ���� ����� � �����������
        struct	_SecurityByteDetail_CODE {
            UCHAR	A:1;			// ��������� � ��������
            UCHAR	R:1;			// ���������� (1) ��� ������ (0) ������ ��������
            UCHAR	C:1;			// ����������� (1) ��� ������������� (0)
            UCHAR	CodeOrData:1;	// ��� (1) ��� ������ (0)
            UCHAR	SystemOrUser:1;	// ��������� (0) ��� ���������������� (1)
            UCHAR	DPL:2;			// ������� ����������
            UCHAR	P:1;			// ����������� � ������
            };

        };
    UCHAR	LimitHigh:4;			// ������� ����� ������� (16-19)
    UCHAR	AVL:1;
    UCHAR	Zero:1;
    UCHAR	D:1;					// ����������� �� ���������: 16(0) ��� 32(1)
    UCHAR	G:1;					// ��� �������������: �����(0) ��� ��������(1)
    UCHAR	BaseHigh;				// ������ ����� ���� (24-31)

} DescriptorCode, *PDescriptorCode;
#pragma pack()



#pragma pack(1)
typedef struct _DescriptorData {		// ���������� ������ � �����

    USHORT	LimitLow;				// ������ ����� ������ (0-15)
    USHORT	BaseLow;				// ������ ����� ���� (0-15)
    UCHAR	BaseMedium;				// ������� ����� ���� (16-23)
    union _ARB_DATA {						// ���� ���������� ��������
        
        UCHAR	SecurityByte;		// ���������� ����� �� ���� �����
                                    // ��������� �� ���� ����� � �����������
        struct	_SecurityByteDetail_DATA {
            UCHAR	A:1;			// ��������� � ��������
            UCHAR	W:1;			// ���������� (1) ��� ������ (0) ������
            UCHAR	E:1;			// ���������� ����� (0) ��� ���������� ���� (1)
            UCHAR	CodeOrData:1;	// ��� (1) ��� ������ (0)
            UCHAR	SystemOrUser:1;	// ��������� (0) ��� ���������������� (1)
            UCHAR	DPL:2;			// ������� ����������
            UCHAR	P:1;			// ����������� � ������
            };

        };

    UCHAR	LimitHigh:4;			// ������� ����� ������� (16-19)
    UCHAR	AVL:1;
    UCHAR	Zero:1;
    UCHAR	B:1;					// ����������� ������� �������(+��������� �����): 16(0) ��� 32(1)
    UCHAR	G:1;					// ��� �������������
    UCHAR	BaseHigh;				// ������ ����� ���� (24-31)

} DescriptorData, *PDescriptorData;
#pragma pack()



#pragma pack(1)
typedef struct _DescriptorSystem {	// ��������� ���������� (LDT, TSS)

    USHORT	LimitLow;				// ������ ����� ������ (0-15)
    USHORT	BaseLow;				// ������ ����� ���� (0-15)
    UCHAR	BaseMedium;				// ������� ����� ���� (16-23)
    union _ARB_SYSTEM {						// ���� ���������� ��������
        
        UCHAR	SecurityByte;		// ���������� ����� �� ���� �����
                                    // ��������� �� ���� ����� � �����������
        struct	_SecurityByteDetail_SYSTEM {
            UCHAR	Type:4;			// ��� ���������� �����������
            UCHAR	SystemOrUser:1;	// ��������� (0) ��� ���������������� (1)
            UCHAR	DPL:2;			// ������� ����������
            UCHAR	P:1;			// ����������� � ������
            };

        };

    UCHAR	LimitHigh:4;			// ������� ����� ������� (16-19)
    UCHAR	Zero:3;
    UCHAR	G:1;					// ��� �������������
    UCHAR	BaseHigh;				// ������ ����� ���� (24-31)

} DescriptorSystem, *PDescriptorSystem;
#pragma pack()



#pragma pack(1)
typedef struct _DescriptorGate {		// ���������� �����

    USHORT	DestinationOffsetLow;	// ����� ���������� (0-15)
    USHORT	DestinationSelector;	// �������� ���������� (0-15)
    UCHAR	WordCount;				// (5 ���) ����� ����������(����) ������(������ ��� ������ ������)
    union _ARB_GATE {						// ���� ���������� ��������
        
        UCHAR	SecurityByte;		// ���������� ����� �� ���� �����
                                    // ��������� �� ���� ����� � �����������
        struct	_SecurityByteDetail_GATE {
            UCHAR	Type:4;			// ��� ���������� �����������
            UCHAR	SystemOrUser:1;	// ��������� (0) ��� ���������������� (1)
            UCHAR	DPL:2;			// ������� ����������
            UCHAR	P:1;			// ����������� � ������
            };

        };

    USHORT	DestinationOffsetHigh;	// ����� ���������� (16-31)

} DescriptorGate, *PDescriptorGate;
#pragma pack()



#pragma pack(1)
typedef struct Descriptor {			// ����������

    union {
        struct {
            USHORT	LimitLow;				// ������ ����� ������ (0-15)
            USHORT	BaseLow;				// ������ ����� ���� (0-15)
            UCHAR	BaseMedium;				// ������� ����� ���� (16-23)
            union {				// ���� ���������� ��������
                
                UCHAR	SecurityByte;		// ��������� ����� �� ���� �����
                                            // ��������� �� ���� ����� � �����������
                struct {
                    UCHAR   NA:4;
                    UCHAR	SystemOrUser:1;	// ��������� (0) ��� ���������������� (1)
                    UCHAR	DPL:2;			// ������� ����������
                    UCHAR	P:1;			// ����������� � ������
                    } SecurityByteDetail;

                };
            UCHAR	LimitAndBits;			// ������� ����� ������� (16-19) � ���� ����������
            UCHAR	BaseHigh;				// ������ ����� ���� (24-31)
            };
        DescriptorCode Code;
        DescriptorData Data;
        DescriptorSystem System;
        DescriptorGate Gate;
        };

} Descriptor, *PDescriptor;
#pragma pack()



// ������� ���� ������������

#define	CODE_OR_DATA	1	// �������� ���� CodeOrData ��� �������� ����
#define DATA_OR_CODE	0	// �������� ���� CodeOrData ��� �������� ������
#define SYSTEM_OR_USER	0	// �������� ���� SystemOrUser ��� ���������� ��������
#define	USER_OR_SYSTEM	1	// �������� ���� SystemOrUser ��� ����������������� ��������
#define	C_COMF			1	// �������� ���� � ��� ������������ �������� ����
#define C_NOCOMF		0	// �������� ���� � ��� �������������� �������� ����
#define R_READ			1	// ������ �������� ���� ���������
#define R_NOREAD		0	// ������ �������� ���� ���������
#define	W_WRITE			1	// ������ � ������� ������ ���������
#define W_NOWRITE		0	// ������ � ������� ������ ���������
#define E_STACK			1	// ���������� ���� ��� �����
#define E_DATA			0	// ���������� ����� ��� ������
#define PRESENT			1<<7


// ���� ������

#define	TYPE_CALLGATE_286		4
#define	TYPE_TASKGATE			5
#define	TYPE_INTERRUPTGATE_286	6
#define	TYPE_TRAPGATE_286		7
#define	TYPE_CALLGATE_386		12
#define TYPE_GATE_RESERVED		13
#define	TYPE_INTERRUPTGATE		14
#define	TYPE_TRAPGATE_386		15


// ���� ��������� ������������

#define	TYPE_AVAILABLE_TSS_286	1	// ��������� TSS-286
#define	TYPE_LDT				2	// LDT
#define	TYPE_BUZY_TSS_286		3	// ������� TSS-286
#define	TYPE_AVAILABLE_TSS_386	9	// ��������� TSS-386
#define	TYPE_RESERVED			10	// ���������������
#define	TYPE_BUZY_TSS_386		11	// ������� TSS-386


// ��������� ��� SecurityByte

#define	DATA_SEGMENT_READ_0		(USER_OR_SYSTEM<<4) | PRESENT
#define DATA_SEGMENT_READ_3		DATA_SEGMENT_READ_0 | (3<<5)
#define DATA_SEGMENT_WRITE_0	(USER_OR_SYSTEM<<4) | (W_WRITE<<1) | PRESENT	
#define DATA_SEGMENT_WRITE_3	DATA_SEGMENT_WRITE_0 | (3<<5)
#define STACK_SEGMENT_0			(USER_OR_SYSTEM<<4) | (E_STACK<<2) | PRESENT
#define STACK_SEGMENT_3			STACK_SEGMENT_0 | (3<<5)
#define	CODE_SEGMENT_NOREAD_0	(USER_OR_SYSTEM<<4) | (CODE_OR_DATA<<3) | PRESENT
#define CODE_SEGMENT_NOREAD_3	CODE_SEGMENT_NOREAD_0 | (3<<5)
#define CODE_SEGMENT_READ_0		(USER_OR_SYSTEM<<4) | (CODE_OR_DATA<<3) | (R_READ<<1) | PRESENT
#define CODE_SEGMENT_READ_3		CODE_SEGMENT_READ_0 | (3<<5)
#define	SYSTEM_SEGMENT_0		PRESENT
#define SYSTEM_SEGMENT_3		SYSTEM_SEGMENT_0 | (3<<5)



#pragma pack(1)
typedef struct _TSS {			// ������ TSS

USHORT	Link;			// 0h
USHORT	Null1;			// 2h
ULONG	ESP0;			// 4h
USHORT	SS0;			// 8h
USHORT	Null2;			// 0Ah
ULONG	ESP1;			// 0Ch
USHORT	SS1;			// 10h
USHORT	Null3;			// 12h
ULONG	ESP2;			// 14h
USHORT	SS2;			// 18h
USHORT	Null4;			// 1Ah

ULONG	CR3;			// 1Ch
ULONG	EIP;			// 20h
ULONG	EFLAGS;			// 24h
ULONG	EAX;			// 28h
ULONG	ECX;			// 2Ch
ULONG	EDX;			// 30h
ULONG	EBX;			// 34h
ULONG	ESP;			// 38h
ULONG	EBP;			// 3Ch
ULONG	ESI;			// 40h
ULONG	EDI;			// 44h

USHORT	ES;				// 48h
USHORT	Null5;			// 4Ah
USHORT	CS;				// 4Ch
USHORT	Null6;			// 4Eh
USHORT	SS;				// 50h
USHORT	Null7;			// 52h
USHORT	DS;				// 54h
USHORT	Null8;			// 56h
USHORT	FS;				// 58h
USHORT	Null9;			// 5Ah
USHORT	GS;				// 5Ch
USHORT	Null10;			// 5Eh
USHORT	LDTR;			// 60h
USHORT	Null11;			// 62h

USHORT	Null12;			// 64h
USHORT	BitMapOffset;	// 66h
} TSS,*PTSS;
#pragma pack()


//*****************************************************************************
// ��������


#pragma pack(1)
typedef struct _PTE {

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ��� ���������� ������������
    ULONG PCD:1;			// ��� ���������� ������������
    ULONG A:1;				// ��� ������� � ��������
    ULONG D:1;				// ��� ��������� ��������
    ULONG PATi:1;			// ��� ���������� ������������ ��������
    ULONG G:1;				// ��� ������������ ��������
    ULONG Used:3;			// ��������
    ULONG Address:20;		// ���������� ����� ��������(������� 20 ���)

} PTE, *PPTE;
#pragma pack()



#pragma pack(1)
typedef struct _PDE {

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� ������� � ��������
    ULONG Zero1:1;			// ��� D
    ULONG PS:1;				// ��� ���������� ������� ��������:4�(0) ��� 2/4�(1)
    ULONG G:1;				// ��� ������������
    ULONG Used:3;
    ULONG Address:20;		// ���������� ����� ������� �������

} PDE, *PPDE;
#pragma pack()


/*
                    PDE

        31              12 11        9  8   7   6   5   4   3   2   1   0
        ___________________________________________________________________
        |                 |   |   |   |   |   |   |   | P | P | U | R |   |
        | ����� �������   |  �������� | G | P | 0 | A | C | W | / | / | P |
        |                 |           |   | S |   |   | D | T | S | W |   |  
        -------------------------------------------------------------------
        

                    PTE

        31              12 11        9  8   7   6   5   4   3   2   1   0
        ___________________________________________________________________
        |                 |   |   |   |   | P |   |   | P | P | U | R |   |
        | ����� ��������  |  �������� | G | A | D | A | C | W | / | / | P |
        |                 |           |   | Ti|   |   | D | T | S | W |   |  
        -------------------------------------------------------------------

*/



#pragma pack(1)
typedef struct _PSE32 {		// 4� �������� PSE 32-���������

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� �������
    ULONG D:1;				// ��� ������
    ULONG PS:1;				// ��� ���������� ������� ��������:4�(0) ��� 4�(1)
    ULONG G:1;				// ��� ������������
    ULONG Used:3;
    ULONG PATi:1;
    ULONG Zero:9;
    ULONG Address:20;		// ���������� ����� �������� �������

} PSE32, *PPSE32;
#pragma pack()



#pragma pack(1)
typedef struct _PSE36 {		// 4� �������� PSE 36-���������

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� �������
    ULONG D:1;				// ��� ������
    ULONG PS:1;				// ��� ���������� ������� ��������:4�(0) ��� 4�(1)
    ULONG G:1;				// ��� ������������
    ULONG Used:3;
    ULONG PATi:1;
    ULONG HighAddress:4;	// ������� 4 ���� ������ (33-36)
    ULONG Zero:5;
    ULONG Address:20;		// ���������� ����� �������� �������

} PSE36, *PPSE36;
#pragma pack()



/*
                    PSE-32

        31              22 21    17 16    13  12 11        9  8   7   6   5   4   3   2   1   0
        _________________________________________________________________________________________
        |                 |                 | P |   |   |   |   | PC|   |   | P | P | U | R |   |
        | ����� �������   |      ������     | A |  �������� | G | = | D | A | C | W | / | / | P |
        |   (PA-1)        |                 | Ti|           |   | 1 |   |   | D | T | S | W |   |  
        -----------------------------------------------------------------------------------------
        

                    PSE-36

        31              22 21    17 16    13  12 11        9  8   7   6   5   4   3   2   1   0
        _________________________________________________________________________________________
        |                 |        |        | P |   |   |   |   | PC|   |   | P | P | U | R |   |
        | ����� �������   | ������ | �����  | A |  �������� | G | = | D | A | C | W | / | / | P |
        |   (PA-1)        |        | (PA-2) | Ti|           |   | 1 |   |   | D | T | S | W |   |  
        -----------------------------------------------------------------------------------------

*/



#pragma pack(1)
typedef struct _PAEPDPTE {		// ��������� �� ������� �������

    ULONG P:1;
    ULONG Zero1:2;
    ULONG PWT:1;
    ULONG PCD:1;
    ULONG Zero2:4;
    ULONG Used:3;
    ULONG Address:20;
    ULONG HighAddress:4;
    ULONG Zero3:28;

} PAEPDPTE, *PPAEPDPTE;
#pragma pack()



#pragma pack(1)
typedef struct _PAEPDE4K {	// 4k PDE �������� PAE

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� �������
    ULONG D:1;				// ��� ������
    ULONG PS:1;				// ��� ���������� ������� ��������:4�(0) ��� 2/4�(1)
    ULONG Zero1:1;			
    ULONG Used:3;			// ��������
    ULONG Address:20;		// ���������� ����� �������� �������
    ULONG HighAddress:4;	// ������� 4 ���� ������ (33-36)
    ULONG Zero:28;

} PAEPDE4K, *PPAEPDE4K;
#pragma pack()



#pragma pack(1)
typedef struct _PAEPDE2M {	// 2M PDE �������� PAE

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� �������
    ULONG D:1;				// ��� ������
    ULONG PS:1;				// ��� ���������� ������� ��������:4�(0) ��� 2/4�(1)
    ULONG G:1;			
    ULONG Used:3;			// ��������
    ULONG PATi:1;			// ��� ���������� ������������
    ULONG Zero2:8;
    ULONG Address:11;		// ���������� ����� �������� �������
    ULONG HighAddress:4;	// ������� 4 ���� ������ (33-36)
    ULONG Zero:28;

} PAEPDE2M, *PPAEPDE2M;
#pragma pack()



#pragma pack(1)
typedef struct _PAEPTE4K {	// 4k PTE �������� PAE

    ULONG P:1;				// ��� �����������
    ULONG R_W:1;			// ��� ���������� ������
    ULONG U_S:1;			// ��� ���������� ������� ������������
    ULONG PWT:1;			// ���� ���������� ������������
    ULONG PCD:1;
    ULONG A:1;				// ��� �������
    ULONG D:1;				// ��� ������
    ULONG PATi:1;				// ��� ���������� ������� ��������:4�(0) ��� 2/4�(1)
    ULONG G:1;			
    ULONG Used:3;			// ��������
    ULONG Address:20;		// ���������� ����� �������� �������
    ULONG HighAddress:4;	// ������� 4 ���� ������ (33-36)
    ULONG Zero:28;

} PAEPTE4K, *PPAEPTE4K;
#pragma pack()


//*****************************************************************************
// ��������


#pragma pack(1)
typedef struct _CR0 {		// ������� CR0
    
    ULONG PE:1;				// ���������� ������(������� � ���������� �����)
    ULONG MP:1;				// ���������� ������������
    ULONG EM:1;				// �������� ������������
    ULONG TS:1;				// ���� ������������ ������, ��������������� �������������
    ULONG ET:1;				// ��������� ��������� ���������� ���.������������
    ULONG NE:1;				// ���������� ��������� �� ������ FPU ����� ��������� ����������
    ULONG Zero1:10;
    ULONG WP:1;				// ���������� ������ ������� ������
    ULONG Zero2:1;
    ULONG AM:1;				// ���������� �������� ������������
    ULONG Zero3:10;
    ULONG NW:1;				// ������ �������� ������ ���� � ������ ������������
    ULONG CD:1;				// ������ ���������� ����
    ULONG PG:1;				// ��������� ���������� ������������� ������
    
} CR0, *PCR0;
#pragma pack()



#pragma pack(1)
typedef struct _CR4 {	// ������� CR4

    ULONG VME:1;		// ���������� ������������� ������������ ����� ���������� � ������ V86
    ULONG PVI:1;		// ���������� ������������� ������������ ����� ���������� � ���������� ������
    ULONG TSD:1;		// ����������� ���������� RDTSC � �����������������
    ULONG DE:1;			// ���������� ������������ �������(����� �������� �� ��������� � �������� ������)
    ULONG PSE:1;		// ���������� ������� ��������
    ULONG PAE:1;		// ���������� ����������� ������
    ULONG MCE:1;		// ���������� ��������� ��������
    ULONG PGE:1;		// ���������� ������������ � ���������� �������������
    ULONG PCE:1;		// ���������� ��������� � ��������� ������� (RDPMC) �� ����� ������ ����������
    ULONG OSFXSR:1;		// ���� ������������� ���������� FXSAVE/FXRSTOR	��� �������� ���������� � �������������� ��������� FPU/MMX
    ULONG OSXMMEXCPT:1;	// ���� ��������� �� ���������� �� ����� XMM
    ULONG Zero:21;

} CR4, *PCR4;
#pragma pack()



/*

            CR0

        0              7 8            15 16           23 24            31
        +_______+_______+_______+_______+_______+_______+_______+_______+
        |P|M|E|T|E|N| | | | | | | | | | |W| |A| | | | | | | | | | |N|C|P| 
        |E|P|M|S|T|E| | | | | | | | | | |P| |M| | | | | | | | | | |W|D|G|
        +-------+-------+-------+-------+-------+-------+-------+-------+
        \--------------\/--------------/
                      MSW


            CR4

        0              7 8            15 16           23 24            31
        +_______+_______+_______+_______+_______+_______+_______+_______+
        | | | | | | | | | | |O| | | | | | | | | | | | | | | | | | | | | | 
        | | | | | | | | | | |S| | | | | | | | | | | | | | | | | | | | | |
        | | | | | | | | | |O|X| | | | | | | | | | | | | | | | | | | | | |
        |V|P|T|D|P|P|M|P|P|S|M| | | | | | | | | | | | | | | | | | | | | |
        |M|V|S|E|S|A|C|G|C|F|M| | | | | | | | | | | | | | | | | | | | | |
        |E|I|D| |E|E|E|E|E|X|E| | | | | | | | | | | | | | | | | | | | | |
        | | | | | | | | | |S|X| | | | | | | | | | | | | | | | | | | | | |
        | | | | | | | | | |R|C| | | | | | | | | | | | | | | | | | | | | |
        | | | | | | | | | | |P| | | | | | | | | | | | | | | | | | | | | |
        | | | | | | | | | | |T| | | | | | | | | | | | | | | | | | | | | |
        +-------+-------+-------+-------+-------+-------+-------+-------+


*/


#pragma pack(1)
typedef struct _DTR {
    
    USHORT	Limit;
    ULONG	Base;

} GDTR, IDTR, *PGDTR, *PIDTR;
#pragma pack()



#pragma pack(1)
typedef struct _EFLAGS {

    ULONG CF:1;				// ���� ��������(����)
    ULONG :1;
    ULONG PF:1;				// ���� �������� (��������)
    ULONG :1;
    ULONG AF:1;				// ���� ��������������� ��������(����) � ������� ��� ���������� ����������
    ULONG :1;				
    ULONG ZF:1;				// ���� �������� ����������
    ULONG SF:1;				// ���� �����
    ULONG TF:1;				// ���� �����������
    ULONG IF:1;				// ���������� ������������
    ULONG DF:1;				// ���� ���������� ����������� � ��������� ���������
    ULONG OF:1;				// ���� ������������
    ULONG IOPL:1;			// ������� ���������� �����/������			
    ULONG NT:1;				// ���� ����������� ������
    ULONG :1;
    ULONG RF:1;				// ���� �������������
    ULONG VM:1;				// ����� ������������ ���������� 8086
    ULONG AC:1;				// �������� ������������ (��� 3 ������)
    ULONG VIF:1;			// ����������� ������ ����� IF
    ULONG VIP:1;			// ����������� ������ ����������
    ULONG ID:1;				// ���� ����������� ������� ������������� CPUID
    ULONG :10;

} EFLAGS, *PEFLAGS;
#pragma pack()


/*

            EFLAGS

        0              7 8             15 16           23 24            31
        +_______+_______+_______+________+_______+_______+_______+_______+
        | | | | | | | | | | | | | |  | | | | | |V|V| | | | | | | | | | | | 
        |C|1|P|0|A|0|Z|S|T|I|D|O|IOPL|N|0|R|V|A|I|I|I|0|0|0|0|0|0|0|0|0|0|
        |F| |F| |F| |F|F|F|F|F|F|    |T| |F|M|C|F|P|D| | | | | | | | | | |
        +-------+-------+-------+--------+-------+-------+-------+-------+


*/


#endif  // _INTEL_H_
