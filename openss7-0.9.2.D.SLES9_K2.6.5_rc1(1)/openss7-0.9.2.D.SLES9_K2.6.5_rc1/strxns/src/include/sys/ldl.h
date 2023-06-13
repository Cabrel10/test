/*****************************************************************************

 @(#) ldl.h,v 0.9.2.5 2006/03/04 04:34:29 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; version 2 of the License.

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

 Last Modified 2006/03/04 04:34:29 by brian

 -----------------------------------------------------------------------------

 ldl.h,v
 Revision 0.9.2.5  2006/03/04 04:34:29  brian
 - corrections for FC4 x86_64 build

 Revision 0.9.2.4  2006/03/03 11:27:48  brian
 - 32/64-bit compatibility

 *****************************************************************************/

#ifndef SYS_LDL_H
#define SYS_LDL_H

#ident "@(#) ldl.h,v OpenSS7-0_9_2_D_rc2(0.9.2.5) Copyright (c) 2001-2006 OpenSS7 Corporation."

/*
 *  Driver device numbering
 */
#if 0				/* Now defined in the LiS configuration */
#define LDL_MAJOR	58	/* Driver major device number */
#endif
#define LDL_N_MINOR	256	/* Number of minors */

/*
 *  Per stream flags
 *
 *  If the flag LDLFLAG_PEDANTIC_STANDARD is in use, ldl will conform
 *  to some alternate interpretations of the DLPI standard.
 *  You should not set this flag unless you have problems with the
 *  normal interpretation of the DLPI standard.
 *  The changed behavior is:
 *   1) DL_INFO_ACK: For the dl_sap_length field, the standard says:
 *	"A zero value indicates that no SAP has yet been bound".
 *	This is interpreted by some people as "The dl_sap_length field
 *	should be 0 if not in the DL_IDLE state".
 *	With this flag, ldl will return the negative SAP length only
 *	when the interface is in the DL_IDLE state. In all other states
 *	0 is returned.
 *	Without this flag, ldl will return the negative SAP length unless
 *	the interface is in the DL_UNATTACHED state. If in the
 *	DL_UNATTACHED state, the sap length is not known, and 0 is returned.
 *
 *  If the flag LDLFLAG_RAW is in use, the entire frame (excluding the
 *  leading 8 bytes preample + Start Frame Delimiter and the trailing 4
 *  bytes Frame Check Sequence) is delivered to the DLPI user on unitdata
 *  reception. Otherwise, only the data portion of the frame is delivered
 *  to the DLPI user on unitdata reception (but parts of the frame header
 *  (source/destination address and SAP) may be supplied in the DL_UNITDATA_IND
 *  portion of the unitdata indication). On transmission an entire frame
 *  including frame header is expected if the flag LDLFLAG_RAW is in use,
 *  but the frame header may be altered if the DL_UNITDATA_REQ portion of the
 *  request contains another data link destination address or SAP than given in
 *  the frame header, or if the DLPI provider was bound to another SAP than the
 *  frame header SSAP.
 */
#define LDLFLAG_PEDANTIC_STANDARD	0x0002
#define LDLFLAG_RAW			0x0004

/*
 *  Default flags
 */
#define LDLFLAG_DEFAULT	0

