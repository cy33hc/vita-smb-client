# vita-smb-client
Simple SMB Client App for Vita for connecting to remote Windows Shares, Linux SMB Shares and NAS SMB shares to transfer files back and forth.

![Preview](/screenshot.jpg)

# Requirements
The app is built with imgui-vita which needs the **libshacccg.suprx** extracted. A message will be displayed that you are missing the libshacccg.suprx files on starting the app.

Follow this wiki to extract the libshaccg.suprx

https://samilops2.gitbook.io/vita-troubleshooting-guide/shader-compiler/extract-libshacccg.suprx

# Know Issues
 - Cannot rename files/folders in subdirectories in Windows Shares. Linux SMB Shares works.
 - Opening the Input Editor multiple times crashes the application. I can't figure out what's wrong. If somebody knows what's the problem. Help me out.

# Multi Language support
The appplication can auto-detect the following standard PSVITA languages
```
Japanese
English
French
Spanish
German
Italian
Dutch
Portuguese_BR
Protugese_PT
Russian
Korean
Chinese
Polish
```

For **Non-Standard** Languages like **Croatian, Catalan, Euskera, Galego, Hungarian, Indonesian, Greek, Thai**

User must modify the file **ux0:data/SMBCLI0001/config.ini** and update the **language** setting to Croatian, Catalan, Euskera, Galego, Hungarian, Indonesian, Greek or Thai

# Credits
Thx to the following people for translating the string used in the app.

Author of libsmb2 used to build this app. https://github.com/sahlberg/libsmb2

Borrowed the Theme colors and Folder/File icon from NXShell. https://github.com/joel16/NX-Shell/releases

Copied some of the language code from yoyoloader https://github.com/Rinnegatamante/yoyoloader_vita

Also thx to @Rinnegatamante for imgui-vita library
