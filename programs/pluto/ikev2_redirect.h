/*
 * IKEv2 Redirect Mechanism (RFC 5685) related functions
 *
 * Copyright (C) 2018 Vukasin Karadzic <vukasin.karadzic@gmail.com>
 * Copyright (C) 2019 D. Hugh Redelmeier <hugh@mimosa.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#ifndef _IKEV2_REDIRECT_H
#define _IKEV2_REDIRECT_H

#include "packet.h"

extern enum allow_global_redirect global_redirect;
extern char *global_redirect_to;

/*
 * Check whether we received v2N_REDIRECT_SUPPORTED (in IKE_SA_INIT request),
 * and if we did, send a response with REDIRECT payload (without creating state -
 * just as in COOKIE case).
 *
 * @param md message digest of IKE_SA_INIT request.
 * @return bool TRUE if redirection is a MUST, FALSE otherwise.
 */
extern bool redirect_global(struct msg_digest *md);

/*
 * Emit IKEv2 Notify REDIRECTED_FROM payload.
 *
 * @param ip_addr IP Address of the previous gateway.
 * @param pbs output stream
 */
extern bool emit_redirected_from_notification(
		const ip_address *ip_addr,
		pb_stream *pbs);

/*
 * Emit IKEv2 Notify REDIRECT payload.
 *
 * @param destination string of IPv4/IPv6/FQDN address.
 * @param pbs output stream
 */
extern bool emit_redirect_notification(
		const char *destination,
		pb_stream *pbs);

/*
 * Extract needed information from IKEv2 Notify Redirect
 * notification.
 *
 * @param data that was transferred in v2_REDIRECT Notify
 * @param char* list of addresses we accept being redirected
 * 	  to, specified with conn option accept-redirect-to
 * @param nonce that was send in IKE_SA_INIT request,
 * 	  we need to compare it with nonce data sent
 * 	  in Notify data. We do all that only if
 * 	  nonce isn't NULL.
 * @param redirect_ip ip address we need to redirect to
 * @return err_t NULL if everything went right,
 * 		 otherwise (non-NULL)  what went wrong
 */
extern err_t parse_redirect_payload(pb_stream *input,
				    const char *allowed_targets_list,
				    const chunk_t *nonce,
				    ip_address *redirect_ip /* result */);

/*
 * Initiate via initiate_connection new IKE_SA_INIT exchange
 */
extern void initiate_redirect(struct state *st);

/*
 * Send IKEv2 INFORMATIONAL exchange with REDIRECT payload.
 * This is the case of redirection during the active tunnel.
 */
extern void send_active_redirect_in_informational(struct state *st);

stf_status process_IKE_SA_INIT_v2N_REDIRECT_response(struct ike_sa *ike,
						     struct child_sa *child,
						     struct msg_digest *md);

#endif
