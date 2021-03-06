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

#ifndef ALLJOYN_SECMGR_STORAGE_NATIVESTORAGESETTINGS_H_
#define ALLJOYN_SECMGR_STORAGE_NATIVESTORAGESETTINGS_H_

#define GROUPS_TABLE_NAME "GROUPS"
#define IDENTITY_TABLE_NAME "IDENTITIES"
#define CLAIMED_APPS_TABLE_NAME "CLAIMED_APPLICATIONS"
#define IDENTITY_CERTS_TABLE_NAME "IDENTITY_CERTS"
#define MEMBERSHIP_CERTS_TABLE_NAME "MEMBERSHIP_CERTS"
#define SERIALNUMBER_TABLE_NAME "SERIALNUMBER"

#define GROUPS_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " GROUPS_TABLE_NAME \
    " (\
        AUTHORITY  BLOB NOT NULL,\
        ID         TEXT NOT NULL,\
        NAME       TEXT,\
        DESC       TEXT,\
        PRIMARY KEY(ID)\
); "

#define IDENTITY_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " IDENTITY_TABLE_NAME \
    " (\
        AUTHORITY BLOB NOT NULL,\
        ID        TEXT NOT NULL,\
        NAME      TEXT,\
        PRIMARY KEY(ID)\
); "

#define CLAIMED_APPLICATIONS_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " CLAIMED_APPS_TABLE_NAME \
    " (\
        APPLICATION_PUBKEY BLOB PRIMARY KEY    NOT NULL,\
        APP_NAME   TEXT,    \
        DEV_NAME   TEXT,    \
        USER_DEF_NAME   TEXT,\
        MANIFEST BLOB,\
        POLICY BLOB,\
        SYNC_STATE INTEGER\
); "

#define IDENTITY_CERTS_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " IDENTITY_CERTS_TABLE_NAME \
    " (\
        SUBJECT_KEYINFO  BLOB NOT NULL,\
        ISSUER BLOB NOT NULL,\
        DER BLOB NOT NULL,\
        GUID TEXT NOT NULL,\
        PRIMARY KEY(SUBJECT_KEYINFO),\
        FOREIGN KEY(SUBJECT_KEYINFO) REFERENCES " CLAIMED_APPS_TABLE_NAME \
    " (APPLICATION_PUBKEY) ON DELETE CASCADE,\
        FOREIGN KEY(GUID) REFERENCES " IDENTITY_TABLE_NAME " (ID) ON DELETE CASCADE ); "

#define MEMBERSHIP_CERTS_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " MEMBERSHIP_CERTS_TABLE_NAME \
    " (\
        SUBJECT_KEYINFO BLOB NOT NULL,\
        ISSUER BLOB NOT NULL,\
        DER BLOB NOT NULL,\
        GUID TEXT NOT NULL,\
        PRIMARY KEY(SUBJECT_KEYINFO, GUID),\
        FOREIGN KEY(SUBJECT_KEYINFO) REFERENCES " CLAIMED_APPS_TABLE_NAME \
    " (APPLICATION_PUBKEY) ON DELETE CASCADE\
        FOREIGN KEY(GUID) REFERENCES " GROUPS_TABLE_NAME " (ID) ON DELETE CASCADE ); "

#define SERIALNUMBER_TABLE_SCHEMA \
    "CREATE TABLE IF NOT EXISTS " SERIALNUMBER_TABLE_NAME \
    " (\
        VALUE INT\
); "

#define DEFAULT_PRAGMAS \
    "PRAGMA encoding = \"UTF-8\";\
    PRAGMA foreign_keys = ON;\
    PRAGMA journal_mode = OFF; "

#endif /* ALLJOYN_SECMGR_STORAGE_NATIVESTORAGESETTINGS_H_ */
