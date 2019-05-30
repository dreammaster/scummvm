/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "glk/tads/os_glk.h"
#include "glk/tads/os_frob_tads.h"
#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/osifcnet.h"
#include "glk/tads/tads3/vmnetfil.h"
#include "glk/tads/tads3/vmnet.h"
#include "glk/tads/tads3/vmfile.h"
#include "glk/tads/tads3/vmerr.h"
#include "glk/tads/tads3/vmerrnum.h"
#include "glk/tads/tads3/vmrun.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmpredef.h"
#include "glk/tads/tads3/vmimport.h"
#include "glk/tads/tads3/sha2.h"
#include "glk/tads/tads3/vmhash.h"
#include "glk/tads/tads3/vmbif.h"
#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmtmpfil.h"
#include "glk/tads/tads3/vmtobj.h"
#include "glk/tads/tads3/vmimage.h"
#include "glk/tads/tads3/vmstack.h"
#include "glk/tads/tads3/charmap.h"
#include "glk/tads/tads3/vmlst.h"
#include "glk/tads/tads3/vmfilnam.h"
#include "glk/tads/tads3/vmfilobj.h"
#include "glk/tads/tads3/vmstr.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Open a network file 
 */
CVmNetFile *CVmNetFile::open(VMG_ const vm_val_t *val, const vm_rcdesc *rc,
                             int mode, os_filetype_t typ,
                             const char *mime_type)
{
    vm_val_t _filespec;
    
    /* check for a TadsObject implementing getFilename */
    if (G_predef->filespec_getFilename != VM_INVALID_PROP
        && vm_val_cast_ok(CVmObjTads, val))
    {
        /* call getFilename - the return value is the file spec */
        G_interpreter->get_prop(
            vmg_ 0, val, G_predef->filespec_getFilename, val, 0, rc);

        /* the result is the real file spec */
        _filespec = *G_interpreter->get_r0();
    }
    else
    {
        /* it's not a TadsObject, so it must directly have the file name */
        _filespec = *val;
    }

    /* check the file spec argument type */
    CVmNetFile *nf = 0;
    CVmObjTemporaryFile *tmp = 0;
    CVmObjFileName *ofn = 0;
    if ((tmp = vm_val_cast(CVmObjTemporaryFile, &_filespec)) != 0)
    {
        /* if the temporary file object is invalid, it's an error */
        if (tmp->get_fname() == 0)
            err_throw(VMERR_CREATE_FILE);

        /* create the local file descriptor for the temp file path */
        nf = open_local(vmg_ tmp->get_fname(), 0, mode, typ);

        /* mark it as a temp file */
        nf->_is_temp = TRUE;
    }
    else if (_filespec.is_numeric(vmg0_)
             || ((ofn = vm_val_cast(CVmObjFileName, &_filespec)) != 0
                 && ofn->is_special_file()))
    {
        /* 
         *   It's a special file ID, either as an integer or as a FileName
         *   wrapping a special file int.  Get the value. 
         */
        int32_t sfid = (ofn != 0 ? ofn->get_sfid()
                                 : _filespec.num_to_int(vmg0_));

        /* resolve the file system path for the given special file ID */
        char fname[OSFNMAX] = { '\0' };
        if (!CVmObjFile::sfid_to_path(vmg_ fname, sizeof(fname), sfid))
            err_throw(VMERR_BAD_VAL_BIF);

        /* create the special file descriptor */
        nf = open(vmg_ fname, sfid, mode, typ, mime_type);
    }
    else
    {
        /* anything else has to be a string */
        char fname[OSFNMAX];
        CVmBif::get_fname_val(vmg_ fname, sizeof(fname), &_filespec);

        /* 
         *   if it's a local file, and it has a relative path, explicitly
         *   apply the image file path as the default working directory 
         */
        if (!os_is_file_absolute(fname) && !is_net_mode(vmg0_))
        {
            /* build the full, absolute name based on the file base path */
            char fnabs[OSFNMAX];
            os_build_full_path(fnabs, sizeof(fnabs), G_file_path, fname);

            /* replace the relative path with the new absolute path */
            lib_strcpy(fname, sizeof(fname), fnabs);
        }

        /* create the regular network file descriptor */
        nf = open(vmg_ fname, 0, mode, typ, mime_type);
    }

    /* if they gave us an object as our file spec, remember it */
    if (nf != 0 && val->typ == VM_OBJ)
        nf->_filespec = val->val.obj;

    /* return the network file descriptor */
    return nf;
}

