#include <stdint.h>
#include <drv/pci/pci.h>
#include "../../util.h"

static uint32_t read_pci_config(uint8_t busid, uint8_t devid, uint8_t func, uint8_t offset) {
  uint32_t conval = (      1U           << 31);  
  conval |=  (((uint32_t) busid)         << 16);
  conval |= ((((uint32_t) devid) & 0x1F) << 11);
  conval |= ((((uint32_t) func)  & 0x03) <<  8);
  conval |= (((uint32_t) offset) & 0xCF);
  
  outl(0xCF8, conval);
  uint32_t config = inl(0xCFC);
  return config;
}

static int secondary_bus_nr = 1;

static void scan_pci_bus(uint8_t busid) {
  uint32_t value;
  uint8_t devid = 0;
  pci_descriptor_t descr;
  
  terminal_writestring("bus  vendID  devID   clas  scls  htype\n");
  char* pHex = "  0x";
  
  while (true) {
    uint8_t func;
    for (func = 0; func < 8; func++) {
      value = read_pci_config(busid, devid, func, 0x0);

      descr.device_id = value >> 16;
      descr.vendor_id = value & 0xFFFF;

      if (descr.vendor_id == 0xFFFF) {
        if (func == 0) {
          break;
        } else {
          continue;
        }
      }
    
          
      value = read_pci_config(busid, devid, func, 0x8);
      descr.device_class = value >> 24;
      descr.device_subclass = (value >> 16) & 0xFF;
    
      value = read_pci_config(busid, devid, func, 0xC);
      descr.header_type = (value >> 16) & 0xFF;
      
    
      terminal_writestring(pHex);
      terminal_writehexdigits( descr.vendor_id >> 8 );
      terminal_writehexdigits( descr.vendor_id & 0xFF );

      terminal_writestring(pHex);
      terminal_writehexdigits( descr.device_id >> 8 );
      terminal_writehexdigits( descr.device_id & 0xFF );

      terminal_writestring(pHex);
      terminal_writehexdigits( descr.device_class );
      terminal_writestring(pHex);
      terminal_writehexdigits( descr.device_subclass );
    
      terminal_writestring(pHex);
      terminal_writehexdigits( descr.header_type );
    
      terminal_putchar('\n');

      if (descr.device_class == 0x06 && descr.device_subclass == 0x04) {
        value = read_pci_config(busid, devid, func, 0x18);
        
        //when we read this, we probably need to configure the secondary bus..
        uint8_t secbusid = (value & 0xFF00) >> 8;
        terminal_writestring("Found PCI-PCI bridge...\nBusses (p,s) = ");
        terminal_writehexdigits( value & 0xFF );
        terminal_putchar( ',' );
        terminal_writehexdigits( value >> 8 );
        terminal_putchar( '\n' );
        if (secbusid == 0) {
          terminal_writestring("Starting reconfigure...\n");
          secbusid = secondary_bus_nr++;
          value = value & ~0x0000FF00UL;
          value |= (secbusid << 8);
          
          uint32_t conval = (      1U           << 31);  
          conval |=  (((uint32_t) busid)         << 16);
          conval |= ((((uint32_t) devid) & 0x1F) << 11);
          conval |= ((((uint32_t) func)  & 0x03) <<  8);
          conval |= (((uint32_t)  0x18) & 0xCF);
  
          outl(0xCF8, conval);
          outl(0xCFC, value);
          terminal_writestring("Checking...\n");
          value = read_pci_config(busid, devid, func, 0x18);
          terminal_writestring("After config!.. \n");
        }
        
        
        if ((value & 0xFF00) != 0) {
          terminal_writestring("Prerecursive\n");
          scan_pci_bus(secbusid);
          terminal_writestring("Postrecursive\n");
        }
        while (1);
      }

      if ((descr.header_type & 0x80) == 0)
        break;

    }
    devid++;
    if ( descr.vendor_id == 0xFFFF && func == 0)
      break;
  }
  
}

void pci_init() {
  scan_pci_bus(0);
}
