#ifndef _STUNATTRIBUTEICECHECKFLAG_H
#define _STUNATTRIBUTEICECHECKFLAG_H
/**
 * @file
 *
 * This file defines the ICE-CHECK-FLAG STUN message attribute.
 */

/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#ifndef __cplusplus
#error Only include StunAddr/StunAttributeIceCheckFlag.h in C++ code.
#endif

#include <string>
#include <qcc/platform.h>
#include <StunAttributeBase.h>
#include <types.h>
#include <alljoyn/Status.h>

/** @internal */
#define QCC_MODULE "STUN_ATTRIBUTE"

/**
 * Use Candidate STUN attribute class.
 */
class StunAttributeIceCheckFlag : public StunAttribute {
  private:

  public:
    /**
     * This constructor just sets the attribute type to STUN_ATTR_ICE_CHECK.
     */
    StunAttributeIceCheckFlag(void) :
        StunAttribute(STUN_ATTR_ICE_CHECK_FLAG, "ICE-CHECK-FLAG") { }

    uint16_t AttrSize(void) const { return 0; /* Attribute has no data */ }
};


#undef QCC_MODULE
#endif