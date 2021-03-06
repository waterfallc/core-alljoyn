#ifndef _ALLJOYN_AUTHLISTENER_H
#define _ALLJOYN_AUTHLISTENER_H
/**
 * @file
 * This file defines the AuthListener class that provides the interface between
 * authentication mechanisms and applications.
 */

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

#ifndef __cplusplus
#error Only include AuthListener.h in C++ code.
#endif

#include <qcc/platform.h>
#include <qcc/String.h>

#include <alljoyn/Message.h>

#include <alljoyn/Status.h>

namespace ajn {

/**
 * Class to allow authentication mechanisms to interact with the user or application.
 */
class AuthListener {
  public:
    /**
     * Virtual destructor for derivable class.
     */
    virtual ~AuthListener() { }

    /**
     * @name Credential indication Bitmasks
     *  Bitmasks used to indicated what type of credentials are being used.
     */
    // @{
    static const uint16_t CRED_PASSWORD     = 0x0001; /**< Bit 0 indicates credentials include a password, pincode, or passphrase */
    static const uint16_t CRED_USER_NAME    = 0x0002; /**< Bit 1 indicates credentials include a user name */
    static const uint16_t CRED_CERT_CHAIN   = 0x0004; /**< Bit 2 indicates credentials include a chain of PEM-encoded X509 certificates */
    static const uint16_t CRED_PRIVATE_KEY  = 0x0008; /**< Bit 3 indicates credentials include a PEM-encoded private key */
    static const uint16_t CRED_LOGON_ENTRY  = 0x0010; /**< Bit 4 indicates credentials include a logon entry that can be used to logon a remote user */
    static const uint16_t CRED_EXPIRATION   = 0x0020; /**< Bit 5 indicates credentials include an expiration time */
    // @}
    /**
     * @name Credential request values
     * These values are only used in a credential request
     */
    // @{
    static const uint16_t CRED_NEW_PASSWORD = 0x1001; /**< Indicates the credential request is for a newly created password */
    static const uint16_t CRED_ONE_TIME_PWD = 0x2001; /**< Indicates the credential request is for a one time use password */
    // @}
    /**
     * Generic class for describing different authentication credentials.
     */
    class Credentials {
      public:

        Credentials() : mask(0) { }

        /**
         * Copy constructor
         *
         * @param other  Credentials to copy
         */
        Credentials(const Credentials& other)
        {
            this->mask = other.mask;
            this->expiration = other.expiration;
            this->pwd = other.pwd;
            this->userName = other.userName;
            this->certChain = other.certChain;
            this->pk = other.pk;
            this->logonEntry = other.logonEntry;
        }

        /**
         * Tests if one or more credentials are set.
         *
         * @param creds  A logical or of the credential bit values.
         * @return true if the credentials are set.
         */
        bool IsSet(uint16_t creds) const { return ((creds & mask) == creds); }

        /**
         * Sets a requested password, pincode, or passphrase.
         *
         * @param password  The password to set.
         */
        void SetPassword(const qcc::String& password) { this->pwd = password; mask |= CRED_PASSWORD; }

        /**
         * Sets a requested user name.
         *
         * @param newUserName  The user name to set.
         */
        void SetUserName(const qcc::String& newUserName) { this->userName = newUserName; mask |= CRED_USER_NAME; }

        /**
         * Sets a requested public key certificate chain. The certificates must be PEM encoded.
         *
         * @param certificateChain  The certificate chain to set.
         */
        void SetCertChain(const qcc::String& certificateChain) { this->certChain = certificateChain; mask |= CRED_CERT_CHAIN; }

        /**
         * Sets a requested private key. The private key must be PEM encoded and may be encrypted. If
         * the private key is encrypted the passphrase required to decrypt it must also be supplied.
         *
         * @param privateKey  The private key to set.
         */
        void SetPrivateKey(const qcc::String& privateKey) { this->pk = privateKey; mask |= CRED_PRIVATE_KEY; }

        /**
         * Sets a logon entry. For example for the Secure Remote Password protocol in RFC 5054, a
         * logon entry encodes the N,g, s and v parameters. An SRP logon entry string has the form
         * N:g:s:v where N,g,s, and v are ASCII encoded hexadecimal strings and are separated by
         * colons.
         *
         * @param newLogonEntry  The logon entry to set.
         */
        void SetLogonEntry(const qcc::String& newLogonEntry) { this->logonEntry = newLogonEntry; mask |= CRED_LOGON_ENTRY; }

