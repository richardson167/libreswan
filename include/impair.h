/* impair operation, for libreswan
 *
 * Copyright (C) 2018-2020 Andrew Cagney
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <https://www.gnu.org/licenses/gpl2.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 */

#ifndef IMPAIR_H
#define IMPAIR_H

#include <stdbool.h>

#include "lswcdefs.h"

struct fd;
struct logger;

/*
 * Meddle with the contents of a payload.
 */

enum send_impairment {
	SEND_NORMAL = 0,
	SEND_OMIT,
	SEND_EMPTY,
	SEND_DUPLICATE,
	SEND_ROOF, /* >= ROOF -> <number> */
};

/*
 * Meddle with a specific exchange.
 */

enum exchange_impairment {
	NO_EXCHANGE = 0,
	NOTIFICATION_EXCHANGE,
	QUICK_EXCHANGE,
	XAUTH_EXCHANGE,
	DELETE_EXCHANGE,
};

/*
 * What can be impaired.
 *
 * See impair.c for documentation.
 *
 * XXX: make this a structure so it can be copied?
 */

struct impair {

	bool revival;
	bool emitting;

	/*
	 * IKEv1: the key-length attribute is at the same level as
	 * other attributes such as encryption.  Just need to know if
	 * the IKE, or CHILD proposal set should be manipulated.
	 *
	 * IKEv2: the key-length attribute is nested within an
	 * encryption transform.  Hence, also need to know which
	 * transform to screw with.
	 */

	enum send_impairment ke_payload;
	enum send_impairment ike_key_length_attribute;
	enum send_impairment child_key_length_attribute;

	unsigned log_rate_limit;

	enum send_impairment v1_hash_payload;
	enum exchange_impairment v1_hash_exchange;
	bool v1_hash_check;

	unsigned ike_initiator_spi;
	unsigned ike_responder_spi;

	bool bust_mi2;
	bool bust_mr2;
	bool drop_i2;
	bool sa_creation;
	bool jacob_two_two;
	bool allow_null_none;
	bool major_version_bump;
	bool minor_version_bump;
	bool timeout_on_retransmit;
	bool delete_on_retransmit;
	bool suppress_retransmits;
	bool send_bogus_payload_flag;
	bool send_bogus_isakmp_flag;
	bool send_no_delete;
	bool send_no_ikev2_auth;
	bool send_no_xauth_r0;
	bool drop_xauth_r0;
	bool send_no_main_r2;
	bool force_fips;
	bool send_key_size_check;
	bool send_bogus_dcookie;
	bool omit_hash_notify_request;
	bool ignore_hash_notify_request;
	bool ignore_hash_notify_response;
	bool ikev2_exclude_integ_none;
	bool ikev2_include_integ_none;
	bool replay_duplicates;
	bool replay_forward;
	bool replay_backward;
	bool replay_encrypted;
	bool corrupt_encrypted;
	bool proposal_parser;
	bool add_unknown_payload_to_sa_init;
	bool add_unknown_payload_to_auth;
	bool add_unknown_payload_to_auth_sk;
	bool unknown_payload_critical;
	bool allow_dns_insecure;
	bool send_pkcs7_thingie;
	bool ikev1_del_with_notify;
	bool bad_ike_auth_xchg;
	bool rekey_initiate_supernet;
	bool rekey_initiate_subnet;
	bool rekey_respond_supernet;
	bool rekey_respond_subnet;

	bool tcp_use_blocking_write;
	bool tcp_skip_setsockopt_espintcp;

	/*
	 * add more here
	 */

};

extern struct impair impair;

/*
 * What whack sends across the wire for a impair.
 */

struct whack_impair {
	unsigned what;
	unsigned how;
};

enum impair_status {
	IMPAIR_OK = 1,
	IMPAIR_HELP,
	IMPAIR_ERROR,
};

enum impair_status parse_impair(const char *optarg, struct whack_impair *whack_impair, bool enable, const char *progname);

enum impair_action {
	IMPAIR_UPDATE,
	INITIATE_v2_DELETE,
	INITIATE_v2_LIVENESS,
	INITIATE_v2_REKEY,
	CALL_GLOBAL_EVENT,
	CALL_STATE_EVENT,
};

bool process_impair(const struct whack_impair *whack_impair,
		    void (*action)(enum impair_action, unsigned what,
				   unsigned how, bool background,
				   struct fd *whackfd),
		    bool background, struct fd *whackfd,
		    struct logger *logger);

bool have_impairments(void);
void jam_impairments(jambuf_t *buf, const char *sep);

#endif
