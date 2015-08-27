//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                        лл
//лл  RTLINK.C                                                              лл
//лл                                                                        лл
//лл  AESOP runtime object linker                                           лл
//лл                                                                        лл
//лл  Version: 1.00 of 13-Jun-92 -- Initial version                         лл
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

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

#include "aesop/defs.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
#include "aesop/rtlink.h"
#include "aesop/rtcode.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"

namespace Aesop {

// External code reference entry
struct XCR_entry {
   ULONG val;
};                    

// External data reference entry
struct XDR_entry {
   UWORD offset;
};

/***************************************************/
//
// Utility subs for thunk creation
//
/***************************************************/

int sort_by_msg(const void *a, const void *b)
{
   UWORD na,nb;

   na = ((MV_entry *) a)->msg;
   nb = ((MV_entry *) b)->msg;

   if (na > nb)
      return 1;
   else if (na == nb)
      return 0;
   else
      return -1;
}

int sort_by_class(const void *a, const void *b)
{
   UWORD na,nb;

   na = ((MV_entry *) a)->SD_offset;
   nb = ((MV_entry *) b)->SD_offset;

   if (na < nb)
      return 1;
   else if (na == nb)
      return 0;
   else
      return -1;
}

/***************************************************/
//
// Create thunk for specified object
//
/***************************************************/

HRES construct_thunk(RTR_class *RTR, RTR_class *LNK, ULONG object)
{
   UWORD depth;              // 0..MAX_G = derived object..base class
   ULONG class1,xclass;
   ULONG tsize;
   ULONG source;
   UWORD target;
   WORD i;
   UWORD j,k,m,n,mcnt;
   PRG_HDR prg,xprg;
   THDR thdr;
   UBYTE *dict;
   BYTE *tag, *tagbase;
   char *def;
   HRES thunk;
   HRES HCRFD;
   HRES impt[MAX_G],expt[MAX_G],code[MAX_G];
   HRES xexpt,xcode;
   ULONG exports[MAX_G];
   UWORD s_S[MAX_G],x_S[MAX_G];
   UWORD SD_offset[MAX_G];
   UWORD index,offset,found;
   UWORD XR_list;
   SD_entry *SD,*SDarray;
   MV_entry *MV;
   void *XR;
   XDR_entry *xdr_ptr;

   void *thunk_ptr;
   ULONG def_off;
   ULONG *XR_ptr, *CR_ptr;

   //
   // Load programs and dictionaries, calculate thunk size
   //

   HCRFD = RTR_get_resource_handle(RTR,CRFD,DA_TEMPORARY | DA_EVANESCENT);
   RTR_lock(RTR,HCRFD);

   thdr.MV_list = sizeof(THDR);
   thdr.SD_list = sizeof(THDR);
   thdr.max_msg = -1U;
   thdr.nprgs = 0;
   thdr.isize = sizeof(IHDR);
   thdr.use_cnt = 0;

   XR_list = sizeof(THDR);

   tsize = sizeof(THDR);

   for (i=0;i<MAX_G;i++)
      s_S[i] = x_S[i] = 0;

   depth = 0;
   class1 = object;

   while (class1 != -1L)
      {
      code[depth] = RTR_get_resource_handle(RTR,class1,DA_DEFAULT);

      if (!code[depth])
         abend(MSG_PONF,class1);     // "Program object %lu not found"

      RTR_lock(RTR,code[depth]);

      prg = *(PRG_HDR *) RTR_addr(code[depth]);

      ++thdr.nprgs;
      tsize += sizeof(SD_entry);
      XR_list += sizeof(SD_entry);

      exports[depth] = prg.exports;

      impt[depth] = RTR_get_resource_handle(LNK,prg.imports,
                                            DA_TEMPORARY | DA_EVANESCENT);
      RTR_lock(LNK,impt[depth]);

      expt[depth] = RTR_get_resource_handle(LNK,prg.exports,
                                            DA_TEMPORARY | DA_EVANESCENT);
      RTR_lock(LNK,expt[depth]);


      //
      // Calculate Size of Message Vector List   
      //

      mcnt = 0;
      dict = (UBYTE *)RTD_first(RTR_addr(expt[depth]));
      while ((dict = (UBYTE *)RTD_iterate(RTR_addr(expt[depth]),dict,&tag,(BYTE **)&def)) != NULL)
         {
         switch (tag[0])
            {
            case 'M':               // Message
               ++thdr.max_msg;
               ++mcnt;
               thdr.SD_list += sizeof(MV_entry);
               XR_list += sizeof(MV_entry);
               tsize += sizeof(MV_entry);
               break;
            }
         }

      //
      // Calculate Size of External Reference List
      //

      dict = (UBYTE *)RTD_first(RTR_addr(impt[depth]));
      while ((dict = (UBYTE *)RTD_iterate(RTR_addr(impt[depth]),dict,&tag,(BYTE **)&def)) != NULL)
         {
         switch (tag[0])
            {
            case 'C':               // Code
               tsize += sizeof(XCR_entry);
               x_S[depth] += sizeof(XCR_entry);
               break;

            case 'B':               // Byte
            case 'W':               // Word
            case 'L':               // Long
               tsize += sizeof(XDR_entry);
               x_S[depth] += sizeof(XDR_entry);
               break;
            }
         }

      s_S[depth] += prg.static_size;
      thdr.isize += prg.static_size;

      class1 = prg.parent;

      if (++depth == MAX_G)
         abend(MSG_AILE);           // "AESOP inheritance limit exceeded"

      if (tsize > 65535L)
         abend(MSG_TTL);            // "Thunk too large"

      }


   //
   // Allocate memory for thunk and construct it
   //

   thunk = RTR_alloc(RTR,tsize,DA_MOVEABLE | DA_PRECIOUS);

   *(THDR *) RTR_addr(thunk) = thdr;

   SD = (SD_entry *) thdr.SD_list;

   i = depth-1;
   j = 0;
   k = sizeof(IHDR);
   m = XR_list;
   n = thdr.SD_list;

   while (i >= 0)
      {
      SDarray = (SD_entry *) ((ULONG)RTR_addr(thunk) + (ULONG)SD);
      SDarray[j].SOP = code[i];
      SDarray[j].exports = exports[i];
      SDarray[j].static_base = k;
      SDarray[j].extern_base = m;

      SD_offset[i] = n;

      XR = (void *) m;

      dict = (UBYTE *)RTD_first(RTR_addr(impt[i]));
      while ((dict = (UBYTE *)RTD_iterate(RTR_addr(impt[i]),dict,&tag,(BYTE **)&def)) != NULL)
         {
         tagbase = (BYTE *)RTR_addr(impt[i]);
         switch (tag[0])
            {
            case 'C':               // Code
               offset = (UWORD) ascnum((const char *)RTD_lookup(HCRFD,&tag[2]));
               if (offset == -1U)
                  abend(MSG_MCR,&tag[2]); // "Missing code resource '%s'"

               thunk_ptr = RTR_addr(thunk);
               def_off = ascnum(def);
               XR_ptr = (ULONG *)((ULONG) thunk_ptr + (ULONG) XR + def_off);
               CR_ptr = (ULONG *)((ULONG) &code_resources + offset);
               *XR_ptr = *CR_ptr;

               break;

            case 'B':               // Byte
            case 'W':               // Word
            case 'L':               // Long   
               target = (UWORD) ascnum(def);
               source = ascnum(strchr(def,',')+1);

               xclass = source;
               index = sizeof(IHDR);
               found = 0;

               while (xclass != -1L)
                  {
                  xcode = RTR_get_resource_handle(RTR,xclass,
                     DA_DEFAULT);

                  if (!xcode)
                     abend(MSG_FPNF,xclass); //"Friend program %lu not found"

                  RTR_lock(RTR,xcode);
                  tag = tag - tagbase + (BYTE *) RTR_addr(impt[i]);

                  xprg = *(PRG_HDR *) RTR_addr(xcode);

                  xclass = xprg.parent;

                  if (!found)
                     {
                     xexpt = RTR_get_resource_handle(LNK,xprg.exports,
                        DA_TEMPORARY | DA_EVANESCENT);

                     RTR_lock(LNK,xexpt);
                     tag = tag - tagbase + (BYTE *) RTR_addr(impt[i]);

                     offset = (UWORD) ascnum((const char *)RTD_lookup(xexpt,tag));

                     if (offset != -1U)
                        {
                        found = 1;
                        index += offset;
                        }

                     RTR_unlock(xexpt);
                     }
                  else
                     index += xprg.static_size;

                  RTR_unlock(xcode);
                  }

               if (!found)
                  abend(MSG_UER,tag);  //"Unresolved external reference '%s'"

               thunk_ptr = RTR_addr(thunk);
               xdr_ptr = (XDR_entry *) ( (ULONG)thunk_ptr + (ULONG)XR + target );
               xdr_ptr->offset = index;

               break;
            }
         }

      n += sizeof(SD_entry);
      m += x_S[i];
      k += s_S[i];
      j++;
      i--;
      }

   MV = (MV_entry *)add_offset(RTR_addr(thunk),thdr.MV_list);

   for (i=m=0;i<depth;i++)
      {
      dict = (UBYTE *)RTD_first(RTR_addr(expt[i]));
      while ((dict = (UBYTE *)RTD_iterate(RTR_addr(expt[i]),dict,&tag,(BYTE **)&def)) != NULL)
         if (tag[0] == 'M')
            {
            MV[m].msg = (UWORD) ascnum((const char *)&tag[2]);
            MV[m].handler = (ULONG) ascnum(def);
            MV[m].SD_offset = SD_offset[i];
            ++m;
            }
      }

   if (m)
      {
      //
      // Sort all message vectors by ascending message number
      //

      qsort(MV,m,sizeof(MV_entry),sort_by_msg);

      //
      // Sort identical message vectors by ascending class
      //

      k = m-1;
      for (i=0;i<k;i++)
         if (MV[i+1].msg == MV[i].msg)
            {
            for (j=i+2;j<m;j++)
               if (MV[j].msg != MV[i].msg) break;

            qsort(&MV[i],j-i,sizeof(MV_entry),sort_by_class);
         
            i = j-1;
            }
      }

   //
   // Unlock dictionaries and exit w/handle to thunk
   // 

   RTR_unlock(HCRFD);
   for (i=0;i<depth;i++)
      {
      RTR_unlock(impt[i]);
      RTR_unlock(expt[i]);
      RTR_unlock(code[i]);
      }

   return thunk;
}

/***************************************************/
//
// Create SOP instance, building thunk if necessary
//
/***************************************************/

HRES create_instance(RTR_class *RTR, ULONG object)
{
   HRES thunk,instance;
   HD_entry *sel;
   ND_entry *entry;
   THDR *thdr;
   IHDR ihdr;

   entry = RTR_search_name_dir(RTR,object);

   if ((entry == NULL) || (entry->thunk == -1U))
      {
      thunk = construct_thunk(RTR,RTR,object);

      RTR_search_name_dir(RTR,object)->thunk = thunk;
      }
   else
      thunk = (HRES) entry->thunk;

   ihdr.thunk = thunk;

   thdr = (THDR *) RTR_addr(thunk);
   ++thdr->use_cnt;

   instance = RTR_alloc(RTR,thdr->isize,DA_MOVEABLE | DA_PRECIOUS);

   sel = (HD_entry *) instance;

   sel->user = object;

   *(IHDR *) RTR_addr(instance) = ihdr;

   return instance;
}

/***************************************************/
//
// Destroy instance of specified SOP
//             
// Delete thunk if no other instances of this object exist
//
/***************************************************/

void destroy_instance(RTR_class *RTR, HRES instance)
{
   HRES thunk;

   thunk = ((IHDR *) RTR_addr(instance))->thunk;

   if (!(--(((THDR *) RTR_addr(thunk))->use_cnt)))
      RTR_free(RTR,thunk);

   RTR_free(RTR,instance);
}

} // End of namespace Aesop
