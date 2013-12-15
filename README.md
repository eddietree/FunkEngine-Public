FunkEngine
==========

> It's livin' in the funk. It's just a put-on!

## Info

Cross-platform C++ game engine. Can be used for video games or audioreactive projects. Uses [GameMonkey](http://www.gmscript.com) for Scripting and [OpenGL](http://open.gl/) for rendering. Has Fmod and OpenAL audio support.

Supported Platforms: Windows (32-bit), Mac OSX (32-bit), and iOS platforms.

FunkEngine was used on such projects as
- [Kyoto](http://funktroniclabs.com/kyoto)
- [Lotus](http://funktroniclabs.com/lotus)
- [Artifacts](http://illogictree.com/app/artifacts)
- [Ballad of the Psychotropic Robots](http://illogictree.com/games/psychotropic/)
- [Funktronic3000](http://illogictree.com/games/funktronic/)
- ...and much more!



## Visual Studio Setup

> Requires Visual Studio 2012

FunkEngine should build right out of the box, but if it doesn't work when you run it the first time in Visual Studio, you need to set just a few **User Settings**.

Right click on the Project "FunkEngine" ->Properties -> Configuration Properties -> Debugging

1. **Working Directory:** $(ProjectDir)../../../../../resources
2. **Environment:** PATH=$(ProjectDir)../../../../../resources
3. **Command arguments** _(Optional)_:  -w 1280 -h 720 --windowed

Make sure to do it for **Release** and **Debug** builds. It should look something like this:

![Project Settings](http://i.imgur.com/dJWsqf9.png)


###User Config

1. **Open File** "FunkEngine\code\cpp\engine\config\UserConfig.h"
2. **define** "CONFIG_USER" and "CONFIG_USER_GM" (ex. EDDIE)


## Xcode Setup

**Framework/Dynamic Libraries** - When adding or upgrading a new Framework, you need to hack the Install Directory to point to the relative path to allow shipping with the bundle..

1. In the directory containing the *.framework file, call: /Applications/Xcode.app/Contents/Developer/usr/bin/install_name_tool -id @executable_path/../Frameworks/Ogg.framework/Ogg Ogg.framework/Ogg
2. This will point the framework to the relative loocation of the package bundle
3. Somtimes, you need to handle cross-framework (Vorbis to orbis) references.. you do this: /Applications/Xcode.app/Contents/Developer/usr/bin/install_name_tool -change /Library/Frameworks/Ogg.framework/Versions/A/Ogg @executable_path/../Frameworks/Ogg.framework/Ogg Vorbis.framework/Vorbis
4. sometimes, you have to manually change directory references for dylib files: /Applications/Xcode.app/Contents/Developer/usr/bin/install_name_tool -id @executable_path/../Frameworks/libLeap.dylib libLeap.dylib


http://stackoverflow.com/questions/1304239/osx-changing-path-of-framework


![ScreenShot](http://funktroniclabs.com/views/images/frontpage/funktronic-header.png)
