//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  RTSYSTEM.C                                                            лл
//лл                                                                        лл
//лл  AESOP runtime system-level services                                   лл
//лл                                                                        лл
//лл  Version: 1.00 of 6-May-92 -- Initial version                          лл
//лл                                                                        лл
//лл  Project: Extensible State-Object Processor (AESOP/16)                 лл
//лл   Author: John Miles                                                   лл
//лл                                                                        лл
//лл  C source compatible with IBM PC ANSI C/C++ implementations            лл
//лл  Large memory model (16-bit DOS)                                       лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  Copyright (C) 1992 Miles Design, Inc.                                 лл
//лл                                                                        лл
//лл  Miles Design, Inc.                                                    лл
//лл  10926 Jollyville #308                                                 лл
//лл  Austin, TX 78759                                                      лл
//лл  (512) 345-2642 / BBS (512) 454-9990 / FAX (512) 338-9630              лл
//лл                                                                        лл
//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

//#include "aesop/vfx.h"
//#include "aesop/ail32.h"
//#include "aesop/gil2vfx.h"

#include "common/scummsys.h"
#include "common/file.h"
#include "common/stream.h"
#include "common/textconsole.h"
#include "aesop/aesop.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/resources.h"
#include "aesop/rtcode.h"
#include "aesop/intrface.h"
#include "aesop/event.h"
#include "aesop/sound.h"
#include "aesop/graphics.h"
#include "aesop/stubs.h"

