#ifndef _CNG_CACHE_H
#define _CNG_CACHE_H
/**
 * @file
 *
 * This file defines the CngCache object used to manage the lifetime of global CNG handles.
 */

/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <windows.h>
#include <bcrypt.h>

#include <qcc/platform.h>
#include <qcc/Mutex.h>
#include <qcc/Crypto.h>

namespace qcc {

/**
 * This struct manages the lifetime of algorithm handles.
 */
struct CngCache {
    CngCache();
    ~CngCache();

    /**
     * Delete the opened algorithm handles.
     */
    void Cleanup();

    /**
     * Open the AES-CCM provider handle.
     *
     * @return #ER_OK if the handle was successfully opened
     *         other error indicating failure
     */
    QStatus OpenCcmHandle();

    /**
     * Open the AES-ECB provider handle.
     *
     * @return #ER_OK if the handle was successfully opened
     *         other error indicating failure
     */
    QStatus OpenEcbHandle();

    /**
     * Open a hash algorithm provider handle.
     *
     * @param algorithm Hash algorithm
     * @param usingMac  true if using MAC, false otherwise
     *
     * @return #ER_OK if the handle was successfully opened
     *         other error indicating failure
     */
    QStatus OpenHashHandle(qcc::Crypto_Hash::Algorithm algorithm, bool usingMac);

    /**
     * Open an elliptic curve Digital Signature Algorithm (ECDSA) provider handle.
     *
     * @param curveType Curve type
     *
     * @return #ER_OK if the handle was successfully opened
     *         other error indicating failure
     */
    QStatus OpenEcdsaHandle(uint8_t curveType);

    /**
     * Open an elliptic curve Diffie-Hellman key exchange provider handle.
     *
     * @param curveType Curve type
     *
     * @return #ER_OK if the handle was successfully opened
     *         other error indicating failure
     */
    QStatus OpenEcdhHandle(uint8_t curveType);


    /**
     * Number of supported algorithms as defined by Crypto_Hash::Algorithm enum
     */
    static const int ALGORITHM_COUNT = qcc::Crypto_Hash::TOTAL_ALGORITHMS_COUNT;

    /**
     * Number of supported curves for ECDSA or ECDH.
     * This should be equal to the number of
     * ECC_* constants defined in the Crypto_ECC
     * class in CryptoECC.h (i.e. ECC_NIST_P256 etc.)
     */
    static const int ECDSA_ALGORITHM_COUNT = 1;

    static const int ECDH_ALGORITHM_COUNT = 1;

    BCRYPT_ALG_HANDLE algHandles[ALGORITHM_COUNT][2];
    BCRYPT_ALG_HANDLE ccmHandle;
    BCRYPT_ALG_HANDLE ecbHandle;
    BCRYPT_ALG_HANDLE ecdsaHandles[ECDSA_ALGORITHM_COUNT];
    BCRYPT_ALG_HANDLE ecdhHandles[ECDH_ALGORITHM_COUNT];

  private:

    Mutex* m_mutex;
};

extern CngCache& cngCache;

} // qcc

#endif
