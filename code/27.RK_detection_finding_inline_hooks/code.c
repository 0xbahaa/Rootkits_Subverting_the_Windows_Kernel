/////////////////////////////////////////////////////

// DWORD CheckForOutsideJump

//

// Description:

//          This function takes the address of the function

//          to check. It then looks at the first few opcodes

//          looking for immediate jumps, etc.

//

DWORD CheckNtoskrnlForOutsideJump (DWORD dw_addr)

{

   BYTE  opcode = *((PBYTE)(dw_addr));

   DWORD hook   = 0;

   WORD  desc   = 0;

   // These are the opcodes for unconditional relative jumps.

   // Opcode 0xeb is a relative jump that takes one byte, so

   // at most it can jump 255 bytes from the current EIP.

   //

   // Currently not sure how to handle opcode 0xea. It looks

   // like jmp XXXX:XXXXXXXX. For now, I guess I will just

   // ignore the first two bytes. In the future, you should

   // add these two bytes as they represent the segment.

   if ((opcode == 0xe8) || (opcode == 0xe9))

   {

      // || (opcode == 0xeb) -> ignoring these short jumps

     hook |= *((PBYTE)(dw_addr+1)) << 0;

     hook |= *((PBYTE)(dw_addr+2)) << 8;

     hook |= *((PBYTE)(dw_addr+3)) << 16;

     hook |= *((PBYTE)(dw_addr+4)) << 24;

     hook += 5 + dw_addr;

   }

   else if (opcode == 0xea)

   {

      hook |= *((PBYTE)(dw_addr+1)) << 0;

      hook |= *((PBYTE)(dw_addr+2)) << 8;

      hook |= *((PBYTE)(dw_addr+3)) << 16;

      hook |= *((PBYTE)(dw_addr+4)) << 24;

      // Should update to reflect GDT entry,

      // but we are ignoring it for now.

      desc = *((WORD *)(dw_addr+5));

   }

   // Now that we have the target of the jump

   // we must check whether the hook is outside of

   // ntoskrnl. If it isn't, return 0.

   if (hook != 0)

   {

     if ((hook < g_ntoskrnl.Base) || (hook > g_ntoskrnl.End))

        hook = hook;

     else

        hook = 0;

   }

   return hook;

}