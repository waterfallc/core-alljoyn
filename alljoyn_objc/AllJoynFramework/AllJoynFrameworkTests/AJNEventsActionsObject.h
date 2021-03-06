////////////////////////////////////////////////////////////////////////////////
//    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
//    Project (AJOSP) Contributors and others.
//
//    SPDX-License-Identifier: Apache-2.0
//
//    All rights reserved. This program and the accompanying materials are
//    made available under the terms of the Apache License, Version 2.0
//    which accompanies this distribution, and is available at
//    http://www.apache.org/licenses/LICENSE-2.0
//
//    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
//    Alliance. All rights reserved.
//
//    Permission to use, copy, modify, and/or distribute this software for
//    any purpose with or without fee is hereby granted, provided that the
//    above copyright notice and this permission notice appear in all
//    copies.
//
//    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
//    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
//    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
//    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
//    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
//    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
//    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
//    PERFORMANCE OF THIS SOFTWARE.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  ALLJOYN MODELING TOOL - GENERATED CODE
//
////////////////////////////////////////////////////////////////////////////////
//
//  DO NOT EDIT
//
//  Add a category or subclass in separate .h/.m files to extend these classes
//
////////////////////////////////////////////////////////////////////////////////
//
//  AJNEventsActionsObject.h
//
////////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import "AJNBusAttachment.h"
#import "AJNBusInterface.h"
#import "AJNProxyBusObject.h"


////////////////////////////////////////////////////////////////////////////////
//
// EventsActionsObjectDelegate Bus Interface
//
////////////////////////////////////////////////////////////////////////////////

@protocol EventsActionsObjectDelegate <AJNBusInterface>


// properties
//
@property (nonatomic, readonly) NSString* TestProperty;

// methods
//
- (NSString*)concatenateString:(NSString*)str1 withString:(NSString*)str2 message:(AJNMessage *)methodCallMessage;

// signals
//
- (void)sendtestEventString:(NSString*)outStr inSession:(AJNSessionId)sessionId toDestination:(NSString*)destinationPath;


@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// EventsActionsObjectDelegate Signal Handler Protocol
//
////////////////////////////////////////////////////////////////////////////////

@protocol EventsActionsObjectDelegateSignalHandler <AJNSignalHandler>

// signals
//
- (void)didReceivetestEventString:(NSString*)outStr inSession:(AJNSessionId)sessionId message:(AJNMessage *)signalMessage;


@end

@interface AJNBusAttachment(EventsActionsObjectDelegate)

- (void)registerEventsActionsObjectDelegateSignalHandler:(id<EventsActionsObjectDelegateSignalHandler>)signalHandler;

@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  AJNEventsActionsObject Bus Object superclass
//
////////////////////////////////////////////////////////////////////////////////

@interface AJNEventsActionsObject : AJNBusObject<EventsActionsObjectDelegate>

// properties
//
@property (nonatomic, readonly) NSString* TestProperty;


// methods
//
- (NSString*)concatenateString:(NSString*)str1 withString:(NSString*)str2 message:(AJNMessage *)methodCallMessage;


// signals
//
- (void)sendtestEventString:(NSString*)outStr inSession:(AJNSessionId)sessionId toDestination:(NSString*)destinationPath;


@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  EventsActionsObject Proxy
//
////////////////////////////////////////////////////////////////////////////////

@interface EventsActionsObjectProxy : AJNProxyBusObject

// properties
//
@property (nonatomic, readonly) NSString* TestProperty;


// methods
//
- (NSString*)concatenateString:(NSString*)str1 withString:(NSString*)str2;


@end

////////////////////////////////////////////////////////////////////////////////
