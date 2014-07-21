mage-sdk-cpp-cocos2dx
=====================

This demonstrate how you can integrate
mage-sdk-cpp with an existing cocos2dx project.

How it is done
--------------

See [this diff](https://github.com/mage/mage-sdk-cpp-cocos2dx/compare/a215aa26903ae38e658f608e2c9a8259514e43a7...master) for more details.

### The bootstrap

```bash
cocos new -l cpp mage-sdk-cpp-cocos2dx
cd mage-sdk-cpp-cocos2dx
git init
vim .gitignore # add what we need to ignore
git add .
git commit -m "First commit"
mkdir -p ./Vendors
git submodule add git@github.com:mage/mage-sdk-cpp.git ./Vendors/mage-sdk-cpp
pushd ./Vendors/mage-sdk-cpp
git submodule update --init
make platforms
popd
```

This will build all the libraries you will
need for the purpose of this example.

### The code

Nothing special here. In your header file, add

```
#include "mage.h"
```

And start using MAGE in your code!

```cpp
# [... snip ...]
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	#
	# Use MAGE!
	#
	mage::RPC mage("game", "localhost:8080");
# [... snip ...]
```

Note that this sample code does quite a bit more than what is described here.
It demonstrate, for instance, some possible flow control using MAGE SDK's asynchronous
callback calls. So you will want to have a look at the code in [./Classes](./Classes) to
really understand the power this SDK can bring you.

### Android

First, we need to modify `./proj.android/jni/Application.mk`
so that it use C++ static for STL and add `-DHTTP_CONNECTOR` to the `APP_CPPFLAGS`:

```Makefile
APP_STL := c++_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 -std=c++11 -DHTTP_CONNECTOR -fsigned-char
```

In `./proj.android/jni/Android.mk`,
add the following at the very top of the file:

```Makefile
#
# cURL - you may choose to include the
# version bundled in your cocos2dx project instead
#
include $(CLEAR_VARS)

LOCAL_MODULE := curl
LOCAL_SRC_FILES := ../../Vendors/mage-sdk-cpp/platforms/externals/curl/prebuilt/android/armeabi/libcurl.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/platforms/externals/curl/include/android

include $(PREBUILT_STATIC_LIBRARY)

#
# Define MAGE-SDK
#
include $(CLEAR_VARS)

LOCAL_MODULE := mage-sdk
LOCAL_SRC_FILES := ../../Vendors/mage-sdk-cpp/platforms/android/obj/local/armeabi/libmage.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/src
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Vendors/mage-sdk-cpp/vendor/libjson-rpc-cpp/src

include $(PREBUILT_STATIC_LIBRARY)
```

Next, in your main application, enable the
use of exceptions:

```
LOCAL_CPPFLAGS := -fexceptions
```

Add the following where you add your main application static libraries:

```Makefile
LOCAL_WHOLE_STATIC_LIBRARIES += curl
LOCAL_WHOLE_STATIC_LIBRARIES += mage-sdk
```

Finally, it seems that older API versions complain about
[missing atomic libs at link time](https://code.google.com/p/android/issues/detail?id=68779
). To solve this issue, let's make sure it is added to our `LOCAL_LDLIBS`:

```
LOCAL_LDLIBS += -latomic
```

You should be good to go!

```bash
cocos run -p android
```

### iOS

iOS requires you to use XCode.

#### Adding the SDK

First, open your `./proj.ios_mac` in XCode.

Next, right-click on your project's name in the left pane,
and select "Add files to mage-sdk-cpp-cocos2dx". Navigate to
`./Vendors/mage-sdk-cpp/platforms/ios/`, and select `mage-sdk.xcodeproj`.

#### Setting your search paths

After that, go under you project's "Build settings", all the way down to
`Search paths`. Make sure that your path contains at least the following
(I have added the newlines for readability):

```
$(inherited)
$(SRCROOT)/../cocos2d/cocos/platform/ios
$(SRCROOT)/../cocos2d/cocos/platform/ios/Simulation
$(SRCROOT)/../Vendors/mage-sdk-cpp/src/
$(SRCROOT)/../Vendors/mage-sdk-cpp/vendor/libjson-rpc-cpp/src
$(SRCROOT)/../Vendors/mage-sdk-cpp/platforms/externals/curl/include/ios
```

Finally, in `./proj.ios_mac/mage-sdk-cpp-cocos2dx.xcodeproj/project.pbxproj`, please make sure to add `"HTTP_CONNECTOR=1"` to all instances of `GCC_PREPROCESSOR_DEFINITIONS`. It should then look like the following:

```
GCC_PREPROCESSOR_DEFINITIONS = (
	USE_FILE32API,
	CC_TARGET_OS_IPHONE,
	"CC_ENABLE_CHIPMUNK_INTEGRATION=1",
	"HTTP_CONNECTOR=1",
);
```

You should be done! Press play and try to build to a device.
From now you should also be able to build using

```bash
cocos run -p ios
```

See also
---------

- [mage-sdk-cpp](https://github.com/mage/mage-sdk-cpp)