        /**
         * Sets an expiration time in seconds relative to the current time for the credentials. This value is optional and
         * can be set on any response to a credentials request. After the specified expiration time has elapsed any secret
         * keys based on the provided credentials are invalidated and a new authentication exchange will be required. If an
         * expiration is not set the default expiration time for the requested authentication mechanism is used.
         *
         * The underlying key store will never expire credentials any sooner than MIN_EXPIRATION_DEFAULT seconds.
         * It is valid to provide a smaller value for expirationSeconds here, but if it is smaller than
         * MIN_EXPIRATION_DEFAULT, the actual expiration time will be MIN_EXPIRATION_DEFAULT seconds.
         *
         * @see KeyBlob::MIN_EXPIRATION_DEFAULT in <qcc/KeyBlob.h>
         *
         * @param expirationSeconds  The expiration time in seconds.
         */
        void SetExpiration(uint32_t expirationSeconds) { this->expiration = expirationSeconds; mask |= CRED_EXPIRATION; }

        /**
         * Gets the password, pincode, or passphrase from this credentials instance.
         *
         * @return A password or an empty string.
         */
        const qcc::String& GetPassword() const { return pwd; }

        /**
         * Gets the user name from this credentials instance.
         *
         * @return A user name or an empty string.
         */
        const qcc::String& GetUserName() const { return userName; }

        /**
         * Gets the PEM encoded X509 certificate chain from this credentials instance.
         *
         * @return An X509 certificate chain or an empty string.
         */
        const qcc::String& GetCertChain() const { return certChain; }

        /**
         * Gets the PEM encode private key from this credentials instance.
         *
         * @return An PEM encode private key or an empty string.
         */
        const qcc::String& GetPrivateKey() const { return pk; }

        /**
         * Gets a logon entry.
         *
         * @return An encoded logon entry or an empty string.
         */
        const qcc::String& GetLogonEntry() const { return logonEntry; }

        /**
         * Get the expiration time in seconds if it is set.
         *
         * @return The expiration or the max 32 bit unsigned value if it was not set.
         */
        uint32_t GetExpiration() { return (mask & CRED_EXPIRATION) ? expiration : 0xFFFFFFFF; }

        /**
         * Clear the credentials.
         */
        void Clear() {
            pwd.clear();
            userName.clear();
            certChain.clear();
            pk.clear();
            logonEntry.clear();
            mask = 0;
        }

      private:

        uint16_t mask;
        uint32_t expiration;

        qcc::String pwd;
        qcc::String userName;
        qcc::String certChain;
        qcc::String pk;
        qcc::String logonEntry;

    };

    /**
     * Authentication mechanism requests user credentials. If the user name is not an empty string
     * the request is for credentials for that specific user. A count allows the listener to decide
     * whether to allow or reject multiple authentication attempts to the same peer.
     *
     * An implementation must provide RequestCredentials or RequestCredentialsAsync but not both.
     *
     * @param authMechanism  The name of the authentication mechanism issuing the request.
     * @param peerName       The name of the remote peer being authenticated.  On the initiating
     *                       side this will be a well-known-name for the remote peer. On the
     *                       accepting side this will be the unique bus name for the remote peer.
     * @param authCount      Count (starting at 1) of the number of authentication request attempts made.
     * @param userName       The user name for the credentials being requested.
     * @param credMask       A bit mask identifying the credentials being requested. The application
     *                       may return none, some or all of the requested credentials.
     * @param[out] credentials    The credentials returned.
     *
     * @return  The caller should return true if the request is being accepted or false if the
     *          requests is being rejected. If the request is rejected the authentication is
     *          complete.
     */
    virtual bool RequestCredentials(const char* authMechanism, const char* peerName, uint16_t authCount, const char* userName, uint16_t credMask, Credentials& credentials) {
        QCC_UNUSED(authMechanism);
        QCC_UNUSED(peerName);
        QCC_UNUSED(authCount);
        QCC_UNUSED(userName);
        QCC_UNUSED(credMask);
        QCC_UNUSED(credentials);
        return false;
    }