namespace Aesop {

void breakpoint(void);
#pragma aux breakpoint = "int 3";

ULONG headroom;
ULONG checksum;
ULONG init;

WORD disk_err;

void mem_init(void)
{
   headroom = init = mem_avail();

   checksum = 0L;
}

void mem_shutdown(void)
{
   ULONG end;

   end = mem_avail();

   if ((init != end) || (checksum != 0L))
      {
//    abend(MSG_UH);    (unbalanced heap normal in flat-model version)
      }
}

ULONG mem_avail(void)
{
   REGS inregs,outregs;
   ULONG memarray[12];

   inregs.x.eax = 0x0500;
   inregs.x.edi = (ULONG) memarray;
   int386(0x31,&inregs,&outregs);

   return memarray[0];
}

ULONG mem_headroom(void)
{
   return headroom;
}

void *mem_alloc(ULONG bytes)
{
   ULONG left;
   void *ptr;

   ptr = (void *) malloc(bytes);

   left = mem_avail();
   if (left < headroom) headroom = left;

   if (ptr == nullptr)
      abend(MSG_OODM);

   checksum ^= (ULONG) ptr;

   return ptr;
}

BYTE *str_alloc(BYTE *str)
{
   BYTE *ptr;

   ptr = (BYTE *)mem_alloc(strlen((const char *)str)+1);
   strcpy((char *)ptr, (const char *)str);

   return ptr;
}

void mem_free(void *ptr)
{
   checksum ^= (ULONG) ptr;

   free(ptr);
}

/***************************************************/
//
// Convert string to number, returning -1 if not valid numeric
// string
//
// Skip leading whitespace; handles unary -/+ operators
//
// Accepts binary numbers with '0b' prefix,
// hexadecimal numbers with '0x' prefix; decimal numbers
// handled via atol() library function for speed
// 
// Accepts single ASCII characters with '\'' prefix
// 
/***************************************************/

LONG ascnum(const char *string)
{
   LONG i,j,len,base,neg,chr;
   LONG total;

   while (aesop_isspace(*string)) string++;

   neg = 0;
   switch (*string)
      {
      case '-': neg = 1; string++; break;
      case '+':          string++; break;
      }

   if (*string == '\'')
      return (LONG) (*(string+1));

   switch (READ_LE_UINT16(string))
      {
      case 'x0': base = 16; string += 2; break;
      case 'b0': base = 2;  string += 2; break;
      default:   base = 10; break;
      }

   if (base == 10) {
      if (aesop_isdigit(*string))
		  return neg ? -atol((const char *)string) : atol((const char *)string);
      else
         return -1;
	}
   total = 0L;
   len = strlen((const char *)string);

   for (i=0;i<len;i++)
      {
      chr = toupper(string[i]);

      for (j=0;j<base;j++)
         if (chr == "0123456789ABCDEF"[j])
            {
            total = (total * base) + j;
            break;
            }

      if (j==base) return -1;
      }

   return total;
}

/***************************************************/
//
// AESOP interpreter opcode fault handler
//
/***************************************************/

void opcode_fault(void *PC, void *stk)
{
   abend(MSG_IAO,*(unsigned char *) PC,PC,stk);
}

/***************************************************/
//
// Abnormal program termination handler
//
// Give debugger a chance to return to the failing function, else
// shut everything down gracefully and exit to DOS
//
/***************************************************/

void abend(const char *msg, ...)
{
	error("%s", msg);
	/*
   va_list argptr;
   WORD recover;
   WORD x,y;

   curpos(&x,&y);
   if (y > 25)
      locate(0,0);

   if (msg != nullptr) {
      va_start(argptr,msg);
      vprintf(msg,argptr);
      va_end(argptr);
      printf("\n");

      if (envval(0,"AESOP_DIAG") == 1)
         printf(MSG_MIE,current_msg,current_index,current_event_type);
      }

   recover = 0;

   breakpoint();

   if (!recover)
      {
      shutdown_sound();
      shutdown_interface();
      AIL_shutdown("Abend");
      GIL2VFX_shutdown_driver();

      exit(1);
      }
	  */
}

/***************************************************/
//
// Open a text file for reading/writing
//
/***************************************************/

TF_class *TF_construct(const char *filename, WORD oflag) {
	Resources &res = *_vm->_resources;
	TF_class *TF;
   HRES hbuf;
   /*
   WORD file;

   if (oflag == TF_WRITE)
      oflag = O_CREAT | O_TRUNC | O_WRONLY;
   else
      oflag = O_RDONLY;

   file = open(filename,oflag | O_BINARY,S_IREAD | S_IWRITE);
   if (file == -1) return NULL;
   */
   Common::File f;
   f.open((const char *)filename);

   hbuf = res.alloc(TF_BUFSIZE,DA_FIXED | DA_PRECIOUS);
   if (hbuf == HRES_NULL)
	   return NULL;

   TF = (TF_class *)mem_alloc(sizeof(TF_class));

   TF->file = f.readStream(f.size());
   TF->hbuf = hbuf;
   TF->buffer = (BYTE *)res.addr(TF->hbuf);
   TF->p = 0;
   TF->mode = oflag;
   TF->len = f.size();
   TF->pos = 0L;

   TF->file->read(TF->buffer, TF_BUFSIZE);
//   if (!(oflag & O_WRONLY))
//      read(TF->file,TF->buffer,TF_BUFSIZE);

   return TF;
}

/***************************************************/
//
// Close text file/dealloc buffer
//
// Return 0 if write attempt failed
//
/***************************************************/

WORD TF_destroy(TF_class *TF)
{
	Resources &res = *_vm->_resources;
	WORD e,f;

   e = f = TF->p;
   /*
   if ((TF->mode & O_WRONLY) && (TF->p != 0))
      e = write(TF->file,TF->buffer,TF->p);
   close(TF->file);
   */
   delete TF->file;

   res.free(TF->hbuf);
   mem_free(TF);

   return (e == f);
}

/***************************************************/
//
// Write character to text file
//
// Return 0 if write attempt failed
//
/***************************************************/

WORD TF_wchar(TF_class *TF, BYTE ch)
{
	error("TODO");
	/*
   TF->buffer[TF->p++] = ch;

   if (TF->p == TF_BUFSIZE)
      {
      TF->p = 0;
      if (write(TF->file,TF->buffer,TF_BUFSIZE) != TF_BUFSIZE) return 0;
      }
	  */
   return 1;
}

/***************************************************/
//
// Read character from text file
//
// Return 0 if EOF reached
//
/***************************************************/

BYTE TF_rchar(TF_class *TF)
{
   if (TF->pos >= TF->len)
      return 0;

   ++TF->pos;

   if (TF->p != TF_BUFSIZE)
      return TF->buffer[TF->p++];

   TF->file->read(TF->buffer,TF_BUFSIZE);
   
   TF->p = 1;
   return TF->buffer[0];
}

/***************************************************/
//
// Read text file line into buffer
//
// \r's are skipped
// \n's are truncated, replaced with \0
//
// Blank lines are ignored
//
// Return 0 if EOF reached
//
/***************************************************/

WORD TF_readln(TF_class *TF, char *buffer, WORD maxlen)
{
   WORD b,c;

   do
      {
      b = 0;

      while (b != maxlen-1)
         {
         c = TF_rchar(TF);

         if (c == '\n') break;
         if (c == '\r') continue;

         buffer[b++] = c;

         if (!c) return 0;
         }

      if (b == maxlen-1)
         while ((c = TF_rchar(TF)) != '\n')
            if (!c) return 0;

      buffer[b] = 0;
      }
   while (!strlen((const char *)buffer));

   return 1;
}

/***************************************************/
//
// Write buffer line to text file
// Return 0 if write attempt failed
//
// \r\n added at end of each buffer line
//
/***************************************************/

WORD TF_writeln(TF_class *TF, const char *buffer)
{
   WORD b,c;

   b = 0;

   while ((c = buffer[b++]) != 0)
      if (!TF_wchar(TF,c)) return 0;

   TF_wchar(TF,'\r');
   return TF_wchar(TF,'\n');
}

/***************************************************/
//
// Delete a file
//
// Return 0 if file did not exist, -1 if deletion failed,
// else 1 if deleted OK
//
/***************************************************/

WORD delete_file(const char *filename)
{
	error("TODO");
	/*
   if (!unlink(filename))
      return 1;

   if (errno == ENOENT)
      return 0;
	  */
   return -1;
}

/***************************************************/
//
// Copy a file
//
// Return 0 if source file not found, -1 if copy error occurred,
// else 1 if copied OK
//
/***************************************************/

WORD copy_file(const char *src_filename, const char *dest_filename)
{
	error("TODO");
	/*
   HRES hbuf;
   BYTE *buffer;
   WORD status;
   WORD s,d,n;

   s = open(src_filename,O_RDONLY | O_BINARY);

   if (s==-1)
      return 0;

   d = open(dest_filename,O_BINARY | O_CREAT | O_TRUNC | O_WRONLY,
      S_IREAD | S_IWRITE);

   if (d==-1)
      {
      close(s);
      return -1;
      }

   hbuf = RTR_alloc(RTR,TF_BUFSIZE,DA_FIXED | DA_PRECIOUS);
   if (hbuf == -1U)
      {
      close(s);
      close(d);
      return -1;
      }

   buffer = RTR_addr(hbuf);
   status = 1;

   while ((n = read(s,buffer,TF_BUFSIZE)) != 0)
      {
      if (n == -1)
         {
         status = -1;
         break;
         }

      if (write(d,buffer,n) != n)
         {
         status = -1;
         break;
         }
      }

   close(s);
   close(d);

   RTR_free(RTR,hbuf);

   return status;
   */
	return 0;
}

/****************************************************************************/
//
// Determine the size in bytes of a file
//
/****************************************************************************/

LONG file_size(BYTE *filename)
{
	Common::File f;
	f.open((const char *)filename);

	LONG filesize = f.size();
	f.close();

	return filesize;
}

/****************************************************************************/
//
// Read a file directly into memory
//
/****************************************************************************/

BYTE *read_file(BYTE *filename, void *dest)
{
   WORD i;
   ULONG len;
   BYTE *buf, *mem;

   disk_err = 0;

   len = file_size(filename);
   if (len==(ULONG)-1)
      {
      disk_err = FILE_NOT_FOUND;
      return NULL;
      }

   buf = mem = (dest == NULL) ? (BYTE *)mem_alloc(len) : (BYTE *)dest;

   if (buf==NULL)
      {
      disk_err = OUT_OF_MEMORY;
      return NULL;
      }

   Common::File f;
   if (!f.open((const char *)filename)) {
      mem_free(mem);
      disk_err = FILE_NOT_FOUND;
      return NULL;
   }

   while (len >= DOS_BUFFSIZE) {
      i = f.read(buf,DOS_BUFFSIZE);
      if (i != (WORD) DOS_BUFFSIZE)
         {
         mem_free(mem);
         disk_err = CANT_READ_FILE;
         return NULL;
         }
      len -= DOS_BUFFSIZE;
      buf = (BYTE *)add_ptr(buf,DOS_BUFFSIZE);
      }

   i = f.read(buf,(UWORD) len);
   if (i != (UWORD) len)
      {
      mem_free(mem);
      disk_err = CANT_READ_FILE;
      return NULL;
      }

   f.close();
   return mem;   
}
   
/****************************************************************************/
//
// Write a binary file to disk
// 
/****************************************************************************/

WORD write_file(BYTE *filename, void *buf, ULONG len)
{
	error("TODO");
	/*
   WORD i,handle;

   disk_err = 0;

   handle = open(filename,O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
      S_IREAD | S_IWRITE);
   if (handle==-1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }

   while (len >= DOS_BUFFSIZE)
      {
      i = write(handle,buf,DOS_BUFFSIZE);
      if (i == -1)
         {
         disk_err = CANT_WRITE_FILE;
         return 0;
         }
      if (i != (WORD) DOS_BUFFSIZE)
         {
         disk_err = DISK_FULL;
         return 0;
         }
      len -= DOS_BUFFSIZE;
      buf = add_ptr(buf,DOS_BUFFSIZE);
      }

   i = write(handle,buf,(UWORD) len);
   if (i == -1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }
   if (i != (UWORD) len)
      {
      disk_err = DISK_FULL;
      return 0;
      }

   close(handle);
   */
   return 1;
}

/****************************************************************************/
//
// Append binary data to an existing file
//
/****************************************************************************/

WORD append_file(BYTE *filename, void *buf, ULONG len)
{
	error("TODO");
	/*
   WORD i,handle;

   disk_err = 0;

   handle = open(filename,O_APPEND | O_RDWR | O_BINARY);
   if (handle==-1)
      {
      disk_err = FILE_NOT_FOUND;
      return 0;
      }

   while (len >= DOS_BUFFSIZE)
      {
      i = write(handle,buf,DOS_BUFFSIZE);
      if (i == -1)
         {
         disk_err = CANT_WRITE_FILE;
         return 0;
         }
      if (i != (WORD) DOS_BUFFSIZE)
         {
         disk_err = DISK_FULL;
         return 0;
         }
      len -= DOS_BUFFSIZE;
      buf = add_ptr(buf,DOS_BUFFSIZE);
      }

   i = write(handle,buf,(UWORD) len);
   if (i == -1)
      {
      disk_err = CANT_WRITE_FILE;
      return 0;
      }
   if (i != (UWORD) len)
      {
      disk_err = DISK_FULL;
      return 0;
      }

   close(handle);
   */
   return 1;
}

/****************************************************************************/
//
// Get file's timestamp
//
/****************************************************************************/

ULONG file_time(const char *filename)
{
	return 0L;
	
	/*
   union REGS in,out;
   WORD handle;

   handle = open(filename,O_RDONLY);

   if (handle==-1) return 0L;

   in.w.ax = 0x5700;
   in.w.bx = handle;
   intdos(&in,&out);

   close(handle);

   return (ULONG) out.w.cx + ((ULONG) out.w.dx << 16);
   */
}

/****************************************************************************/
//
// Position text cursor
//
/****************************************************************************/

void locate(WORD x, WORD y)
{
   union REGS inregs,outregs;

   inregs.h.bh = 0x00;
   inregs.h.ah = 0x02;
   inregs.h.dh = y;
   inregs.h.dl = x;
   int386(0x10,&inregs,&outregs);
}

/****************************************************************************/
//
// Get text cursor location
//
/****************************************************************************/

void curpos(WORD *x, WORD *y)
{
   union REGS inregs,outregs;

   inregs.h.ah = 0x0f;
   int386(0x10,&inregs,&outregs);

   inregs.h.bh = outregs.h.bh; inregs.h.ah = 0x03;
   int386(0x10,&inregs,&outregs);
   *x = outregs.h.dl; *y =outregs.h.dh;
}

} // End of namespace Aesop

