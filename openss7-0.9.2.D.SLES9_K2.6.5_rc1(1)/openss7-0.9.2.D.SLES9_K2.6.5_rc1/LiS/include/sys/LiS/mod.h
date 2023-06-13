/*****************************************************************************

 @(#) mod.h,v 1.1.1.4.4.8 2005/12/19 03:22:22 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2005  OpenSS7 Corporation <http://www.openss7.com>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 Ave, Cambridge, MA 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 -----------------------------------------------------------------------------

 Last Modified 2005/12/19 03:22:22 by brian

 *****************************************************************************/

/*                               -*- Mode: C -*- 
 * mod.h --- mod management 
 * Author          : Francisco J. Ballesteros
 * Created On      : Tue May 31 21:40:37 1994
 * Last Modified By: David Grothe
 * RCS Id          : : mod.h,v 1.5 1995/10/13 23:54:13 nemo Exp $
 * Purpose         : keep close all the mod related stuff.
 * ----------------______________________________________________
 *
 *    Copyright (C) 1995  Francisco J. Ballesteros,  Denis Froschauer
 *    Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
 *    You can reach us by email to any of
 *    nemo@ordago.uc3m.es, 100741.1151@compuserve.com
 */

#ifndef _MOD_H
#define _MOD_H 1

#ident "@(#) mod.h,v OpenSS7-0_9_2_D_rc2(1.1.1.4.4.8) 2005/12/19 03:22:22"

/*  -------------------------------------------------------------------  */
/*				 Dependencies                            */

#ifndef _STRPORT_H
#include <sys/strport.h>
#endif
#ifndef _LIS_CONFIG_H
#include <sys/strconfig.h>	/* config definitions */
#endif
#ifndef _SHARE_H
#include <sys/LiS/share.h>	/* streams shared defs */
#endif
#ifndef _STR_OPTS_H
#include <sys/stropts.h>	/* struct str_list */
#endif
#ifndef _SYS_SAD_H
#include <sys/sad.h>		/* autopush defs */
#endif

/*  -------------------------------------------------------------------  */
/*                                 Symbols                               */

#ifdef __KERNEL__
/* module open flags
 */
#define DRVOPEN		0x00	/* device open */
#define MODOPEN		0x01	/* plain module open */
#define CLONEOPEN	0x02	/* clone open -> pick new minor dev */

#endif				/* __KERNEL__ */

#ifdef LIS_OBJNAME		/* Some macros that do something useful with LIS_OBJNAME */

/*
 *  LIS_OBJNAME_STR expands to the quoted object file name,
 *  ready to be used as a string constant.
 */
#define LIS_OBJNAME_STR ___LIS_OBJNAME_XSTR(LIS_OBJNAME)
#define ___LIS_OBJNAME_XSTR(s) ___LIS_OBJNAME_STR(s)
#define ___LIS_OBJNAME_STR(s) #s

/*
 *  LIS_OBJNAME_PREFIX prefixes an identifier with "lis_<objname>_".
 *  This is handy for global identifiers.
 */
#define LIS_OBJNAME_PREFIX(name) ___LIS_OBJNAME_XCAT(name, LIS_OBJNAME)
#define ___LIS_OBJNAME_XCAT(name, objname) ___LIS_OBJNAME_CAT(name, objname)
#define ___LIS_OBJNAME_CAT(name, objname) lis_##objname##_##name

#endif				/* ifdef LIS_OBJNAME */

/*  -------------------------------------------------------------------  */
/*				    Types                                */

/*
 * The streamtab data structure. This one is used to find modules and
 * drivers entry points.
 * Actually this struct defines a module or driver.
 */

typedef struct streamtab {
	struct qinit *st_rdinit;	/* read queue */
	struct qinit *st_wrinit;	/* write queue */
	struct qinit *st_muxrinit;	/* mux read queue */
	struct qinit *st_muxwinit;	/* mux write queue */
} streamtab_t;

#ifdef __KERNEL__

/* Module ID number */
typedef unsigned short modID_t;

#define LIS_NULL_MID ((modID_t)0)

/*
 *  Module information structure
 */
typedef struct lis_module_info {
	modID_t mi_idnum;		/* module id number */
	const char *mi_idname;		/* module name */
	long mi_minpsz;			/* min packet size accepted */
	long mi_maxpsz;			/* max packet size accepted */
	ulong mi_hiwat;			/* hi-water mark */
	ulong mi_lowat;			/* lo-water mark */
} lis_module_info_t;

/*
 *  Linux 2.1 and later declares a struct module_info too,
 *  but this is hidden by including this file.
 */
#define module_info   lis_module_info
#define module_info_t lis_module_info_t

/*
 *  Per-Module statistic record
 */

