PROT: PPU_ECC 
=============
Description
^^^^^^^^^^^
- This example tests the the ECC injection function of the PPU attribute registers 
  which is configured in SRAM.

Target Device
^^^^^^^^^^^^^
- Traveo-II CYT2B7x devices

CPU Board
^^^^^^^^^
- CYTVII-B-E-1M-176-CPU Rev. C Board

Dependency
^^^^^^^^^^
- None.

Expectation
^^^^^^^^^^^
- Set the correct ECC parity and enable the ECC injection.
- Read the attribute registers (expecting no error)
- Write 1-bit inverted data to one of the attribute registers.
- Read the attribute register (expecting correctable error)
- Write 2-bits inverted data to one of the attribute registers.
- Read the attribute register (expecting non-correctable error)