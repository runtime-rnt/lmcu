MEMORY
{
  IROM(rx):  ORIGIN = 0x08000000, LENGTH = 512k
  IRAM(rwx): ORIGIN = 0x20000000, LENGTH = 96k
}

INCLUDE cortex-m4.ld
