#define FLASH_BOOT_ADDR 0x00a000

/* NVMCON<15> */
#define WR 15

/* PORTB, Pin 13, Drive P22 high */
#define P22BIT 13

/* Flash Unit Erase Page Command */
#define ERASEPAGE 0x4042

/* Flash Unit Write Row Command */
#define WRITEROW  0x4001

	.globl _assert_p22
_assert_p22:
	bset PORTB,#P22BIT
	return

	.globl _deassert_p22
_deassert_p22:
	bclr PORTB, #P22BIT
	return

  .globl _flash_erase
_flash_erase:
  add #20, W0     ; base address of destination is 0x5000
  mov #ERASEPAGE, W1
  mov W1, NVMCON
  clr W1          ; we are always working with < 0x10000
  mov W1, TBLPAG
  sl W0, #10, W0  ; cheap out on the math here to get 1024x
  tblwtl W0, [W0] ; set base address of erase block
  disi #5
  mov #0x55, W0
  mov W0, NVMKEY
  mov #0xaa, W1
  mov W1, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR ; skip branch when WR is cleared
  bra $-2
  return


  .globl _flash_page
_flash_page:
  add #20, W0     ; base address of destination is 0x6000
  sl W0, #10, W0  ; cheap out on the math here to get 1024x
  mov W1, W5
  mov #WRITEROW, W2
  mov W2, NVMCON
  clr W2
  mov W2, TBLPAG
  mov #8, W1      ; Row Counter

__flash_row:
  mov #32, W6     ; Instruction Counter / 2

__flash_instructions:
  mov [W5++], W2
  mov [W5++], W3
  tblwtl W2, [W0]
  tblwth W3, [W0++]

  lsr W3, #8, W2
  mov [W5++], W3
  sl W3, #8, W4
  ior W4, W2, W2
  lsr W3, #8, W3

  tblwtl W2, [W0]
  tblwth W3, [W0++]
  dec W6, W6
  bra nz, __flash_instructions

; done with row, program flash
  disi #5
  mov #0x55, W2
  mov W2, NVMKEY
  mov #0xaa, W3
  mov W3, NVMKEY
  bset NVMCON, #WR ; initiate write operation
  nop
  nop
  btsc NVMCON, #WR ; skip branch when WR is cleared
  bra $-2

  dec W1, W1
  bra nz, __flash_row

  return

  ; read the value at 0xa400
  ; did this in assembly because builtins need symbols
  ; symbols need definition, and making an a400 defined
  ; data piece causes a huge emission of data in the hex
  .globl _flash_boot_chn_rpt_tx_power
_flash_boot_chn_rpt_tx_power:
  mov #tblpage(0xa400), W0
  mov W0, TBLPAG
  mov #tbloffset(0xa400), W0
  tblrdl [W0], W0
  return

  .globl _flash_boot_gateway
_flash_boot_gateway:
  mov #tblpage(0xa402), W2
  mov W2, TBLPAG
  mov #tbloffset(0xa402), W2
  tblrdl [W2++], W0
  tblrdl [W2], W1
  return

  .globl _flash_boot_entry
_flash_boot_entry:
  goto FLASH_BOOT_ADDR

  ; intended to be copied to 0xa000
  ; does not return

	;; Calling Convention
	;; W0 - Page Count
	;; W1 - Channel/Repeat/Tx_Power
	;; W2 - Gateway Least Significant 16 bits
	;; W3 - Gateway Most Significant 16 bits

_flash_boot:
  mov W0, W6 ; page counter
  ; disable user interrupts
  mov SR, W0 ; full scratch use of W0
  ior #0x00e0, W0 ; cpu interrupt priority SR(5:7) = 7 (15)
  mov W0, SR

  ; write our channel/repeat/tx_power/gateway information to a400
  ; where we can obviously keep a lot more information
  ; erase first, then write
  mov #ERASEPAGE, W0
  mov W0, NVMCON

  clr W0
  mov W0, TBLPAG
  mov #tbloffset(0xa400), W0
  tblwtl W0, [W0] ; set base address of write block

  mov #0x55, W0
  mov W0, NVMKEY
  mov #0xaa, W0
  mov W0, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR ; skip branch when WR is cleared
  bra $-2

  mov #WRITEROW, W0
  mov W0, NVMCON

  mov #tbloffset(0xa400), W0

  tblwtl W1, [W0++] ; 1 - channel/repeat/tx_power
  tblwtl W2, [W0++] ; 2,3 - gateway
  tblwtl W3, [W0++] ;

  mov #0x55, W0
  mov W0, NVMKEY
  mov #0xAA, W0
  mov W0, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR
  bra $-2

  clr W1 ; our page offset counter
  mov W1, TBLPAG
  mov #tbloffset(0x5000), W2 ; our source offset

__fb_erase_page:

  ; erase our current page
  mov #ERASEPAGE, W0
  mov W0, NVMCON
  tblwtl W1, [W1]
  mov #0x55, W0
  mov W0, NVMKEY
  mov #0xaa, W0
  mov W0, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR
  bra $-2

  ; program from program space
  mov #WRITEROW, W0
  mov W0, NVMCON

  mov #8, W5 ; row counter

__fb_flash_row:
  mov #64, W4 ; instruction counter

  ; program space makes this easier, no shifting needed
__fb_flash_instruction:
  tblrdl [W2], W3
  tblwtl W3, [W1]
  tblrdh [W2++], W3
  tblwth W3, [W1++]
  dec W4, W4
  bra nz, __fb_flash_instruction

  mov #0x55, W0
  mov W0, NVMKEY
  mov #0xaa, W0
  mov W0, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR
  bra $-2

  dec W5, W5
  bra nz, __fb_flash_row

  dec W6, W6
  bra nz, __fb_erase_page
  reset				; Reset Processor

  ; must be aligned after _flash_boot to copy correct size
  .globl _flash_boot_init
_flash_boot_init:
  mov #20, W0
  call _flash_erase ; erase 0xa000-0xa3ff

  clr W0
  mov W0, TBLPAG
  mov #tbloffset(_flash_boot), W1

  mov #(((_flash_boot_init - _flash_boot) + 127) / 128), W2

  mov #WRITEROW, W0
  mov W0, NVMCON

  clr W0
  mov W0, TBLPAG
  mov #tbloffset(_flash_boot), W1
  ; tblpag for code->code copies will always be the same
  mov #tbloffset(FLASH_BOOT_ADDR), W0

__ifb_copy_row:
  mov #64, W5

__ifb_copy_instruction:
  tblrdl [W1], W4
  tblwtl W4, [W0]
  tblrdh [W1++], W4
  tblwth W4, [W0++]
  dec W5, W5
  bra nz, __ifb_copy_instruction

  disi #5
  mov #0x55, W4
  mov W4, NVMKEY
  mov #0xaa, W4
  mov W4, NVMKEY
  bset NVMCON, #WR
  nop
  nop
  btsc NVMCON, #WR
  bra $-2

  dec W2, W2
  bra nz, __ifb_copy_row

  return
