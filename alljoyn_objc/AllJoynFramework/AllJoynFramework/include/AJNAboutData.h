/**
 * @file
 * This contains the AboutData class responsible for holding the org.alljoyn.About
 * interface data fields.
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

#import "AJNObject.h"
#import "AJNMessageArgument.h"
#import "AJNTranslator.h"
#import "AJNAboutDataListener.h"
#import "String.h"


@interface AJNAboutData : AJNObject<AJNAboutDataListener>

@property (nonatomic, weak) id<AJNTranslator>translatorDelegate;

- (id)initWithLanguage: (NSString*)language;
/**
 * Instance/reference object
 *
 * @param msgArg The message argument
 * @param language A locale containing a IETF language tag specified by RFC 5646
 *
 * @returns An instance of AJNAboutData
 */
- (instancetype)initWithMsgArg:(AJNMessageArgument *)msgArg andLanguage:(NSString *)language;


- (BOOL)isHexChar:(char)c;



/**
 * use xml definition of AboutData to set the about data.
 @code
 "<AboutData>"
 "  <AppId>000102030405060708090A0B0C0D0E0C</AppId>"
 "  <DefaultLanguage>en</DefaultLanguage>"
 "  <DeviceName>My Device Name</DeviceName>"
 "  <DeviceName lang = 'es'>Nombre de mi dispositivo</DeviceName>"
 "  <DeviceId>93c06771-c725-48c2-b1ff-6a2a59d445b8</DeviceId>"
 "  <AppName>My Application Name</AppName>"
 "  <AppName lang = 'es'>Mi Nombre de la aplicacion</AppName>"
 "  <Manufacturer>Company</Manufacturer>"
 "  <Manufacturer lang = 'sp'>Empresa</Manufacturer>"
 "  <ModelNumber>Wxfy388i</ModelNumber>"
 "  <Description>A detailed description provided by the application.</Description>"
 "  <Description lang = 'es'>Una descripcion detallada proporcionada por la aplicacion.</Description>"
 "  <DateOfManufacture>2014-01-08</DateOfManufacture>"
 "  <SoftwareVersion>1.0.0</SoftwareVersion>"
 "  <HardwareVersion>1.0.0</HardwareVersion>"
 "  <SupportUrl>www.example.com</SupportUrl>"
 "</AboutData>"
 @endcode
 *
 * The createFromXml method will attempt to process the entire xml passed
 * in. If a parsing error is encountered the last error found will be thrown
 * as a BusException.
 *
 * Note: AJSoftwareVersion is automatically set to the version of Alljoyn that
 * is being used. The SupportedLanguages tag is automatically implied from
 * the DefaultLanguage tag and the lang annotation from tags that are
 * localizable.
 *
 * @param aboutXmlData A string that contains an XML representation of
 *                     the AboutData fields.
 * @return ER_OK on success
 */
- (QStatus)createFromXml:(NSString *)aboutXmlData;

 /**
  * The AboutData has all of the required fields
  *
  * If a language field is given this will return if all required fields are
  * listed for the given language.
  *
  * If no language is given, the default language will be checked
  *
  * @param[in] language IETF language tags specified by RFC 5646
  *
  * @return true if all required field are listed for the given language
  */
- (BOOL)isValid:(NSString *)language;



/**
 * The AboutData has all of the required fields.
 *
 * Only fields for the default language will be checked.
 *
 * @return true if all required field are listed for the given language
 */
- (BOOL)isValid;


/**
 * Fill in the AboutData fields using a String/Variant Map.
 *
 * The MsgArg must contain a dictionary of type a{sv} The expected use of this
 * class is to fill in the AboutData using a Map obtain from the Announce
 * signal or the GetAboutData method from org.alljoyn.about interface.
 *
 * @param msgArg The message argument
 * @param language A Locale containing a IETF language tag specified by RFC 5646
 *
 * @return A status indicating that the msgArg Map is missing a
 *         required field. Typically this means the default
 *         language was not specified.
 */
- (QStatus)createFromMsgArg:(AJNMessageArgument *)msgArg andLanguage:(NSString *)language;

/**
 * Set the AppId for the AboutData using a hex encoded String.
 *
 * AppId IS required
 * AppId IS part of the Announce signal
 * AppId CAN NOT be localized for other languages
 *
 * @param appId Hex encoded String representing a globally unique array of bytes used as an ID for the application.
 *
 * @return A status indicating failure to set the AppId
 */
- (QStatus)setAppId:(uint8_t[])appId;


