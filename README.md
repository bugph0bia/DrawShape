DrawShape
===

![Software Version](http://img.shields.io/badge/Version-v1.1.1-green.svg?style=flat)
![VS Version](http://img.shields.io/badge/VisualStudio-2019-blue.svg?style=flat)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

[Japanese Page](./README.ja.md)

## Overview
CAD-like ActiveX control to easily draw shapes and easily zoom and pan the displayed content.

- Usage examples 1
    - ![use1](https://user-images.githubusercontent.com/18702413/71414988-ecee1100-269c-11ea-895c-096abe43e30e.png)

- Usage examples 2
    - ![use2](https://user-images.githubusercontent.com/18702413/71182037-ac674f80-22b8-11ea-8676-d9a71077a38e.gif)

## Version
v1.1.1

## Development Environment
Visual Studio 2019

## License
MIT License

## OSS used
- [PicoJSON](https://github.com/kazuho/picojson)

## Install/Uninstall
### How to install
- Case Windows (x86)
    1. Place `DrawShape.ocx` in a folder of your choice.
    2. Launch the command prompt with administrative privileges.
    3. Run `regsvr32 DrawShape.ocx`.
- Case Windows (x64)
    1. Place `DrawShape.ocx` in a folder of your choice.
    2. Launch the command prompt with administrative privileges.
    3. Run `C:\Windows\SysWow64\regsvr32 DrawShape.ocx`.

### How to uninstall
- Case Windows (x86)
    1. Place `DrawShape.ocx` in a folder of your choice.
    2. Launch the command prompt with administrative privileges.
    3. Run `regsvr32 /u DrawShape.ocx`.
- Case Windows (x64)
    1. Place `DrawShape.ocx` in a folder of your choice.
    2. Launch the command prompt with administrative privileges.
    3. Run `C:\Windows\SysWow64\regsvr32 /u DrawShape.ocx`.

## How to use
### C++ / MFC Applications (on Visual Studio 2019)
1. How to Insert a control into a Dialog.
    1. Open the dialog editor from the resource view.
    2. Right-click on the dialog and select "Insert ActiveX Control...".
    3. Select "DrawShape Control" from the "Insert ActiveX Control" window and click the "OK" button.
2. How to add properties and methods.
    1. Right click on the inserted DrawShape control and select "Add Variable...". to add a control variable.
    2. The IDispatch wrapper class will be automatically inserted into the project and the control variables of the type of the class will be added.
       The member functions of the wrapper class will have Getter/Setter properties and methods.
        - This is supposed to be the case, but it does not work in Visual Studio 2017 / 2019; it works fine in 2013.
3. How to add an event handlers.
    1. Right click on the inserted DrawShape control and select "Add Event Handler...".
    2. In the wizard that appears, specify the event you want to add and the member function of the handler, and insert it.

### C# / Windows Form Applications (on Visual Studio 2019)
1. How to insert a control into a form.
    1. Open the Form Designer from the Solution Explorer.
    2. Right-click on an empty area of the toolbox and select "Select Item...".
    3. Open the "COM Components" tab in the "Select Toolbox Items" window, check the "DrawShape Control" checkbox, and click the "OK" button.
    4. The "DrawShape Control" is added to the toolbox and inserted into the form.
    5. If the insertion fails, click "Add Reference..." in the "Project" menu. If the insertion fails, check "DrawShapeLib" in the "COM" page of the "Project" menu and click the "OK" button.
2. How to add properties and methods.
    1. By inserting the control, the AxDrawShape class (AxDrawShapeLib namespace - AxDrawShape class) is added to the project reference and the control variables are of the type of that class. The control variables are of the class type. The properties and methods are also added and ready to use.
3. How to add the event handlers.
    1. Event handler functions can be added by using the event variables and delegate types that have been added to the AxDrawShape class (AxDrawShapeLib namespace - AxDrawShape class).

[C# Sample Application](/DrawShapeTest)

## Reference
[Reference](REFERENCE.md)
