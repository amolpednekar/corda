/*
 * Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _EVENT_STRINGS_H_
#define _EVENT_STRINGS_H_
typedef enum {
    SGX_EVENT_EPID_PROV_FAILURE,                  /*network*/
    SGX_EVENT_EPID_BLOB_PERSISTENT_STROAGE_FAILURE,
    SGX_EVENT_PID_SIGNATURE_FAILURE,
    SGX_EVENT_PSE_CERT_PROV_FAILURE,              
    SGX_EVENT_OCSP_FAILURE,                       
    SGX_EVENT_DAL_COMM_FAILURE,                   /*communication*/
    SGX_EVENT_DISABLED,
    SGX_EVENT_SERVICE_UNAVAILABLE,
    SGX_EVENT_AESM_EXIT,
    SGX_EVENT_PSE_CERT_REVOCATION,
    SGX_EVENT_ME_EPID_GROUP_REVOCATION,
    SGX_EVENT_EPID_REVOCATION,                    /*SigRL fail*/
    SGX_EVENT_EPID_INTEGRITY_ERROR,
    SGX_EVENT_LTP_BLOB_INTEGRITY_ERROR,
    SGX_EVENT_LTP_BLOB_INVALID_ERROR,
    SGX_EVENT_EPID11_SIGRL_INTEGRITY_ERROR,
    SGX_EVENT_EPID11_PRIVRL_INTEGRITY_ERROR,
    SGX_EVENT_EPID20_SIGRL_INTEGRITY_ERROR,
    SGX_EVENT_EPID11_RL_RETRIEVAL_FAILURE,
    SGX_EVENT_EPID_PROV_INTEGRITY_ERROR,
    SGX_EVENT_PSE_CERT_PROV_INTEGRITY_ERROR,
    SGX_EVENT_OCSP_RESPONSE_ERROR,                /*based on non-crypto checks that AESM does*/
    SGX_EVENT_DAL_SIGMA_ERROR,                    /*DAL returns error during Sigma protocol*/
    SGX_EVENT_LTP_FAILURE,                        /*other than above*/
    SGX_EVENT_DAL_NOT_AVAILABLE_ERROR,            
    SGX_EVENT_EPID11_GROUP_CERT_PROV_ERROR,
    SGX_EVENT_PSE_CERT_PROV_PROTOCOL_RESPONSE_FAILURE,
    SGX_EVENT_PSE_CERT_PROV_GENERAL_RESPONSE_FAILURE,
    SGX_EVENT_ME_EPID_SIG_REVOCATION,
    SGX_EVENT_ME_EPID_KEY_REVOCATION,
    SGX_EVENT_SIGMA_S2_INTEGRITY_ERROR,
    SGX_EVENT_DAL_SERVICE_ERROR,
    SGX_EVENT_PSE_ATTESTATION_ERROR
}sgx_event_string_table_t;

extern const char* g_event_string_table[];
#endif