/**
 *Set the default language.
 *
 * This Locale language tag is automatically added to the SupportedLanguage
 * list. The Locale language tag should be an IETF language tag specified by RFC 5646.
 *
 * DefaultLanguage is Required
 * DefaultLanguage is part of the Announce signal
 *
 * @param language A Locale specifying the IETF language tag for the default language
 *
 * @return The status indicating failure to set the default language
 */
- (QStatus)setDefaultLanguage:(NSString *)language;

/**
 *Get the DeviceName from the About data
 *
 * DeviceName is not required
 * DeviceName is part of the Announce signal
 * DeviceName can be localized for other languages
 *
 * @param defaultLanguage A Locale for the IETF language tag specified by RFC 5646
 *                 if language is null the DeviceName for the default
 *                 language will be returned.
 *
 * @return A status indicating failure to obtain the device name
 */
- (QStatus)getDefaultLanguage:(NSString **)defaultLanguage;

/**
 *Set the DeviceName to the AboutData
 *
 * DeviceName is not required
 * DeviceName is part of the Announce signal
 * DeviceName can be localized for other languages
 *
 * @param deviceName the deviceName
 * @param language Locale language tag specified by RFC 5646 if language is
 *                 null the DeviceName will be set for the default language.
 *
 * @return A status indicating failure to set the device name
 */
- (QStatus)setDeviceName:(NSString *)deviceName andLanguage:(NSString *)language;

/**
 * Get the DeviceName from the About data
 *
 * DeviceName is not required
 * DeviceName is part of the Announce signal
 * DeviceName can be localized for other languages
 *
 * @param language Locale for the IETF language tag specified by RFC 5646
 *                 if language is null the DeviceName for the default
 *                 language will be returned.
 *
 * @param deviceName The device name found in the AboutData
 * @return A status indicating failure to obtain the device name
 */
- (QStatus)getDeviceName:(NSString **)deviceName andLanguage:(NSString *)language;

/**
 * Set the DeviceId from the AboutData
 *
 * DeviceId IS required
 * DeviceId IS part of the announce signal
 * DeviceId CAN NOT be localized for other languages
 *
 * @param deviceId A string with a value generated using platform specific
 *                 means
 *
 * @return A status to obtain the device id.
 */
- (QStatus)setDeviceId:(NSString *)deviceId;

/**
 * Get the DeviceId from the AboutData
 *
 * DeviceId IS required
 * DeviceId IS part of the announce signal
 * DeviceId CAN NOT be localized for other languages
 *
 * @param deviceId string pointer
 *
 * @return A status to find device id.
 */
- (QStatus)getDeviceId:(NSString **)deviceId;

/**
 * Set the AppName to the AboutData
 *
 * AppName is required
 * AppName is part of the announce signal
 * AppName can be localized for other languages
 *
 * @param appName the AppName (UTF8 encoded string)
 * @param language Locale for the IETF language tag specified by RFC 5646
 *                 if language is null the AppName will be set for the
 *                 default language.
 *
 * @returns A status indicating failure to set the AppName.
 */
- (QStatus)setAppName:(NSString *)appName andLanguage:(NSString *)language;

/**
 * Get the AppName from the About data
 *
 * AppName is required
 * AppName is part of the announce signal
 * AppName can be localized for other languages
 *
 * @param[out] appName the AppName found in the AboutData (UTF-8 encoded string)
 * @param[in] language the IETF language tag specified by RFC 5646
 *            if language is NULL the AppName for the default language will be returned.
 *
 * @return ER_OK on success
 */
- (QStatus)getAppName:(NSString **)appName andLanguage:(NSString *)language;


/**
 * Set the Manufacture for the AboutData
 *
 * Manufacture is required
 * Manufacture is part of the announce signal
 * Manufacture can be localized for other languages
 *
 * @param manufacturer the Manufacturer (UTF8 encoded string)
 * @param language Locale containing the IETF language tag specified by
 *                 RFC 5646 if language is null the Manufacture will be set
 *                 for the default language.
 *
 * @return A status unable to set the Manufacture
 */
- (QStatus)setManufacturer:(NSString *)manufacturer andLanguage:(NSString *)language;

/**
 * Get the Manufacturer from the About data.
 *
 * Manufacture is required
 * Manufacture is part of the announce signal
 * Manufacture can be localized for other languages
 *
 * @param language Locale containing the IETF language tag specified by
 *                 RFC 5646 if language is null the Manufacturer for the
 *                 default language will be returned.
 * @param manufacturer The name of the manufacturer
 *
 * @return A status indicating failure to obtain the manufacturer name
 */