/*
 *  Ethernet framing types
 *  Add one of these to the PPA when attaching.
 *
 *  LDL_FRAME_EII
 *    This is the default. If you don't know what frame type you are using on
 *    your ethernet, you are probably using Ethernet II.
 *    The frame header is defined as:
 *	unsigned char destination[6];
 *	unsigned char source[6];
 *	unsigned short type;
 *    There is no distinction between source and destination SAP, both source
 *    and destination SAP is the two byte "type" field of the frame header in
 *    network order.
 *    Thus, the DLSAP length is 8 bytes (6 bytes address + 2 bytes SAP).
 *
 *  LDL_FRAME_802_2
 *    NOTE: The semantics of this frame type when running in raw mode has
 *          temporarily been changed to work like the LDL_FRAME_RAW_LLC
 *          frame type (non-UI frames passed up). That will be changed back
 *          when the LDL_FRAME_RAW_LLC frame type has been implemented.
 *    This is the IEEE-compliant Logical Link Control frame type.
 *    The frame header is defined as:
 *	unsigned char destination[6];
 *	unsigned char source[6];
 *	unsigned short length;
 *	unsigned char DSAP;
 *	unsigned char SSAP;
 *	unsigned char control;
 *    Note that only the unnumbered format (control == 0x03) is supported.
 *    All frames received with control != 0x03 are silently discarded, and
 *    the control field is set to 0x03 on all transmitted frames.
 *    The DSAP is the destination SAP and must match the bound SAP on received
 *    frames. The SSAP field is the source SAP and is set to the bound SAP on
 *    unitdata transmission.
 *    Thus, the DLSAP length is 7 bytes (6 bytes address + 1 byte SAP).
 *
 *  LDL_FRAME_802_3
 *    This is ugly and non-standard. Avoid it if at all possible.
 *    Is is used for the IPX protocol in raw 802.3 encapsulation. While
 *    similar to a true 802.3 compliant frame, it lacks the Logical Link
 *    Control information in the frame header. Fortunately this datalink
 *    encapsulation of the IPX packets can only be used if IPX packet checksums
 *    are disabled, and an IPX packet without checksums has the value 0xffff in
 *    the checksum field (the first two bytes of the packet just after the
 *    frame header). By looking at this checksum field we know when we have a
 *    frame of this type.
 *    The frame header is defined as:
 *	unsigned char destination[6];
 *	unsigned char source[6];
 *	unsigned short length;
 *     (unsigned short checksum; { Not frame header, but start of data } )
 *    There is no SAP for this frame type.
 *    Thus, the DLSAP length is 6 bytes (6 bytes address + 0 bytes SAP).
 *    The DL_SUBS_BIND_REQ primitive is not supported for this frame type.
 *
 *  LDL_FRAME_SNAP
 *    This is similar to the 802.2 frame type. In fact this is a 802.2 LLC
 *    frame type with DSAP == SSAP == 0xaa.
 *    The frame header is defined as:
 *	unsigned char destination[6];
 *	unsigned char source[6];
 *	unsigned short length;
 *	unsigned char DSAP;
 *	unsigned char SSAP;
 *	unsigned char control;
 *	unsigned char OUI[3];
 *	unsigned short type;
 *    As with 802.2, the control field must be 0x03. The DSAP and SSAP fields
 *    must contain 0xaa. The OUI field is a three byte organization code that
 *    denotes the organization that assigned the type field.
 *    There is no distinction between source and destination SAP, both source
 *    and destination SAP is the three byte OUI followed by the two byte type
 *    field of the frame header in network order.
 *    Thus, the DLSAP length is 11 bytes (6 bytes address + 5 bytes SAP).
 *    But a 5 byte SAP address cannot be specified with the DL_BIND_REQ
 *    primitive. To get around this restriction the following scheme is used:
 *	- The OUI defaults to three bytes of zero.
 *	- The OUI part of the SAP is never specified with the DL_BIND_REQ
 *	  primitive and the peer binding mode of the DL_SUBS_BIND_REQ
 *	  primitive.
 *	- The OUI part of all bindings are set by the hierarchical binding
 *	  mode of the DL_SUBS_BIND_REQ primitive.
 *
 *  LDL_FRAME_RAW_LLC
 *    NOTE: This frame type has not yet been implemented for non-raw mode.
 *          It currently works exactly like LDL_FRAME_802_2.
 *    This is a variant of LDL_FRAME_802_2 that can handle non-UI frames.
 *    The frame header is defined as:
 *      unsigned char destination[6];
 *      unsigned char source[6];
 *      unsigned short length;
 *      unsigned char DSAP;
 *      unsigned char SSAP;
 *      unsigned char control;
 *    The last three header fields (DSAP, SSAP and control) are also part of
 *    the datagram.
 *    The DLP user should set the third byte of the datagram to the desired
 *    LLC control field value when using the DL_UNITDATA_REQ primitive.
 *    With the DL_UNITDATA_IND primitive, any LLC control field value can
 *    be returned. The DLP user should check this field.
 *    The DSAP is the destination SAP and must match the bound SAP on received
 *    frames. The SSAP field is the source SAP and is set to the bound SAP on
 *    unitdata transmission.
 *    Thus, the DLSAP length is 7 bytes (6 bytes address + 1 byte SAP).
 *
 *
 */
#define LDL_FRAME_MASK         0x00f00000	/* Mask for framing */
#define LDL_FRAME_EII          0x00000000	/* Ethernet 2 framing (default) */
#define LDL_FRAME_802_2        0x00100000	/* IEEE 802.2 LLC framing */
#define LDL_FRAME_SNAP         0x00200000	/* Sub-Network Access Protocol */
#define LDL_FRAME_802_3        0x00300000	/* Ulgy IPX over raw 802.3 framing */
#define LDL_FRAME_RAW_LLC      0x00400000	/* Raw LLC frames */

