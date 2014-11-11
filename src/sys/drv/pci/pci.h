#ifndef PCI_PCI_H
#define PCI_PCI_H

typedef struct {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t status;
    uint16_t command;
    uint8_t device_class;
    uint8_t device_subclass;
    uint8_t programming_interface;
    uint8_t revision_id;
    uint8_t bist;
    uint8_t header_type;
} pci_descriptor_t;

void pci_init();
typedef void (*pci_probe_callback) (pci_descriptor_t * device );

void pci_register_probe_callback( pci_probe_callback callback );
void pci_probe();

pci_descriptor_t pci_getDescriptor(uint8_t busid, uint8_t devid, uint8_t func, uint8_t offset);

#endif