    /**
     * Authentication mechanism asynchronous request for credentials. If the user name is not an empty string
     * the request is for credentials for that specific user. A count allows the listener to decide
     * whether to allow or reject multiple authentication attempts to the same peer.
     *
     * An implementation must provide RequestCredentials or RequestCredentialsAsync but not both.
     *
     * @param authMechanism  The name of the authentication mechanism issuing the request.
     * @param peerName       The name of the remote peer being authenticated.  On the initiating
     *                       side this will be a well-known-name for the remote peer. On the
     *                       accepting side this will be the unique bus name for the remote peer.
     * @param authCount      Count (starting at 1) of the number of authentication request attempts made.
     * @param userName       The user name for the credentials being requested.
     * @param credMask       A bit mask identifying the credentials being requested.
     * @param authContext    Callback context for associating the request with the returned credentials.
     *
     * @return  Return ER_OK if the request is handled.
     */
    virtual QStatus RequestCredentialsAsync(const char* authMechanism, const char* peerName, uint16_t authCount, const char* userName, uint16_t credMask, void* authContext) {
        QCC_UNUSED(authMechanism);
        QCC_UNUSED(peerName);
        QCC_UNUSED(authCount);
        QCC_UNUSED(userName);
        QCC_UNUSED(credMask);
        QCC_UNUSED(authContext);
        return ER_NOT_IMPLEMENTED;
    }

    /**
     * Respond to a call to RequestCredentialsAsync.
     *
     * @param authContext    Context that was passed in the call out to RequestCredentialsAsync.
     * @param accept         Returns true to accept the credentials request or false to reject it.
     * @param credentials    The credentials being returned if accept is true.
     *
     * @return   Returns ER_OK if the credential verification response was expected. Returns an error status if
     *           the credentials verification response was not expected.
     */
    static QStatus AJ_CALL RequestCredentialsResponse(void* authContext, bool accept, Credentials& credentials);

    /**
     * Authentication mechanism requests verification of credentials from a remote peer.
     *
     * @param authMechanism  The name of the authentication mechanism issuing the request.
     * @param peerName       The name of the remote peer being authenticated.  On the initiating
     *                       side this will be a well-known-name for the remote peer. On the
     *                       accepting side this will be the unique bus name for the remote peer.
     * @param credentials    The credentials to be verified.
     *
     * @return  The listener should return true if the credentials are acceptable or false if the
     *          credentials are being rejected.
     */
    virtual bool VerifyCredentials(const char* authMechanism, const char* peerName, const Credentials& credentials) {
        QCC_UNUSED(authMechanism);
        QCC_UNUSED(peerName);
        QCC_UNUSED(credentials);
        return true;
    }

    /**
     * Authentication mechanism asynchronous request for verification of credentials from a remote peer.
     *
     * @param authMechanism  The name of the authentication mechanism issuing the request.
     * @param peerName       The name of the remote peer being authenticated.  On the initiating
     *                       side this will be a well-known-name for the remote peer. On the
     *                       accepting side this will be the unique bus name for the remote peer.
     * @param credentials    The credentials to be verified.
     * @param authContext    Callback context for associating the request with the verification response.
     *
     * @return  Return ER_OK if the request is handled.
     */
    virtual QStatus VerifyCredentialsAsync(const char* authMechanism, const char* peerName, const Credentials& credentials, void* authContext) {
        QCC_UNUSED(authMechanism);
        QCC_UNUSED(peerName);
        QCC_UNUSED(credentials);
        QCC_UNUSED(authContext);
        return ER_NOT_IMPLEMENTED;
    }

    /**
     * Respond to a call to VerifyCredentialsAsync.
     *
     * @param authContext    Context that was passed in the call out to VerifyCredentialsAsync.
     * @param accept         Returns true to accept the credentials or false to reject it.
     *
     * @return   Returns ER_OK if the credential verification response was expected. Returns an error status if
     *           the credentials verification response was not expected.
     */
    static QStatus AJ_CALL VerifyCredentialsResponse(void* authContext, bool accept);

    /**
     * Optional method that if implemented allows an application to monitor security violations. This
     * function is called when an attempt to decrypt an encrypted messages failed or when an unencrypted
     * message was received on an interface that requires encryption. The message contains only
     * header information.
     *
     * @param status  A status code indicating the type of security violation.
     * @param msg     The message that cause the security violation.
     */
    virtual void SecurityViolation(QStatus status, const Message& msg) {
        QCC_UNUSED(status);
        QCC_UNUSED(msg);
    }

    /**
     * Reports successful or unsuccessful completion of authentication.
     *
     * @param authMechanism  The name of the authentication mechanism that was used or an empty
     *                       string if the authentication failed.
     * @param peerName       The name of the remote peer being authenticated.  On the initiating
     *                       side this will be a well-known-name for the remote peer. On the
     *                       accepting side this will be the unique bus name for the remote peer.
     * @param success        true if the authentication was successful, otherwise false.
     */
    virtual void AuthenticationComplete(const char* authMechanism, const char* peerName, bool success) = 0;
};

/**
 * AuthListener that provides the default operations for ECDHE authentication
 * mechanisms.
 */
class DefaultECDHEAuthListener : public AuthListener {