/*
 *  IOCTLs
 *
 *  These come in the ic_cmd field of a struct strioctl pointed to by an I_STR
 *  type of ioctl.
 */
#define LDL_IOCTL	('L' << 8)
#define LDL_GETLSTATS	(LDL_IOCTL + 240)	/* ic_dp -> ldl_lstats_ioctl */
#define LDL_GETGSTATS	(LDL_IOCTL + 241)	/* ic_dp -> ldl_gstats_ioctl */
#define LDL_SETDEBUG	(LDL_IOCTL + 242)	/* ic_dp -> long */
#define LDL_SETFLAGS	(LDL_IOCTL + 243)	/* ic_dp -> ldl_flags_ioctl */
#define LDL_FINDPPA	(LDL_IOCTL + 244)	/* ic_dp -> long */
#define LDL_GETNAME	(LDL_IOCTL + 245)	/* ic_dp -> name */

/* Structure used for LDL_SETFLAGS IOCTL */
struct ldl_flags_ioctl {
	unsigned long flags;
	unsigned long mask;
};

typedef struct ldl_gstats_ioctl {	/* global statistics *//* for entire driver */
	long attach_req_cnt;
	long detach_req_cnt;
	long bind_req_cnt;
	long unbind_req_cnt;
	long subs_bind_req_cnt;
	long subs_unbind_req_cnt;
	long udqos_req_cnt;
	long ok_ack_cnt;
	long error_ack_cnt;
	long unitdata_req_cnt;		/* including M_DATA */
	long unitdata_req_q_cnt;	/* including M_DATA */
	long unitdata_ind_cnt;		/* including M_DATA */
	long unitdata_q_cnt;		/* including M_DATA */
	long unitdata_drp_cnt;		/* including M_DATA */
	long uderror_ind_cnt;
	long ioctl_cnt;
	long net_rx_cnt;		/* # pkts from below */
	long net_rx_drp_cnt;		/* usually mem-alloc fail */
	long net_tx_cnt;		/* # pkts xmitted */
	long net_tx_fail_cnt;
} ldl_gstats_ioctl_t;

typedef struct ldl_lstats_ioctl {	/* local statistics *//* per stream */
	int to_be_done;
} ldl_lstats_ioctl_t;

#ifdef __LP64__
struct ldl_flags_ioctl32 {
	u_int32_t flags;
	u_int32_t mask;
};

typedef struct ldl_gstats_ioctl32 {	/* global statistics *//* for entire driver */
	int32_t attach_req_cnt;
	int32_t detach_req_cnt;
	int32_t bind_req_cnt;
	int32_t unbind_req_cnt;
	int32_t subs_bind_req_cnt;
	int32_t subs_unbind_req_cnt;
	int32_t udqos_req_cnt;
	int32_t ok_ack_cnt;
	int32_t error_ack_cnt;
	int32_t unitdata_req_cnt;	/* including M_DATA */
	int32_t unitdata_req_q_cnt;	/* including M_DATA */
	int32_t unitdata_ind_cnt;	/* including M_DATA */
	int32_t unitdata_q_cnt;		/* including M_DATA */
	int32_t unitdata_drp_cnt;	/* including M_DATA */
	int32_t uderror_ind_cnt;
	int32_t ioctl_cnt;
	int32_t net_rx_cnt;		/* # pkts from below */
	int32_t net_rx_drp_cnt;		/* usually mem-alloc fail */
	int32_t net_tx_cnt;		/* # pkts xmitted */
	int32_t net_tx_fail_cnt;
} ldl_gstats_ioctl32_t;

typedef struct ldl_lstats_ioctl32 {	/* local statistics *//* per stream */
	int32_t to_be_done;
} ldl_lstats_ioctl32_t;
#endif

/*
 * Debug bits
 */
#define	LDL_DEBUG_RCV_FUNC	0x00000001	/* raw frm from net driver */
#define	LDL_DEBUG_UDIND		0x00000002	/* UNITDATA_IND upstream */
#define	LDL_DEBUG_UDREQ		0x00000004	/* UNITDATA_REQ downstream */
#define	LDL_DEBUG_TX		0x00000008	/* Tx to net driver */
#define	LDL_DEBUG_ALLDATA	0x00000010	/* dmp full bfr, not hdr */
#define	LDL_DEBUG_ATTACH	0x00000020	/* debug info for DL_ATTACH */
#define	LDL_DEBUG_BIND		0x00000040	/* debug info for DL_BIND */

#endif				/* SYS_LDL_H */