/* ------------------------------------------------------------------------ */
/*
 *   Rename a file
 */
void CVmNetFile::rename_to_local(VMG_ CVmNetFile *newname)
{
    /* if the new name isn't local, this isn't supported */
    if (newname->is_net_file())
        err_throw(VMERR_RENAME_FILE);

    /* if the destination file already exists, it's an error */
    if (!osfacc(newname->_lclfname))
        err_throw(VMERR_RENAME_FILE);

    /* do the rename */
    if (!os_rename_file(_lclfname, newname->_lclfname))
        err_throw(VMERR_RENAME_FILE);
}

/* ------------------------------------------------------------------------ */
/*
 *   Create a local directory 
 */
void CVmNetFile::makedir_local(VMG_ int create_parents)
{
    /* try creating the directory */
    if (!os_mkdir(_lclfname, create_parents))
        err_throw(VMERR_CREATE_FILE);
}

/* ------------------------------------------------------------------------ */
/*
 *   Empty a local directory 
 */
static void empty_dir(VMG_ const char *dir)
{
    /* open the directory search */
    osdirhdl_t dirhdl;
    if (os_open_dir(dir, &dirhdl))
    {
        err_try
        {
            /* keep going until we're out of files */
            char fname[OSFNMAX];
            while (os_read_dir(dirhdl, fname, sizeof(fname)))
            {
                /* get the full path */
                char path[OSFNMAX];
                os_build_full_path(path, sizeof(path), dir, fname);

                /* get the mode */
                unsigned long fmode;
                unsigned long fattr;
                if (osfmode(path, FALSE, &fmode, &fattr))
                {
                    /* check whether it's a directory or an ordinary file */
                    if ((fmode & OSFMODE_DIR) != 0)
                    {
                        /* 
                         *   directory - skip the special '.' and '..' links,
                         *   since they'd get us stuck in a loop 
                         */
                        os_specfile_t st = os_is_special_file(fname);
                        if (st != OS_SPECFILE_SELF && st != OS_SPECFILE_PARENT)
                        {
                            /* recursively empty the directory */
                            empty_dir(vmg_ path);
                            
                            /* remove this directory */
                            if (!os_rmdir(path))
                                err_throw(VMERR_DELETE_FILE);
                        }
                    }
                    else
                    {
                        /* ordinary file - delete it */
                        if (osfdel(path))
                            err_throw(VMERR_DELETE_FILE);
                    }
                }
            }
        }
        err_finally
        {
            /* close the directory search handle */
            os_close_dir(dirhdl);
        }
        err_end;
    }
}       

/*
 *   Remove a local directory 
 */
void CVmNetFile::rmdir_local(VMG_ int remove_contents)
{
    /* if desired, recursively remove the directory's contents */
    if (remove_contents)
        empty_dir(vmg_ _lclfname);

    /* try removing the directory */
    if (!os_rmdir(_lclfname))
        err_throw(VMERR_DELETE_FILE);
}

/* ------------------------------------------------------------------------ */
/* 
 *   static directory lister 
 */
