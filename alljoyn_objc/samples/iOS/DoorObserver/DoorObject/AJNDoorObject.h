/*------------------------------------------------------------------------------
* This is an autogenerated or copied file which AllSeen Alliance,
* as author hereby dedicates to the public domain.  Additionally, THIS
* SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND ANY CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
* AND OF GOOD TITLE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*------------------------------------------------------------------------------*/
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
//  AJNDoorObject.h
//
////////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import "AJNBusAttachment.h"
#import "AJNBusInterface.h"
#import "AJNProxyBusObject.h"


////////////////////////////////////////////////////////////////////////////////
//
// DoorObjectDelegate Bus Interface
//
////////////////////////////////////////////////////////////////////////////////

@protocol DoorObjectDelegate <AJNBusInterface>


// properties
//
@property (nonatomic, readonly) BOOL IsOpen;
@property (nonatomic, readonly) NSString* Location;
@property (nonatomic, readonly) NSNumber* KeyCode;

// methods
//
- (void)open:(AJNMessage *)methodCallMessage;
- (void)close:(AJNMessage *)methodCallMessage;
- (void)knockAndRun:(AJNMessage *)methodCallMessage;

// signals
//
- (void)sendPersonPassedThroughName:(NSString*)name inSession:(AJNSessionId)sessionId toDestination:(NSString*)destinationPath;


@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// DoorObjectDelegate Signal Handler Protocol
//
////////////////////////////////////////////////////////////////////////////////

@protocol DoorObjectDelegateSignalHandler <AJNSignalHandler>

// signals
//
- (void)didReceivePersonPassedThroughName:(NSString*)name inSession:(AJNSessionId)sessionId message:(AJNMessage *)signalMessage;


@end

@interface AJNBusAttachment(DoorObjectDelegate)

- (void)registerDoorObjectDelegateSignalHandler:(id<DoorObjectDelegateSignalHandler>)signalHandler;

@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  AJNDoorObject Bus Object superclass
//
////////////////////////////////////////////////////////////////////////////////

@interface AJNDoorObject : AJNBusObject<DoorObjectDelegate>

// properties
//
@property (nonatomic) BOOL IsOpen;
@property (nonatomic) NSString* Location;
@property (nonatomic) NSNumber* KeyCode;


// methods
//
- (void)open:(AJNMessage *)methodCallMessage;
- (void)close:(AJNMessage *)methodCallMessage;
- (void)knockAndRun:(AJNMessage *)methodCallMessage;


// signals
//
- (void)sendPersonPassedThroughName:(NSString*)name inSession:(AJNSessionId)sessionId toDestination:(NSString*)destinationPath;


@end

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  DoorObject Proxy
//
////////////////////////////////////////////////////////////////////////////////

@interface DoorObjectProxy : AJNProxyBusObject

// properties
//
@property (nonatomic, readonly) BOOL IsOpen;
@property (nonatomic, readonly) NSString* Location;
@property (nonatomic, readonly) NSNumber* KeyCode;


// methods
//
- (void)open;
- (void)close;
- (void)knockAndRun;

@end

////////////////////////////////////////////////////////////////////////////////