- (QStatus)getManufacturer:(NSString **)manufacturer andLanguage:(NSString *)language;

/**
 * Set the ModelNumber to the AboutData
 *
 * ModelNumber is required
 * ModelNumber is part of the announce signal
 * ModelNumber can not be localized for other languages
 *
 * @param modelNumber the application model number
 *
 * @return A status indicating failure to obtain the model number
 */
- (QStatus)setModelNumber:(NSString *)modelNumber;

/**
 * Get the ModelNumber from the AboutData
 *
 * ModelNumber IS required
 * ModelNumber IS part of the announce signal
 * ModelNumber CAN NOT be localized for other languages
 *
 * @param modelNumber the model number
 *
 * @return A status indicating failure to obtain the model number
 */
- (QStatus)getModelNumber:(NSString **)modelNumber;

/**
 * Set a supported language.
 *
 * This is a Locale representing the a single language. The language is
 * specified in a Locale using IETF language tags specified by the RFC 5646.
 *
 * If the language tag has already been added no error will be thrown. The
 * method will returned with no changes being made.
 *
 * @param language Locale containing the IETF language tag
 *
 * @return A status indicating failure to set the language tag.
 */
- (QStatus)setSupportedLanguage:(NSString *)language;

/**
 * Get an array of supported languages
 *
 * The language is specified in a Locale using IETF language tags specified by the RFC 5646.
 *
 * @return A NSArray with supported languages
 *
 */
- (NSArray<NSString *> *)getSupportedLanguages;

/**
 * Deprecated API. Use getSupportedLanguages: instead of this.
 *
 * Get and length of supported languages
 *
 * @return length of locals containing languageTags.
 */

//TODO: 17.10 annotate as deprecated
- (size_t)getSupportedLanguages:(NSString**)languageTags num:(size_t)num;

/**
 * Set the Description to the AboutData
 *
 * Description IS required
 * Description IS NOT part of the announce signal
 * Description CAN BE localized for other languages
 *
 * @param description The Description (UTF8 encoded string)
 * @param language Locale containing the IETF language tag specified by
 *                 RFC 5646 if language is null the Description will be set
 *                 for the default language.
 *
 * @return A status indicating failure to set the description.
 */
- (QStatus)setDescription:(NSString *)description andLanguage:(NSString *)language;

/**
 * Set the Description to the AboutData for the default language.
 *
 * Description IS required
 * Description IS NOT part of the announce signal
 * Description CAN BE localized for other languages
 *
 * @param[out] description the Description (UTF8 encoded string)
 *
 * @param language The IETF language tag specified by RFC 5646
 *        if language is NULL the Description for the default language will be returned.
 *
 * @return A status indicating failure to set the description.
 */
- (QStatus)getDescription:(NSString **)description language:(NSString *)language;

/**
 * Set the DateOfManufacture to the AboutData
 *
 * The date of manufacture using the format YYYY-MM-DD.  Known as XML
 * DateTime format.
 *
 * ModelNumber IS NOT required
 * ModelNumber IS NOT part of the announce signal
 * ModelNumber CAN NOT be localized for other languages
 *
 * @param dateOfManufacture The date of manufacture using YYYY-MM-DD format
 *
 * @returnA A status indicating failure to set the date of manufacture
 */
- (QStatus)setDateOfManufacture:(NSString *)dateOfManufacture;

/**
 * Get the DatOfManufacture from the AboutData
 *
 * The date of manufacture using the format YYYY-MM-DD.  Known as XML
 * DateTime format.
 *
 * ModelNumber IS NOT required
 * ModelNumber IS NOT part of the announce signal
 * ModelNumber CAN NOT be localized for other languages
 *
 * @param[out] dateOfManufacture A pointer to expect the value of date Of Manufacture
 *
 * @return A status indicating failure to get the date of manufacture.
 */
- (QStatus)getDateOfManufacture:(NSString **)dateOfManufacture;

/**
 * Set the SoftwareVersion to the AboutData
 *
 * SoftwareVersion IS required
 * SoftwareVersion IS NOT part of the announce signal
 * SoftwareVersion CAN NOT be localized for other languages
 *
 * @param softwareVersion The software version for the OEM software
 *
 * @return A status indicating failure to set the software version.
 */
- (QStatus)setSoftwareVersion:(NSString *)softwareVersion;

