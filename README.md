# Syncthing Tray
Qt 5-based tray application for [Syncthing](https://github.com/syncthing/syncthing)

## Supported platforms
* Designed to work under any desktop environment supported by Qt 5 with tray icon
  support
* No desktop environment specific libraries required
* Tested under \*
  * Plasma 5
  * Openbox/qt5ct/Tint2
  * Awesome
  * Cinnamon
  * Windows 10
* Can be shown as regular window if tray icon support is not available

\* If you can confirm it works under other desktop environments, please add it
to the list. Maybe someone could check whether it works under Mac OS X.

## Features
* Provides quick access to most frequently used features but does not intend to replace the official web UI
  * Check state of directories and devices
  * Check current traffic statistics
  * Display further details about direcoties and devices, like last file, last
    scan, items out of sync, ...
  * Display ongoing downloads
  * Display Syncthing log
  * Trigger re-scan of a specific directory or all directories at once
  * Open a directory with the default file browser
  * Pause/resume a specific device or all devices at once
* Shows Syncthing notifications
* Does *not* allow configuring Syncthing itself (currently I do not intend to add this feature as it could
  cause more harm than good when not implemented correctly)
* Can read the Syncthing configuration file for quick setup when just connecting to local instance
* Can shows the status of the Syncthing systemd unit and allows to start and stop it
* Provides an option to conveniently add the tray to the applications launched when the desktop environment starts
* Can launch Syncthing automatically when started and display stdout/stderr (useful under Windows)
* Provides quick access to the official web UI
  * Utilizes either Qt WebKit or Qt WebEngine
  * Can be built without web view support as well (then the web UI is opened in the regular browser)
* Allows quickly switching between multiple Syncthing instances
* Shows notifications via Qt or uses D-Bus notification daemon directly
* Features a simple command line utility `syncthingctl` to check Syncthing status and trigger rescan/pause/resume/restart
* English and German localization

## Planned features
The tray is still under development; the following features are planned:
* Show recently processed items
* Improve notification handling
* Create Plasmoid for Plasma 5 desktop

## Screenshots

### Under Openbox/Tint2
![Openbox/Tint2](/tray/resources/screenshots/tint2.png?raw=true)

### Under Plasma 5
![Plasma 5](/tray/resources/screenshots/plasma.png?raw=true)
![Plasma 5 (directory error)](/tray/resources/screenshots/plasma-2.png?raw=true)
![Plasma 5 (dark)](/tray/resources/screenshots/plasma-dark.png?raw=true)

### Settings dialog
![Settings dialog](/tray/resources/screenshots/settings.png?raw=true)

### Web view
![Web view](/tray/resources/screenshots/webview.png?raw=true)
![Web view (dark)](/tray/resources/screenshots/webview-dark.png?raw=true)

## Hotkeys
To create hotkeys, you can use the same approach as for any other
application. Just make it invoke the `syncthingctl` application with
the arguments for the desired action.

### Hotkey for web UI
Just add `--webui` to the `syncthingtray` arguments to trigger the web UI.
Syncthing Tray ensures that no second instance will be spawned if it is already
running and just trigger the web UI.

## Download
### Source
See the release section on GitHub.

### Packages and binaries
* Arch Linux
  * for PKGBUILDs checkout [my GitHub repository](https://github.com/Martchus/PKGBUILDs) or
    [the AUR](https://aur.archlinux.org/packages?SeB=m&K=Martchus)
  * for binary repository checkout [my website](http://martchus.no-ip.biz/website/page.php?name=programming)
* Tumbleweed
  * for RPM \*.spec files and binary repository checkout
    [openSUSE Build Servide](https://build.opensuse.org/project/show/home:mkittler)
* Windows
  * for mingw-w64 PKGBUILDs checkout [my GitHub repository](https://github.com/Martchus/PKGBUILDs)
  * for binaries checkout [my website](http://martchus.no-ip.biz/website/page.php?name=programming) and the
    release section on GitHub

## Build instructions
The application depends on [c++utilities](https://github.com/Martchus/cpp-utilities) and [qtutilities](https://github.com/Martchus/qtutilities) and is built the same way as these libaries. For basic instructions checkout the README file of [c++utilities](https://github.com/Martchus/cpp-utilities). For building this straight, see the next section.

The following Qt 5 modules are requried: core network gui widgets svg webenginewidgets/webkitwidgets

#### Building this straight
0. Install (preferably the latest version of) g++ or clang, the required Qt 5 modules and CMake.
1. Get the sources. For the lastest version from Git clone the following repositories:

   ```
   cd $SOURCES
   git clone https://github.com/Martchus/cpp-utilities.git
   git clone https://github.com/Martchus/qtutilities.git
   git clone https://github.com/Martchus/syncthingtray.git
   git clone https://github.com/Martchus/subdirs.git
   ```
2. Build and install everything in one step:

   ```
   cd $BUILD_DIR
   cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="/install/prefix" \
    $SOURCES/subdirs/syncthingtray
   make install -j$(nproc)
   ```

#### Select Qt module for WebView
* If Qt WebKitWidgets is installed on the system, the tray will link against it. Otherwise it will link against Qt WebEngineWidgets.
* To force usage of Qt WebKit/Qt WebEngine or to disable both add `-DWEBVIEW_PROVIDER=webkit/webengine/none` to the CMake arguments.
* To use Qt WebKit revived/ng, set the web view provider to `webkit`. It works already without any (known) issues.

#### BTW: I still prefer the deprecated Qt WebKit because
* Currently there is no way to allow a particular self-signed certificate in Qt
  WebEngine. Currently any self-signed certificate is accepted! See:
  https://bugreports.qt.io/browse/QTBUG-51176
* Qt WebEngine can not be built with mingw-w64.
* QWebEngineView seems to eat `keyPressEvent`.
* Qt WebEngine is more buggy in my experience.
* Security issues are not a concern because no other website than the
  Syncthing web UI is shown. Any external links will be opened in the
  regular web browser anyways.
* It will be replaced by the compatible Qt WebKit revived/ng soon and hence no longer be deprecated.

## Adding translations
Currently translations for English and German are available. Further translations
can be added quite easily:

1. Append a new translation file for the desired locale to the `TS_FILES` list
   in `connector/CMakeLists.txt`, `model/CMakeLists.txt` and `tray/CMakeLists.txt`.
2. Trigger a new build, eg. follow steps under *Building this straight*.
3. New translation files should have been created by the build system under
   `connector/translations`, `model/translations` and `tray/translations`.
4. Open the files with Qt Linguist to add translations. Qt Linguist is part of
   the [Qt Tools repository](http://code.qt.io/cgit/qt/qttools.git/) and its usage
   is [well documented](http://doc.qt.io/qt-5/linguist-translators.html).
g
