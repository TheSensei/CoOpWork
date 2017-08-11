#ifndef FLASH_FUNCTIONS_H
#define FLASH_FUNCTIONS_H

extern void flash_erase( uint16_t page_index );
extern void flash_page( uint16_t page_index, void *page_mem );
extern void flash_boot_init( void );
extern void flash_boot_entry( uint16_t page_count, uint16_t chn_rpt, DWORD gw ); /* does not return */
extern WORD flash_boot_chn_rpt_tx_power( void );
extern DWORD flash_boot_gateway( void );

#endif