static int s_readdir_local(VMG_ const char *_lclfname,
                           const char *nominal_path,
                           vm_val_t *retval, const vm_rcdesc *rc,
                           const vm_val_t *cb, int recursive)
{
    /* note the nominal path string length */
    size_t nominal_path_len = strlen(nominal_path);

    /* 
     *   If the caller wants a result list, create a list for the return
     *   value.  We don't know how many elements the list will need, so
     *   create it with an arbitrary initial size, and expand it later as
     *   needed.
     */
    CVmObjList *lst = 0;
    if (retval != 0)
    {
        /* create the list */
        const size_t initlen = 32;
        retval->set_obj(CVmObjList::create(vmg_ FALSE, initlen));
        lst = (CVmObjList *)vm_objp(vmg_ retval->val.obj);

        /* clear it */
        lst->cons_clear(0, initlen - 1);

        /* save the new list on the stack for gc protection */
        G_stk->push(retval);
    }

    /* presume success */
    int ok = TRUE;

    /* open the directory */
    osdirhdl_t dirhdl;
    if (os_open_dir(_lclfname, &dirhdl))
    {
        err_try
        {
            /* iterate over the directory's contents */
            int idx = 0;
            char curname[OSFNMAX];
            while (os_read_dir(dirhdl, curname, sizeof(curname)))
            {
                char *unm = 0;
                vm_obj_id_t fnobj = VM_INVALID_OBJ;

//                err_try
//                {
                    /* map the filename to UTF8 */
                    size_t unmlen = G_cmap_from_fname->map_str_alo(
                        &unm, curname);
                
                    /* create the FileName object */
                    fnobj = CVmObjFileName::combine_path(
                        vmg_ nominal_path, nominal_path_len,
                        unm, unmlen, TRUE);
                
                    /* push it for gc protection */
                    G_stk->push()->set_obj(fnobj);
/*              }
                err_finally
                {
                    if (unm != 0)
                        t3free(unm);
                }
                err_end;
*/            
                /* if we're building a list, add the file to the list */
                if (retval != 0)
                {
                    vm_val_t ele;
                    ele.set_obj(fnobj);
                    lst->cons_ensure_space(vmg_ idx, 16);
                    lst->cons_set_element(idx, &ele);
                    lst->cons_set_len(++idx);
                }
            
                /* if there's a callback, invoke it */
                if (cb != 0)
                {
                    G_stk->push()->set_obj(fnobj);
                    G_interpreter->call_func_ptr(vmg_ cb, 1, rc, 0);
                }
            
                /* 
                 *   If we're doing a recursive listing, and this is a
                 *   directory, list its ocntents.  Skip self and parent
                 *   directory links ('.'  and '..' on Unix), since those
                 *   would cause infinite recursion.
                 */
                os_specfile_t st;
                if (recursive
                    && (st = os_is_special_file(curname)) != OS_SPECFILE_SELF
                    && st != OS_SPECFILE_PARENT)
                {
                    /* build the full path name */
                    char fullname[OSFNMAX];
                    os_build_full_path(fullname, sizeof(fullname),
                                       _lclfname, curname);

                    /* check to see if it's a directory */
                    unsigned long fmode;
                    unsigned long fattr;
                    if (osfmode(fullname, FALSE, &fmode, &fattr)
                        && (fmode & OSFMODE_DIR) != 0)
                    {
                        /* get the combined path from the FileName object */
                        const char *path = vm_objid_cast(CVmObjFileName, fnobj)
                                           ->get_path_string();
                
                        /* build the actual combined path */
                        char subfname[OSFNMAX];
                        os_build_full_path(
                            subfname, sizeof(subfname), _lclfname, curname);
                
                        /* do the recursive listing */
                        ok |= s_readdir_local(vmg_ subfname, path + VMB_LEN,
                                              retval, rc, cb, TRUE);
                    }
                }
            
                /* we're done with the FileName object for this iteration */
                G_stk->discard(1);
            }
        }
        err_finally
        {
            /* close the directory handle */
            os_close_dir(dirhdl);
        }
        err_end;
    }

    /* discard the gc protection for the list, if applicable */
    if (retval != 0)
        G_stk->discard(1);

    /* return the result */
    return ok;
}

/*
 *   Get a local directory listing, returning a list of FileName objects
 *   and/or invoking a callback for each file found.
 */
int CVmNetFile::readdir_local(VMG_ const char *nominal_path,
                              vm_val_t *retval,
                              const struct vm_rcdesc *rc,
                              const vm_val_t *cb, int recursive)
{
    /* verify that the path exists and refers to a directory */
    unsigned long mode;
    unsigned long attr;
    if (!osfmode(_lclfname, TRUE, &mode, &attr)
        || (mode & OSFMODE_DIR) == 0)
        return FALSE;

    /* 
     *   if the caller didn't specify a nominal path to use in constructed
     *   FileName objects, use the actual local path 
     */
    if (nominal_path == 0)
        nominal_path = _lclfname;

    /* call our static implementation with our local filename path */
    return s_readdir_local(vmg_ _lclfname, nominal_path,
                           retval, rc, cb, recursive);
}

/* ------------------------------------------------------------------------ */
/*
 *   Mark references for the garbage collector 
 */
void CVmNetFile::mark_refs(VMG_ uint state)
{
    /* if we have a _filespec object, mark it */
    if (_filespec != VM_INVALID_OBJ)
        G_obj_table->mark_all_refs(_filespec, state);
}

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