  public:
    /**
     * Create an instance of AuthListener that provides the default operations
     * for ECDHE authentication mechanisms.
     * For ECDHE_NULL authentication mechanism, the RequestCredentials callback
     *     returns true.
     * For ECDHE_PSK authentication mechanism, the RequestCredentials callback
     *     returns false.
     * For ECDHE_SPEKE authentication mechanism, the RequestCredentials callback
     *     returns false.
     * For ECDHE_ECDSA authentication mechanism, the RequestCredentials callback
     *     returns true without providing any credential.
     * This AuthListener is suitable to be used in Claimed application since
     * the framework will provide the Identity certificate chain to the peer.
     */
    DefaultECDHEAuthListener();

    /**
     * Create an instance of AuthListener that provides the default operations
     * for ECDHE authentication mechanisms.
     * For ECDHE_NULL authentication mechanism, the RequestCredentials callback
     *     returns true.
     * For ECDHE_PSK authentication mechanism, the RequestCredentials callback
     *     returns true using the provided PSK.
     * For ECDHE_SPEKE authentication mechanism, the RequestCredentials callback
     *     returns true using the provided password.
     * For ECDHE_ECDSA authentication mechanism, the RequestCredentials callback
     *     returns true without providing any credential.
     * This AuthListener is suitable to be used in Claimed application since
     * the framework will provide the Identity certificate chain to the peer.
     * @param[in] psk the pre-shared secret (or password) for ECDHE_PSK
     * @param[in] pskSize the size of the pre-shared key.  Must be zero or at
     *                    least 16 bytes.  See the remark below on secret length.
     *
     * @remarks This constructor is deprecated because ECHDE_PSK is deprecated.
     *          See the constructor DefaultECDHEAuthListener() and the
     *          SetPassword() method, to set a shared credential for the ECDHE_SPEKE
     *          mechanism.
     */
    QCC_DEPRECATED(DefaultECDHEAuthListener(const uint8_t* psk, size_t pskSize));

    ~DefaultECDHEAuthListener();

    /**
     * @brief updates the PSK used by this DefaultECDHEAuthListener
     * for the ECDHE_PSK authentication method.
     *
     * This method updates the current set secret value (or sets
     * when it is not yet defined) or clears it by providing a NULL array.
     *
     * @param[in] password the pre-shared key or nullptr.
     * @param[in] password the size of the secret.  It must be at least 16 bytes
     *                     when setting a value or 0 bytes when clearing current
     *                     value.
     * @return ER_OK on success
     *
     * @remarks This method is deprecated because ECHDE_PSK is deprecated.
     *          See SetPassword(), to set a shared credential for the ECDHE_SPEKE
     *          mechanism.
     */
    QCC_DEPRECATED(virtual QStatus SetPSK(const uint8_t * secret, size_t secretSize));

    /**
     * @brief updates the password used by this DefaultECDHEAuthListener.
     *
     * This method allows to update the current set password (or set
     * when it is not yet defined) or clears it by providing a NULL array.
     *
     * @param[in] password the pre-shared secret or nullptr.
     * @param[in] passwordSize the size of the password secret.  It must be at
     *                    least 4 characters or 0 to clear the
     *                    current value.
     * @return ER_OK on success
     */
    virtual QStatus SetPassword(const uint8_t* password, size_t passwordSize);

    /**
     * @copydoc AuthListener::RequestCredentials
     * @see AuthListener::RequestCredentials
     */
    virtual bool RequestCredentials(const char* authMechanism, const char* peerName, uint16_t authCount, const char* userName, uint16_t credMask, Credentials& credentials);

    /**
     * @copydoc AuthListener::AuthenticationComplete
     * @see AuthListener::AuthenticationComplete
     */
    virtual void AuthenticationComplete(const char* authMechanism, const char* peerName, bool success);

  private:
    /**
     * Assignment operator is private.
     */
    DefaultECDHEAuthListener& operator=(const DefaultECDHEAuthListener& other);

    /**
     * Copy constructor is private.
     */
    DefaultECDHEAuthListener(const DefaultECDHEAuthListener& other);

    uint8_t* m_psk;
    size_t m_pskSize;
    uint8_t* m_password;
    size_t m_passwordSize;
};

}

#endif