/**
 * Get the SoftwareVersion from the AboutData
 *
 * SoftwareVersion IS required
 * SoftwareVersion IS NOT part of the announce signal
 * SoftwareVersion CAN NOT be localized for other languages
 *
 * @param softwareVersion A pointer to expect the vale software version
 *
 * @return A status indicating failure to get the date of manufacture.
 */
- (QStatus)getSoftwareVersion:(NSString **)softwareVersion;

/**
 * Get the AJSoftwareVersion from the AboutData
 *
 * The AJSoftwareVersion is automatically set when the AboutData is created
 * or when it is read from remote device.
 *
 * ModelNumber IS required
 * ModelNumber IS NOT part of the announce signal
 * ModelNumber CAN NOT be localized for other languages
 *
 * @param ajSoftwareVersion A pointer to expect the AllJoyn software version
 *
 * @return A status indicating failure to get the AllJoyn software version
 */
- (QStatus)getAJSoftwareVersion:(NSString **)ajSoftwareVersion;

/**
 * Set the HardwareVersion to the AboutData
 *
 * HardwareVersion IS NOT required
 * HardwareVersion IS NOT part of the announce signal
 * HardwareVersion CAN NOT be localized for other languages
 *
 * @param hardwareVersion The device hardware version
 *
 * @returns A status indicating failure to set the hardware version
 */
- (QStatus)setHardwareVersion:(NSString *)hardwareVersion;

/**
 * Get the HardwareVersion from the AboutData
 *
 * HardwareVersion IS NOT required
 * HardwareVersion IS NOT part of the announce signal
 * HardwareVersion CAN NOT be localized for other languages
 *
 * @param hardwareVersion A pointer to expect the hardware version
 *
 * @return A status indicating failure to read the hardware version.
 */
- (QStatus)getHardwareVersion:(NSString **)hardwareVersion;

/**
 * Set the SupportUrl to the AboutData
 *
 * SupportUrl IS NOT required
 * SupportUrl IS NOT part of the announce signal
 * SupportUrl CAN NOT be localized for other languages
 *
 * @param supportUrl The support URL to be populated by OEM
 *
 * @returns A status indicating failure to set the support URL
 */
- (QStatus)setSupportUrl:(NSString*)supportUrl;

/**
 * Get the SupportUrl from the AboutData
 *
 * SupportUrl IS NOT required
 * SupportUrl IS NOT part of the announce signal
 * SupportUrl CAN NOT be localized for other languages
 *
 * @param supportUrl The pointer to expet the value of the support URL
 * @return A status indicating failure to get the support URL
 */
- (QStatus)getSupportUrl:(NSString**)supportUrl;

/**
     * generic way to Set new field.  Everything could be done this way.
     *
     * Unless the generic field is one of the pre-defined fields when they are
     * set they will have the following specifications
     *   NOT required
     *   NOT part of the announce signal
     *   CAN be localized
     *
     * Since every field can be localized even if the field is not localized it
     * must be set for every language.
     *
     * @param[in] name The name of the field to set
     * @param[in] msgArg The message argument that contains the value that is set for the field
     *
     * @return
     *  ER_OK on success,
     *  ER_ABOUT_DEFAULT_LANGUAGE_NOT_SPECIFIED if language tag was not specified
     *                                             and the default language is also
     *                                             not found.
     */
- (QStatus)setField:(NSString*)name msgArg:(AJNMessageArgument*)msgArg;

/**
 * generic way to Set new field.  Every field could be set this way.
 *
 * Unless the generic field is one of the pre-defined fields when they are
 * set they will have the following specifications
 *   NOT required
 *   NOT part of the announce signal
 *   CAN be localized if it is a string NOT localized otherwise
 *
 * Since every field can be localized even if the field is not localized it
 * must be set for every language.
 *
 * @param name     The name of the field to set
 * @param msgArg   The message argument that contains the value that is set for the field
 * @param language The IETF language tag specified by RFC 5646 if language
 *                 is null the default language will be used.  Only used for
 *                 fields that are marked as localizable.
 *
 * @return A status indicating failure to set the field
 */
- (QStatus)setField:(NSString*)name msgArg:(AJNMessageArgument*)msgArg andLanguage:(NSString*)language;

/**
     * generic way to get field.
     *
     * @param[in] name The name of the field to get
     * @param[out] msgArg The message argument holding a variant value that represents the field
     * @param[in] language The IETF language tag specified by RFC 5646
     *            if language is NULL the field for the default language will be
     *            returned.
     *
     * @return A status indicating failure to get the field
     */
- (QStatus)getField:(NSString*)name messageArg:(AJNMessageArgument*)msgArg language:(NSString*)language;

@end