typedef struct module_stat {
	char *ms_xptr;			/* pointer to private statistics */
	short ms_xsize;			/* length of private statistics buffer */
	uint ms_flags;			/* bool stats -- for future use */
#ifndef LIS_ATOMIC_STATS
	long ms_pcnt;			/* count of calls to put proc */
	long ms_scnt;			/* count of calls to service proc */
	long ms_ocnt;			/* count of calls to open proc */
	long ms_ccnt;			/* count of calls to close proc */
	long ms_acnt;			/* count of calls to admin proc */
#else
/* use non-standard but MP-safe statistics */
	lis_atomic_t ms_pcnt;		/* count of calls to put proc */
	lis_atomic_t ms_scnt;		/* count of calls to service proc */
	lis_atomic_t ms_ocnt;		/* count of calls to open proc */
	lis_atomic_t ms_ccnt;		/* count of calls to close proc */
	lis_atomic_t ms_acnt;		/* count of calls to admin proc */
#endif
} module_stat_t;

typedef struct fmodsw {
	struct streamtab *f_str;
	ushort f_count;			/* open count */
	short f_flags;			/* module/driver flags */
	char f_name[LIS_NAMESZ + 1];
	char f_objname[LIS_NAMESZ + 1];
	int f_state;			/* state of module */
	lis_semaphore_t f_sem;		/* to synchronize loading */
	int f_qlock_option;		/* for initializing queues */
} fmodsw_t;

#define LIS_MODFLG_CLONE   0x0001	/* module is 'clone' */
#define LIS_MODFLG_FIFO    0x0002	/* module is 'fifo' */
#define LIS_MODFLG_REOPEN  0x0004	/* cloned minors can reopen */

/*
 * States
 */
#define LIS_MODSTATE_MASK	0x0FF	/* isolates state variable */
#define	LIS_MODSTATE_LINKED	0	/* linked in with LiS */
#define	LIS_MODSTATE_UNLOADED	1	/* not loaded */
#define	LIS_MODSTATE_LOADING	2	/* loading */
#define	LIS_MODSTATE_LOADED	3	/* loaded */
#define	LIS_MODSTATE_UNKNOWN	4	/* not known yet */

#define LIS_MODSTATE_INITED	0x100	/* initialized */

#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */
/*				 Glob. Vars.                             */

#ifdef __KERNEL__

extern struct fmodsw lis_fstr_sw[MAX_STRDEV];	/* streams devices */
extern struct fmodsw lis_fmod_sw[MAX_STRMOD];	/* streams modules */

#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */
/*			Exported functions & macros                      */

#ifdef __KERNEL__

/* Register and unregister streams modules and drivers */
extern modID_t _RP lis_register_strmod(struct streamtab *strtab, const char *name);
extern int _RP lis_unregister_strmod(struct streamtab *strtab);
extern int _RP lis_register_strdev(major_t major, struct streamtab *strtab, int nminor,
				   const char *name);
extern int _RP lis_unregister_strdev(major_t major);
extern int _RP lis_register_driver_qlock_option(major_t major, int qlock_option);
extern int _RP lis_register_module_qlock_option(modID_t id, int qlock_option);

/* Back compatible: Will go away when no longer used */
#define	register_strdev		lis_register_strdev

#ifdef __LIS_INTERNAL__
/* Find streamtab of a device */
extern streamtab_t *lis_find_strdev(major_t major);
#endif

/* Find/load a module id by name */
extern modID_t _RP lis_findmod(const char *name);

#ifdef __LIS_INTERNAL__
extern modID_t lis_loadmod(const char *name);
extern modID_t lis_findmod_strtab(struct streamtab *strtab);
#endif

/* Autopush */
#ifdef __LIS_INTERNAL__
extern int lis_apushm(dev_t dev, const char *mods[]);
#endif
extern int _RP lis_apush_set(struct strapush *ap);
extern int _RP lis_apush_get(struct strapush *ap);
extern int _RP lis_apush_vml(struct str_list *mlp);

/* mod.c initialization and cleanup functions */
#ifdef __LIS_INTERNAL__
extern void lis_init_mod(void);
extern void lis_terminate_mod(void);
#endif

/* Get strtab for mod or NULL if not a valid id */
#define lis_modstr(i)		( ((i) <= 0) ? NULL : lis_fmod_sw[i].f_str )

/* Get the streamtab for a streams device */
#define LIS_DEVST(majnum)	lis_fstr_sw[majnum]

/* This is to check for a valid STREAMS device */
#define LIS_DEVOK(majnum)	(lis_fstr_sw[majnum].f_str!=NULL)

/* for checking/setting f_flag */
#define LIS_MODFLAGS(i)           lis_fmod_sw[i].f_flags
#define LIS_DEVFLAGS(m)           lis_fstr_sw[m].f_flags

#define LIS_DEV_IS_CLONE(m) \
    (LIS_DEVOK(m) && (LIS_DEVFLAGS(m) & LIS_MODFLG_CLONE))
#define LIS_DEV_IS_FIFO(m) \
    (LIS_DEVOK(m) && (LIS_DEVFLAGS(m) & LIS_MODFLG_FIFO))
#define LIS_DEV_CAN_REOPEN(m) \
    (LIS_DEVOK(m) && (LIS_DEVFLAGS(m) & LIS_MODFLG_REOPEN))

/* This will check what kind of device is this, there're a few predefined
 * streams drivers -- see LIS_ symbols in the kernel major number defs.
 * (linux/major.h for Linux)
 */
#define LIS_ISDEV(majnum,kind)	((majnum)==(kind))

#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */
#endif				/* !_MOD_H */
