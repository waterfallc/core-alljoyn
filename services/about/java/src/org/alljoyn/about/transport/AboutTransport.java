/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
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
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/

/**
 *
 */
package org.alljoyn.about.transport;

import java.util.Map;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.Variant;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;
import org.alljoyn.bus.annotation.BusProperty;
import org.alljoyn.bus.annotation.BusSignal;
import org.alljoyn.services.common.BusObjectDescription;

/**
 * Definition of the About BusInterface
 * @deprecated see org.alljoyn.bus.ifaces.About
 */
@Deprecated
@BusInterface (name = AboutTransport.INTERFACE_NAME)
public interface AboutTransport extends BusObject
{
    public static final String INTERFACE_NAME = "org.alljoyn.About";
    public final static String OBJ_PATH       = "/About";

    /**
     * @deprecated see org.alljoyn.bus.ifaces.About
     * @return the version of the protocol
     * @throws BusException indicating failure obtaining Version property
     */
    @Deprecated
    @BusProperty(signature="q")
    public short getVersion() throws BusException;


    /**
     * @deprecated see org.alljoyn.bus.ifaces.About
     * @param languageTag IETF language tags specified by  RFC 5646
     * @return all the configuration fields based on the language tag.
     *         See The list of known configuration fields in About interface for
     *         more details.
     * @throws BusException indicating failure making GetAboutData method call
     */
    @Deprecated
    @BusMethod(signature = "s", replySignature="a{sv}")
    public Map<String, Variant> GetAboutData(String languageTag) throws BusException;

    /**
     * @deprecated see org.alljoyn.bus.ifaces.About
     * Return the array of object paths and the list of all interfaces available
     * at the given object path.
     * @return the array of object paths and the list of all interfaces available
     *         at the given object path.
     * @throws BusException indicating failure making GetObjectDescription method call
     */
    @Deprecated
    @BusMethod(replySignature="a(oas)")
    public BusObjectDescription[] GetObjectDescription() throws BusException;

    /**
     * @deprecated see org.alljoyn.bus.ifaces.About
     * This signal is used to announce the list of all interfaces available at
     * the given object path.
     * @param version The interface version is added since it might help with the
     * decision to connect back.
     * @param port The global gateway port for the services for this application
     * @param objectDescriptions Descriptions of the BusObjects that make up the service. "a(sas)"
     * @param serviceMetadata Service specific key/value pairs. (Service implementers
     *                        are free to populate this dictionary with any key/value
     *                        pairs that are meaningful to the service and its
     *                        potential consumers) "a{sv}"
     */
    @Deprecated
    @BusSignal (signature="qqa(oas)a{sv}")
    public void Announce(short version, short port, BusObjectDescription[] objectDescriptions, Map<String,Variant> serviceMetadata);

}
