MEMORY
{
  IROM(rx):  ORIGIN = 0x20000000, LENGTH = 40k
  IRAM(rwx): ORIGIN = 0x2000a000, LENGTH = 24k
}

INCLUDE cortex-m4.ld
